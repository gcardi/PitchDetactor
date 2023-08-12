//---------------------------------------------------------------------------

#ifndef MusicalNotesH
#define MusicalNotesH

#include <cstdint>
#include <array>

namespace PitchDet {

[[nodiscard]] extern float MidiToFreq( uint8_t Note );

class NoteDetector {
public:
    static constexpr std::array<LPCTSTR,12> Names {
        _D( "A" ),
        _D( "A#/Bb" ),
        _D( "B" ),
        _D( "C1" ),
        _D( "C#/Db" ),
        _D( "D" ),
        _D( "D#/Eb" ),
        _D( "E" ),
        _D( "F" ),
        _D( "F#/Gb" ),
        _D( "G" ),
        _D( "G#/Ab" ),
    };
private:
};

}

//---------------------------------------------------------------------------
#endif
