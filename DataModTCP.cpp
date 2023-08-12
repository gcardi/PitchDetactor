//---------------------------------------------------------------------------


#pragma hdrstop

#include <System.DateUtils.hpp>

#include <atomic>
#include <algorithm>

#include <q/fx/lowpass.hpp>

#define __GNUC__    // q_lib non supporta nativamente bcc32c|bcc64
#include <q/pitch/pitch_detector.hpp>
#undef __GNUC__

#include <anafestica/FileVersionInfo.h>

#include "DataModTCP.h"
#include "Globals.h"
#include "MusicalNotes.h"
#include "SysUt.Fmt.h"
#include "Utils.h"
#include "WaveIn.h"

using SysUt::Fmt;

using std::atomic;
using std::max;

using PitchDet::LogMessage;
using PitchDet::LogException;
using PitchDet::MidiToFreq;
using PitchDet::WaveInDevsCont;
using PitchDet::GetWaveInDevices;
using PitchDet::WaveInCO;
using PitchDet::ValueTodB;
using PitchDet::dBToValue;

using cycfi::q::frequency;
using cycfi::q::decibel;
using cycfi::q::literals::operator""_Hz;

using Anafestica::TFileVersionInfo;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "System.Classes.TPersistent"
#pragma resource "*.dfm"
TdmTCP *dmTCP;
//---------------------------------------------------------------------------

namespace ValidCtrl {
static void Test() {
    new ::TdmTCP( nullptr );
}
}
//---------------------------------------------------------------------------

__fastcall TdmTCP::TdmTCP(TComponent* Owner)
    : TDataModule(Owner)
{
    Anafestica::TFileVersionInfo const Info(
        GetModuleName( reinterpret_cast<unsigned>( HInstance ) )
    );
    IdCmdTCPServer1->Greeting->Text->Clear();
    IdCmdTCPServer1->Greeting->Text->Append(
        Format(
            _D( "%s, Ver %s" ),
            Application->Title,
            Info.ProductVersion
        )
    );
}
//---------------------------------------------------------------------------

void TdmTCP::DoStart()
{
    IdCmdTCPServer1->Active = true;
}
//---------------------------------------------------------------------------

void TdmTCP::DoStop()
{
    IdCmdTCPServer1->Active = false;
}
//---------------------------------------------------------------------------

bool TdmTCP::DoIsRunning() const
{
    return IdCmdTCPServer1->Active;
}
//---------------------------------------------------------------------------

int TdmTCP::DoGetPort() const
{
    return IdCmdTCPServer1->DefaultPort;
}
//---------------------------------------------------------------------------

void TdmTCP::DoSetPort( int Val )
{
    IdCmdTCPServer1->DefaultPort = Val;
}
//---------------------------------------------------------------------------

void __fastcall TdmTCP::IdCmdTCPServer1CommandHandlers2Command(TIdCommand *ASender)
{
    ASender->Context->Connection->Disconnect();
}
//---------------------------------------------------------------------------

void __fastcall TdmTCP::IdCmdTCPServer1Connect(TIdContext *AContext)
{
    LogMessage( _D( "Client connected\n" ), clNavy );
}
//---------------------------------------------------------------------------

void __fastcall TdmTCP::IdCmdTCPServer1Disconnect(TIdContext *AContext)
{
    LogMessage( _D( "Client disconnected\n" ), clNavy );
}
//---------------------------------------------------------------------------

void __fastcall TdmTCP::IdCmdTCPServer1Exception(TIdContext *AContext, Exception *AException)
{
    if ( dynamic_cast<EIdConnClosedGracefully*>( AException ) ) {
        return;
    }
    LogException( *AException );
}
//---------------------------------------------------------------------------

