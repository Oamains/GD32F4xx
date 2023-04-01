cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER_WORKS TRUE) # 设置编译器可用
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

set(TOOLCHAIN_PREFIX arm-none-eabi-) # 设置ARM交叉编译器前缀
set(ARM_TOOLCHAIN_DIR D:\\software\\GUN-Tools-ARM-Embedded\\bin) # 设置ARM交叉编译器路径

# 设置交叉编译器
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_OBJCOPY ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}size)

# 设置项目名称和语言类型
project(GD32F4xx C ASM)
message(STATUS "Start to deal with the project: ${PROJECT_NAME}")
set(CMAKE_C_STANDARD 99)

# 设置编译类型为Debug
set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")
set(MCU_FLAGS "-DCPU_GD32F450 -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(CMAKE_C_FLAGS "${MCU_FLAGS} -W -Wall -fno-common -ffunction-sections -fdata-sections -mthumb") # 设置编译选项
set(CMAKE_C_FLAGS_DEBUG "-g -O2")
#set(CMAKE_C_FLAGS_RELEASE "-O3")

# 包含头文件
include_directories(${PROJECT_SOURCE_DIR}/Firmware/CMSIS)
include_directories(${PROJECT_SOURCE_DIR}/Firmware/CMSIS/GD/GD32F4xx/Include)
include_directories(${PROJECT_SOURCE_DIR}/Firmware/GD32F4xx_standard_peripheral/Include)
include_directories(${PROJECT_SOURCE_DIR}/Hardware/Include)
include_directories(${PROJECT_SOURCE_DIR}/User)

# 源文件
file(GLOB_RECURSE DRIVER
        ${PROJECT_SOURCE_DIR}/Firmware/GD32F4xx_standard_peripheral/Source/*.c
        ${PROJECT_SOURCE_DIR}/Firmware/CMSIS/GD/GD32F4xx/Source/*.c
        ${PROJECT_SOURCE_DIR}/Hardware/Source/*.c
        )

file(GLOB_RECURSE USER ${PROJECT_SOURCE_DIR}/User/*.c)
add_compile_definitions(GD32F450)
# 创建库
add_library(GD32F4xx_driver STATIC ${DRIVER} Hardware/Source/timer.c)
set(STARTUP ${PROJECT_SOURCE_DIR}/Startup/startup_gd32f450.S Hardware/Include/timer.h)

target_include_directories(GD32F4xx_driver PUBLIC ${PROJECT_SOURCE_DIR}/Firmware/CMSIS)
target_include_directories(GD32F4xx_driver PUBLIC ${PROJECT_SOURCE_DIR}/Firmware/GD32F4xx_standard_peripheral/Include)
target_include_directories(GD32F4xx_driver PUBLIC ${PROJECT_SOURCE_DIR}/Firmware/CMSIS/GD/GD32F4xx/Include)


# 创建可执行文件
set(LINKER_SCRIPT ${PROJECT_SOURCE_DIR}/Firmware/GD32F4xx_lds/gd32f450z_flash.ld)
set(CMAKE_EXE_LINKER_FLAGS
        "--specs=nano.specs -T ${LINKER_SCRIPT} -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map,--cref -Wl,--gc-sections"
        )
add_executable(${PROJECT_NAME}.elf ${USER} ${STARTUP})
target_link_libraries(${PROJECT_NAME}.elf GD32F4xx_driver)
#set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS "-T ${PROJECT_SOURCE_DIR}/Firmware/GD32F4xx_lds/gd32f450z_flash.ld")

set(ELF_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf)
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

# display the size of the output binary after a build is finished
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD COMMAND ${CMAKE_SIZE} --format=berkeley ${ELF_FILE})
add_custom_target(${PROJECT_NAME}.hex ALL DEPENDS ${PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${PROJECT_NAME}.elf ${HEX_FILE})
add_custom_target(${PROJECT_NAME}.bin ALL DEPENDS ${PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary ${PROJECT_NAME}.elf ${BIN_FILE})

