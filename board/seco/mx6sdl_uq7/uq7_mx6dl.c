/*
 * (C) Copyright 2015 Seco
 *
 * Author: Davide Cardillo <davide.cardillo@seco.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */


#include "pin_muxing_uq7.h"
#include "../common/proto_seco.h"
#include "detectboard.h"

#define MX6_GPIO_BOOT_VALIDATE                   IMX_GPIO_NR(2, 4)




/* I2C1 - Embedded Controller */
struct i2c_pads_info i2c_pad_info0 = {
	.scl = {
		.i2c_mode  = MX6_PAD_EIM_D21__I2C1_SCL   | PC,
		.gpio_mode = MX6_PAD_EIM_D21__GPIO3_IO21 | PC,
		.gp        = IMX_GPIO_NR(3, 21),
	},
	.sda = {
		.i2c_mode  = MX6_PAD_EIM_D28__I2C1_SDA   | PC,
		.gpio_mode = MX6_PAD_EIM_D28__GPIO3_IO28 | PC,
		.gp        = IMX_GPIO_NR(3, 28),
	},
};


/* I2C2 - PMIC */
struct i2c_pads_info i2c_pad_info1 = {
	.scl = {
		.i2c_mode  = MX6_PAD_KEY_COL3__I2C2_SCL   | PC,
		.gpio_mode = MX6_PAD_KEY_COL3__GPIO4_IO12 | PC,
		.gp        = IMX_GPIO_NR(4, 12),
	},
	.sda = {
		.i2c_mode  = MX6_PAD_KEY_ROW3__I2C2_SDA   | PC,
		.gpio_mode = MX6_PAD_KEY_ROW3__GPIO4_IO13 | PC,
		.gp        = IMX_GPIO_NR(4, 13),
	},
};


/* I2C2 - Q7 connector */
struct i2c_pads_info i2c_pad_info2 = {
	.scl = {
		.i2c_mode  = MX6_PAD_GPIO_5__I2C3_SCL   | PC,
		.gpio_mode = MX6_PAD_GPIO_5__GPIO1_IO05 | PC,
		.gp        = IMX_GPIO_NR(1, 5),
	},
	.sda = {
		.i2c_mode  = MX6_PAD_GPIO_16__I2C3_SDA   | PC,
		.gpio_mode = MX6_PAD_GPIO_16__GPIO7_IO11 | PC,
		.gp        = IMX_GPIO_NR(7, 11),
	},
};




void ldo_mode_set(int ldo_bypass) {} 




int dram_init (void) {
	gd->ram_size = ((ulong)CONFIG_DDR_MB * 1024 * 1024);
	return 0;
}




/*  __________________________________________________________________________
 * |                                                                          |
 * |                                   UART                                   |
 * |__________________________________________________________________________|
 */

static void setup_iomux_uart (void) {
	imx_iomux_v3_setup_multiple_pads(uart_debug_pads, ARRAY_SIZE(uart_debug_pads));
}
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                   SPI                                    |
 * |__________________________________________________________________________|
 */

#ifdef CONFIG_SYS_USE_SPINOR

static void setup_spinor(void) {
	imx_iomux_v3_setup_multiple_pads(ecspi1_pads, ARRAY_SIZE(ecspi1_pads));
	gpio_direction_output(IMX_GPIO_NR(3, 19), 0);
}

int board_spi_cs_gpio(unsigned bus, unsigned cs)
{
	return (bus == 0 && cs == 0) ? (IMX_GPIO_NR(3, 19)) : cs >> 8;
}

#endif   /*  CONFIG_SYS_USE_SPINOR  */
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */

#ifdef CONFIG_FSL_FASTBOOT
#ifdef CONFIG_OF_LOAD_MANUALLY

