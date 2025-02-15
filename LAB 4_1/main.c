#include "gd32vf103.h"
#include "drivers.h"
#include "lcd.h"

int main(void)
{
  int key;
  int lookUpTbl[16] = {1, 4, 7, 14, 2, 5, 8, 0, 3, 6, 9, 15, 10, 11, 12, 13};
  char ch[] = {126, 127, 128, 129, 130};

  t5omsi();                // Initialize timer5 1kHz
  colinit();               // Initialize column toolbox
  l88init();               // Initialize 8*8 led toolbox
  keyinit();               // Initialize keyboard toolbox
  Lcd_SetType(LCD_NORMAL); // INVERTED);                // or use LCD_INVERTED!
  Lcd_Init();
  LCD_Clear(RED);

  while (1)
  {
    LCD_WR_Queue(); // Manage LCD com queue!

    if (t5expq())
    {                   // Manage periodic tasks
      l88row(colset()); // ...8*8LED and Keyboard

      if ((key = keyscan()) >= 0)
      { // ...Any key pressed?
        switch (key)
        {
        case 1: // 4 - 100%
          LCD_ShowChar(10, 50, ch[0], OPAQUE, WHITE);
          break;
        case 8: // 3 - 75%
          LCD_ShowChar(10, 50, ch[1], OPAQUE, WHITE);
          break;
        case 4: // 2 - 50%
          LCD_ShowChar(10, 50, ch[2], OPAQUE, WHITE);
          break;
        case 0: // 1 - 25%
          LCD_ShowChar(10, 50, ch[3], OPAQUE, WHITE);
          break;
        case 7: // 0 - 0%
          LCD_ShowChar(10, 50, ch[4], OPAQUE, WHITE);
          break;
        }
      }
    }
  }
}
