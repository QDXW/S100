/*
 * Interface_Language.h
 *
 *  Created on: 2018Äê11ÔÂ29ÈÕ
 *      Author: Administrator
 */

#ifndef MANAGEMENT_INTERFACE_INTERFACE_LANGUAGE_H_
#define MANAGEMENT_INTERFACE_INTERFACE_LANGUAGE_H_

/******************************************************************************/
#include "Version_selection.h"

/******************************************************************************/
typedef struct {
	uint8 char_status;
	uint8 char_enabled;					/* Support char or not */
	char_attr char_attr;				/* char attribute */
} block_Language_attr;

/******************************************************************************/
void Language_Set(void);
void UI_Draw_Window_Language(uint16 blockNum);

#endif /* MANAGEMENT_INTERFACE_INTERFACE_LANGUAGE_H_ */
