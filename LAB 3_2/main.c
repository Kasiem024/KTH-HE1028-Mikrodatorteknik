#include "gd32vf103.h"
#include "drivers.h"
#include "adc.h"
#include "pwm.h"

int main(void)
{
  int ms = 0, s = 0, key, pKey = -1, c = 0, idle = 0, adcr, tmpr;
  int vehicles[49] = {0, 192, 192, 193, 192, 4, 4, 4, 4, 4, 4, 17, 21, 17, 31, 8, 194, 194, 192, 192, 3, 3, 35, 35, 8, 8, 1, 1, 248, 136, 168, 136, 248, 64, 3, 19, 19, 3, 70, 70, 6, 38, 32};

  t5omsi();           // Initialize timer5 1kHz
  colinit();          // Initialize column toolbox
  l88init();          // Initialize 8*8 led toolbox
  keyinit();          // Initialize keyboard toolbox
  ADC3powerUpInit(0); // Initialize ADC0, Ch3

  int death = 0, pixelToControl = 0, score = 0;

  while (1)
  {
    idle++; // Manage Async events

    if (adc_flag_get(ADC0, ADC_FLAG_EOC) == SET)
    { // ...ADC done?
      if (adc_flag_get(ADC0, ADC_FLAG_EOIC) == SET)
      { //...ch3 or ch16?
        tmpr = adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_0);

        l88mem(6, ((0x680 - tmpr) / 5) + 25);
        l88mem(6, tmpr >> 8); // ......move data
        l88mem(7, tmpr);      // ......(view each ms)

        adc_flag_clear(ADC0, ADC_FLAG_EOC);
        adc_flag_clear(ADC0, ADC_FLAG_EOIC);
      }
      else
      {
        adcr = adc_regular_data_read(ADC0); // ......get data
        adc_flag_clear(ADC0, ADC_FLAG_EOC); // ......clear IF
      }
    }

    if (t5expq())
    {                   // Manage periodic tasks
      l88row(colset()); // ...8*8LED and Keyboard
      ms++;             // ...One second heart beat
      if (ms == 300)
      {
        ms = 0;
        pixelToControl = 7 - ((adcr * 8) / 4096); // johan = imponerad

        l88mem(7, 1 << pixelToControl);

        if (!death)
        {
          for (int counter = 0; counter < 7; counter++)
          {
            l88mem(counter, vehicles[s + 7 - counter]);
          }

          s++;
          score++;
        }

        if ((vehicles[s] & (2 ^ (1 << (pixelToControl)))) > 0)
        {
          death = 1;

          for (int counter = 0; counter < 7; counter++)
          {
            l88mem(counter, 0);
          }

          l88mem(3, score);
        }

        if (s == 49)
        {
          s = 0;
        }
      }
      adc_software_trigger_enable(ADC0, // Trigger another ADC conversion!
                                  ADC_REGULAR_CHANNEL);
    }
  }
}