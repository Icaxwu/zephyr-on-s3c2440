#include <gpio.h>
#include <hal/s3c24xx_gpio.h>
#include <s3c2440_soc.h>

#include "gpio_utils.h"

struct gpio_s3c24xx_cfg {
	S3C24XX_GPIO_Type *port;
};

struct gpio_s3c24xx_data {
	sys_slist_t callbacks;

};

static inline struct gpio_s3c24xx_data *get_port_data(struct device *port)
{
	return port->driver_data;
}

static inline const struct gpio_s3c24xx_cfg *get_port_cfg(struct device *port)
{
	return port->config->config_info;
}

int gpio_s3c24xx_config(struct device *port, int access_op,
			     u32_t pin, int flags)
{
	const struct gpio_s3c24xx_cfg * cfg = get_port_cfg(port);

	cfg->port->GP_CON &= ~(3 << (pin*2));
	if ((flags & GPIO_DIR_MASK) == GPIO_DIR_OUT)
	{
		cfg->port->GP_CON |= 1 << (pin*2);
	}
	else
	{
		cfg->port->GP_CON &= ~(3 << (pin*2));
	}
	
	return 0;
}
				 
int gpio_s3c24xx_write(struct device *port, int access_op,
			    u32_t pin, u32_t value)
{
	const struct gpio_s3c24xx_cfg * cfg = get_port_cfg(port);

	if (value == 1)
	{	
		cfg->port->GP_DAT |= (1 << pin);
	}
	else
	{
		cfg->port->GP_DAT &= ~(1 << pin);
	}
	
	return 0;
}
				
int gpio_s3c24xx_read(struct device *port, int access_op,
			   u32_t pin, u32_t *value)
{
	return 0;
}
			   
int gpio_s3c24xx_manage_callback(struct device *port,
		      struct gpio_callback *callback,
		      bool set)
{
	return 0;
}
			  
int gpio_s3c24xx_pin_enable_callback(struct device *port,
		      int access_op,
		      u32_t pin)
{
	return 0;
}
			  
int gpio_s3c24xx_pin_disable_callback(struct device *port,
				       int access_op,
				       u32_t pin)
{
	return 0;
}



static const struct gpio_driver_api gpio_s3c24xx_drv_api_funcs = {
	.config = gpio_s3c24xx_config,
	.write = gpio_s3c24xx_write,
	.read = gpio_s3c24xx_read,
	.manage_callback = gpio_s3c24xx_manage_callback,
	.enable_callback = gpio_s3c24xx_pin_enable_callback,
	.disable_callback = gpio_s3c24xx_pin_disable_callback
};


static int gpio_s3c24xx_init(struct device *port)
{
    
	return 0;
}


#define GPIO_S3C24XX_DEVICE(id)						                    \
	static const struct gpio_s3c24xx_cfg gpio_s3c24xx_p##id##_cfg = {	\
		.port = (S3C24XX_GPIO_Type *)&GP##id##CON,			            \
	};								                                    \
									                                    \
	static struct gpio_s3c24xx_data gpio_s3c24xx_p##id##_data;		    \
									                                    \
	DEVICE_AND_API_INIT(gpio_s3c24xx_p##id,				                \
			    DT_SAMSUNG_S3C24XX_GPIO_GPIO_##id##_LABEL,	            \
			    gpio_s3c24xx_init,				                        \
 			    &gpio_s3c24xx_p##id##_data,			                    \
 			    &gpio_s3c24xx_p##id##_cfg,			                    \
			    POST_KERNEL,				                            \
			    CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,	                \
			    &gpio_s3c24xx_drv_api_funcs)


#ifdef CONFIG_GPIO_S3C24XX_PF
GPIO_S3C24XX_DEVICE(F);
#endif


