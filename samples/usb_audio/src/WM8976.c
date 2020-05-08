#include <zephyr.h>
#include <device.h>
#include <gpio.h>

#define WM8976_CSB                             2
#define WM8976_DAT                             3
#define WM8976_CLK                             4
#define WM8976_CTL_INTERFACE_PORT              GPIO_B_LABEL

static struct device *wm8976_ctl_gpio_dev = NULL;

static void wm8976_control_interface_init(void)
{
    wm8976_ctl_gpio_dev = device_get_binding(WM8976_CTL_INTERFACE_PORT);
    if(wm8976_ctl_gpio_dev == NULL){
        printk("device_get_binding %s error!\n", WM8976_CTL_INTERFACE_PORT);
        return;
    }
    /*wm8976引脚csb,dat,clk分别对应2440芯片的GPB2,3,4引脚;
      Set pin as output */
    gpio_pin_configure(wm8976_ctl_gpio_dev, WM8976_CSB, GPIO_DIR_OUT);
    gpio_pin_configure(wm8976_ctl_gpio_dev, WM8976_DAT, GPIO_DIR_OUT);
    gpio_pin_configure(wm8976_ctl_gpio_dev, WM8976_CLK, GPIO_DIR_OUT);
}

void wm8976_write_reg(unsigned char reg, unsigned int data)
{
    int i;

    if(wm8976_ctl_gpio_dev == NULL){
        return;
    }

    //对于wm8976来说，数据的高七位表示寄存器地址，低9位表示寄存器的值
    unsigned short val = (reg << 9) | (data & 0x1ff);

    /*把val值写入wm8976,共16位,从高到低传输*/
    for (i = 0; i < 16; i++){
        if (val & (1<<15))
        {
            gpio_pin_write(wm8976_ctl_gpio_dev, WM8976_CLK, 0);
            gpio_pin_write(wm8976_ctl_gpio_dev, WM8976_DAT, 1);

            k_busy_wait(1);
            gpio_pin_write(wm8976_ctl_gpio_dev, WM8976_CLK, 1);                  
        }
        else
        {
            gpio_pin_write(wm8976_ctl_gpio_dev, WM8976_CLK, 0);
            gpio_pin_write(wm8976_ctl_gpio_dev, WM8976_DAT, 0);

            k_busy_wait(1);
            gpio_pin_write(wm8976_ctl_gpio_dev, WM8976_CLK, 1);                       
        }
        val = val << 1;
    }

    //传输完成，需要让csb信号产生低脉冲，写入wm8976
    gpio_pin_write(wm8976_ctl_gpio_dev, WM8976_CSB, 0);

    k_busy_wait(1);

    //引脚恢复到高电平状态
    gpio_pin_write(wm8976_ctl_gpio_dev, WM8976_CSB, 1);  
    gpio_pin_write(wm8976_ctl_gpio_dev, WM8976_DAT, 1);  
    gpio_pin_write(wm8976_ctl_gpio_dev, WM8976_CLK, 1);  
}

void init_wm8976(void) 
{

    wm8976_control_interface_init();

	/* software reset */
	wm8976_write_reg(0, 0);

	/* OUT2的左/右声道打开
	 * 左/右通道输出混音打开
	 * 左/右DAC打开
	 */
	wm8976_write_reg(0x3, 0x6f);
	wm8976_write_reg(0x1, 0x1f);//biasen,BUFIOEN.VMIDSEL=11b
	wm8976_write_reg(0x2, 0x185);//ROUT1EN LOUT1EN, inpu PGA enable ,ADC enable
	wm8976_write_reg(0x6, 0x0);//SYSCLK=MCLK
	wm8976_write_reg(0x4, 0x10);//16bit
	wm8976_write_reg(0x2B, 0x10); //BTL OUTPUT
	wm8976_write_reg(0x9, 0x50);//Jack detect enable
	wm8976_write_reg(0xD, 0x21);//Jack detect
	wm8976_write_reg(0x7, 0x01);//Jack detect
	//wm8976_set_volume(0);
}