# Basileus source files
file(GLOB BOT_SOURCES "*.cc" ".cpp" "*.h")

include_directories(SYSTEM 
	${PROJECT_SOURCE_DIR}/s2client-api/include
	${PROJECT_SOURCE_DIR}/s2client-api/contrib/protobuf/src
	${PROJECT_BINARY_DIR}/s2client-api/generated
)

link_directories(${PROJECT_BINARY_DIR}/s2client-api/bin)

# Setup MSVC parallelized builds
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)

#bot_simple
add_executable(basileus ${BOT_SOURCES})
set_target_properties(basileus PROPERTIES FOLDER src)
target_link_libraries(basileus sc2api sc2lib sc2utils sc2renderer)

if (APPLE)
	target_link_libraries(basileus "-framework Carbon")
endif ()

# Linux specific.
if (UNIX AND NOT APPLE)
	target_link_libraries(basileus pthread dl)
endif ()
