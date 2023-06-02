# GD32F4xx

#### 介绍

# GD32F4xx 模板工程

# 目录

```
+---Doc
+---Firmware
|   +---CMSIS
|   +---GD32F4xx_lds
|   +---GD32F4xx_standard_peripheral
|   \---GD32F4xx_usb_library
+---Gui
|   +---demo
|   \---examples
+---Hardware
+---Lib
|   \---LVGL
+---Startup
\---User


```

# 配置修改

> 直接修改CMakeLists.txt 文件中ARM_TOOLCHAIN_DIR 的目录，如下所示

```text
set(ARM_TOOLCHAIN_DIR {Yours ToolChains dir})
```

```text
set(ARM_TOOLCHAIN_DIR /Applications/ArmGNUToolchain/12.2.mpacbti-rel1/arm-none-eabi/bin)
```

> CMakeListsPrivate.txt 文件是导入工程依赖可仿照 LVGL写就可以

# Toolchains 安装

## Windows

- 可参考稚晖君的[配置CLion用于STM32开发【优雅の嵌入式开发】](https://zhuanlan.zhihu.com/p/145801160)
- STM32相关配置可以选配，arm-none-eabi和Openocd是必选要配置的

## MacOS

- 前期准备 安装Homebrew

```shell
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

- 安装arm-none-eabi 使用如下命令并 记录ToolChains安装目录

```shell
brew tap homebrew/cask-versions
brew install --cask gcc-arm-embedded
```

- 安装Openocd

```shell
brew install openocd
```

- 安装完成后验证

```shell
openocd --version
```

# Clion 设置

- File > Settings > Build, Execution, Deployment> Toolchains

```text
C Compiler: arm-none-eabi-gcc
C++ Compiler: arm-none-eabi-g++
Debugger: arm-none-eabi-gdb
```

# LVGL配置修改

> 1. lvgl在目录```Lib/lv_conf.h```的文件中以下几项可修改（其他项暂时没研究不太懂）：

```c
/**
 * 屏幕水平分辨率
 */
#define LV_HOR_RES_MAX

/**
 * 屏幕垂直分辨率
 */
#define LV_VER_RES_MAX
/**
 * 此项为DPI配置 找了一个计算简单的方式 DPI = ((√240*240+272*272) / 1.54) ≈ 220
 */
#define LV_DPI_DEF
/***
* 此项设置刷新率，lvgl内部定时器刷新，默认是30ms刷新，设置小一点应该会清晰一点
*/
#define LV_DISP_DEF_REFR_PERIOD
#define LV_INDEV_DEF_READ_PERIOD
```

> 实际渲染内容位于```porting/lv_port_disp.c```文件中的```disp_flush```函数

# [LVGL_v8.0.2](https://docs.lvgl.io/master/index.html)

> 例程中使用是中景园电子```LCD 1.54 240x240(RGB)```屏幕拓展板 ```8Pin ST7789 ```,同款可以直接使用