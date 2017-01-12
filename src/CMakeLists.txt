# library sources
add_library(AuLib
Oscil.cpp 
Phasor.cpp
TableRead.cpp
FourierTable.cpp
SampleTable.cpp
Pan.cpp
Envel.cpp
EnvelTable.cpp
Iir.cpp
Tone.cpp
Balance.cpp
Delay.cpp
SoundIn.cpp
fft.cpp
Stft.cpp
SoundOut.cpp
Tap.cpp
)



# installing headers
FILE(GLOB files "${PROJECT_SOURCE_DIR}/src/*.h")
INSTALL(FILES ${files} DESTINATION include/AuLib)

# installing library
INSTALL(TARGETS AuLib
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
