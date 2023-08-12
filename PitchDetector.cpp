//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

#if !defined( DONT_USE_SCALE_BITMAP )
# include "GdiPlusUtils.h"
#endif

//---------------------------------------------------------------------------
USEFORM("FrameLevelMeter.cpp", frmeLevelMeter); /* TFrame: File Type */
USEFORM("FormTestInput.cpp", frmTestInput);
USEFORM("FormMain.cpp", frmMain);
USEFORM("DataModTCP.cpp", dmTCP); /* TDataModule: File Type */
USEFORM("FrameReLog.cpp", frmeReLog); /* TFrame: File Type */
USEFORM("FormSettings.cpp", frmSettings);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
#if !defined( DONT_USE_SCALE_BITMAP )
    GdiplusUtils::GdiPlusSessionManager gdiplusmngr_;
#endif
    try
    {
         Application->Initialize();
         Application->MainFormOnTaskBar = true;
         Application->Title = "Server Pitch Detector";
         Application->CreateForm(__classid(TfrmMain), &frmMain);
         Application->CreateForm(__classid(TdmTCP), &dmTCP);
         Application->Run();
         while ( auto const Cnt = Screen->FormCount ) {
            delete Screen->Forms[Cnt - 1];
         }
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}
//---------------------------------------------------------------------------
