/**
 * @file libarm.c
 * @author ljgabc (ljgabc@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-06-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "config.h"

#ifdef CONFIG_ARM_CM0
#include "libcpu/arm/cortex-m0/cpuport.c"
#endif

#ifdef CONFIG_ARM_CM3
#include "libcpu/arm/cortex-m3/cpuport.c"
#endif

#ifdef CONFIG_ARM_CM4
#include "libcpu/arm/cortex-m4/cpuport.c"
#endif

#ifdef CONFIG_ARM_CM7
#include "libcpu/arm/cortex-m7/cpuport.c"
#include "libcpu/arm/cortex-m7/cpu_cache.c"
#endif

#ifdef CONFIG_ARM_CM23
#include "libcpu/arm/cortex-m23/cpuport.c"
#endif

#ifdef CONFIG_ARM_CM33
#include "libcpu/arm/cortex-m33/cpuport.c"
#include "libcpu/arm/cortex-m33/trustzone.c"
#endif