void __fastcall TdmTCP::IdCmdTCPServer1CommandHandlers1Command(TIdCommand *ASender)
{
// DETF <chn> <sps> <len> <hys> <flo> <fhi> <listen_for_ms> <amplitude_dB>
// DETF 0 24000 1024 12.0 20 10000 5000 0.0
    auto AudioDevices = GetWaveInDevices();

    auto& Params = *ASender->Params;

    TFormatSettings FS;
    FS.DecimalSeparator = _D( '.' );

    if ( Params.Count < 8 ) {
        throw Exception( _D( "Parameters must be 8. See help." ) );
    }

    auto Chn = Params[0].ToInt();
    if ( Chn < 0 || Chn >= AudioDevices.size() ) {
        throw Exception( Fmt( _D( "The device %d doesn't exists" ), Chn ) );
    }

    auto Sps = Params[1].ToInt();
    if ( Sps < 8000 || Sps > 96000 ) {
        throw Exception( _D( "Sps is out of range" ) );
    }

    auto BufLen = Params[2].ToInt();
    if ( BufLen < 512 || BufLen > 8192 ) {
        throw Exception( _D( "Buffer lenght is out of range" ) );
    }

    auto Hys = StrToFloat( Params[3], FS );
    if ( Hys < 0.0 /*|| Hys > 10.0*/ ) {
        throw Exception( _D( "Hysteresys is out of range" ) );
    }

    auto FLo = StrToFloat( Params[4], FS );
    if ( FLo <= 0.0 ) {
        throw Exception( _D( "FreqLo is too small" ) );
    }
    if ( FLo >= static_cast<float>( Sps ) / 2 ) {
        throw Exception( _D( "FreqLo is over the nyquist frequency" ) );
    }

    auto FHi = StrToFloat( Params[5], FS );
    if ( FHi >= static_cast<float>( Sps ) / 2 ) {
        throw Exception( _D( "FreqHi is over the nyquist frequency" ) );
    }

    if ( FLo >= FHi ) {
        throw Exception( _D( "FreqHi is lower than FreqLo" ) );
    }

    auto ListenFor_ms = max( 500, Params[6].ToInt() );
    if ( ListenFor_ms < 100 || ListenFor_ms > 10000 ) {
        throw Exception( _D( "ListenFor_ms is out of range" ) );
    }

    auto Amplitude_db = StrToFloat( Params[7], FS );
    if ( Amplitude_db < -40.0 || Amplitude_db > 40.0 ) {
        throw Exception( _D( "Amplitude_db is out of range" ) );
    }

    auto Amplitude = dBToValue( Amplitude_db );

    using WaveInType = WaveInCO<int16_t>;
    using PitchDetType = cycfi::q::pitch_detector;
    using IntegratorType = cycfi::q::leaky_integrator;

    atomic<float> Freq;
    atomic<size_t> ReadyCount;
    atomic<float> OutLevel;

    LogMessage(
        Fmt(
            _D( "" )
            "Executing DETF\r\n"
            "  Chn: %d (%s)\r\n"
            "  Sps: %d\r\n"
            "  Buffer length: %d bytes\r\n"
            "  Hysteresis: %.1f\r\n"
            "  Frequency range: [%.1f, %.1f]\r\n"
            "  Waiting time: %d ms\r\n"
            "  Amplitude: %.1f dB (%g)\r\n",
            Chn, AudioDevices[Chn],
            Sps,
            BufLen,
            Hys,
            FLo, FHi,
            ListenFor_ms,
            Amplitude_db, Amplitude
        )
    );

    PitchDetType Pd(
        frequency( FLo ),
        frequency( FHi ),
        Sps,
        decibel( Hys )
    );

    IntegratorType Li( 20_Hz, Sps );

    WaveInType Wi(
        Chn, 1, BufLen, Sps,
        [Amplitude,&Freq,&ReadyCount,&Pd,&Li,&OutLevel]( auto& Buffer ) {
            size_t ReadyCnt {};
            auto Len = Buffer.size();
            for ( WaveInType::BufferCont::size_type Idx {} ; Idx < Len ; ++Idx ) {
                auto Signal = Amplitude * static_cast<float>( Buffer[Idx] );
                if ( Pd( Signal ) ) {
                    ++ReadyCnt;
                }
                OutLevel =
                   Li( fabs( Signal ) / std::numeric_limits<WaveInType::SampleType>::max() );
            }
            Freq = Pd.get_frequency();
            ReadyCount = ReadyCnt;
        }
    );

    Wi.Start();

    auto StartTime = Now();
    LogMessage( _D( "Listening...\r\n" ) );
    ::Sleep( ListenFor_ms );
    Wi.Stop();

    auto Frequency = Freq.load();
    auto OutLvl = OutLevel.load();

    LogMessage( Fmt( _D( "Frequency: %g Hz\r\n" ), Frequency ) );
    LogMessage( Fmt( _D( "Output Level: %g dB (%g)\r\n" ), ValueTodB( OutLvl ), OutLvl ) );

    ASender->Reply->Text->Append( Fmt( _D( "Freq=%s" ), FloatToStr( Frequency, FS ) ) );
    ASender->Reply->Text->Append( Fmt( _D( "OutLevel=%s" ), FloatToStr( OutLvl, FS ) ) );
    ASender->Reply->Text->Append( Fmt( _D( "OutLevel_dB=%s" ), FloatToStr( ValueTodB( OutLvl ), FS ) ) );
}
//---------------------------------------------------------------------------

void __fastcall TdmTCP::IdCmdTCPServer1CommandHandlers0Command(TIdCommand *ASender)

{
    auto Devices = GetWaveInDevices();
    for ( WaveInDevsCont::size_type Idx {} ; Idx < Devices.size() ; ++Idx ) {
        ASender->Reply->Text->Append(
            Format( _D( "%d=\"%s\"" ), Idx, Devices[Idx] )
        );
    }
}
//---------------------------------------------------------------------------


