# PitchDetactor
Pitch detector using bit-stream autocorrelation

## Rationale
A VCL project for accurately measuring the frequency of notes consisting of signals with complex/problematic frequency spectrum.

## Getting Started

### Prerequisites / Dependecies

It depends on Ethea's TSVGIconImageList component which you can download with GetIt or from this link:

https://github.com/EtheaDev/SVGIconImageList

Also, it strongly depends on 'q' library from https://github.com/cycfi/q. Since C++ Builder does not support concepts, it necessary to use the 440c297b8fc784f65536f00afbe64d8a7bce4a1d checkout of 'q' which works with C++17.  



git clone --recurse-submodules

git submodule add https://github.com/cycfi/q.git

git submodule update --init --recursive

git update-index --cacheinfo 160000,440c297b8fc784f65536f00afbe64d8a7bce4a1d,q
