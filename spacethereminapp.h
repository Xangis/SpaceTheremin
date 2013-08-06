#ifndef _WXSPECTRUM_H_
#define _WXSPECTRUM_H_

/*!
 * Includes
 */
#include "wx/image.h"
#include "spacetheremindlg.h"

/*!
 * MouseThereminApp class declaration
 */
class MouseThereminApp: public wxApp
{
    DECLARE_CLASS( MouseThereminApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    MouseThereminApp();
    ~MouseThereminApp();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

private:
	MouseThereminDlg* _spectrumDlg;
};

DECLARE_APP(MouseThereminApp)

#endif
    // _WXSPECTRUM_H_
