
# find the dependencies
if(APPLE)
 message(STATUS "platform is OSX")
 find_library(LIBSNDFILE libsndfile.dylib)
 find_library(PORTAUDIO libportaudio.dylib)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
 message(STATUS "platform is linux")
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
add_executable(ringmod ringmod.cpp)
target_link_libraries(ringmod AuLib ${LIBSNDFILE} ${PORTAUDIO})
add_executable(ping ping.cpp)
target_link_libraries(ping AuLib ${LIBSNDFILE} ${PORTAUDIO})
add_executable(envelope envelope.cpp)
target_link_libraries(envelope AuLib ${LIBSNDFILE} ${PORTAUDIO})
add_executable(adsr adsrex.cpp)
target_link_libraries(adsr AuLib ${LIBSNDFILE} ${PORTAUDIO})
add_executable(glide glide.cpp)
target_link_libraries(glide AuLib ${LIBSNDFILE} ${PORTAUDIO})
add_executable(filterex filterex.cpp)
target_link_libraries(filterex AuLib ${LIBSNDFILE} ${PORTAUDIO})
add_executable(play play.cpp)
target_link_libraries(play AuLib ${LIBSNDFILE} ${PORTAUDIO})
add_executable(echoex echoex.cpp)
target_link_libraries(echoex AuLib ${LIBSNDFILE} ${PORTAUDIO})

# installation targets
INSTALL(TARGETS sampleplay saw readtable sine fm panning ringmod ping
  envelope adsr glide filterex play echoex RUNTIME DESTINATION bin
)
