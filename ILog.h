//---------------------------------------------------------------------------

#ifndef ILogH
#define ILogH

#include <System.hpp>  // <-- for IUnknown
#include <Vcl.Graphics.hpp>  // <-- TColor

namespace PitchDet {

class ILog : public IUnknown {
public:
    // These three stupid methods are needed to get rid of even more stupid
    // warnings from the Embarcadero clang compilers in **NON COM** Interfaces
    //
    // [bcc(32c|64) Warning]: Interface 'I????' does not derive
    // from IUnknown. (Interfaces should derive from IUnknown)
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject) override { return {}; }
    virtual ULONG   STDMETHODCALLTYPE AddRef() override { return {}; }
    virtual ULONG   STDMETHODCALLTYPE Release() override { return {}; }

    void LogMessage( String Text, TColor Color = clWindowText ) {
        DoLogMessage( Text, Color );
    }

    void LogMessage( String FormatText, TVarRec const *Args, int Args_High,
                     TColor Color = clWindowText )
    {
        LogMessage( Format( FormatText, Args, Args_High ), Color );
    }

    void LogException( Exception const & E ) {
        LogMessage( _T( "Exception: %s\n" ), ARRAYOFCONST( ( E.Message ) ), clRed );
    }

    void LogException( std::exception const & e ) {
        LogMessage( _T( "std::exception: %s\n" ), ARRAYOFCONST( ( e.what() ) ), clRed );
    }

    void LogUnknownException() {
        LogMessage( _T( "Unknown exception\n" ), clRed );
    }

    void ClearLog() { DoClearLog(); }

    //virtual ~ILog() = default;

protected:
    virtual void DoLogMessage( String Text, TColor Color ) = 0;
    virtual void DoClearLog() = 0;
};

}

//---------------------------------------------------------------------------
#endif
