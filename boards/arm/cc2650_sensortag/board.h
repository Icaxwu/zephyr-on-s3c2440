/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Board-specific information for Texas Instruments'
 * SensorTag device.
 */


#ifndef _CC2650_SENSORTAG_BOARD_H_
#define _CC2650_SENSORTAG_BOARD_H_

/* Match a feature on the board to an I/O pin
 * (DIOx in local jargon)
 */
#define SENSORTAG_BUTTON2	0
#define SENSORTAG_TMP_RDY	1
#define SENSORTAG_AUDIO_D	2
#define SENSORTAG_REED		3
#define SENSORTAG_BUTTON1	4
#define SENSORTAG_SDA		5
#define SENSORTAG_SCL		6
#define SENSORTAG_MPU_INT	7
#define SENSORTAG_SDA_HP	8
#define SENSORTAG_SCL_HP	9
#define SENSORTAG_LED1		10
#define SENSORTAG_DP7		11
#define SENSORTAG_AUDIO_CLK	SENSORTAG_DP7
#define SENSORTAG_MPU_PWR	12
#define SENSORTAG_MIC_PWR	13
#define SENSORTAG_FLASH_CS	14
#define SENSORTAG_LED2		15
#define SENSORTAG_DP12		16
#define SENSORTAG_AUDIO_FS	SENSORTAG_DP12
#define SENSORTAG_TDO		SENSORTAG_DP12
#define SENSORTAG_DP8		17
#define SENSORTAG_SCLK		SENSORTAG_DP8
#define SENSORTAG_TDI		SENSORTAG_DP8
#define SENSORTAG_DP9		18
#define SENSORTAG_MISO		SENSORTAG_DP9
#define SENSORTAG_DP10		19
#define SENSORTAG_MOSI		SENSORTAG_DP10
#define SENSORTAG_DP11		20
#define SENSORTAG_CSN		SENSORTAG_DP11
#define SENSORTAG_BUZZER	21
#define SENSORTAG_DP6		22
#define SENSORTAG_AUDIO_DO	SENSORTAG_DP6
#define SENSORTAG_DP2		23
#define SENSORTAG_DP1		24
#define SENSORTAG_DP0		25
#define SENSORTAG_VDD		26
#define SENSORTAG_DP3		27
#define SENSORTAG_DP4		28
#define SENSORTAG_UART_RX	SENSORTAG_DP4
#define SENSORTAG_DP5		29
#define SENSORTAG_UART_TX	SENSORTAG_DP5
#define SENSORTAG_DP_D		30

#endif /* _CC2650_SENSORTAG_BOARD_H_ */
