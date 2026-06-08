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

If You Want To debug with gbd run the commands below

- initialize gdb server

```shell
cmake --build build --target gdb_server
```

- run another Terminal Session to run the gdb client
```shell
arm-none-eabi-gdb build/blink.elf
```

## Resources

### Official Documentation (STMicroelectronics)
- [STM32F10xxx Flash Programming Manual (PM0075)](https://www.st.com/resource/en/programming_manual/pm0075-stm32f10xxx-flash-memory-microcontrollers-stmicroelectronics.pdf)
- [STM32F10xxx Reference Manual (RM0008)](https://www.st.com/resource/en/reference_manual/rm0008-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- [STM32F103C8 Datasheet](https://www.st.com/resource/en/datasheet/stm32f103c8.pdf)

### Bare-Metal STM32 Development (Without CubeIDE)
- [Part 1: The Bare Necessities](https://kleinembedded.com/stm32-without-cubeide-part-1-the-bare-necessities/)
- [Part 2: CMSIS, Make, and Clock Configuration](https://kleinembedded.com/stm32-without-cubeide-part-2-cmsis-make-and-clock-configuration/)
- [Part 3: The C Standard Library and printf](https://kleinembedded.com/stm32-without-cubeide-part-3-the-c-standard-library-and-printf/)
- [Part 4: CMake, FPU, and STM32 Libraries](https://kleinembedded.com/stm32-without-cubeide-part-4-cmake-fpu-and-stm32-libraries/)
