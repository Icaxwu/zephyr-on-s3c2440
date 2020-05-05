#include <uart.h>
#include <s3c2440_soc.h>


struct uart_s3c2440_data {
	struct uart_config uart_config;
};


struct uart_s3c2440_config {
	bool rts_cts_pins_set;
};

#define TXD0READY   (1<<2)
#define RXD0READY   (1)

#define PCLK            (400000000/8)                              // clock_init中FCLK:HCLK:PCLK配置1:4:8, FCLK配置400M
#define UART_CLK        PCLK                                       // 时钟UART0的源设为PCLK
#define UART_BAUD_RATE  115200                                     // 波特率
#define UART_BRD        ((UART_CLK  / (UART_BAUD_RATE * 16)) - 1)

/** Console I/O function */
int uart_s3c2440_poll_in(struct device *dev, unsigned char *p_char)
{
    /* 等待，直到接收缓冲区中的有数据 */
    while (!(UTRSTAT0 & RXD0READY));
    
    /* 直接读取URXH0寄存器，即可获得接收到的数据 */
    return (*p_char = URXH0);
}

void uart_s3c2440_poll_out(struct device *dev, unsigned char out_char)
{
    /* 等待，直到发送缓冲区中的数据已经全部发送出去 */
    while (!(UTRSTAT0 & TXD0READY));
    
    /* 向UTXH0寄存器中写入数据，UART即自动将它发送出去 */
    UTXH0 = out_char;
}

int uart_s3c2440_configure(struct device *dev, const struct uart_config *cfg)
{
    
}

/* ret等于0表示驱动初始化成功，否则device_get_binding()会失败，如果调用者对这个失败情况没有做处理
   ，继续在device_get_binding()返回的NULL结果上调用驱动相关的函数，则可能导致死机 */
static int uart_s3c2440_init(struct device *dev)
{
    GPHCON  |= 0xa0;    // GPH2,GPH3用作TXD0,RXD0
    GPHUP   = 0x0c;     // GPH2,GPH3内部上拉

    ULCON0  = 0x03;     // 8N1(8个数据位，无较验，1个停止位)
    UCON0   = 0x05;     // 查询方式，UART时钟源为PCLK
    UFCON0  = 0x00;     // 不使用FIFO
    UMCON0  = 0x00;     // 不使用流控
    UBRDIV0 = UART_BRD; // 波特率为115200

    return 0;
}

static const struct uart_driver_api uart_driver_api = {
	.poll_in          = uart_s3c2440_poll_in,
	.poll_out         = uart_s3c2440_poll_out,
	.configure        = uart_s3c2440_configure,
};

#ifdef CONFIG_UART_0_S3C2440_UART
static struct uart_s3c2440_data uart_s3c2440_uart0_data = {
	.uart_config = {
		.stop_bits = UART_CFG_STOP_BITS_1,
		.data_bits = UART_CFG_DATA_BITS_8,
		.baudrate  = 115200,
		.parity    = UART_CFG_PARITY_NONE,
		.flow_ctrl = UART_CFG_FLOW_CTRL_NONE,
	}
};

static const struct uart_s3c2440_config uart_s3c2440_uart0_config = {

};

DEVICE_DEFINE(uart_s3c2440_uart0,
	      "uart_s3c2440_drv",
	      uart_s3c2440_init,
	      NULL,
	      &uart_s3c2440_uart0_data,
	      &uart_s3c2440_uart0_config,
	      /* Initialize UART device before UART console. */
	      PRE_KERNEL_1,
	      CONFIG_KERNEL_INIT_PRIORITY_DEVICE,
	      &uart_driver_api);
#endif
