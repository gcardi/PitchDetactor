//---------------------------------------------------------------------------

#ifndef UtilsH
#define UtilsH

#include <System.hpp>

#include <vector>
#include <cmath>
#include <utility>
#include <iterator>
#include <algorithm>

namespace PitchDet {

using WaveInDevsCont = std::vector<String>;

[[nodiscard]] extern WaveInDevsCont GetWaveInDevices();
extern void AppendWaveInDevices( TStrings& DeviceNames );

[[nodiscard]] inline float dBToValue( float Value )
{
    return std::pow( 10.0F, Value / 20.0F );
}
//---------------------------------------------------------------------------

[[nodiscard]] inline float ValueTodB( float Value )
{
    static constexpr float Neg300dBValue = 1e-15F;

    return 20.0F * std::log10( std::max( Value, Neg300dBValue ) );
}
//---------------------------------------------------------------------------

template<typename I, typename T>
auto DiscretizeValueOnSortedRange( I Begin, I End, T const & Value )
  -> std::pair<int,T>
{
    auto It = std::lower_bound( Begin, End, Value );
    if ( It != End ) {
        if ( It != Begin && *It != Value ) {
            --It;
        }
        return { static_cast<int>( std::distance( Begin, It ) ), *It };
    }
    else if ( It != Begin ) {
        --It;
        return { static_cast<int>( std::distance( Begin, It ) ), *It };
    }
    return { -1, Value };
}
//---------------------------------------------------------------------------

}

#endif
