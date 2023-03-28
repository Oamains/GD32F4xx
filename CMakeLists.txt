cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

#设置GCC工具链,
set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(ARM_TOOLCHAIN_DIR /usr/bin)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_OBJCOPY ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}objcopy)
#set(CMAKE_SIZE ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}size)

project(GD32F4xx C ASM)
message(STATUS "Start to deal with the project: ${PROJECT_NAME}")

#GCC编译flags
set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")
set(MCU_FLAGS "-DCPU_GD32F450 -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(CMAKE_C_FLAGS "${MCU_FLAGS} -W -Wall -fno-common -ffunction-sections -fdata-sections -mthumb") # 公共设置
set(CMAKE_C_FLAGS_DEBUG "-g -O2")
#set(CMAKE_C_FLAGS_RELEASE "-O3")


# Include header files
include_directories(${PROJECT_SOURCE_DIR}/Firmware/CMSIS)
include_directories(${PROJECT_SOURCE_DIR}/Firmware/CMSIS/GD/GD32F4xx/Include)
include_directories(${PROJECT_SOURCE_DIR}/Firmware/GD32F4xx_standard_peripheral/Include)
include_directories(
        ${PROJECT_SOURCE_DIR}/User
        ${PROJECT_SOURCE_DIR}/Hardware/Led
)

# Source files
file(GLOB_RECURSE DRIVER
        ${PROJECT_SOURCE_DIR}/Firmware/GD32F4xx_standard_peripheral/Source/*.c
        ${PROJECT_SOURCE_DIR}/Firmware/CMSIS/GD/GD32F4xx/Source/*.c
        )

file(GLOB_RECURSE USER
        ${PROJECT_SOURCE_DIR}/User/*.c
        ${PROJECT_BINARY_DIR}/Hardware/Led/*.c
        )

# Create library
add_library(GD32F4xx_driver STATIC ${DRIVER})
target_include_directories(GD32F4xx_driver PUBLIC ${PROJECT_SOURCE_DIR}/Firmware/CMSIS)
target_include_directories(GD32F4xx_driver PUBLIC ${PROJECT_SOURCE_DIR}/Firmware/GD32F4xx_standard_peripheral/Include)
target_include_directories(GD32F4xx_driver PUBLIC ${PROJECT_SOURCE_DIR}/Firmware/CMSIS/GD/GD32F4xx/Include)


# Create executable
set(LINKER_SCRIPT ${PROJECT_SOURCE_DIR}/Firmware/GD32F4xx_lds/gd32f450z_flash.ld)
set(CMAKE_EXE_LINKER_FLAGS
        "--specs=nano.specs -specs=nosys.specs -T${LINKER_SCRIPT} -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map,--cref -Wl,--gc-sections"
        )
add_executable(${PROJECT_NAME}.elf ${USER})
target_link_libraries(${PROJECT_NAME}.elf GD32F4xx_driver)
#set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS "-T ${PROJECT_SOURCE_DIR}/Firmware/GD32F4xx_lds/gd32f450z_flash.ld")

set(ELF_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf)
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

# display the size of the output binary after a build is finished
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD COMMAND ${CMAKE_SIZE} --format=berkeley ${PROJECT_NAME}.elf)
add_custom_target(${PROJECT_NAME}.hex ALL DEPENDS ${PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex)
add_custom_target(${PROJECT_NAME}.bin ALL DEPENDS ${PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin)