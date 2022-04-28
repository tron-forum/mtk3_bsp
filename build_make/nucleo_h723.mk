################################################################################
# micro T-Kernel 3.0 BSP makefile
#     Target Board: STM32H723 Nucleo-144
################################################################################

GCC := arm-none-eabi-gcc
AS := arm-none-eabi-gcc
LINK := arm-none-eabi-gcc

CFLAGS := -mcpu=cortex-m7 -mthumb -ffreestanding\
    -std=gnu11 \
    -O0 -g3 \
    -MMD -MP \
    -mfloat-abi=soft \

ASFLAGS := -mcpu=cortex-m7 -mthumb -ffreestanding\
    -x assembler-with-cpp \
    -O0 -g3 \
    -MMD -MP \

LFLAGS := -mcpu=cortex-m7 -mthumb -ffreestanding \
    -nostartfiles \
    -O0 -g3 \
    -mfloat-abi=soft \

LNKFILE := "../etc/linker/nucleo_h723/tkernel_map.ld"

include mtkernel_3/lib/libtm/sysdepend/nucleo_h723/subdir.mk
include mtkernel_3/lib/libtm/sysdepend/no_device/subdir.mk
include mtkernel_3/lib/libtk/sysdepend/cpu/stm32h7/subdir.mk
include mtkernel_3/lib/libtk/sysdepend/cpu/core/armv7m/subdir.mk
include mtkernel_3/kernel/sysdepend/nucleo_h723/subdir.mk
include mtkernel_3/kernel/sysdepend/cpu/stm32h7/subdir.mk
include mtkernel_3/kernel/sysdepend/cpu/core/armv7m/subdir.mk
