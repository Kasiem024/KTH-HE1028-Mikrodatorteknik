#include "gd32vf103.h"
#include "drivers.h"
#include "adc.h"
#include "lcd.h"
#include "usart.h"

#include <stdio.h>

#define EI 1
#define DI 0

void rtcInit(void)
{
  // enable power managemenet unit - perhaps enabled by default
  rcu_periph_clock_enable(RCU_PMU);
  // enable write access to the registers in the backup domain
  pmu_backup_write_enable();
  // enable backup domain
  rcu_periph_clock_enable(RCU_BKPI);
  // reset backup domain registers
  bkp_deinit();
  // set the results of a previous calibration procedure
  // bkp_rtc_calibration_value_set(x);

  // setup RTC
  // enable external low speed XO
  // rcu_osci_on(RCU_LXTAL);
  if (rcu_osci_stab_wait(RCU_HXTAL))
  {
    // use external low speed oscillaotr, i.e. 32.768 kHz
    rcu_rtc_clock_config(RCU_RTCSRC_HXTAL_DIV_128);
    rcu_periph_clock_enable(RCU_RTC);
    // wait until shadow registers are synced from the backup domain
    // over the APB bus
    rtc_register_sync_wait();
    // wait until shadow register changes are synced over APB
    // to the backup doamin
    rtc_lwoff_wait();
    // prescale to 1 second
    rtc_prescaler_set(62500 - 1);
    rtc_lwoff_wait();
    rtc_flag_clear(RTC_INT_FLAG_SECOND);
    // rtc_interrupt_enable(RTC_INT_SECOND);
    rtc_lwoff_wait();
  }
}

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
    // u0_TX_Queue();                      // Manage U(S)ART TX Queue!

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

          firstKey = -1;
          secondKey = -1;
          amountChar++; // Increase the amount of characters
        }
      }

      adc_software_trigger_enable(ADC0, // Trigger another ADC conversion!
                                  ADC_REGULAR_CHANNEL);
    }
  }
}