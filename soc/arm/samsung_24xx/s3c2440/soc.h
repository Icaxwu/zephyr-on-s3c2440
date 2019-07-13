/*
 * Copyright (c) 2018, Cypress
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Board configuration macros
 *
 * This header file is used to specify and describe board-level aspects
 */

#ifndef _SOC__H_
#define _SOC__H_

#include "s3c2440_soc.h"

#ifndef _ASMLANGUAGE


void _WdogInit(void);
void _PlatformInit(void);
void _copy2sdram(void);

#else
	
GTEXT(_WdogInit)
GTEXT(_PlatformInit)
GTEXT(_copy2sdram)
#endif /* !_ASMLANGUAGE */


#endif /* _SOC__H_ */
