#library sources
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
Note.cpp
MidiIn.cpp
)

if(CLANG_FORMAT)
 message(STATUS "formatting source files")
 add_custom_target(format ${CLANG_FORMAT}  -i  ${PROJECT_SOURCE_DIR}/src/*.h ${PROJECT_SOURCE_DIR}/src/*.cpp )
 else()
message(STATUS "clang-format not found: not formatting source files")
endif()

# installing headers
FILE(GLOB files "${PROJECT_SOURCE_DIR}/src/*.h")
INSTALL(FILES ${files} DESTINATION include/AuLib)

# installing library
INSTALL(TARGETS AuLib
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
