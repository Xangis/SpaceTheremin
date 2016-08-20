#ifndef _SPECTRUMDLG_H_
#define _SPECTRUMDLG_H_

/*!
 * Includes
 */
#include "wx/wx.h"
#include "wx/spinctrl.h"
#include "wx/thread.h"
#include "portaudio.h"
#include "Wavetable.h"

/*!
 * Forward declarations
 */
class wxSpinCtrl;

/*!
 * Control identifiers
 */
#define ID_SPECTRUMDIALOG 10000
#define SYMBOL_SPECTRUMDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX|wxMAXIMIZE_BOX
#define SYMBOL_SPECTRUMDLG_TITLE _("SpaceTheremin")
#define SYMBOL_SPECTRUMDLG_IDNAME ID_SPECTRUMDIALOG
#define SYMBOL_SPECTRUMDLG_SIZE wxSize(440, 300)
#define SYMBOL_SPECTRUMDLG_POSITION wxDefaultPosition
#define ID_WAVEFORM 10001
#define ID_BUTTON_STARTSTOP 10002
#define ID_VIBRATO_WAVEFORM 10003
#define ID_PANEL 10004
#define ID_SPIN_MAXFREQ 10005
#define ID_SPIN_MINFREQ 10006
#define ID_SPIN_VIBRATODEPTH 10007
#define ID_SLIDER_VIBRATOFREQ 10008
#define ID_CHECK_LOGPITCH 10009
#define ID_CHECK_LOGVOLUME 10010
#define ID_CHECK_MODULATION 10011
#define ID_ABOUT 10012

/*!
 * Compatibility
 */
#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * MouseThereminDlg class declaration
 */
class MouseThereminDlg: public wxDialog
{
    DECLARE_DYNAMIC_CLASS( MouseThereminDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    MouseThereminDlg( );
    ~MouseThereminDlg( );
    MouseThereminDlg( wxWindow* parent, bool use_openal, wxWindowID id = SYMBOL_SPECTRUMDLG_IDNAME, const wxString& caption = SYMBOL_SPECTRUMDLG_TITLE, const wxPoint& pos = SYMBOL_SPECTRUMDLG_POSITION, const wxSize& size = SYMBOL_SPECTRUMDLG_SIZE, long style = SYMBOL_SPECTRUMDLG_STYLE );
    /// Creation
    bool Create( wxWindow* parent, bool use_openal, wxWindowID id = SYMBOL_SPECTRUMDLG_IDNAME, const wxString& caption = SYMBOL_SPECTRUMDLG_TITLE, const wxPoint& pos = SYMBOL_SPECTRUMDLG_POSITION, const wxSize& size = SYMBOL_SPECTRUMDLG_SIZE, long style = SYMBOL_SPECTRUMDLG_STYLE );
    void CreateControls();
    void OnButtonStartStop( wxCommandEvent& event );
	void OnWaveform( wxCommandEvent& event );
	void OnVibratoWaveform( wxCommandEvent& event );
    void OnVibratoSlider( wxCommandEvent& event );
    void OnMouseWheel( wxMouseEvent& event );
	void OnMouseMove( wxMouseEvent& event );
	void OnMouseLeave( wxMouseEvent& event );
	void OnMouseEnter( wxMouseEvent& event );
    void OnMinFreqT( wxCommandEvent& event );
    void OnMaxFreqT( wxCommandEvent& event );
    void OnVibratoDepth( wxSpinEvent& event );
    void OnMinFreq( wxSpinEvent& event );
    void OnMaxFreq( wxSpinEvent& event );
    void OnVibratoDepthT( wxCommandEvent& event );
    void OnQuit( wxCloseEvent &event );
    void OnLogVolume( wxCommandEvent& event );
    void OnLogPitch( wxCommandEvent& event );
    void OnPaintThereminWindow(wxPaintEvent& event);
	void OnSize( wxSizeEvent& event );
	void OnModulation( wxCommandEvent& event );
	void OnAbout(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );
	void RefreshPanel();
    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
    /// Should we show tooltips?
    static bool ShowToolTips();
	void RenderAudio( const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags);
private:
    wxButton* _btnStartStop;
    wxPanel* _pnlDisplay;
	int _wave;
    wxChoice* _waveform;
    wxChoice* _vibratoWaveform;
    wxSpinCtrl* _spnMinFreq;
    wxSpinCtrl* _spnMaxFreq;
    wxSpinCtrl* _spnVibratoDepth;
    wxSlider* _vibratoFreq;
    wxCheckBox* _chkLogPitch;
    wxCheckBox* _chkLogVolume;
	wxCheckBox* _chkModulation;
	wxStaticText* _vibratoText;
	wxBitmapButton* _btnAbout;
    bool _logPitch;
    bool _logVolume;
	double _volume; // 0 to 1
	double _pitch;  // 0 to 1
	double _phase;  // 0 to 1
	bool _started;
    int _vibratoWave;
    int _minFreq;
    int _maxFreq;
    double _vibratoDepth; // 0 to 1
    double _vibratoPitch;  // 0 to 1
    double _vibratoPhase; // 0 to 1
	wxBitmap _bitmap;
	wxImage _image;
    unsigned int _sampleRate;
    bool _useopenal;
	wxIcon _icon;
	WaveTable* _waveTable;
        PaStream *_buffer;
	wxSizer* _bottomRowSizer;
};

int AudioCallback( const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData );

#endif
    // _SPECTRUMDLG_H_
