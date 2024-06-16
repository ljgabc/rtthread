/**
 * @file libcpu.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-06-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "config.h"

#ifdef CONFIG_ARCH_ARM
#include "libcpu/arm/libarm.c"
#endif

#ifdef CONFIG_ARCH_RISCV
#include "libcpu/risc-v/libriscv.c"
#endif
