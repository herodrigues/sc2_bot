cmake_minimum_required(VERSION 3.10)

project(Basileus)

# Basileus source files
file(GLOB BASILEUS_SOURCES "*.cpp" "*.h")

include_directories(SYSTEM
    ${PROJECT_SOURCE_DIR}/s2client-api/include
    ${PROJECT_SOURCE_DIR}/s2client-api/contrib/protobuf/src
    ${PROJECT_BINARY_DIR}/s2client-api/generated
)

link_directories(${PROJECT_BINARY_DIR}/s2client-api/bin)

# Create the executable.
add_executable(${PROJECT_NAME} ${BASILEUS_SOURCES})
target_link_libraries(${PROOJECT_NAME}
    sc2api sc2lib sc2utils sc2protocol civetweb protobuf
)

# Linux specific.
if (UNIX AND NOT APPLE)
	target_link_libraries(${PROJECT_NAME} pthread dl)
endif ()
