//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <anafestica/FileVersionInfo.h>

#include "FormMain.h"
#include "DataModTCP.h"
#include "Globals.h"
#include "FormTestInput.h"
#include "FormSettings.h"

using Anafestica::TConfigNode;

using PitchDet::LogObj;
using PitchDet::WaveInCO;

using std::make_unique;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SVGIconImageCollection"
#pragma link "FrameReLog"
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------

namespace ValidCtrl {
static void Test() {
    new ::TfrmMain( nullptr );
}
}

//---------------------------------------------------------------------------

__fastcall TfrmMain::TfrmMain(TComponent* Owner)
    : TfrmMain( Owner, StoreOpts::All, nullptr )
{
}
//---------------------------------------------------------------------------

__fastcall TfrmMain::TfrmMain( TComponent* Owner, StoreOpts StoreOptions,
                               Anafestica::TConfigNode* const RootNode )
    : TMIfrmMain( Owner, StoreOptions, RootNode )
    , log_{
        [this]( auto Txt, auto Color ) {
          frmeReLog1->LogMessage( Txt, Color );
        }
      }
{
    LogObj = log_;
    SetupCaption();
    RestoreProperties();
}
//---------------------------------------------------------------------------

__fastcall TfrmMain::~TfrmMain()
{
    try {
        SaveProperties();
    }
    catch ( ... ) {
    }
}
//---------------------------------------------------------------------------

String TfrmMain::GetModuleFileName()
{
    return GetModuleName( reinterpret_cast<unsigned>( HInstance ) );
}
//---------------------------------------------------------------------------

void TfrmMain::SetupCaption()
{
    Anafestica::TFileVersionInfo const Info( GetModuleFileName() );
    Caption =
        Format(
            _D( "%s, Ver %s" ),
            Application->Title,
            Info.ProductVersion
        );
}
//---------------------------------------------------------------------------

auto& TfrmMain::GetCfgServerNode()
{
    return GetConfigRootNode().GetSubNode( _D( "Server" ) );
}
//---------------------------------------------------------------------------

void TfrmMain::RestoreProperties()
{
    auto& CfgServerNode = GetCfgServerNode();
    RESTORE_PROPERTY( CfgServerNode, ServerPort );

    RESTORE_LOCAL_PROPERTY( StartMinimized );
}
//---------------------------------------------------------------------------

void TfrmMain::SaveProperties() const
{
    auto& CfgServerNode = GetCfgServerNode();
    SAVE_PROPERTY( CfgServerNode, ServerPort );

    SAVE_LOCAL_PROPERTY( StartMinimized );
}
//---------------------------------------------------------------------------

void TfrmMain::DoLogMessage( String Text, TColor Color )
{
    log_.LogMessage( Text, Color );
}
//---------------------------------------------------------------------------

void TfrmMain::DoClearLog()
{
    frmeReLog1->Clear();
}
//---------------------------------------------------------------------------

PitchDet::ICmdSource& TfrmMain::GetCmdSource()
{
    return *dmTCP;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actLogClearExecute(TObject *Sender)
{
    frmeReLog1->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actServerStartExecute(TObject *Sender)
{
    auto& Server = GetCmdSource();
    Server.SetPort( ServerPort );
    Server.Start();
    LogMessage( Format( _D( "Server listening on port %d\r\n" ), ServerPort ) );
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actServerStartUpdate(TObject *Sender)
{
    TAction& Act = static_cast<TAction&>( *Sender );
    Act.Enabled = !GetCmdSource().IsRunning();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actServerStopExecute(TObject *Sender)
{
    GetCmdSource().Stop();
    LogMessage( Format( _D( "Server Stopped\r\n" ), ServerPort ) );
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actServerStopUpdate(TObject *Sender)
{
    TAction& Act = static_cast<TAction&>( *Sender );
    Act.Enabled = GetCmdSource().IsRunning();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actTestInputExecute(TObject *Sender)
{
    auto Form = make_unique<TfrmTestInput>( nullptr );
    Form->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ApplicationEvents1Hint(TObject *Sender)
{
    StatusBar1->SimpleText = Application->Hint;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::ApplicationEvents1Minimize(TObject *Sender)
{
    WindowState = wsMinimized;
    TrayIcon1->Visible = true;
    Hide();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::TrayIcon1DblClick(TObject *Sender)
{
    TrayIcon1->Visible = false;
    Show();
    WindowState = oldWindowState_;
    Application->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::WndProc( Winapi::Messages::TMessage &Message )
{
    if ( Message.Msg == WM_SYSCOMMAND && Message.WParam == SC_MINIMIZE ) {
        oldWindowState_ = WindowState;
    }
    TMIfrmMain::WndProc( Message );
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
    if ( StartMinimized ) {
        WindowState = wsMinimized;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::actFileSettingsExecute(TObject *Sender)
{
    auto Form = make_unique<TfrmSettings>( nullptr );
    Form->GeneralStartMinimized = StartMinimized;
    Form->ServerPort = ServerPort;
    if ( Form->ShowModal() == mrOk ) {
        ServerPort = Form->ServerPort;
        StartMinimized = Form->GeneralStartMinimized;
    }
}
//---------------------------------------------------------------------------

