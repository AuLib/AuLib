find_library(LIBSNDFILE libsndfile.dylib)
find_library(PORTAUDIO libportaudio.dylib)

add_executable(sampleplay sampleplay.cpp)
target_link_libraries(sampleplay AuLib ${LIBSNDFILE} ${PORTAUDIO})
add_executable(saw saw.cpp)
target_link_libraries(saw AuLib ${LIBSNDFILE} ${PORTAUDIO})
add_executable(readtable readtable.cpp)
target_link_libraries(readtable AuLib ${LIBSNDFILE} ${PORTAUDIO})
