# library sources
add_library(AuLib
Oscil.cpp 
Oscili.cpp
Oscilic.cpp
Phasor.cpp
TableRead.cpp
TableReadi.cpp
TableReadic.cpp
SoundOut.cpp
FourierTable.cpp
SampleTable.cpp
Pan.cpp
Envel.cpp
)



# installing headers
FILE(GLOB files "${PROJECT_SOURCE_DIR}/src/*.h")
INSTALL(FILES ${files} DESTINATION include/AuLib)

# installing library
INSTALL(TARGETS AuLib
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
