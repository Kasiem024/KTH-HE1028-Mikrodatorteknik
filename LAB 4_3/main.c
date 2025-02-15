#include "gd32vf103.h"
#include "drivers.h"
#include "adc.h"
#include "lcd.h"

int main(void)
{
  int ms = 0, s = 0, key, potentiometerValue, speed = 1000;
  int lookUpTbl[16] = {1, 4, 7, 14, 2, 5, 8, 0, 3, 6, 9, 15, 10, 11, 12, 13};
  uint32_t avr = 0;

  t5omsi();                // Initialize timer5 1kHz
  colinit();               // Initialize column toolbox
  l88init();               // Initialize 8*8 led toolbox
  keyinit();               // Initialize keyboard toolbox
  Lcd_SetType(LCD_NORMAL); // or use LCD_INVERTED!
  Lcd_Init();
  LCD_Clear(BLACK);
  ADC3powerUpInit(0); // Initialize ADC0, Ch3
  LCD_DrawPoint(0, 40, WHITE);

  while (1)
  {
    LCD_WR_Queue(); // Manage LCD com queue!

    if (adc_flag_get(ADC0, ADC_FLAG_EOC) == SET) // If ADC is done
    {
      potentiometerValue = adc_regular_data_read(ADC0); // Get value of potentiometer
      adc_flag_clear(ADC0, ADC_FLAG_EOC);               // ......clear IF
      avr += potentiometerValue;
    }

    if (t5expq())
    {                   // Manage periodic tasks
      l88row(colset()); // ...8*8LED and Keyboard
      ms++;             // ...One second heart beat
      if (ms == speed)
      {
        ms = 0;
        avr = 0;
        LCD_DrawPoint(0, potentiometerValue, WHITE);
        s++;
      }

      if ((key = keyscan()) >= 0)
      { // ...Any key pressed?
        switch (key)
        {
        case 8: // 3 - 1
          speed = 1000;
          break;
        case 4: // 2 - 0.1
          speed = 100;
          break;
        case 0: // 1 - 0.01
          speed = 10;
          break;
        }
      }

      adc_software_trigger_enable(ADC0, // Trigger another ADC conversion!
                                  ADC_REGULAR_CHANNEL);
    }
  }
}