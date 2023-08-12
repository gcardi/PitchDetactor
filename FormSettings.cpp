//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <algorithm>

#include "FormSettings.h"

using std::max;
using std::min;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmSettings *frmSettings;
//---------------------------------------------------------------------------

__fastcall TfrmSettings::TfrmSettings(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

bool TfrmSettings::GetGeneralStartMinimized() const
{
    return checkboxStartMinimized->Checked;
}
//---------------------------------------------------------------------------

void TfrmSettings::SetGeneralStartMinimized( bool Val )
{
    checkboxStartMinimized->Checked = Val;
}
//---------------------------------------------------------------------------

int TfrmSettings::GetServerPort() const
{
    return edtServerPort->Text.ToIntDef( -1 );
}
//---------------------------------------------------------------------------

void TfrmSettings::SetServerPort( int Val )
{
    edtServerPort->Text = max( 0, min( Val, 65535 ) );
}
//---------------------------------------------------------------------------

void __fastcall TfrmSettings::actCancelExecute(TObject *Sender)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSettings::actOKExecute(TObject *Sender)
{
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSettings::actOKUpdate(TObject *Sender)
{
    auto& Act = static_cast<TAction&>( *Sender );
}
//---------------------------------------------------------------------------
