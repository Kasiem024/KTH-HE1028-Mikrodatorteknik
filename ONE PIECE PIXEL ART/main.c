#include "gd32vf103.h"
#include "drivers.h"
#include "lcd.h"

int main(void)
{
  int ms = 0, x = 0, y = 32, right = 1, down = 1;
  char ch[] = {126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149};

  t5omsi();                // Initialize timer5 1kHz
  colinit();               // Initialize column toolbox
  l88init();               // Initialize 8*8 led toolbox
  keyinit();               // Initialize keyboard toolbox
  Lcd_SetType(LCD_NORMAL); // INVERTED);              // or use LCD_INVERTED!
  Lcd_Init();
  LCD_Clear(WHITE);

  while (1)
  {
    LCD_WR_Queue(); // Manage LCD com queue!

    if (t5expq())
    {                   // Manage periodic tasks
      l88row(colset()); // ...8*8LED and Keyboard
      ms++;

      if (ms == 1)
      {

        // 1. Check boundaries *BEFORE* anything else.
        if (x + 48 >= 160) // Right edge
        {
          right = 0;
        }
        if (x <= 0) // Left edge
        {
          right = 1;
        }
        if (y + 48 >= 80) // Bottom edge
        {
          down = 0;
        }
        if (y <= 0) // Top edge
        {
          down = 1;
        }

        // 2. Update x and y *after* boundary checks, but *before* prev_x/y
        if (right)
        {
          x++;
        }
        else
        {
          x--;
        }

        if (down)
        {
          y++;
        }
        else
        {
          y--;
        }

        // 3. Calculate prev_x and prev_y *AFTER* x and y are updated.
        int prev_x = x - 1; // Simplified
        int prev_y = y - 1; // Simplified
        if (!right)
          prev_x = x + 1;
        if (!down)
          prev_y = y + 1;

        // Clear the previous frame.
        LCD_ShowChar(prev_x + 8, prev_y, ch[0], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 16, prev_y, ch[1], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 24, prev_y, ch[2], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 32, prev_y, ch[3], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 40, prev_y, ch[4], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x, prev_y, ch[23], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 40, prev_y, ch[5], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 16, prev_y, ch[6], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 24, prev_y, ch[7], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 32, prev_y, ch[8], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 8, prev_y, ch[9], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 16, prev_y, ch[10], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 24, prev_y, ch[11], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 32, prev_y, ch[12], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 8, prev_y + 16, ch[13], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 16, prev_y + 16, ch[14], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 24, prev_y + 16, ch[15], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 32, prev_y + 16, ch[16], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 40, prev_y + 16, ch[17], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 8, prev_y + 32, ch[18], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 16, prev_y + 32, ch[19], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 24, prev_y + 32, ch[20], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 32, prev_y + 32, ch[21], TRANSPARENT, WHITE);
        LCD_ShowChar(prev_x + 40, prev_y + 32, ch[22], TRANSPARENT, WHITE);

        // Draw new position
        LCD_ShowChar(x + 8, y, ch[0], TRANSPARENT, BLACK);
        LCD_ShowChar(x + 16, y, ch[1], TRANSPARENT, BLACK);
        LCD_ShowChar(x + 24, y, ch[2], TRANSPARENT, BLACK);
        LCD_ShowChar(x + 32, y, ch[3], TRANSPARENT, BLACK);
        LCD_ShowChar(x + 40, y, ch[4], TRANSPARENT, BLACK);
        LCD_ShowChar(x, y, ch[23], TRANSPARENT, BLACK);
        LCD_ShowChar(x + 40, y, ch[5], TRANSPARENT, YELLOW);
        LCD_ShowChar(x + 16, y, ch[6], TRANSPARENT, YELLOW);
        LCD_ShowChar(x + 24, y, ch[7], TRANSPARENT, YELLOW);
        LCD_ShowChar(x + 32, y, ch[8], TRANSPARENT, YELLOW);
        LCD_ShowChar(x + 8, y, ch[9], TRANSPARENT, YELLOW);
        LCD_ShowChar(x + 16, y, ch[10], TRANSPARENT, RED);
        LCD_ShowChar(x + 24, y, ch[11], TRANSPARENT, RED);
        LCD_ShowChar(x + 32, y, ch[12], TRANSPARENT, RED);
        LCD_ShowChar(x + 8, y + 16, ch[13], TRANSPARENT, BLACK);
        LCD_ShowChar(x + 16, y + 16, ch[14], TRANSPARENT, BLACK);
        LCD_ShowChar(x + 24, y + 16, ch[15], TRANSPARENT, BLACK);
        LCD_ShowChar(x + 32, y + 16, ch[16], TRANSPARENT, BLACK);
        LCD_ShowChar(x + 40, y + 16, ch[17], TRANSPARENT, BLACK);
        LCD_ShowChar(x + 8, y + 32, ch[18], TRANSPARENT, BLACK);
        LCD_ShowChar(x + 16, y + 32, ch[19], TRANSPARENT, BLACK);
        LCD_ShowChar(x + 24, y + 32, ch[20], TRANSPARENT, BLACK);
        LCD_ShowChar(x + 32, y + 32, ch[21], TRANSPARENT, BLACK);
        LCD_ShowChar(x + 40, y + 32, ch[22], TRANSPARENT, BLACK);

        ms = 0;
      }
    }
  }
}