void board_fastboot_setup(void)
{
	switch (get_boot_device()) {
#if defined(CONFIG_FASTBOOT_STORAGE_SATA)
	case SATA_BOOT:
		if (!getenv("root_device"))
			setenv("root_device", "sata");
		if (!getenv("fastboot_dev"))
			setenv("fastboot_dev", "sata");
		if (!getenv("bootcmd"))
			setenv("bootcmd", "boota sata");
		break;
#endif /*CONFIG_FASTBOOT_STORAGE_SATA*/
#if defined(CONFIG_FASTBOOT_STORAGE_MMC)
	case SD2_BOOT:
	case MMC1_BOOT:
	case MMC2_BOOT:
	case MMC3_BOOT:
	case MMC4_BOOT:
	    if (!getenv("mmc_cur"))
			setenv("mmc_cur", "0");
	    if (!getenv("root_device"))
			setenv("root_device", "emmc");
	    if (!getenv("fastboot_dev"))
			setenv("fastboot_dev", "mmc0");
	    if (!getenv("bootcmd"))
			setenv("bootcmd", "mmc dev 0; run seco_boot_init; boota mmc0 mmcsda1");
	    break;
	case SD1_BOOT:
	case SD3_BOOT:
	case SD4_BOOT:
//	case MMC3_BOOT:
	    if (!getenv("mmc_cur"))
			setenv("mmc_cur", "1");
	    if (!getenv("root_device"))
			setenv("root_device", "sd");
	    if (!getenv("fastboot_dev"))
			setenv("fastboot_dev", "mmc1");
	    if (!getenv("bootcmd"))
			setenv("bootcmd", "mmc dev 1; run seco_boot_init; boota mmc1 mmcsdb1");
	    break;
//	case MMC4_BOOT:
//	    if (!getenv("fastboot_dev"))
//			setenv("fastboot_dev", "mmc2");
//	    if (!getenv("bootcmd"))
//			setenv("bootcmd", "run seco_boot_init; boota mmc2");
//	    break;
#endif /*CONFIG_FASTBOOT_STORAGE_MMC*/
	default:
		printf("unsupported boot devices 0x%04x.\n", get_boot_device());
		break;
	}
}

#ifdef CONFIG_ANDROID_RECOVERY
// int check_recovery_cmd_file(void)
// {
// 	return recovery_check_and_clean_flag();
// }

void board_recovery_setup(void)
{
	int bootdev = get_boot_device();

	switch (bootdev) {
#if defined(CONFIG_FASTBOOT_STORAGE_SATA)
	case SATA_BOOT:
		if (!getenv("bootcmd_android_recovery"))
			setenv("bootcmd_android_recovery",
				"boota sata recovery");
		break;
#endif /*CONFIG_FASTBOOT_STORAGE_SATA*/
#if defined(CONFIG_FASTBOOT_STORAGE_MMC)
	case SD2_BOOT:
	case MMC1_BOOT:
	case MMC2_BOOT:
	case MMC4_BOOT:
	    	if (!getenv("mmc_cur"))
			setenv("mmc_cur", "0");
	    	if (!getenv("root_device"))
			setenv("root_device", "emmc");
	    	if (!getenv("fastboot_dev"))
			setenv("fastboot_dev", "mmc0");
		if (!getenv("bootcmd_android_recovery"))
			setenv("bootcmd_android_recovery",
				"boota mmc0 recovery");
		break;
	case SD3_BOOT:
	case SD4_BOOT:
	case MMC3_BOOT:
	    	if (!getenv("mmc_cur"))
			setenv("mmc_cur", "1");
	    	if (!getenv("root_device"))
			setenv("root_device", "sd");
	    	if (!getenv("fastboot_dev"))
			setenv("fastboot_dev", "mmc1");
		if (!getenv("bootcmd_android_recovery"))
			setenv("bootcmd_android_recovery",
				"boota mmc1 recovery");
		break;
//	case MMC4_BOOT:
//		if (!getenv("bootcmd_android_recovery"))
//			setenv("bootcmd_android_recovery",
//				"boota mmc2 recovery");
//		break;
#endif /*CONFIG_FASTBOOT_STORAGE_MMC*/
	default:
		printf("Unsupported bootup device for recovery: dev: %d\n",
			bootdev);
		return;
	}

	printf("setup env for recovery..\n");
	setenv("bootcmd", "run bootcmd_android_recovery");
}
#endif /*CONFIG_OF_LOAD_MANUALLY*/

#endif /*CONFIG_ANDROID_RECOVERY*/

int is_recovery_key_pressing(void)
{
	        return 0;
}

#endif /*CONFIG_FSL_FASTBOOT*/

/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */



/*  __________________________________________________________________________
 * |                                                                          |
 * |                                   ETHERNET                               |
 * |__________________________________________________________________________|
 */
