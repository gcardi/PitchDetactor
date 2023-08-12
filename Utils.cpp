//---------------------------------------------------------------------------

#pragma hdrstop

#include <MMSystem.h>

#include "Utils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

namespace PitchDet {

WaveInDevsCont GetWaveInDevices()
{
    WaveInDevsCont Devs;

    /* Get the number of Digital Audio In devices in this computer */
    auto iNumDevs = ::waveInGetNumDevs();
    Devs.reserve( iNumDevs );
    WAVEINCAPS wic;

    /* Go through all of those devices, displaying their names */
    for ( decltype( iNumDevs ) i {} ; i < iNumDevs ; ++i ) {
        /* Get info about the next device */
        if ( !waveInGetDevCaps( i, &wic, sizeof wic ) ) {
            Devs.emplace_back( wic.szPname );
        }
    }
    return Devs;
}
//---------------------------------------------------------------------------

void AppendWaveInDevices( TStrings& DeviceNames )
{
    struct UpdateMngr {
        TStrings& sl_;
        UpdateMngr( TStrings& SL ) : sl_{ SL } { SL.BeginUpdate(); }
        ~UpdateMngr() { try { sl_.EndUpdate(); } catch ( ... ) {} }
    }
    Mngr { DeviceNames };

    WAVEINCAPS wic;
    /* Get the number of Digital Audio In devices in this computer */
    auto iNumDevs = ::waveInGetNumDevs();
    /* Go through all of those devices, displaying their names */
    for ( decltype( iNumDevs ) i {} ; i < iNumDevs ; ++i ) {
        /* Get info about the next device */
        if ( !waveInGetDevCaps( i, &wic, sizeof wic ) ) {
            DeviceNames.Append( wic.szPname );
        }
    }
}
//---------------------------------------------------------------------------

}