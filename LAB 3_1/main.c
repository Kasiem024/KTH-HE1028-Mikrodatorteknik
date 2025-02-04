#include "gd32vf103.h"
#include "drivers.h"
#include "adc.h"
#include "pwm.h"

int main(void)
{
  int ms = 0, s = 0, key, pKey = -1, c = 0, idle = 0, adcr, tmpr;
  int lookUpTbl[16] = {1, 4, 7, 14, 2, 5, 8, 0, 3, 6, 9, 15, 10, 11, 12, 13};

  t5omsi();           // Initialize timer5 1kHz
  colinit();          // Initialize column toolbox
  l88init();          // Initialize 8*8 led toolbox
  keyinit();          // Initialize keyboard toolbox
  ADC3powerUpInit(0); // Initialize ADC0, Ch3
  // T1powerUpInitPWM(0x4);                      // Timer #1, Ch #2 PWM
  // T1powerUpInitPWM(0xC);                      // Timer #1, Ch #2 & 3 PWM

  while (1)
  {
    idle++; // Manage Async events

    if (adc_flag_get(ADC0, ADC_FLAG_EOC) == SET)
    { // ...ADC done?
      if (adc_flag_get(ADC0, ADC_FLAG_EOIC) == SET)
      { //...ch3 or ch16?
        tmpr = adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_0);
        // l88mem(6,((0x680-tmpr)/5)+25);
        // l88mem(6,tmpr>>8);                  // ......move data
        // l88mem(7,tmpr);                     // ......(view each ms)
        adc_flag_clear(ADC0, ADC_FLAG_EOC);
        adc_flag_clear(ADC0, ADC_FLAG_EOIC);
      }
      else
      {
        adcr = adc_regular_data_read(ADC0); // ......get data
        l88mem(4, adcr & 0xFF);             // ......move data
        l88mem(5, adcr >> 8);               // ......(view each ms)
        adc_flag_clear(ADC0, ADC_FLAG_EOC); // ......clear IF
      }
    }

    if (t5expq())
    {                   // Manage periodic tasks
      l88row(colset()); // ...8*8LED and Keyboard
      ms++;             // ...One second heart beat
      if (ms == 1000)
      {
        ms = 0;
        l88mem(0, s++);
      }
      if ((key = keyscan()) >= 0)
      { // ...Any key pressed?
        if (pKey == key)
          c++;
        else
        {
          c = 0;
          pKey = key;
        }
        l88mem(1, lookUpTbl[key] + (c << 4));
      }
      l88mem(2, idle >> 8); // ...Performance monitor
      l88mem(3, idle);
      idle = 0;
      adc_software_trigger_enable(ADC0, // Trigger another ADC conversion!
                                  ADC_REGULAR_CHANNEL);
    }
  }
}