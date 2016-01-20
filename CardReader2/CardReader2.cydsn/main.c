/*******************************************************************************
* File Name: main.c
*
* Description:
*  This code example demonstrates USB HID interface class operation by 
*  implementing a 3-button mouse. When the code is run, the mouse cursor moves 
*  from the right to the left, and vice-versa.
*
*
* Based on Cypress AN58726 - USB HID Intermediate with PSoC 3 and PSoC 5LP
*
*******************************************************************************/

#include <project.h>
#include "kb_usb_hid.h"


/** @brief global buffer for keyboard led state */
uint8_t g_led_state = 0x0;


int main()
{
    CyGlobalIntEnable;
    kb_init();
    
    kb_puts("I can type anything and everything!!! =)");
    while(1) {}
}


/* [] END OF FILE */
