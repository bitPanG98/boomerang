/*
 * Copyright (c) 2016, Xilinx Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PLATFORM_CONFIG_H
#define PLATFORM_CONFIG_H

#define PLATFORM_FLAVOR_ID_zc1751_dc1	0
#define PLATFORM_FLAVOR_ID_zc1751_dc2	1
#define PLATFORM_FLAVOR_ID_zcu102	2
#define PLATFORM_FLAVOR_IS(flav) \
	(PLATFORM_FLAVOR_ID_ ## flav == PLATFORM_FLAVOR)

/* Make stacks aligned to data cache line length */
#define STACK_ALIGNMENT		64
#define HEAP_SIZE		(24 * 1024)

#ifdef CFG_WITH_PAGER
#error "Pager not supported for zynqmp"
#endif

#if PLATFORM_FLAVOR_IS(zc1751_dc1) || PLATFORM_FLAVOR_IS(zc1751_dc2) || \
	PLATFORM_FLAVOR_IS(zcu102)

#define GIC_BASE		0xF9010000
#define UART0_BASE		0xFF000000
#define UART1_BASE		0xFF001000

#define IT_UART0		53
#define IT_UART1		54

#define UART0_CLK_IN_HZ		100000000
#define UART1_CLK_IN_HZ		100000000
#define CONSOLE_UART_BASE	UART0_BASE
#define IT_CONSOLE_UART		IT_UART0
#define CONSOLE_UART_CLK_IN_HZ	UART0_CLK_IN_HZ

#define DRAM0_BASE		0
#define DRAM0_SIZE		0x80000000

/* Location of trusted dram */
#define TZDRAM_BASE		0x60000000
#define TZDRAM_SIZE		0x10000000

#define CFG_SHMEM_START		0x70000000
#define CFG_SHMEM_SIZE		0x10000000

#define GICD_OFFSET		0
#define GICC_OFFSET		0x20000

#else
#error "Unknown platform flavor"
#endif

#define CFG_TEE_CORE_NB_CORE	4

#define CFG_TEE_RAM_VA_SIZE	(1024 * 1024)

#ifndef CFG_TEE_LOAD_ADDR
#define CFG_TEE_LOAD_ADDR	CFG_TEE_RAM_START
#endif

/*
 * Assumes that either TZSRAM isn't large enough or TZSRAM doesn't exist,
 * everything is in TZDRAM.
 * +------------------+
 * |        | TEE_RAM |
 * + TZDRAM +---------+
 * |        | TA_RAM  |
 * +--------+---------+
 */
#define CFG_TEE_RAM_PH_SIZE	CFG_TEE_RAM_VA_SIZE
#define CFG_TEE_RAM_START	TZDRAM_BASE
#define CFG_TA_RAM_START	ROUNDUP((TZDRAM_BASE + CFG_TEE_RAM_VA_SIZE), \
					CORE_MMU_DEVICE_SIZE)
#define CFG_TA_RAM_SIZE		ROUNDDOWN((TZDRAM_SIZE - CFG_TEE_RAM_VA_SIZE), \
					  CORE_MMU_DEVICE_SIZE)


#define DEVICE0_PA_BASE		ROUNDDOWN(CONSOLE_UART_BASE, \
					  CORE_MMU_DEVICE_SIZE)
#define DEVICE0_VA_BASE		DEVICE0_PA_BASE
#define DEVICE0_SIZE		CORE_MMU_DEVICE_SIZE
#define DEVICE0_TYPE		MEM_AREA_IO_SEC

#define DEVICE1_PA_BASE		ROUNDDOWN(GIC_BASE, CORE_MMU_DEVICE_SIZE)
#define DEVICE1_VA_BASE		DEVICE1_PA_BASE
#define DEVICE1_SIZE		CORE_MMU_DEVICE_SIZE
#define DEVICE1_TYPE		MEM_AREA_IO_SEC

#define DEVICE2_PA_BASE		ROUNDDOWN(GIC_BASE + GICD_OFFSET, \
					  CORE_MMU_DEVICE_SIZE)
#define DEVICE2_VA_BASE		DEVICE2_PA_BASE
#define DEVICE2_SIZE		CORE_MMU_DEVICE_SIZE
#define DEVICE2_TYPE		MEM_AREA_IO_SEC

#ifndef UART_BAUDRATE
#define UART_BAUDRATE		115200
#endif
#ifndef CONSOLE_BAUDRATE
#define CONSOLE_BAUDRATE	UART_BAUDRATE
#endif

/* For virtual platforms where there isn't a clock */
#ifndef CONSOLE_UART_CLK_IN_HZ
#define CONSOLE_UART_CLK_IN_HZ	1
#endif

#endif /*PLATFORM_CONFIG_H*/
