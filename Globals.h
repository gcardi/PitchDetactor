//---------------------------------------------------------------------------

#ifndef GlobalsH
#define GlobalsH

#include <functional>

#include "ILog.h"

class TdmMain;

namespace PitchDet {

class NullLogger : public ILog {
protected:
    virtual void DoLogMessage( String Text, TColor Color ) override {}
    virtual void DoClearLog() override {}
};

extern NullLogger NilLog;

extern std::reference_wrapper<ILog> LogObj;
extern TdmMain* DmMain;

inline void LogMessage( String Text, TColor Color = clWindowText ) {
    LogObj.get().LogMessage( Text, Color );
}

template <TColor Color=clWindowText, typename... Args>
inline void LogMessage( String fmt, const Args&... args )
{
    System::TVarRec arg_list[] = { args... };
    LogMessage(
        Format( System::String( fmt ), arg_list, sizeof...( Args ) - 1 ),
        Color
    );
}

inline void LogMessage( String FormatText, TVarRec const *Args, int Args_High,
                        TColor Color = clWindowText )
{
    LogMessage( Format( FormatText, Args, Args_High ), Color );
}

inline void LogException( Exception const & E ) {
    LogObj.get().LogException( E );
}

inline void LogException( std::exception const & e ) {
    LogObj.get().LogException( e );
}

inline void LogUnknownException() {
    LogObj.get().LogUnknownException();
}

}

//---------------------------------------------------------------------------
#endif
