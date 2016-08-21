#include "wx/wx.h"
#include "wx/image.h"
#include "wx/aboutdlg.h"
#include "wx/stdpaths.h"
#include "spacetheremindlg.h"
#include "info.xpm"
#include <math.h>
#define MIN_PITCH 20
#define MAX_PITCH 8000
#define BUFFERLENGTH 512

#include <iostream>

#include <wx/apptrait.h>

MouseThereminDlg* dlg = NULL;

/*!
 * MouseThereminDlg type definition
 */
IMPLEMENT_DYNAMIC_CLASS( MouseThereminDlg, wxDialog )

/*!
 * MouseThereminDlg event table definition
 */
BEGIN_EVENT_TABLE( MouseThereminDlg, wxDialog )
    EVT_BUTTON( ID_BUTTON_STARTSTOP, MouseThereminDlg::OnButtonStartStop )
    EVT_MOUSEWHEEL( MouseThereminDlg::OnMouseWheel )
    EVT_CHOICE( ID_WAVEFORM, MouseThereminDlg::OnWaveform )
    EVT_CHOICE( ID_VIBRATO_WAVEFORM, MouseThereminDlg::OnVibratoWaveform )
    EVT_SLIDER( ID_SLIDER_VIBRATOFREQ, MouseThereminDlg::OnVibratoSlider )
    EVT_SPINCTRL( ID_SPIN_MINFREQ, MouseThereminDlg::OnMinFreq )
    EVT_SPINCTRL( ID_SPIN_MAXFREQ, MouseThereminDlg::OnMaxFreq )
    EVT_SPINCTRL( ID_SPIN_VIBRATODEPTH, MouseThereminDlg::OnVibratoDepth )
    EVT_TEXT( ID_SPIN_MINFREQ, MouseThereminDlg::OnMinFreqT )
    EVT_TEXT( ID_SPIN_MAXFREQ, MouseThereminDlg::OnMaxFreqT )
    EVT_TEXT( ID_SPIN_VIBRATODEPTH, MouseThereminDlg::OnVibratoDepthT )
    EVT_CHECKBOX( ID_CHECK_LOGPITCH, MouseThereminDlg::OnLogPitch )
    EVT_CHECKBOX( ID_CHECK_LOGVOLUME, MouseThereminDlg::OnLogVolume )
	EVT_CHECKBOX( ID_CHECK_MODULATION, MouseThereminDlg::OnModulation )
	EVT_SIZE( MouseThereminDlg::OnSize )
	EVT_BUTTON( ID_ABOUT, MouseThereminDlg::OnAbout )
	EVT_CLOSE( MouseThereminDlg::OnClose )
END_EVENT_TABLE()

/*!
 * MouseThereminDlg constructors
 */
MouseThereminDlg::MouseThereminDlg( )
{
	_buffer = NULL;
}

MouseThereminDlg::~MouseThereminDlg()
{
    PaError err = Pa_AbortStream( _buffer );
	if( _buffer != NULL )
	{
		err = Pa_CloseStream( _buffer );
	}
	err = Pa_Terminate();

    // Give ourselves a few milliseconds for things to stop.
    Sleep(12);
}

MouseThereminDlg::MouseThereminDlg( wxWindow* parent, bool use_openal, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, use_openal, id, caption, pos, size, style);
}

/*!
 * MouseThereminDlg creator
 */
bool MouseThereminDlg::Create( wxWindow* parent, bool use_openal, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	_bottomRowSizer = NULL;
	_buffer = NULL;
	_pnlDisplay = NULL;
	_waveTable = new WaveTable();
    _useopenal = use_openal;
    _sampleRate = 44100; // Default. 
	// Create a new stereo buffer.
    _waveform = NULL;
	_btnStartStop = NULL;
    _vibratoWaveform;
    _spnMinFreq = NULL;
    _spnMaxFreq = NULL;
    _spnVibratoDepth = NULL;
    _vibratoFreq = NULL;
    _chkLogPitch = NULL;
    _chkLogVolume = NULL;
	_started = false;
	_volume = 0.0;
	_pitch = 0.1;
	_phase = 0.0;
	_btnAbout = NULL;
    _vibratoWave = WAVEFORM_SINE;
	_wave = WAVEFORM_SINE;
    _vibratoDepth = 0.0;
    _vibratoPitch = 0.1;
    _vibratoPhase = 0.0;
    _minFreq = MIN_PITCH;
    _maxFreq = MAX_PITCH;
    _logPitch = false;
    _logVolume = false;

    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();

    // Create wavetables
    _waveTable->CreateWavetables();

    // Initialize audio
    PaError err = Pa_Initialize();
    if( err != paNoError )
    {
        wxMessageBox( _("PortAudio error: %s\n"), wxString::FromAscii(Pa_GetErrorText( err )) );
    }

    dlg = this;

    return true;
}

