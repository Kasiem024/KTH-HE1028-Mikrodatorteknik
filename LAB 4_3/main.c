#include "gd32vf103.h"
#include "drivers.h"
#include "adc.h"
#include "lcd.h"

int main(void)
{
  int ms = 0, key, potentiometerValue, speed = 10, xPosition = 0, yPosition;
  int lookUpTbl[16] = {1, 4, 7, 14, 2, 5, 8, 0, 3, 6, 9, 15, 10, 11, 12, 13};
  int array[160] = {0};

  t5omsi();                // Initialize timer5 1kHz
  colinit();               // Initialize column toolbox
  l88init();               // Initialize 8*8 led toolbox
  keyinit();               // Initialize keyboard toolbox
  Lcd_SetType(LCD_NORMAL); // or use LCD_INVERTED!
  Lcd_Init();
  LCD_Clear(BLACK);
  ADC3powerUpInit(0); // Initialize ADC0, Ch3

  while (1)
  {
    LCD_WR_Queue(); // Manage LCD com queue!

    if (adc_flag_get(ADC0, ADC_FLAG_EOC) == SET) // If ADC is done
    {
      potentiometerValue = adc_regular_data_read(ADC0); // Get value of potentiometer
      adc_flag_clear(ADC0, ADC_FLAG_EOC);               // ......clear IF
    }

    if (t5expq())
    {                   // Manage periodic tasks
      l88row(colset()); // ...8*8LED and Keyboard
      ms++;             // ...One second heart beat

      yPosition = (potentiometerValue * 79) / 4096;

      if (ms >= speed)
      {
        array[1] = yPosition; // Insert new y value

        int currentX = 159;
        while (currentX >= 0)
        {
          int previousX = currentX - 1;

          array[currentX] = array[previousX];                    // Shift pixel to the right
          LCD_DrawPoint_big(currentX, array[currentX], WHITE);   // Draw new position
          LCD_DrawPoint_big(previousX, array[previousX], BLACK); // Clear old position

          currentX--;
        }

        xPosition++;
        ms = 0;
      }

      if ((key = keyscan()) >= 0)
      { // ...Any key pressed?
        ms = 0;
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
        default:
          break;
        }
      }

      adc_software_trigger_enable(ADC0, // Trigger another ADC conversion!
                                  ADC_REGULAR_CHANNEL);
    }
  }
}