cmake_minimum_required(VERSION 3.17)
set(PICO_SDK_PATH "C:/RP2040/pico-sdk")
#include(${CMAKE_CURRENT_SOURCE_DIR}/../../../hw/bsp/family_support.cmake)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

# Initialize the SDK
pico_sdk_init()

project(midi_test C CXX ASM)

## Checks this example is valid for the family and initializes the project
#family_initialize_project(midi_test ${CMAKE_CURRENT_LIST_DIR})
## Espressif has its own cmake build system
#if(FAMILY STREQUAL "espressif")
#  return()
#endif()

add_executable(midi_test)

#target_compile_definitions( midi_test PRIVATE CFG_TUSB_MCU=OPT_MCU_RP2040 )

target_link_libraries( midi_test PUBLIC
        pico_stdlib
)

get_target_property(OUT midi_test LINK_LIBRARIES)
message(STATUS ${OUT})


target_link_libraries(midi_test PUBLIC
        #tinyusb_port
        #tinyusb_common
        tinyusb_device
        tinyusb_board
        hardware_adc
        hardware_pwm
)
get_target_property(OUT midi_test LINK_LIBRARIES)
message(STATUS ${OUT})

# Example source
target_sources(midi_test PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/midi_test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/usb_descriptors.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/midi/midi_handler.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Inputs/Button/button.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Inputs/RotarySW/rotarty_switch.c
        )

# Example include
target_include_directories(midi_test PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/midi/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Inputs/Button
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Inputs/RotarySW
        )

## add tinyusb files
#target_include_directories( midi_test PUBLIC
#        ${PICO_SDK_PATH}/lib/tinyusb/src 
#        ${PICO_SDK_PATH}/lib/tinyusb/hw
#)
#
#target_sources( midi_test PUBLIC 
#        ${PICO_SDK_PATH}/lib/tinyusb/src/tusb.c
#)


pico_enable_stdio_uart(midi_test 1)
pico_enable_stdio_usb(midi_test 0)


message("${CFG_TUSB_MCU}")

pico_add_extra_outputs(midi_test)

# Configure compilation flags and libraries for the example without RTOS.
# See the corresponding function in hw/bsp/FAMILY/family.cmake for details.
#family_configure_device_example(midi_test noos)