/*!
 * Control creation for MouseThereminDlg
 */
void MouseThereminDlg::CreateControls()
{
	wxInitAllImageHandlers();
#ifdef __WXMAC__
    wxString path = wxStandardPaths::Get().GetResourcesDir();
    _image.LoadFile( wxString::Format( _("%s/background.jpg"), path.c_str() ), wxBITMAP_TYPE_JPEG );
#else
    _image.LoadFile( _("background.jpg"), wxBITMAP_TYPE_JPEG );
#endif
	if( !_image.IsOk() )
	{
		wxMessageBox(_("Could not load background.jpg"), _("Image Not Found"));
	}

	_bitmap = wxBitmap(_image);
	if( _icon.LoadFile(_("theremin.ico"), wxBITMAP_TYPE_ICO ))
	{
		SetIcon(_icon);
	}

    MouseThereminDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    _pnlDisplay = new wxPanel( itemDialog1, ID_PANEL, wxDefaultPosition, wxSize(800, 600), wxTAB_TRAVERSAL );
    _pnlDisplay->SetBackgroundColour(wxColour(0, 0, 0));
    itemBoxSizer2->Add(_pnlDisplay, 1, wxALIGN_CENTER_HORIZONTAL|wxGROW|wxALL, 5);
	_pnlDisplay->Connect(ID_PANEL, wxEVT_MOTION, wxMouseEventHandler(MouseThereminDlg::OnMouseMove), NULL, this);
	_pnlDisplay->Connect(ID_PANEL, wxEVT_LEAVE_WINDOW, wxMouseEventHandler(MouseThereminDlg::OnMouseLeave), NULL, this );
	_pnlDisplay->Connect(ID_PANEL, wxEVT_ENTER_WINDOW, wxMouseEventHandler(MouseThereminDlg::OnMouseEnter), NULL, this );
    _pnlDisplay->Connect(ID_PANEL, wxEVT_PAINT, wxPaintEventHandler(MouseThereminDlg::OnPaintThereminWindow), NULL, this );

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer9, 0, wxGROW, 0);

    wxStaticText* itemStaticText101 = new wxStaticText( itemDialog1, wxID_STATIC, _("Waveform"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemStaticText101, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxString waveforms[] = {
        _("Sine"),
        _("Square"),
        _("Saw"),
        _("Triangle"),
        _("Sinc"),
        _("Pulse 25%")
    };
    _waveform = new wxChoice( itemDialog1, ID_WAVEFORM, wxDefaultPosition, wxSize(90, -1), 6, waveforms, 0 );
    _waveform->SetStringSelection(_("Sine"));
    itemBoxSizer9->Add(_waveform, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText105 = new wxStaticText( itemDialog1, wxID_STATIC, _("Min Freq"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemStaticText105, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	_spnMinFreq = new wxSpinCtrl( itemDialog1, ID_SPIN_MINFREQ, _("55"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, MIN_PITCH, MAX_PITCH, MIN_PITCH );
    itemBoxSizer9->Add(_spnMinFreq, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText106 = new wxStaticText( itemDialog1, wxID_STATIC, _("Max Freq"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemStaticText106, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	_spnMaxFreq = new wxSpinCtrl( itemDialog1, ID_SPIN_MAXFREQ, _("880"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, MIN_PITCH, MAX_PITCH, MAX_PITCH );
    itemBoxSizer9->Add(_spnMaxFreq, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    _chkLogPitch = new wxCheckBox( itemDialog1, ID_CHECK_LOGPITCH, _("Log Pitch") );
	_chkLogPitch->SetValue(true);
    itemBoxSizer9->Add(_chkLogPitch, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2 );

    _chkLogVolume = new wxCheckBox( itemDialog1, ID_CHECK_LOGVOLUME, _("Log Volume") );
    itemBoxSizer9->Add(_chkLogVolume, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2 );

	_btnStartStop = new wxButton( itemDialog1, ID_BUTTON_STARTSTOP, _("Start"));
	itemBoxSizer9->Add(_btnStartStop, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

	_bottomRowSizer = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(_bottomRowSizer, 0, wxGROW, 0);

    wxStaticText* itemStaticText102 = new wxStaticText( itemDialog1, wxID_STATIC, _("Vibrato Wave"), wxDefaultPosition, wxDefaultSize, 0 );
    _bottomRowSizer->Add(itemStaticText102, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxString waveforms2[] = {
        _("Sine"),
        _("Square"),
        _("Saw"),
        _("Triangle"),
        _("Sinc"),
        _("Pulse 25%")
    };
    _vibratoWaveform = new wxChoice( itemDialog1, ID_VIBRATO_WAVEFORM, wxDefaultPosition, wxSize(90, -1), 6, waveforms2, 0 );
    _vibratoWaveform->SetStringSelection(_("Sine"));
    _bottomRowSizer->Add(_vibratoWaveform, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText107 = new wxStaticText( itemDialog1, wxID_STATIC, _("Vibrato Depth"), wxDefaultPosition, wxDefaultSize, 0 );
    _bottomRowSizer->Add(itemStaticText107, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    _spnVibratoDepth = new wxSpinCtrl( itemDialog1, ID_SPIN_VIBRATODEPTH, _("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
    _bottomRowSizer->Add(_spnVibratoDepth, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2 );

    wxStaticText* itemStaticText108 = new wxStaticText( itemDialog1, wxID_STATIC, _("Vibrato Freq"), wxDefaultPosition, wxDefaultSize, 0 );
    _bottomRowSizer->Add(itemStaticText108, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	_vibratoFreq = new wxSlider( itemDialog1, ID_SLIDER_VIBRATOFREQ, 10, 1, 120, wxDefaultPosition, wxSize(160,22), wxSL_HORIZONTAL );
    _bottomRowSizer->Add(_vibratoFreq, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2 );

    _vibratoText = new wxStaticText( itemDialog1, wxID_STATIC, _("1.0"), wxDefaultPosition, wxSize(42,16), 0 );
    _bottomRowSizer->Add(_vibratoText, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    _chkModulation = new wxCheckBox( itemDialog1, ID_CHECK_MODULATION, _("Modulation") );
    _bottomRowSizer->Add(_chkModulation, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2 );

	wxBitmap infoBitmap(info_xpm, wxBITMAP_TYPE_XPM );
	_btnAbout = new wxBitmapButton( itemDialog1, ID_ABOUT, infoBitmap, wxDefaultPosition, wxSize( 24, 24 ) );
	_bottomRowSizer->Add(_btnAbout, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
}

/*!
 * Should we show tooltips?
 */
bool MouseThereminDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */
wxBitmap MouseThereminDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
}

/*!
 * Get icon resources
 */
wxIcon MouseThereminDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
}

/*!
 * wxEVT_MOUSEWHEEL event handler for ID_PANEL
 */
void MouseThereminDlg::OnMouseWheel( wxMouseEvent& event )
{
	// Use the mousewheel to implement a zoom in/zoom out

	// when we are zooming in, decrease the displayed frequency range.
	// when we are zooming out, increase it.

    event.Skip(false);
}


/**
 @brief  EVT_QUIT handler.
*/
void MouseThereminDlg::OnQuit(wxCloseEvent&)
{
	Destroy();
}

void MouseThereminDlg::OnWaveform(wxCommandEvent &event)
{
	_wave = _waveform->GetSelection();
	event.Skip(false);
}

void MouseThereminDlg::OnVibratoWaveform(wxCommandEvent &event)
{
	_vibratoWave = _vibratoWaveform->GetSelection();
	event.Skip(false);
}

void MouseThereminDlg::OnButtonStartStop(wxCommandEvent &event)
{
	PaError value;
	_started = !_started;
	if( !_started )
	{
            value = Pa_AbortStream(_buffer);
            value = Pa_CloseStream(_buffer);
            _buffer = NULL;
			_btnStartStop->SetLabel(_("Start"));
	}
	else
	{
            value = Pa_OpenDefaultStream( &_buffer, 0, 2, paFloat32, _sampleRate, BUFFERLENGTH, AudioCallback, this );
            if( value != 0 )
            {
                wxMessageBox(wxString::FromAscii(Pa_GetErrorText(value)));
                return;
            }
            value = Pa_StartStream( _buffer );
			_btnStartStop->SetLabel(_("Stop"));
	}
	event.Skip(false);
}

// Turn noise back on when entering window if button is set to on.
void MouseThereminDlg::OnMouseEnter( wxMouseEvent& event )
{
	//PaError value;
	//if( _started )
	//{
 //           value = Pa_OpenDefaultStream( &_buffer, 0, 2, paFloat32, _sampleRate, BUFFERLENGTH, AudioCallback, this );
 //           if( value != 0 )
 //           {
 //               wxMessageBox(wxString::FromAscii(Pa_GetErrorText(value)));
 //               return;
 //           }
 //           value = Pa_StartStream( _buffer );
	//}
	event.Skip(false);
}

// Turn off noise when leaving window.
void MouseThereminDlg::OnMouseLeave( wxMouseEvent& )
{
	if( _started )
	{
            //value = Pa_AbortStream(_buffer);
            //value = Pa_CloseStream(_buffer);
            //_buffer = NULL;
	}
}

void MouseThereminDlg::OnMouseMove(wxMouseEvent& event)
{
	wxPoint point = event.GetPosition();
	wxSize size = _pnlDisplay->GetSize();
	_volume = 1.0 - ((double)point.y / (double)size.GetHeight());
    // Since it is 0 to 1, squaring the value will give us a log scale.
    if( _logVolume )
    {
        _volume = _volume * _volume;
    }
    // Since it is 0 to 1, squaring the value will give us a log scale.
	_pitch = (double)point.x / (double)size.GetWidth();
    if( _logPitch )
    {
        _pitch = _pitch * _pitch;
    }
	event.Skip();
}

int AudioCallback( const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData )
{
	MouseThereminDlg* dlg = (MouseThereminDlg*)userData;
	if( dlg != NULL )
	{
		dlg->RenderAudio(input, output, frameCount, timeInfo, statusFlags);
	}
	return 0;
}

void MouseThereminDlg::RenderAudio( const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags)
{
	float* buffer = (float*) output;
	memset(output, 0, (sizeof(float) * frameCount * 2));

	static int mismatches = 0;
	static int matches = 0;
	// Fill the buffer based on our waveform and frequency.
	// We should add 10 msec of data per tick.
	//
	// We may need to tinker with data size and the timeBeginPeriod()
	// and timeEndPeriod() functions from winmm.lib.
	double samplesPerPeriod = _sampleRate / ((_pitch * (_maxFreq-_minFreq))+_minFreq);
	// Get sampleSkip expressed as a percentage.
	double sampleSkip = 1.0 / samplesPerPeriod;
    double vibratoPerPeriod = _sampleRate / (_vibratoPitch * _vibratoFreq->GetMax());
    double vibratoSkip = 1.0 / vibratoPerPeriod;
	// Fill the buffer with samples.
	int count;
    float tmpval;
	for( count = 0; count < frameCount; count++ )
	{
		while( _phase > 1.0 )
		{
			_phase -= 1.0;
		}
		while( _vibratoPhase > 1.0 )
		{
			_vibratoPhase -= 1.0;
		}
		// We use 75% volume level as standard so changing vibrato level doesn't decrease volume.
		// This means that excessive vibrato could cause clipping.
		tmpval = ((_waveTable->_waveformTable[_wave][(int)(_phase * TABLESIZE)] * 0.75) +
					(_waveTable->_waveformTable[_vibratoWave][(int)(_vibratoPhase * TABLESIZE)] * _vibratoDepth ));
		buffer[count * 2] = (tmpval * _volume);
		buffer[count * 2 + 1] = buffer[count * 2];
		_phase += sampleSkip;
		_vibratoPhase += vibratoSkip;
	}
	return;
}

void MouseThereminDlg::OnVibratoSlider(wxCommandEvent &event )
{
    _vibratoPitch = ((double)_vibratoFreq->GetValue() / (double)_vibratoFreq->GetMax());
    _vibratoText->SetLabel( wxString::Format( _("%4.1f"), ((double)_vibratoFreq->GetValue() / 10.0) ));
    event.Skip(false);
}

void MouseThereminDlg::OnMinFreqT(wxCommandEvent& event)
{
    _minFreq = _spnMinFreq->GetValue();
    if( _maxFreq < _minFreq )
    {
        _maxFreq = _minFreq;
    }
    event.Skip(false);
}

void MouseThereminDlg::OnMinFreq(wxSpinEvent& event)
{
    _minFreq = _spnMinFreq->GetValue();
    if( _maxFreq < _minFreq )
    {
        _maxFreq = _minFreq;
    }
    event.Skip(false);
}

void MouseThereminDlg::OnMaxFreqT(wxCommandEvent& event)
{
    _maxFreq = _spnMaxFreq->GetValue();
    if( _maxFreq < _minFreq )
    {
        _minFreq = _maxFreq;
    }
    event.Skip(false);
}

void MouseThereminDlg::OnMaxFreq(wxSpinEvent& event)
{
    _maxFreq = _spnMaxFreq->GetValue();
    if( _maxFreq < _minFreq )
    {
        _minFreq = _maxFreq;
    }
    event.Skip(false);
}

void MouseThereminDlg::OnVibratoDepth(wxSpinEvent& event)
{
    _vibratoDepth = ((double)_spnVibratoDepth->GetValue() / 100.0);
    event.Skip(false);
}

void MouseThereminDlg::OnVibratoDepthT(wxCommandEvent& event)
{
    _vibratoDepth = ((double)_spnVibratoDepth->GetValue() / 100.0);
    event.Skip(false);
}

void MouseThereminDlg::OnLogPitch(wxCommandEvent& event)
{
    _logPitch = _chkLogPitch->GetValue();
    event.Skip(false);
}

void MouseThereminDlg::OnLogVolume(wxCommandEvent& event)
{
    _logVolume = _chkLogVolume->GetValue();
    event.Skip(false);
}

void MouseThereminDlg::OnPaintThereminWindow(wxPaintEvent& event)
{
    wxPaintDC dc(_pnlDisplay);
    // Shifts the device origin so we don't have to worry
    // about the current scroll position ourselves
    PrepareDC(dc);
    // Paint the background
    //PaintBackground(dc);
    if( _bitmap.Ok() )
    {
		dc.DrawBitmap(_bitmap, 0, 0);
    }
    dc.FloodFill(0, 0, *wxBLUE, wxFLOOD_SURFACE );
    event.Skip(false);
}

void MouseThereminDlg::OnSize(wxSizeEvent &event)
{
	wxDialog::OnSize(event);
	if( _pnlDisplay != NULL )
	{
		wxSize size = _pnlDisplay->GetSize();
		_bitmap = wxBitmap(_image.Scale(size.GetWidth(), size.GetHeight() ));
		Refresh();
		int screenWidth = size.GetWidth();
		// Initial size: wxSize(160,22)
		//wxMessageBox(wxString::Format(_("%d"), screenWidth));
		int sliderWidth = screenWidth - 819;
		if( sliderWidth < 160 )
		{
			sliderWidth = 160;
		}
		_vibratoFreq->SetSize(sliderWidth, 22);
		if( _bottomRowSizer != NULL )
		{
			_bottomRowSizer->SetItemMinSize(_vibratoFreq, wxSize(sliderWidth, 22));
			_bottomRowSizer->Layout();
			//_bottomRowSizer->Fit();
		}
	}
	else
	{
		//wxMessageBox::Show(_("Something's broken."));
	}
	event.Skip(false);
}

void MouseThereminDlg::OnModulation(wxCommandEvent&)
{
	if( _chkModulation->GetValue() )
	{
		_vibratoFreq->SetRange( 1, (MAX_PITCH * 10));
	}
	else
	{
		if( _vibratoFreq->GetValue() > 120 )
		{
			_vibratoFreq->SetValue(120);
		}
		_vibratoFreq->SetRange( 1, 120 );
	}
	_vibratoText->SetLabel( wxString::Format( _("%4.1f"), ((double)_vibratoFreq->GetValue() / 10.0) ));
}

void MouseThereminDlg::OnAbout( wxCommandEvent& event )
{
	// Show about box.
    wxAboutDialogInfo info;
    info.SetName(_("SpaceTheremin"));
    info.SetVersion(_("1.1"));
    info.SetCopyright(_("(c) 2007-2016 Zeta Centauri"));
	info.AddDeveloper(_("Jason Champion"));
	info.SetIcon(_icon);
	info.SetLicense(_("SpaceTheremin is free software and may be distributed freely.\nBackground image courtesy of NASA's hubblesite.org."));
	info.SetWebSite(_("http://zetacentauri.com"));
	info.SetDescription(_("SpaceTheremin uses the wxWidgets and PortAudio libraries."));

    wxAboutBox(info);
    event.Skip(false);
}

void MouseThereminDlg::OnClose( wxCloseEvent& event )
{
	Destroy();
	event.Skip(false);
}

