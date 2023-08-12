//---------------------------------------------------------------------------

#ifndef ICmdSourceH
#define ICmdSourceH

#include <System.hpp>  // <-- for IUnknown

namespace PitchDet {

class ICmdSource : public IUnknown {
public:
    // These three stupid methods are needed to get rid of even more stupid
    // warnings from the Embarcadero clang compilers in **NON COM** Interfaces
    //
    // [bcc(32c|64) Warning]: Interface 'I????' does not derive
    // from IUnknown. (Interfaces should derive from IUnknown)
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject) override { return {}; }
    virtual ULONG   STDMETHODCALLTYPE AddRef() override { return {}; }
    virtual ULONG   STDMETHODCALLTYPE Release() override { return {}; }

    void Start() { DoStart(); }
    void Stop() { DoStop(); }
    bool IsRunning() const { return DoIsRunning(); }
    int GetPort() const { return DoGetPort(); }
    void SetPort( int Val ) { DoSetPort( Val ); }

    // WARNING !!!
    // Uncommenting the virtual destructor causes the following linker error:
    //   [ilink32 Error] Error: Unresolved external
    //   '__fastcall _ZTh-104_TdmTCP::~_ZTh-104_TdmTCP()'
    //   referenced from DATAMODTCP.OBJ
    // Tutto ciò implica che si debba comunque costruire e distruggere
    // il datamodule usando un puntatore a TdmTCP e fare un alias con un
    // secondo puntatore a ICmdSource per mantenere il codice driver più
    // generico possibile.
    //virtual ~ICmdSource() = default;

protected:
    virtual void DoStart() = 0;
    virtual void DoStop() = 0;
    virtual bool DoIsRunning() const = 0;
    virtual int DoGetPort() const = 0;
    virtual void DoSetPort( int Val ) = 0;
};

}

//---------------------------------------------------------------------------
#endif
