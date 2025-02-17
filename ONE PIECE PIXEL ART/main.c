#include "gd32vf103.h"
#include "drivers.h"
#include "lcd.h"

void print25(int *x, int *forwards)
{
  char ch[] = {126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
               150, 151, 152, 153, 154};

  LCD_ShowChar(*x + 8, 32, ch[0], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 16, 32, ch[1], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 24, 32, ch[2], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 32, 32, ch[3], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 40, 32, ch[4], TRANSPARENT, BLACK);
  LCD_ShowChar(*x, 32, ch[23], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 40, 32, ch[5], TRANSPARENT, YELLOW);
  LCD_ShowChar(*x + 16, 32, ch[6], TRANSPARENT, YELLOW);
  LCD_ShowChar(*x + 24, 32, ch[7], TRANSPARENT, YELLOW);
  LCD_ShowChar(*x + 32, 32, ch[8], TRANSPARENT, YELLOW);
  LCD_ShowChar(*x + 8, 32, ch[9], TRANSPARENT, YELLOW);
  LCD_ShowChar(*x + 16, 32, ch[10], TRANSPARENT, RED);
  LCD_ShowChar(*x + 24, 32, ch[11], TRANSPARENT, RED);
  LCD_ShowChar(*x + 32, 32, ch[12], TRANSPARENT, RED);
  LCD_ShowChar(*x + 8, 48, ch[13], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 16, 48, ch[14], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 24, 48, ch[15], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 32, 48, ch[16], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 40, 48, ch[17], TRANSPARENT, BLACK);

  LCD_ShowChar(*x + 8, 64, ch[24], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 16, 64, ch[25], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 24, 64, ch[26], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 32, 64, ch[27], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 40, 64, ch[28], TRANSPARENT, BLACK);

  if (*forwards)
  {
    *x++;
    LCD_ShowChar(*x - 1 + 8, 32, ch[0], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 16, 32, ch[1], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 24, 32, ch[2], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 32, 32, ch[3], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 40, 32, ch[4], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1, 32, ch[23], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 40, 32, ch[5], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 16, 32, ch[6], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 24, 32, ch[7], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 32, 32, ch[8], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 8, 32, ch[9], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 16, 32, ch[10], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 24, 32, ch[11], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 32, 32, ch[12], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 8, 48, ch[13], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 16, 48, ch[14], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 24, 48, ch[15], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 32, 48, ch[16], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 40, 48, ch[17], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 8, 64, ch[18], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 16, 64, ch[19], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 24, 64, ch[20], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 32, 64, ch[21], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 40, 64, ch[22], TRANSPARENT, WHITE);
  }
  else
  {
    *x--;
    LCD_ShowChar(*x + 1 + 8, 32, ch[0], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 16, 32, ch[1], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 24, 32, ch[2], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 32, 32, ch[3], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 40, 32, ch[4], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1, 32, ch[23], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 40, 32, ch[5], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 16, 32, ch[6], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 24, 32, ch[7], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 32, 32, ch[8], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 8, 32, ch[9], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 16, 32, ch[10], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 24, 32, ch[11], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 32, 32, ch[12], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 8, 48, ch[13], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 16, 48, ch[14], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 24, 48, ch[15], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 32, 48, ch[16], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 40, 48, ch[17], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 8, 64, ch[18], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 16, 64, ch[19], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 24, 64, ch[20], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 32, 64, ch[21], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 40, 64, ch[22], TRANSPARENT, WHITE);
  }

  if (*x == 112)
  {
    *forwards = 0;
  }

  if (*x == 0)
  {
    *forwards = 1;
  }
}

