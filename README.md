# GD32F4xx

#### 介绍
# GD32F4xx 模板工程

# 目录
```
├─App
├─Doc      
├─Firmware
│  ├─CMSIS
│  ├─GD32F4xx_standard_peripheral
│  └─GD32F4xx_usb_library
├─Hardware
├─Startup
└─User

```


# 配置修改
> 直接修改CMakeLists.txt 文件中ARM_TOOLCHAIN_DIR 的目录，如下所示
```text
set(ARM_TOOLCHAIN_DIR {Yours ToolChains dir})
```

```text
set(ARM_TOOLCHAIN_DIR /Applications/ArmGNUToolchain/12.2.mpacbti-rel1/arm-none-eabi/bin)
```


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
