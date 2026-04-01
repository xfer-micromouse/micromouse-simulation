cmake_minimum_required(VERSION 3.16)
set(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

####################################
#  Target-specific stuff
set(TARGET_CPU ATmega328)
#  End of target-specific stuff
####################################

project(arduino_test CXX C)

set(CMAKE_CXX_STANDARD 17)

include_directories(inc)

add_compile_definitions("__AVR_${TARGET_CPU}__")

add_link_options(-Wl,--print-memory-usage)

add_executable(${PROJECT_NAME} main.c)

target_compile_options(${PROJECT_NAME} PRIVATE
        -mmcu=atmega328p
        -O2
)

# 3. Add the preprocessor definition (-DF_CPU)
# Note: You drop the '-D' here, CMake handles that automatically
target_compile_definitions(${PROJECT_NAME} PRIVATE
        F_CPU=16000000UL
)