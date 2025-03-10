#library sources
set(SRCS
Oscil.cpp 
Phasor.cpp
TableRead.cpp
FourierTable.cpp
SampleTable.cpp
Pan.cpp
Envel.cpp
EnvelTable.cpp
Iir.cpp
Balance.cpp
Delay.cpp
SoundIn.cpp
fft.cpp
Stft.cpp
SoundOut.cpp
Tap.cpp
Note.cpp
MidiIn.cpp
PConv.cpp
Circular.cpp
)

add_library(AuLib-static ${SRCS})

if(PORTAUDIO AND PORTMIDI AND LIBSNDFILE)
message(STATUS "building dynamic lib")
add_library(AuLib SHARED ${SRCS})
set(LIBS ${LIBSNDFILE} ${PORTAUDIO} ${PORTMIDI} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(AuLib ${LIBS})
else()
message(STATUS "dependencies not found: not building dynamic lib")
endif()

if(CLANG_FORMAT)
 message(STATUS "formatting source files")
 add_custom_target(format ${CLANG_FORMAT}  -i
 ${PROJECT_SOURCE_DIR}/src/*.h ${PROJECT_SOURCE_DIR}/src/*.cpp
 ${PROJECT_SOURCE_DIR}/examples/*.cpp  )
 else()
message(STATUS "clang-format not found: not formatting source files")
endif()

# installing headers
FILE(GLOB files "${PROJECT_SOURCE_DIR}/src/*.h")
INSTALL(FILES ${files} DESTINATION include/AuLib)

# installing library
INSTALL(TARGETS AuLib AuLib-static
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
