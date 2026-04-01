cmake_minimum_required(VERSION 3.5)

# Setup the project.
# Its name is defined to be the controller directory name.
get_filename_component(PROJECT ${CMAKE_SOURCE_DIR} NAME)
project(${PROJECT})

add_subdirectory(arduino_src/)

# Get C or C++ sources in the current directory (only).
file(GLOB C_SOURCES *.c)
file(GLOB CPP_SOURCES *.cpp)
set(SOURCES ${C_SOURCES} ${CPP_SOURCES})

# Set the  Webots home path (change it according to your installation method)
set(WEBOTS_HOME "/usr/local/webots")

find_path(SIMAVR_INCLUDE_DIR
        NAMES simavr/sim_avr.h
        PATHS /usr/local/include
)

# 3. Find the compiled libraries (.so or .a files)
find_library(SIMAVR_LIB NAMES simavr PATHS /usr/local/lib)
find_library(SIMAVRPARTS_LIB NAMES simavrparts PATHS /usr/local/lib)

# 4. Find libelf (Crucial dependency for simavr)
find_library(ELF_LIB NAMES elf)

# Link with the Webots controller library.
link_directories(${WEBOTS_HOME}/lib/controller)
set (LIBRARIES m ${CMAKE_SHARED_LIBRARY_PREFIX}Controller${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_SHARED_LIBRARY_PREFIX}CppController${CMAKE_SHARED_LIBRARY_SUFFIX})
include_directories(${WEBOTS_HOME}/include/controller/c ${WEBOTS_HOME}/include/controller/cpp ${SIMAVR_INCLUDE_DIR})

# Setup the target executable.
add_executable(${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} ${LIBRARIES} ${SIMAVR_LIB}
        ${SIMAVRPARTS_LIB}
        ${ELF_LIB})

# Copy the target executable at the right location.
add_custom_command(TARGET ${PROJECT} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
  copy ${CMAKE_BINARY_DIR}/${PROJECT} ${CMAKE_SOURCE_DIR}
)
#[[

# SimAVR test executable setup
add_executable(simavr_test tests/simavr_test.c)
find_path(SIMAVR_INCLUDE_DIR
        NAMES simavr/sim_avr.h
        PATHS /usr/local/include
)

# 3. Find the compiled libraries (.so or .a files)
find_library(SIMAVR_LIB NAMES simavr PATHS /usr/local/lib)
find_library(SIMAVRPARTS_LIB NAMES simavrparts PATHS /usr/local/lib)

# 4. Find libelf (Crucial dependency for simavr)
find_library(ELF_LIB NAMES elf)

# 5. Tell your executable where the headers are
target_include_directories(simavr_test PRIVATE ${SIMAVR_INCLUDE_DIR})

# 6. Link all the libraries together
target_link_libraries(simavr_test PRIVATE
        ${SIMAVR_LIB}
        ${SIMAVRPARTS_LIB}
        ${ELF_LIB}
)
]]
