#include "wx/wx.h"
#include "spacethereminapp.h"

/*!
 * Application instance implementation
 */
IMPLEMENT_APP( MouseThereminApp )

/*!
 * MouseThereminApp type definition
 */
IMPLEMENT_CLASS( MouseThereminApp, wxApp )

/*!
 * MouseThereminApp event table definition
 */
BEGIN_EVENT_TABLE( MouseThereminApp, wxApp )
END_EVENT_TABLE()

/*!
 * Constructor for MouseThereminApp
 */
MouseThereminApp::MouseThereminApp()
{
}

MouseThereminApp::~MouseThereminApp()
{
}

/*!
 * Initialization for MouseThereminApp
 */
bool MouseThereminApp::OnInit()
{
    // Remove the comment markers above and below this block
    // to make permanent changes to the code.
    wxInitAllImageHandlers();

#ifdef linux
    // Create the dialog.  On Linux we have a choice between openal and jack with
    // jack being the default.  On windows we have OpenAL only.
// TODO: FIXME: OpenAL is disabled for Linux because sound generation does not work right for it.
//    if( argc > 1  && (wxString(argv[1]) == wxString( "-openal" )))
//    {
//        std::cout << "-openal flag detected.  Using OpenAL for audio." << std::endl;
//        _spectrumDlg = new MouseThereminDlg(NULL,true);
//    }
//    else
//    {
//        std::cout << "No -openal flag detected.  Using JACK for audio." << std::endl;
        _spectrumDlg = new MouseThereminDlg(NULL,false);
#else
        _spectrumDlg = new MouseThereminDlg(NULL,true);
#endif
    _spectrumDlg->Show(true);

    return true;
}

/*!
 * Cleanup for MouseThereminApp
 */
int MouseThereminApp::OnExit()
{
    return wxApp::OnExit();
}

