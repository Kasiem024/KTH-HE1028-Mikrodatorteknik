#include "gd32vf103.h"
#include "drivers.h"
#include "adc.h"
#include "pwm.h"

int main(void)
{
  int ms = 0, s = 0, key, pKey = -1, c = 0, idle = 0, adcr, tmpr;
  // int lookUpTbl[16] = {7, 0, 4, 8, 1, 5, 9, 2, 6, 10, 12, 13, 14, 15, 3, 11};
  //                  {0, 1, 2, 3, 4, 5, 6, 7, 8,  9,   A, B,  C,  D, *,  #};
  int lookUpTbl[16] = {1, 4, 7, 14, 2, 5, 8, 0, 3, 6, 9, 15, 10, 11, 12, 13};
  //                  {1, 1, 2, 3, 4, 5, 6, 7, 8,  9,   A, B,  C,  D, *,  #};

  t5omsi();           // Initialize timer5 1kHz
  colinit();          // Initialize column toolbox
  l88init();          // Initialize 8*8 led toolbox
  keyinit();          // Initialize keyboard toolbox
  ADC3powerUpInit(0); // Initialize ADC0, Ch3
  T1powerUpInitPWM(0x2);

  int tenNumber = -1, oneNumber = -1;
  double finalDimStrength = 0;

  while (1)
  {
    int continueLoop = 1;
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
        while (continueLoop)
        {
          if (key == 12)
          {
            T1setPWMch2(16000);
            continueLoop = 0;
            break;
          }
          if (key == 14)
          {
            T1setPWMch2(1);
            continueLoop = 0;
            break;
          }
          if (key == 15 && tenNumber != -1)
          {
            finalDimStrength = (((tenNumber * 10) + oneNumber) / 100.0) * 16000;
            T1setPWMch2(finalDimStrength);
            tenNumber = -1;
            oneNumber = -1;
            continueLoop = 0;
            break;
          }
          if (key == 11)
          {
            tenNumber = -1;
            oneNumber = -1;
            continueLoop = 0;
            break;
          }
          if (key == 3 && tenNumber != -1 && oneNumber == -1)
          {
            tenNumber = -1;
            continueLoop = 0;
            break;
          }
          if (key == 3 && tenNumber != -1 && oneNumber != -1)
          {
            oneNumber = -1;
            continueLoop = 0;
            break;
          }

          for (int counter = 0; counter < 16; counter++)
          {
            if (counter == lookUpTbl[key])
            {
              if (tenNumber == -1)
              {
                tenNumber = counter;
              }
              else if (oneNumber == -1)
              {
                oneNumber = counter;
              }
            }
          }

          break;
        }
      }
      l88mem(2, idle >> 8); // ...Performance monitor
      l88mem(3, idle);
      idle = 0;
      adc_software_trigger_enable(ADC0, // Trigger another ADC conversion!
                                  ADC_REGULAR_CHANNEL);
    }
  }
}

/*

Knapp 0 = 7
Knapp 1 = 0
Knapp 2 = 4
Knapp 3 = 8
Knapp 4 = 1
Knapp 5 = 5
Knapp 6 = 9
Knapp 7 = 2
Knapp 8 = 6
Knapp 9 = 10
Knapp A = 12
Knapp B = 13
Knapp C = 14
Knapp D = 15
Knapp * = 3
Knapp # = 11

*/