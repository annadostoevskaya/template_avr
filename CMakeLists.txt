cmake_minimum_required(VERSION 3.5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Variables regarding the AVR chip
set(AVRDUDE_PROGTYPE    avrisp)
if (WIN32)
    set(AVRDUDE_PORT    COM3)
elseif(UNIX)
    set(AVRDUDE_PORT    /dev/ttyACM0)
endif()
set(AVRDUDE_DEVICE      t2313)
set(AVRDUDE_BAUD        19200)
set(AVRDUDE_FUSES       -U lfuse:w:0xde:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m)

set(MCU         attiny2313)
set(F_CPU       16E6)
set(UART_BAUD   9600UL)
add_definitions(-DF_CPU=${F_CPU})
add_definitions(-D__AVR_ATtiny2313__)
add_definitions(-DBAUD=${UART_BAUD})

# program names
set(AVRCPP   avr-g++)
set(AVRC     avr-gcc)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)
set(OBJDUMP  avr-objdump)
set(AVRSIZE  avr-size)
set(AVRDUDE  avrdude)

# Sets the compiler
# Needs to come before the project function
set(CMAKE_SYSTEM_NAME       Generic)
set(CMAKE_CXX_COMPILER      ${AVRCPP})
set(CMAKE_C_COMPILER        ${AVRC})
set(CMAKE_ASM_COMPILER      ${AVRC})

project (blink C CXX ASM)

# Important project paths
set(BASE_PATH    "${${PROJECT_NAME}_SOURCE_DIR}")
set(INC_PATH     "${BASE_PATH}/include")
set(SRC_PATH     "${BASE_PATH}/src")
set(LIB_DIR_PATH "${BASE_PATH}/lib")

# Files to be compiled
file(GLOB SRC_FILES "${SRC_PATH}/*.cpp"
                    "${SRC_PATH}/*.cc"
                    "${SRC_PATH}/*.c"
                    "${SRC_PATH}/*.cxx"
                    "${SRC_PATH}/*.S"
                    "${SRC_PATH}/*.s"
                    "${SRC_PATH}/*.sx"
                    "${SRC_PATH}/*.asm")

# Compiler flags
set(CSTANDARD "-std=gnu99")
set(CDEBUG    "-g -ggdb")
set(CWARN     "-Wall -Wextra -Werror -Wstrict-prototypes -Wl,--gc-sections -Wl,--relax")
set(CTUNING   "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
set(COPT      "-Os -lm -lprintf_flt")
set(CMCU      "-mmcu=${MCU}")

set(CFLAGS   "${CMCU} ${CDEBUG} ${COPT} ${CWARN} ${CSTANDARD} ${CTUNING}")
set(CXXFLAGS "${CMCU} ${CDEBUG} ${COPT} ${CTUNING}")

set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS "${CFLAGS}")

# Project setup
include_directories(${INC_PATH} ${LIB_INC_PATH})
add_executable(${PROJECT_NAME} ${SRC_FILES} ${LIB_SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

# Compiling targets
add_custom_target(strip ALL     ${AVRSTRIP} "${PROJECT_NAME}.elf" DEPENDS ${PROJECT_NAME})
add_custom_target(hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex" DEPENDS strip)
add_custom_target(eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.eeprom" DEPENDS strip)
add_custom_target(flash         ${AVRDUDE} -v -c ${AVRDUDE_PROGTYPE} -b ${AVRDUDE_BAUD} -p ${AVRDUDE_DEVICE} 
                                           -P ${AVRDUDE_PORT} -U flash:w:${PROJECT_NAME}.hex ${AVRDUDE_FUSES} DEPENDS hex)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_NAME}.hex;${PROJECT_NAME}.eeprom;${PROJECT_NAME}.lst")
