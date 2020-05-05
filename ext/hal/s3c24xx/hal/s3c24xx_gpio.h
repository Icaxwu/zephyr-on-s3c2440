#ifndef S3C24XX_GPIO__
#define S3C24XX_GPIO__


/**
  * @brief GPIO Port
  */
typedef struct {
  volatile unsigned int GP_CON;                          
  volatile unsigned int GP_DAT;                       
  volatile unsigned int GP_UP;                       
} S3C24XX_GPIO_Type;       

#endif