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

    _spectrumDlg = new MouseThereminDlg(NULL,true);
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