inline void enable_ethernet_rail (void) {
}
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */



/*  __________________________________________________________________________
 * |                                                                          |
 * |                                   USDHC                                  |
 * |__________________________________________________________________________|
 */
#ifdef CONFIG_FSL_ESDHC

#define USDHC4_CD_GPIO	                          IMX_GPIO_NR(2, 6)
#define USDHC4_PWR_GPIO                           IMX_GPIO_NR(2, 5)

boot_mem_dev_t boot_mem_dev_list[SECO_NUM_BOOT_DEV] = {
	{ 0x3, 0x3, -1,  -1,  0, "SPI Nor" },
	{ 0x4, 0x5, -1,   3, -1, "External SD" },
	{ 0x6, 0x7, -1,  -1, -1, "eMMC" },
};


struct fsl_esdhc_cfg usdhc_cfg[CONFIG_SYS_FSL_USDHC_NUM] = {
	{USDHC3_BASE_ADDR, 0, 8},
	{USDHC4_BASE_ADDR, 0, 4},
};

struct usdhc_l usdhc_list[CONFIG_SYS_FSL_USDHC_NUM] = {
	{usdhc3_pads, ARRAY_SIZE(usdhc3_pads), -1, -1},
	{usdhc4_pads, ARRAY_SIZE(usdhc4_pads), USDHC4_CD_GPIO, USDHC4_PWR_GPIO},
};


enum mxc_clock usdhc_clk[CONFIG_SYS_FSL_USDHC_NUM] = {
	MXC_ESDHC3_CLK,
	MXC_ESDHC4_CLK,
};

/* map the usdhc controller id to the devno given to the board device */
int usdhc_devno[4] = {-1, -1, -1, -1};

void setup_usdhc_devno(void) {

	switch (detect_board()) {
	   case MX6_A75DL_1GB_2x512:
	   case MX6_A75DL_512MB_2x256:
	   case MX6_A75DL_256MB_1x256:
	   case MX6_A75SOLO_256MB_1x256:
		usdhc_devno[2] = 0;
		usdhc_devno[3] = 1;
		break;
	   case MX6_962DL_512MB_2x256:
	   case MX6_962DL_1GB_4x256:
		usdhc_devno[2] = 0;
		usdhc_devno[3] = 1;
		break;
	   default:
		usdhc_devno[2] = 0;
		usdhc_devno[3] = 1;
		break;
	}
}

int board_mmc_getcd (struct mmc *mmc) {
	struct fsl_esdhc_cfg *cfg = (struct fsl_esdhc_cfg *)mmc->priv;
	int ret = 0;

	switch (cfg->esdhc_base) {
		case USDHC4_BASE_ADDR:
			ret = !gpio_get_value(USDHC4_CD_GPIO);
			break;
		case USDHC3_BASE_ADDR:
			ret = 1; /* eMMC/uSDHC4 is always present */
			break;
	}

	return ret;
}

#endif  /*  CONFIG_FSL_ESDHC  */

/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */



/*  __________________________________________________________________________
 * |                                                                          |
 * |                                    USB                                   |
 * |__________________________________________________________________________|
 */
#ifdef CONFIG_USB_EHCI_MX6

#define USB_HOST1_PWR                            IMX_GPIO_NR(7, 12)
#define USB_OTG_PWR                              IMX_GPIO_NR(3, 22)


#define USB_OTHERREGS_OFFSET                     0x800
#define UCTRL_PWR_POL                            (1 << 9)


int board_ehci_hcd_init (int port) {
	u32 *usbnc_usb_ctrl;

	switch (port) {
	case 0:
		imx_iomux_v3_setup_multiple_pads(usb_otg_pads,
			ARRAY_SIZE(usb_otg_pads));

		/*set daisy chain for otg_pin_id on 6q. for 6dl, this bit is reserved*/
		imx_iomux_set_gpr_register (1, 13, 1, 0);
		break;
	case 1:
		imx_iomux_v3_setup_multiple_pads(usb_hc1_pads,
			ARRAY_SIZE(usb_hc1_pads));
		break;
	default:
		printf("MXC USB port %d not yet supported\n", port);
		return -EINVAL;
	}

	usbnc_usb_ctrl = (u32 *)(USB_BASE_ADDR + USB_OTHERREGS_OFFSET +
			port * 4);

	setbits_le32(usbnc_usb_ctrl, UCTRL_PWR_POL);

	return 0;
}


