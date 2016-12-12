
# find the dependencies
if(APPLE)
 find_library(LIBSNDFILE libsndfile.dylib)
 find_library(PORTAUDIO libportaudio.dylib)
elseif(LINUX)
 find_library(LIBSNDFILE libsndfile.so)
 find_library(PORTAUDIO libportaudio.so) 
endif()

# example programs
add_executable(sine sine.cpp)
target_link_libraries(sine AuLib ${LIBSNDFILE} ${PORTAUDIO})
add_executable(saw saw.cpp)
target_link_libraries(saw AuLib ${LIBSNDFILE} ${PORTAUDIO})
add_executable(sampleplay sampleplay.cpp)
target_link_libraries(sampleplay AuLib ${LIBSNDFILE} ${PORTAUDIO})
add_executable(readtable readtable.cpp)
target_link_libraries(readtable AuLib ${LIBSNDFILE} ${PORTAUDIO})
add_executable(fm fm.cpp)
target_link_libraries(fm AuLib ${LIBSNDFILE} ${PORTAUDIO})
add_executable(panning panning.cpp)
target_link_libraries(panning AuLib ${LIBSNDFILE} ${PORTAUDIO})

# installation targets
INSTALL(TARGETS sampleplay saw readtable sine fm
 RUNTIME DESTINATION bin
)
