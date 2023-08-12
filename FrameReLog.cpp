//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <algorithm>

#include "FrameReLog.h"

using std::max;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

namespace ValidCtrl {
static void Test() {
    new ::TfrmeReLog( nullptr );
}
}
//---------------------------------------------------------------------------

__fastcall TfrmeReLog::TfrmeReLog(TComponent* Owner)
    : TFrame(Owner)
{
}
//---------------------------------------------------------------------------

void TfrmeReLog::SetMaxLogSize( int Val )
{
    Val = max( Val, 1024 );
    if ( Val < maxLogSize_ ) {
        RichEdit1->Lines->BeginUpdate();
        while ( RichEdit1->Text.Length() > maxLogSize_ ) {
            RichEdit1->Lines->Delete( 0 );
        }
    }
    maxLogSize_ = Val;
}
//---------------------------------------------------------------------------

void TfrmeReLog::LogMessage( String Text, TColor Color )
{
    if ( RichEdit1->Text.Length() > maxLogSize_ ) {
        RichEdit1->Lines->BeginUpdate();
        while ( RichEdit1->Text.Length() > maxLogSize_ ) {
            RichEdit1->Lines->Delete( 0 );
        }
        RichEdit1->Lines->EndUpdate();
    }

    RichEdit1->SetFocus();
    RichEdit1->SelStart = RichEdit1->GetTextLen();
    RichEdit1->SelAttributes->Color = Color;
    RichEdit1->SelText = Text;

    RichEdit1->Perform(
        EM_SCROLLCARET,
        static_cast<NativeUInt>( 0 ),
        static_cast<NativeInt>( 0 )
    );
}
//---------------------------------------------------------------------------

void TfrmeReLog::Clear()
{
    RichEdit1->Clear();
}
//---------------------------------------------------------------------------

bool TfrmeReLog::IsEmpty() const
{
    TGetTextLengthEx TextLen;
    TextLen.flags = GTL_NUMCHARS;
    TextLen.codepage = 1200;  // Unicode
    return !RichEdit1->Perform(
        EM_GETTEXTLENGTHEX,
        reinterpret_cast<NativeUInt>( &TextLen ),
        static_cast<NativeInt>( 0 )
    );
}
//---------------------------------------------------------------------------

