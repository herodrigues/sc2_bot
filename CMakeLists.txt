# Bot source files
file(GLOB BOT_SOURCES "*.cc" ".cpp" "*.h")

include_directories(SYSTEM
	${PROJECT_SOURCE_DIR}/s2client-api/include
	${PROJECT_SOURCE_DIR}/s2client-api/contrib/protobuf/src
	${PROJECT_BINARY_DIR}/s2client-api/generated
)

link_directories(${PROJECT_BINARY_DIR}/s2client-api/bin)

# Setup MSVC parallelized builds
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)

# bot_simple
add_executable(sc2_bot ${BOT_SOURCES})
set_target_properties(sc2_bot PROPERTIES FOLDER src)
target_link_libraries(sc2_bot sc2api sc2lib sc2utils sc2renderer)

# Boost
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(sc2_bot ${Boost_LIBRARIES})
endif()

# OS X
if (APPLE)
	target_link_libraries(sc2_bot "-framework Carbon")
endif ()

# Linux
if (UNIX AND NOT APPLE)
	target_link_libraries(sc2_bot pthread dl)
endif ()
