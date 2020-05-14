#include <s3c2440_soc.h>
#include <zephyr.h>

#define PCLK 50000000
#define ABS(a, b) ((a>b)?(a-b):(b-a))

#define IISCON_PRESCALER_ENABLE (0x1<<1)
#define IISCON_INTERFACE_ENABLE (0x1<<0)
#define IISCON_INTERFACE_DISABLE (0x0<<0)
#define IISCON_RX_IDLE (0x1<<2)
#define IISCON_TX_IDLE (0x1<<3)
#define IISCON_TX_DMA (0x1<<5)
#define IISCON_RX_DMA (0x1<<4)
#define IISCON_ENABLE_IIS (0x1<<0)

#define IISMOD_SCLK_32FS (0x1<<0)
#define IISMOD_MCLK_384FS (0x1<<2)
#define IISMOD_MCLK_256FS (0x0<<2)
#define IISMOD_SERIAL_BIT_PER_CH_16 (0x1<<3)
#define IISMOD_MSB_FORMAT (0x1<<4)
#define IISMOD_LOW_FOR_LEFT_CH (~(0x0<<5))
#define IISMOD_TXMOD (0x2<<6)

#define IISFCON_TX_ENABLE (0x1<<13)
#define IISFCON_RX_ENABLE (0x1<<12)
#define IISFCON_RX_FIFO_DMA (0x1<<14)
#define IISFCON_TX_FIFO_DMA (0x1<<15)
#define IISFCON_TX_FIFO_NORMAL (0x0<<15)


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

void init_IIS_bus(unsigned int fs) {
    int tmp_fs;
    int mclk_type;

    init_IIS_port();

	IISCON = 0;
	IISMOD = 0;
	IISFCON = 0;

	//fs:采样频率
	/* bit[9] : Master clock select, 0-PCLK
	 * bit[8] : 0 = Master mode
	 * bit[7:6] : 10 = Transmit mode
	 * bit[4] : 1 = MSB (Left)-justified format
	 * bit[2] : 384fs, 确定了MASTER CLOCK之后, fs = MASTER CLOCK/384
	 * bit[1:0] : Serial bit clock frequency select, 32fs
	 */
    mclk_type = IISMOD_MCLK_384FS;
	IISMOD |= IISMOD_SCLK_32FS | mclk_type | IISMOD_SERIAL_BIT_PER_CH_16 | IISMOD_TXMOD | IISMOD_MSB_FORMAT;


	/*
	 * bit15 : Transmit FIFO access mode select, 1-DMA 0-normal(default)
	 * bit13 : Transmit FIFO, 1-enable
	 */
	IISFCON =  IISFCON_TX_ENABLE | IISFCON_TX_FIFO_NORMAL;
	unsigned int min = 0xffffffff;
	int pre	= 0;
	for (int i = 0; i < 32; i++) {
		tmp_fs = PCLK / (mclk_type == IISMOD_MCLK_384FS ? 384 : 256) / (i + 1);
		if (ABS(tmp_fs, fs) < min) {
			min = ABS(tmp_fs, fs);
			pre = i;

            printk("tmp_fs %d\n", tmp_fs);
		}
	}
    printk("min %d\n", min);
	IISPSR = (pre << 5) | pre;

    IISCON |= IISCON_PRESCALER_ENABLE | IISCON_RX_IDLE; 

    /* enable IIS interface */
    IISCON |= IISCON_ENABLE_IIS;

    printk("IISCON %x, IISFCON %x, IISPSR %x, IISMOD %x\n", IISCON, IISFCON, IISPSR, IISMOD);
}

