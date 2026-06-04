# STM32 without CubeIDE (Smart Pharmacy Project)

This Project is a fork of `https://github.com/kristianklein/stm32-without-cubeide`

This version targets **STM32F103C8T6** (Cortex-M3, no FPU) using STM32CubeF1.

## Initializing submodules
The official STM32CubeF1 package is added as a git submodule. This makes it easy to get the newest updates and bugfixes whenever a new version is published.

After cloning the stm32-without-cubeide repository, initialize the top-level submodule:

`git submodule update --init`

Enter the submodule and initialize the CMSIS device driver and the HAL/LL library:

```shell
cd vendor/STM32CubeF1
git submodule update --init Drivers/CMSIS/Device/ST/STM32F1xx/ Drivers/STM32F1xx_HAL_Driver/
```

In order to update STM32CubeF1 to the newest version, simply navigate to the submodule's root directory, pull from remote and update the 
(already initialized) submodules:

```shell
cd vendor/STM32CubeF1
git pull origin master
git submodule update --recursive
```

## Building with CMake
After cloning the repository and initializing the submodules as described
above, you can configure and build the project with CMake from the root
project directory:

```shell
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=cmake/stm32f103c8.cmake
cmake --build build
```

Then flash the program to the target with:

```shell
cmake --build build --target flash
```
