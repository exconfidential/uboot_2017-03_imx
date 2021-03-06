/*
 * Copyright (C) Seco s.r.l.
 *
 * Author: Giuseppe Pagano <giuseppe.pagano@seco.it>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/arch/mx6-ddr.h>

static struct mx6_ddr3_cfg mem_ddr_2G_4x512 = {
	.mem_speed = 1600,
	.density = 4,
	.width = 64,
	.banks = 8,
	.rowaddr = 15,
	.coladdr = 10,
	.pagesz = 2,
	.trcd = 1375,
	.trcmin = 4875,
	.trasmin = 3500,
};

const struct mx6dq_iomux_ddr_regs mx6_ddr_2G_4x512_ioregs = {
	.dram_dqm0 	 = 0x00000030,
	.dram_dqm1 	 = 0x00000030,
	.dram_dqm2 	 = 0x00000030,
	.dram_dqm3 	 = 0x00000030,
	.dram_dqm4 	 = 0x00000030,
	.dram_dqm5 	 = 0x00000030,
	.dram_dqm6 	 = 0x00000030,
	.dram_dqm7 	 = 0x00000030,
	.dram_ras 	 = 0x00000030,
	.dram_cas 	 = 0x00000030,
	.dram_sdba2 	 = 0x00000000,
	.dram_sdodt0 	 = 0x00000030,
	.dram_sdodt1 	 = 0x00000030,
	.dram_sdcke0 	 = 0x00003000,
	.dram_sdcke1 	 = 0x00003000,
	.dram_sdclk_0 	 = 0x00000030,
	.dram_sdclk_1 	 = 0x00000030,
	.dram_sdqs0 	 = 0x00000030,
	.dram_sdqs1 	 = 0x00000030,
	.dram_sdqs2 	 = 0x00000030,
	.dram_sdqs3 	 = 0x00000030,
	.dram_sdqs4 	 = 0x00000030,
	.dram_sdqs5 	 = 0x00000030,
	.dram_sdqs6 	 = 0x00000030,
	.dram_sdqs7 	 = 0x00000030,
	.dram_reset 	 = 0x00000030,
};

const struct mx6dq_iomux_grp_regs mx6_grp_2G_4x512_ioregs = {
	.grp_addds 	 = 0x00000030,
	.grp_ddrmode_ctl = 0x00020000,
	.grp_ddrpke 	 = 0x00000000,
	.grp_ddrmode 	 = 0x00020000,
	.grp_b0ds 	 = 0x00000030,
	.grp_b1ds 	 = 0x00000030,
	.grp_b2ds 	 = 0x00000030,
	.grp_b3ds 	 = 0x00000030,
	.grp_b4ds 	 = 0x00000030,
	.grp_b5ds 	 = 0x00000030,
	.grp_b6ds 	 = 0x00000030,
	.grp_b7ds 	 = 0x00000030,
	.grp_ctlds 	 = 0x00000030,
	.grp_ddr_type 	 = 0x000C0000,
};
