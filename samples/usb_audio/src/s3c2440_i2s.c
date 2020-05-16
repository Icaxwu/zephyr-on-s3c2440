#include <s3c2440_soc.h>
#include <zephyr.h>

#define IISCON_INTERFACE_ENABLE                         (0x1<<0)
#define IISCON_PRESCALER_ENABLE                         (0x1<<1)
#define IISCON_RX_IDLE                                  (0x1<<2)
#define IISCON_TX_IDLE                                  (0x1<<3)
#define IISCON_RX_DMA                                   (0x1<<4)
#define IISCON_TX_DMA                                   (0x1<<5)

#define IISMOD_SCLK_SHIFT                               (0)
#define IISMOD_SCLK_SEL(X)                              ((X) << IISMOD_SCLK_SHIFT)
#define IISMOD_MCLK_SHIFT                               (2)
#define IISMOD_MCLK_SEL(X)                              ((X) << IISMOD_MCLK_SHIFT)
#define IISMOD_DATA_WIDTH_SHIFT                         (3)
#define IISMOD_DATA_WIDTH_SEL(X)                        ((X) << IISMOD_DATA_WIDTH_SHIFT)
#define IISMOD_FORMAT_SHIFT                             (4)
#define IISMOD_FORMAT_SEL(X)                            ((X) << IISMOD_FORMAT_SHIFT)
#define IISMOD_ACTIVE_LEVEL_SHIFT                       (5)
#define IISMOD_ACTIVE_LEVEL_SEL(X)                      ((X) << IISMOD_ACTIVE_LEVEL_SHIFT)
#define IISMOD_MOD_SHIFT                                (6)
#define IISMOD_MOD_SEL(X)                               ((X) << IISMOD_MOD_SHIFT)
#define IISMOD_ROLE_SHIFT                               (8)
#define IISMOD_ROLE_SEL(X)                              ((X) << IISMOD_ROLE_SHIFT)    
#define IISMOD_MCKL_SRC_SHIFT                           (9)    
#define IISMOD_MCLK_SRC_SEL(X)                          ((X) << IISMOD_MCKL_SRC_SHIFT)

#define IISFCON_TRANSMIT_FIFO_ACCESS_MOD_SEL(X)         ((X) << 15)
#define IISFCON_RECEIVE_FIFO_ACCESS_MOD_SEL(X)          ((X) << 14)
#define IISFCON_TRANSMIT_FIFO_ENABLE                    (1 << 13)
#define IISFCON_RECEIVE_FIFO_ENABLE                     (1 << 12)

#define GPE0_I2SLRCK (0x2<<(2*0))
#define GPE1_I2SSCKL (0X2<<(2*1))
#define GPE2_CDCLK   (0X2<<(2*2))
#define GPE3_I2SDI   (0x2<<(2*3))
#define GPE4_I2SDO   (0x2<<(2*4))

#define GPE0_NOT_PULL_UP ((0x1<<0))
#define GPE1_NOT_PULL_UP ((0x1<<1))
#define GPE2_NOT_PULL_UP ((0x1<<2))
#define GPE3_NOT_PULL_UP ((0x1<<3))
#define GPE4_NOT_PULL_UP ((0x1<<4))

void init_IIS_port() {
	/*[0-1] [2-3] [4-5] [6-7] [8-9]*/
	//*** PORT E GROUP
	//Ports  : GPE15  GPE14 GPE13   GPE12   GPE11   GPE10   GPE9    GPE8     GPE7  GPE6  GPE5   GPE4
	//Signal : IICSDA IICSCL SPICLK SPIMOSI SPIMISO SDDATA3 SDDATA2 SDDATA1 SDDATA0 SDCMD SDCLK I2SDO
	//Binary :  10     10  ,  10      10  ,  10      10   ,  10      10   ,   10    10  , 10     10  ,
	//-------------------------------------------------------------------------------------------------------
	//Ports  :  GPE3   GPE2  GPE1    GPE0
	//Signal :  I2SDI  CDCLK I2SSCKL I2SLRCK
	//Binary :  10     10  , 10      10
	//rGPECON |= 0x2aa;
	//rGPEUP  |= 0x1f;     // The pull up function is disabled GPE[15:0]
	GPECON |= GPE0_I2SLRCK | GPE1_I2SSCKL | GPE2_CDCLK | GPE3_I2SDI | GPE4_I2SDO;
	GPEUP |= GPE0_NOT_PULL_UP | GPE1_NOT_PULL_UP | GPE2_NOT_PULL_UP | GPE3_NOT_PULL_UP | GPE4_NOT_PULL_UP;
}

void init_IIS_bus(unsigned int fs) 
{
    init_IIS_port();

	IISCON = 0;
	IISMOD = 0;
	IISFCON = 0;

	/* mclk: pclk
       role: slave
       transmit/receive mode: transmit mode
       Serial interface format:  MSB (Left)-justified format
       Serial data bit per channel: 16-bit
	 */
	IISMOD |= IISMOD_MCLK_SRC_SEL(0) | IISMOD_ROLE_SEL(1) | IISMOD_MOD_SEL(2)
           | IISMOD_FORMAT_SEL(1) | IISMOD_DATA_WIDTH_SEL(1);

	/*
	 * Transmit FIFO access mode select : Normal
	 * Transmit FIFO                    : Enable
	 */
	IISFCON =  IISFCON_TRANSMIT_FIFO_ACCESS_MOD_SEL(0) | IISFCON_TRANSMIT_FIFO_ENABLE;

    /*  CDCLK: 25M = pclk / 2
        division factor is N+1 
    */
    IISPSR = 1;

    IISCON |= IISCON_PRESCALER_ENABLE | IISCON_RX_IDLE; 
    IISCON |= IISCON_INTERFACE_ENABLE;

    printk("IISCON %x, IISFCON %x, IISPSR %x, IISMOD %x\n", IISCON, IISFCON, IISPSR, IISMOD);
}

