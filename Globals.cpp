//---------------------------------------------------------------------------

#pragma hdrstop

#include "Globals.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

namespace PitchDet {

NullLogger NilLog;

std::reference_wrapper<ILog> LogObj { NilLog };

TdmMain* DmMain;

}
