//---------------------------------------------------------------------------

#ifndef FormMainH
#define FormMainH

//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "SVGIconImageCollection.hpp"
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.BaseImageCollection.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.PlatformDefaultStyleActnCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.VirtualImageList.hpp>
#include "FrameReLog.h"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.AppEvnts.hpp>
#include <Vcl.ActnMenus.hpp>
#include <Vcl.StdActns.hpp>

#include <memory>

#include <anafestica/PersistFormVCL.h>
#include <anafestica/CfgRegistrySingleton.h>

#include "ILog.h"
#include "LogAsync.h"

#include "ICmdSource.h"

//---------------------------------------------------------------------------

using TConfigRegistryForm =
    Anafestica::TPersistFormVCL<Anafestica::TConfigRegistrySingleton>;

// This class is needed to fool the Embarcadero's IDE which doesn't like
// multiple inheritance at the level of TObject derived classes
class TMIfrmMain : public TConfigRegistryForm, public PitchDet::ILog {
public:
    template<typename...A>
    TMIfrmMain( A&&... Args ) : TConfigRegistryForm( std::forward<A>( Args )... ) {}
};

class TfrmMain : public TMIfrmMain
{
__published:	// IDE-managed Components
    TActionManager *ActionManager1;
    TActionToolBar *ActionToolBar1;
    TAction *actServerStart;
    TAction *actServerStop;
    TVirtualImageList *VirtualImageList1;
    TSVGIconImageCollection *SVGIconImageCollection1;
    TStatusBar *StatusBar1;
    TfrmeReLog *frmeReLog1;
    TPanel *Panel2;
    TPanel *Panel3;
    TAction *actTestInput;
    TAction *actLogClear;
    TApplicationEvents *ApplicationEvents1;
    TSpeedButton *SpeedButton1;
    TTrayIcon *TrayIcon1;
    TActionMainMenuBar *ActionMainMenuBar1;
    TFileExit *actFileExit;
    TAction *actFileSettings;
    void __fastcall actServerStartExecute(TObject *Sender);
    void __fastcall actServerStartUpdate(TObject *Sender);
    void __fastcall actServerStopExecute(TObject *Sender);
    void __fastcall actServerStopUpdate(TObject *Sender);
    void __fastcall actTestInputExecute(TObject *Sender);
    void __fastcall actLogClearExecute(TObject *Sender);
    void __fastcall ApplicationEvents1Hint(TObject *Sender);
    void __fastcall ApplicationEvents1Minimize(TObject *Sender);
    void __fastcall TrayIcon1DblClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall actFileSettingsExecute(TObject *Sender);
protected:
    virtual void __fastcall WndProc( Winapi::Messages::TMessage &Message ) override;
    virtual void DoLogMessage( String Text, TColor Color ) override;
    virtual void DoClearLog() override;
private:	// User declarations
    PitchDet::LogAsync log_;
    TWindowState oldWindowState_ { wsNormal };

    static String GetModuleFileName();
    void SetupCaption();
    void RestoreProperties();
    void SaveProperties() const;
    [[nodiscard]] static PitchDet::ICmdSource& GetCmdSource();
    static auto& GetCfgServerNode();

    bool StartMinimized { false };
    int ServerPort { 6060 };

public:		// User declarations
    __fastcall TfrmMain( TComponent* Owner ) override;
    __fastcall TfrmMain( TComponent* Owner, StoreOpts StoreOptions,
                         Anafestica::TConfigNode* const RootNode = nullptr );
    __fastcall ~TfrmMain();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