void print0(int *x, int *forwards)
{
  char ch[] = {126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
               150, 151, 152, 153, 154};

  LCD_ShowChar(*x + 8, 32, ch[0], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 16, 32, ch[1], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 24, 32, ch[2], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 32, 32, ch[3], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 40, 32, ch[4], TRANSPARENT, BLACK);
  LCD_ShowChar(*x, 32, ch[23], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 40, 32, ch[5], TRANSPARENT, YELLOW);
  LCD_ShowChar(*x + 16, 32, ch[6], TRANSPARENT, YELLOW);
  LCD_ShowChar(*x + 24, 32, ch[7], TRANSPARENT, YELLOW);
  LCD_ShowChar(*x + 32, 32, ch[8], TRANSPARENT, YELLOW);
  LCD_ShowChar(*x + 8, 32, ch[9], TRANSPARENT, YELLOW);
  LCD_ShowChar(*x + 16, 32, ch[10], TRANSPARENT, RED);
  LCD_ShowChar(*x + 24, 32, ch[11], TRANSPARENT, RED);
  LCD_ShowChar(*x + 32, 32, ch[12], TRANSPARENT, RED);
  LCD_ShowChar(*x + 8, 48, ch[13], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 16, 48, ch[14], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 24, 48, ch[15], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 32, 48, ch[16], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 40, 48, ch[17], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 8, 64, ch[18], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 16, 64, ch[19], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 24, 64, ch[20], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 32, 64, ch[21], TRANSPARENT, BLACK);
  LCD_ShowChar(*x + 40, 64, ch[22], TRANSPARENT, BLACK);

  if (*forwards)
  {
    *x++;
    LCD_ShowChar(*x - 1 + 8, 32, ch[0], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 16, 32, ch[1], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 24, 32, ch[2], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 32, 32, ch[3], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 40, 32, ch[4], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1, 32, ch[23], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 40, 32, ch[5], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 16, 32, ch[6], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 24, 32, ch[7], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 32, 32, ch[8], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 8, 32, ch[9], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 16, 32, ch[10], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 24, 32, ch[11], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 32, 32, ch[12], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 8, 48, ch[13], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 16, 48, ch[14], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 24, 48, ch[15], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 32, 48, ch[16], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 40, 48, ch[17], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 8, 64, ch[18], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 16, 64, ch[19], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 24, 64, ch[20], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 32, 64, ch[21], TRANSPARENT, WHITE);
    LCD_ShowChar(*x - 1 + 40, 64, ch[22], TRANSPARENT, WHITE);
  }
  else
  {
    *x--;
    LCD_ShowChar(*x + 1 + 8, 32, ch[0], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 16, 32, ch[1], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 24, 32, ch[2], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 32, 32, ch[3], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 40, 32, ch[4], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1, 32, ch[23], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 40, 32, ch[5], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 16, 32, ch[6], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 24, 32, ch[7], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 32, 32, ch[8], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 8, 32, ch[9], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 16, 32, ch[10], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 24, 32, ch[11], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 32, 32, ch[12], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 8, 48, ch[13], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 16, 48, ch[14], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 24, 48, ch[15], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 32, 48, ch[16], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 40, 48, ch[17], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 8, 64, ch[18], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 16, 64, ch[19], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 24, 64, ch[20], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 32, 64, ch[21], TRANSPARENT, WHITE);
    LCD_ShowChar(*x + 1 + 40, 64, ch[22], TRANSPARENT, WHITE);
  }

  if (*x == 112)
  {
    *forwards = 0;
  }

  if (*x == 0)
  {
    *forwards = 1;
  }
}

int main(void)
{
  int key, ms = 0, x = 0, forwards = 1, procent = 7;
  int lookUpTbl[16] = {1, 4, 7, 14, 2, 5, 8, 0, 3, 6, 9, 15, 10, 11, 12, 13};

  t5omsi();                // Initialize timer5 1kHz
  colinit();               // Initialize column toolbox
  l88init();               // Initialize 8*8 led toolbox
  keyinit();               // Initialize keyboard toolbox
  Lcd_SetType(LCD_NORMAL); // INVERTED);                // or use LCD_INVERTED!
  Lcd_Init();
  LCD_Clear(WHITE);

  while (1)
  {
    LCD_WR_Queue(); // Manage LCD com queue!

    if (t5expq())
    {                   // Manage periodic tasks
      l88row(colset()); // ...8*8LED and Keyboard
      ms++;

      if (ms == 10)
      {
        switch (procent)
        {
        case 1: // 4 - 100%
          break;
        case 8: // 3 - 75%
          break;
        case 4: // 2 - 50%
          break;
        case 0: // 1 - 25%
          print25(&x, &forwards);
          break;
        case 7: // 0 - 0%
          print0(&x, &forwards);
          break;
        default:
          break;
        }
        ms = 0;
      }

      if ((key = keyscan()) >= 0)
      { // ...Any key pressed?
        switch (key)
        {
        case 1: // 4 - 100%
          break;
        case 8: // 3 - 75%
          break;
        case 4: // 2 - 50%
          break;
        case 0: // 1 - 25%
          procent = 0;
          break;
        case 7: // 0 - 0%
          procent = 7;
          break;
        default:
          break;
        }
      }
    }
  }
}