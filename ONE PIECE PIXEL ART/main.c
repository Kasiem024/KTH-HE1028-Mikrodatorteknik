#include "gd32vf103.h"
#include "drivers.h"
#include "lcd.h"

int main(void)
{
  int key;
  int lookUpTbl[16] = {1, 4, 7, 14, 2, 5, 8, 0, 3, 6, 9, 15, 10, 11, 12, 13};
  char ch[] = {126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143};

  t5omsi();                // Initialize timer5 1kHz
  colinit();               // Initialize column toolbox
  l88init();               // Initialize 8*8 led toolbox
  keyinit();               // Initialize keyboard toolbox
  Lcd_SetType(LCD_NORMAL); // INVERTED);                // or use LCD_INVERTED!
  Lcd_Init();
  LCD_Clear(WHITE);

  LCD_ShowChar(8, 0, ch[0], TRANSPARENT, BLACK);
  LCD_ShowChar(16, 0, ch[1], TRANSPARENT, BLACK);
  LCD_ShowChar(24, 0, ch[2], TRANSPARENT, BLACK);
  LCD_ShowChar(32, 0, ch[3], TRANSPARENT, BLACK);
  LCD_ShowChar(40, 0, ch[4], TRANSPARENT, BLACK);

  LCD_ShowChar(40, 0, ch[5], TRANSPARENT, YELLOW);
  LCD_ShowChar(16, 0, ch[6], TRANSPARENT, YELLOW);
  LCD_ShowChar(24, 0, ch[7], TRANSPARENT, YELLOW);
  LCD_ShowChar(32, 0, ch[8], TRANSPARENT, YELLOW);
  LCD_ShowChar(8, 0, ch[9], TRANSPARENT, YELLOW);

  LCD_ShowChar(16, 0, ch[10], TRANSPARENT, RED);
  LCD_ShowChar(24, 0, ch[11], TRANSPARENT, RED);
  LCD_ShowChar(32, 0, ch[12], TRANSPARENT, RED);

  LCD_ShowChar(8, 16, ch[13], TRANSPARENT, BLACK);
  LCD_ShowChar(16, 16, ch[14], TRANSPARENT, BLACK);
  LCD_ShowChar(24, 16, ch[15], TRANSPARENT, BLACK);
  LCD_ShowChar(32, 16, ch[16], TRANSPARENT, BLACK);
  LCD_ShowChar(40, 16, ch[17], TRANSPARENT, BLACK);

  while (1)
  {
    LCD_WR_Queue(); // Manage LCD com queue!

    if (t5expq())
    {                   // Manage periodic tasks
      l88row(colset()); // ...8*8LED and Keyboard
    }
  }
}
