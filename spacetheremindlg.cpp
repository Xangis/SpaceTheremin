#include "wx/wx.h"
#include "wx/image.h"
#include "wx/aboutdlg.h"
#include "wx/stdpaths.h"
#include "spacetheremindlg.h"
#include <math.h>
#define MIN_PITCH 20
#define MAX_PITCH 8000

#include <iostream>

#include <wx/apptrait.h>

MouseThereminDlg* dlg = NULL;
#ifdef linux
int process (jack_nframes_t nframes, void *arg) { return dlg->Process( nframes, arg ); };
void jack_shutdown (void *arg) { dlg->JackShutdown( arg ); };
#endif

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
}

MouseThereminDlg::~MouseThereminDlg()
{
#ifdef linux
    if( !_useopenal )
	{
      jack_transport_stop( _jackClient );
	  jack_client_close( _jackClient );
	}
#else
	_openal->Stop();
#endif
	this->Pause();
	// Give ourselves a few milliseconds for things to stop.
	Sleep(12);
}

void jackerror(const char *errorstr)
{
    std::cout << "JACK error: '" << errorstr << "'" << std::endl;
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
	_pnlDisplay = NULL;
	_waveTable = new WaveTable();
    _useopenal = use_openal;
    _sampleRate = 44100; // Default.  Jack can change it below if he wants to.
	// Create a new stereo buffer.
#ifdef WIN32
    if( _useopenal )
	_openal = new OpenALManager(2);
    else
        _openal = NULL;
#endif
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
#ifdef WIN32
        if( _useopenal )
	  _openal->Init(0, 0, "Generic Hardware" );
#endif
	// Above normal thread priority so we can monitor the sound buffer a little better.
	if( wxThread::Create(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR )
	{
		wxMessageBox( _("Unable to create oscillator thread."), _("ERROR"), wxOK );
		return false;
	}
	SetPriority(75);
	Run();
        dlg = this;
#ifdef linux
        if( !_useopenal )
        {
	const char **ports;
	jack_options_t options = JackNullOption;
	jack_status_t status;
	jack_set_error_function(jackerror);
        _jackClient = jack_client_open ("SpaceTheremin Client", options, &status, NULL );
	if (_jackClient == NULL) 
        {
		fprintf (stderr, "jack_client_open() failed, " 
			 "status = 0x%2.0x\n", status);
		if (status & JackServerFailed)
                {
			fprintf (stderr, "Unable to connect to JACK server.\n");
		}
		exit (1);
	}
	if (status & JackServerStarted) 
        {
		fprintf (stderr, "JACK server started.\n");
	}
	if (status & JackNameNotUnique) 
        {
                char * client_name = jack_get_client_name(_jackClient);
		fprintf (stderr, "Unique name `%s' assigned.\n", client_name);
	}
	/* tell the JACK server to call `process()' whenever
	   there is work to be done.
	*/
	jack_set_process_callback (_jackClient, process, 0);
	/* tell the JACK server to call `jack_shutdown()' if
	   it ever shuts down, either entirely, or if it
	   just decides to stop calling us.
	*/
	jack_on_shutdown (_jackClient, jack_shutdown, 0);
	/* display the current sample rate. 
	 */
        _sampleRate = jack_get_sample_rate( _jackClient );
	printf ("Engine sample rate: %d\n", _sampleRate);
	/* create two ports */
	_jackIn = jack_port_register (_jackClient, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	_jackOut = jack_port_register (_jackClient, "output", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	if ((_jackIn == NULL) || (_jackOut == NULL)) 
        {
		fprintf(stderr, "No more JACK ports available.\n");
		exit (1);
	}
	/* Tell the JACK server that we are ready to roll.  Our
	 * process() callback will start running now. */
	if (jack_activate (_jackClient)) 
        {
		fprintf (stderr, "Cannot activate client.\n");
		exit (1);
	}
        else
        {
                fprintf( stderr, "JACK client activated.\n" );
        }
	/* Connect the ports.  You can't do this before the client is
	 * activated, because we can't make connections to clients
	 * that aren't running.  Note the confusing (but necessary)
	 * orientation of the driver backend ports: playback ports are
	 * "input" to the backend, and capture ports are "output" from
	 * it.
	 */
	ports = jack_get_ports (_jackClient, NULL, NULL, JackPortIsPhysical|JackPortIsOutput);
	if (ports == NULL) 
        {
		fprintf(stderr, "No physical capture ports.\n");
		exit (1);
	}

	if (jack_connect (_jackClient, ports[0], jack_port_name (_jackIn))) 
        {
		fprintf (stderr, "Cannot connect input ports.\n");
	}

	free (ports);

	ports = jack_get_ports (_jackClient, NULL, NULL, JackPortIsPhysical|JackPortIsInput);
	if (ports == NULL) 
        {
		fprintf(stderr, "No physical playback ports.\n");
		exit (1);
	}

	if (jack_connect (_jackClient, jack_port_name (_jackOut), ports[0])) 
        {
		fprintf (stderr, "Cannot connect output ports.\n");
	}
        else
        {
		fprintf (stderr, "Output ports connected.\n");
	}
	free (ports);
        } // end if( !_useopenal )
#endif
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

	_spnMinFreq = new wxSpinCtrl( itemDialog1, ID_SPIN_MINFREQ, _("30"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, MIN_PITCH, MAX_PITCH, MIN_PITCH );
    itemBoxSizer9->Add(_spnMinFreq, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText106 = new wxStaticText( itemDialog1, wxID_STATIC, _("Max Freq"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemStaticText106, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	_spnMaxFreq = new wxSpinCtrl( itemDialog1, ID_SPIN_MAXFREQ, _("2000"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, MIN_PITCH, MAX_PITCH, MAX_PITCH );
    itemBoxSizer9->Add(_spnMaxFreq, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    _chkLogPitch = new wxCheckBox( itemDialog1, ID_CHECK_LOGPITCH, _("Log Pitch") );
	_chkLogPitch->SetValue(true);
    itemBoxSizer9->Add(_chkLogPitch, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2 );

    _chkLogVolume = new wxCheckBox( itemDialog1, ID_CHECK_LOGVOLUME, _("Log Volume") );
    itemBoxSizer9->Add(_chkLogVolume, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2 );

	_btnStartStop = new wxButton( itemDialog1, ID_BUTTON_STARTSTOP, _("Start"));
	itemBoxSizer9->Add(_btnStartStop, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

	wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer19, 0, wxGROW, 0);

    wxStaticText* itemStaticText102 = new wxStaticText( itemDialog1, wxID_STATIC, _("Vibrato Wave"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemStaticText102, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

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
    itemBoxSizer19->Add(_vibratoWaveform, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxStaticText* itemStaticText107 = new wxStaticText( itemDialog1, wxID_STATIC, _("Vibrato Depth"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemStaticText107, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    _spnVibratoDepth = new wxSpinCtrl( itemDialog1, ID_SPIN_VIBRATODEPTH, _("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
    itemBoxSizer19->Add(_spnVibratoDepth, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2 );

    wxStaticText* itemStaticText108 = new wxStaticText( itemDialog1, wxID_STATIC, _("Vibrato Freq"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemStaticText108, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	_vibratoFreq = new wxSlider( itemDialog1, ID_SLIDER_VIBRATOFREQ, 10, 1, 120, wxDefaultPosition, wxSize(160,22), wxSL_HORIZONTAL );
    itemBoxSizer19->Add(_vibratoFreq, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2 );

    _vibratoText = new wxStaticText( itemDialog1, wxID_STATIC, _("1.0"), wxDefaultPosition, wxSize(42,16), 0 );
    itemBoxSizer19->Add(_vibratoText, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    _chkModulation = new wxCheckBox( itemDialog1, ID_CHECK_MODULATION, _("Modulation") );
    itemBoxSizer19->Add(_chkModulation, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2 );

	wxBitmap infoBitmap( _("info.xpm"), wxBITMAP_TYPE_XPM );
	_btnAbout = new wxBitmapButton( itemDialog1, ID_ABOUT, infoBitmap, wxDefaultPosition, wxSize( 24, 24 ) );
	itemBoxSizer19->Add(_btnAbout, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
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
	_started = !_started;
	if( _started )
	{
#ifdef linux
                if( !_useopenal )
                jack_transport_start( _jackClient );
#else
		_openal->Play();
#endif
		_btnStartStop->SetLabel(_("Stop"));
	}
	else
	{
#ifdef linux
                if( !_useopenal )
                jack_transport_stop( _jackClient );
#else
		_openal->Stop();
#endif
		_btnStartStop->SetLabel(_("Start"));
	}
	event.Skip(false);
}

// Turn noise back on when entering window if button is set to on.
void MouseThereminDlg::OnMouseEnter( wxMouseEvent& event )
{
	if( _started )
	{
#ifdef linux
                if( !_useopenal )
                  jack_transport_start( _jackClient );
#else
		  _openal->Play();
#endif
	}
	event.Skip(false);
}

// Turn off noise when leaving window.
void MouseThereminDlg::OnMouseLeave( wxMouseEvent& )
{
	if( _started )
	{
//		_openal->Stop();
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

void* MouseThereminDlg::Entry()
{
	static int mismatches = 0;
	static int matches = 0;
	while( !TestDestroy())
	{
		// Fill the buffer based on our waveform and frequency.
		// We should add 10 msec of data per tick.
		//
		// We may need to tinker with data size and the timeBeginPeriod()
		// and timeEndPeriod() functions from winmm.lib.
#ifdef WIN32
		if( _useopenal && _openal->IsBufferPlaying(0) )
		{
			int left = _openal->GetNumSamplesQueued(0);
			int right = _openal->GetNumSamplesQueued(1);
			if( left != right )
			{
				++mismatches;
				//wxMessageBox( "Synchronization mismatch!", "Error", wxOK );
			}
			else
			{
				++matches;
			}
			if( left < 1764 )
			{
				double samplesPerPeriod = _sampleRate / ((_pitch * (_maxFreq-_minFreq))+_minFreq);
				// Get sampleSkip expressed as a percentage.
				double sampleSkip = 1.0 / samplesPerPeriod;
                                double vibratoPerPeriod = _sampleRate / (_vibratoPitch * _vibratoFreq->GetMax());
                                double vibratoSkip = 1.0 / vibratoPerPeriod;
				// Fill the buffer with samples.
				int count;
				short buffer[882];
                                float tmpval;
				memset( buffer, 0, 882 );
				for( count = 0; count < 882; count++ )
				{
					while( _phase > 1.0 )
					{
						_phase -= 1.0;
					}
                                        while( _vibratoPhase > 1.0 )
                                        {
                                           _vibratoPhase -= 1.0;
                                        }
					//buffer[count] = sineTable[(int)(_phase * TABLESIZE)];
                                        tmpval = ((_waveTable->_waveformTable[_wave][(int)(_phase * TABLESIZE)] * (1.0 - _vibratoDepth)) +
                                                  (_waveTable->_waveformTable[_vibratoWave][(int)(_vibratoPhase * TABLESIZE)] * _vibratoDepth ));
					buffer[count] = (short)(tmpval * (_volume * 32767.0));
                                        std::cout << "Sample 0 of frame = " << buffer[0] << " sample 10 = " << buffer[9] << " and sample 881 of frame = " << buffer[881] << std::endl;
					_phase += sampleSkip;
                                        _vibratoPhase += vibratoSkip;
				}
				_openal->FillBuffer(0, (unsigned char*)&buffer, 1764, _sampleRate );
				_openal->FillBuffer(1, (unsigned char*)&buffer, 1764, _sampleRate );
			}
		}
#endif
		Sleep(1);
	}
	return NULL;
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
    info.SetVersion(_("1.01"));
    info.SetCopyright(_("(c) 2007-2013 Zeta Centauri"));
	info.AddDeveloper(_("Jason Champion"));
	info.SetIcon(_icon);
	info.SetLicense(_("SpaceTheremin is free software and may be distributed freely.\nBackground image courtesy of NASA's hubblesite.org."));
	info.SetWebSite(_("http://zetacentauri.com"));
	info.SetDescription(_("SpaceTheremin uses the wxWidgets and OpenAL libraries."));

    wxAboutBox(info);
    event.Skip(false);
}

void MouseThereminDlg::OnClose( wxCloseEvent& event )
{
	Destroy();
	event.Skip(false);
}

#ifdef linux
void MouseThereminDlg::JackShutdown (void *arg)
{
	fprintf (stderr, "JACK shutdown\n");
	// exit (0);
	abort();
}

// We don't have to use if( _useopenal ) calls here because we use them
// during init.  If openal is on we will never have jack created and initialized
// and will never get to this point.
int MouseThereminDlg::Process (jack_nframes_t nframes, void *arg)
{
	jack_default_audio_sample_t *in, *out;
	jack_transport_state_t ts = jack_transport_query(_jackClient, NULL);

	if (ts == JackTransportRolling)
        {
		double samplesPerPeriod = _sampleRate / ((_pitch * (_maxFreq-_minFreq))+_minFreq);
		// Get sampleSkip expressed as a percentage.
		double sampleSkip = 1.0 / samplesPerPeriod;
                double vibratoPerPeriod = _sampleRate / (_vibratoPitch * _vibratoFreq->GetMax());
                double vibratoSkip = 1.0 / vibratoPerPeriod;
		// Fill the buffer with samples.
		int count;
		float * buffer = new float[nframes];
		memset( buffer, 0, (nframes * sizeof(float)) );
		for( count = 0; count < nframes; count++ )
		{
			while( _phase > 1.0 )
			{
				_phase -= 1.0;
			}
                        while( _vibratoPhase > 1.0 )
                        {
                                _vibratoPhase -= 1.0;
                        }
			buffer[count] = ((_waveTable->_waveformTable[_wave][(int)(_phase * TABLESIZE)] * (1.0 - _vibratoDepth)) + // Base waveform
                            (_waveTable->_waveformTable[_vibratoWave][(int)(_vibratoPhase * TABLESIZE)] * _vibratoDepth )) * // Vibrato
                             _volume;
			_phase += sampleSkip;
                        _vibratoPhase += vibratoSkip;
		}

		//in = (jack_default_audio_sample_t *)jack_port_get_buffer (_jackIn, nframes);
		out = (jack_default_audio_sample_t *)jack_port_get_buffer (_jackOut, nframes);
		memcpy (out, buffer, (sizeof (float) * nframes));
		//std::cout << "Wrote " << nframes << " frames to out buffer." << std::endl;
                delete[] buffer;
	}
        else if (ts == JackTransportStopped)
        {
                //std::cout << "Jack transport stopped, doing nothing." << std::endl;
	}

	return 0;
}
#endif
