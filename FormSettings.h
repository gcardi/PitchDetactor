//---------------------------------------------------------------------------

#ifndef FormSettingsH
#define FormSettingsH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------

class TfrmSettings : public TForm
{
__published:	// IDE-managed Components
    TPageControl *PageControl1;
    TPanel *Panel1;
    TButton *Button1;
    TButton *Button2;
    TTabSheet *tabshtServer;
    TTabSheet *tbshtLog;
    TActionList *ActionList1;
    TAction *actOK;
    TAction *actCancel;
    TEdit *edtServerPort;
    TLabel *Label1;
    TCheckBox *checkboxStartMinimized;
    void __fastcall actCancelExecute(TObject *Sender);
    void __fastcall actOKExecute(TObject *Sender);
    void __fastcall actOKUpdate(TObject *Sender);
private:	// User declarations
    bool GetGeneralStartMinimized() const;
    void SetGeneralStartMinimized( bool Val );
    int GetServerPort() const;
    void SetServerPort( int Val );
public:		// User declarations
    __fastcall TfrmSettings(TComponent* Owner);
    __property bool GeneralStartMinimized = {
        read = GetGeneralStartMinimized, write = SetGeneralStartMinimized
    };
    __property int ServerPort = { read = GetServerPort, write = SetServerPort };
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmSettings *frmSettings;
//---------------------------------------------------------------------------
#endif