int board_ehci_power (int port, int on) {
	switch (port) {
	case 0:
		if (on)
			gpio_direction_output(USB_OTG_PWR, 1);
		else
			gpio_direction_output(USB_OTG_PWR, 0);
		break;
	case 1:
		if (on)
			gpio_direction_output(USB_HOST1_PWR, 1);
		else
			gpio_direction_output(USB_HOST1_PWR, 0);
		break;
	default:
		printf("MXC USB port %d not yet supported\n", port);
		return -EINVAL;
	}
	return 0;
}

#endif  /*  CONFIG_USB_EHCI_MX6  */
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */


/*  __________________________________________________________________________
 * |                                                                          |
 * |                            DISPLAY INFO - INIT                           |
 * |__________________________________________________________________________|
 */
/*
 * Do not overwrite the console
 * Use always serial for U-Boot console
 */
int overwrite_console (void) {
	return 1;
}


#ifdef CONFIG_SECO_BOARD_NAME
char *board_name = CONFIG_SECO_BOARD_NAME;
#else
char *board_name = "Seco uQ7-i.MX6 (uQ7-J) - A75";
#endif


int board_early_init_f (void) {

	setup_iomux_uart();

#if defined(CONFIG_VIDEO_IPUV3)
	setup_display();
#endif

	return 0;
}

#define I2C_PMIC                                 0x1

#ifdef CONFIG_SYS_I2C_MXC
//	static struct pmic *pfuze;
#endif
int board_init (void) {

#ifdef CONFIG_SYS_I2C_MXC
	int s, i;
	unsigned char *boardrev = NULL;
#endif

	/* address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

	boot_validate (MX6_GPIO_BOOT_VALIDATE, gpio_boot_validate_pad);

#ifdef CONFIG_FSL_ESDHC
	setup_usdhc_devno();
#endif

#ifdef CONFIG_SYS_USE_SPINOR
	setup_spinor();
#endif

#ifdef CONFIG_CMD_SATA
	setup_sata();
#endif

#ifdef CONFIG_SYS_I2C_MXC
	s = get_seco_board_revision(&i2c_pad_info0, boardrev);
	for ( i = 0 ; i < s ; i++ )
		gd->bd->board_revision[i] = boardrev[i];

	printf ("ID: 9%X, rev %c%X\n", gd->bd->board_revision[1],
		((gd->bd->board_revision[0] & 0xF0) >> 4) + 'a',
		(gd->bd->board_revision[0] & 0x0F));


	setup_i2c (I2C_PMIC, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info1);
//	pfuze = pfuze_common_init(I2C_PMIC);
#endif

	print_boot_device ();

	return 0;
}


#ifdef CONFIG_CMD_BMODE
static const struct boot_mode board_boot_modes[] = {
	/* 4 bit bus width */
	{"sd3",	 MAKE_CFGVAL(0x40, 0x30, 0x00, 0x00)},
	/* 8 bit bus width */
	{"emmc", MAKE_CFGVAL(0x60, 0x58, 0x00, 0x00)},
	{NULL,	 0},
};
#endif


int board_late_init (void) {

	int ret = 0;

#ifdef CONFIG_CMD_BMODE
	add_board_boot_modes(board_boot_modes);
#endif

#ifdef CONFIG_PFUZE100_PMIC_I2C
	ret = setup_pmic_voltages(&i2c_pad_info1);
	if (ret)
		return -1;
#endif

#ifdef CONFIG_ENV_IS_IN_MMC
	board_late_mmc_env_init();
#endif

	return ret;
}
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */


iomux_v3_cfg_t const pcie_pads[] = {
	MX6_PAD_EIM_D19__GPIO3_IO19 | MUX_PAD_CTRL(NO_PAD_CTRL),	/* POWER */
	MX6_PAD_GPIO_17__GPIO7_IO12 | MUX_PAD_CTRL(NO_PAD_CTRL),	/* RESET */
};

// static void setup_pcie(void)
// {
// 	imx_iomux_v3_setup_multiple_pads(pcie_pads, ARRAY_SIZE(pcie_pads));
// }
