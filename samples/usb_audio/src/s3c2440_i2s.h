#ifndef S3C2440_IIS_H_
#define S3C2440_IIS_H_

#include <s3c2440_soc.h>

void init_IIS_bus(unsigned int fs);

static inline void send_iis_data(signed short data) 
{
    /* waiting empty */
	while (IISCON & 0x80);
	
    FENTRY = data;
}

static inline void fill_iis_data(short (*next)(void)) 
{
    /* waiting empty */
	while (IISCON & 0x80);

    int i = 32;
    while(i--){
        FENTRY = next();
    }
}

#endif