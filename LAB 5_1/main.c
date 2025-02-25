#include "gd32vf103.h"
#include "drivers.h"
#include "adc.h"
#include "lcd.h"
#include "usart.h"

#include <stdio.h>

#define EI 1
#define DI 0

int main(void)
{
  int key, firstKey = -1, secondKey = -1, amountChar = 0;
  int lookUpTbl[16] = {1, 4, 7, 14, 2, 5, 8, 0, 3, 6, 9, 15, 10, 11, 12, 13};

  t5omsi();                // Initialize timer5 1kHz
  colinit();               // Initialize column toolbox
  l88init();               // Initialize 8*8 led toolbox
  keyinit();               // Initialize keyboard toolbox
  ADC3powerUpInit(1);      // Initialize ADC0, Ch3
  Lcd_SetType(LCD_NORMAL); // or use LCD_INVERTED!
  Lcd_Init();
  LCD_Clear(BLACK);
  u0init(EI); // Initialize USART0 toolbox

  eclic_global_interrupt_enable(); // !!! INTERRUPT ENABLED !!!

  while (1)
  {
    LCD_WR_Queue(); // Manage LCD com queue!

    if (usart_flag_get(USART0, USART_FLAG_RBNE))
    { // USART0 RX?
      LCD_ShowChar(amountChar * 10, 10, usart_data_receive(USART0), OPAQUE, WHITE);
    }

    if (t5expq())
    {                   // Manage periodic tasks
      l88row(colset()); // ...8*8LED and Keyboard

      if ((key = keyscan()) >= 0)
      { // ...Any key pressed?

        int currentKey = lookUpTbl[key];

        if (firstKey == -1 && currentKey != 0 && currentKey != 1) // If firstKey doesn't have a value and the pressed key isn't 1 or 0
        {
          firstKey = currentKey;
        }
        else if (firstKey != -1) // If firstKey has a value
        {
          secondKey = currentKey;

          uint8_t hexValue = (firstKey << 4) | secondKey; // Left shift firstKey and do an OR with secondKey
          putch(hexValue);

          firstKey = -1;  // Reset
          secondKey = -1; // Reset
          amountChar++;   // Increase the amount of characters
        }
      }
    }
  }
}