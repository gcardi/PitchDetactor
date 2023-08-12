//---------------------------------------------------------------------------

#ifndef DataModTCPH
#define DataModTCPH

//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdContext.hpp>
#include <IdCustomTCPServer.hpp>
#include <IdTCPServer.hpp>
#include <IdCmdTCPServer.hpp>
#include <System.SysUtils.hpp>

#include "ICmdSource.h"

//---------------------------------------------------------------------------

class TdmTCP : public TDataModule, public PitchDet::ICmdSource
{
__published:	// IDE-managed Components
    TIdCmdTCPServer *IdCmdTCPServer1;
    void __fastcall IdCmdTCPServer1Connect(TIdContext *AContext);
    void __fastcall IdCmdTCPServer1Disconnect(TIdContext *AContext);
    void __fastcall IdCmdTCPServer1Exception(TIdContext *AContext, Exception *AException);
    void __fastcall IdCmdTCPServer1CommandHandlers1Command(TIdCommand *ASender);
    void __fastcall IdCmdTCPServer1CommandHandlers2Command(TIdCommand *ASender);
    void __fastcall IdCmdTCPServer1CommandHandlers0Command(TIdCommand *ASender);
private:	// User declarations
protected:
    virtual void DoStart() override;
    virtual void DoStop() override;
    virtual bool DoIsRunning() const override;
    virtual int DoGetPort() const override;
    virtual void DoSetPort( int Val ) override;
public:		// User declarations
    __fastcall TdmTCP(TComponent* Owner) override;
};
//---------------------------------------------------------------------------
extern PACKAGE TdmTCP *dmTCP;
//---------------------------------------------------------------------------
#endif
