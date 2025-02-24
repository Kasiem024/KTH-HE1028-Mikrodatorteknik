#include "gd32vf103.h"
#include "drivers.h"
#include "adc.h"
#include "lcd.h"
#include "usart.h"
#define EI 1
#define DI 0

int main(void)
{
  int key, currentReceiveIndex = 0, currentSendIndex = 0, sendDataNow = 0, amountCharSent = 0, currentLine = 0;
  char transmitData[7];
  int lookUpTbl[16] = {1, 4, 7, 14, 2, 5, 8, 0, 3, 6, 9, 15, 10, 11, 12, 13};

  t5omsi();                // Initialize timer5 1kHz
  colinit();               // Initialize column toolbox
  l88init();               // Initialize 8*8 led toolbox
  keyinit();               // Initialize keyboard toolbox
  ADC3powerUpInit(1);      // Initialize ADC0, Ch3
  Lcd_SetType(LCD_NORMAL); // or use LCD_INVERTED!
  Lcd_Init();
  LCD_Clear(BLACK);
  u0init(EI);                      // Initialize USART0 toolbox
  eclic_global_interrupt_enable(); // !!! INTERRUPT ENABLED !!!

  while (1)
  {
    LCD_WR_Queue(); // Manage LCD com queue!
    u0_TX_Queue();  // Manage U(S)ART TX Queue!

    if (t5expq())
    {
      l88row(colset()); // ...8*8LED and Keyboard

      if (sendDataNow)
      {
        char currentChar = (int)transmitData[currentSendIndex];
        usart_data_transmit(USART0, currentChar);
        currentSendIndex++;

        if (currentSendIndex == amountCharSent) // If we have sent the whole word
        {
          sendDataNow = 0;
          currentSendIndex = 0;
          amountCharSent = 0;
        }
      }

      if ((key = keyscan()) >= 0)
      {
        if (key == 12) // A
        {
          adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL); // Trigger another ADC conversion!

          if (adc_flag_get(ADC0, ADC_FLAG_EOC) == SET)
          { // ...ADC done?
            if (adc_flag_get(ADC0, ADC_FLAG_EOIC) == SET)
            { //...ch3 or ch16?
              int tmpr = adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_0);

              adc_flag_clear(ADC0, ADC_FLAG_EOC);
              adc_flag_clear(ADC0, ADC_FLAG_EOIC);

              tmpr = ((0x680 - tmpr) / 5) + 25;

              int sign = '+'; // +
              if (tmpr < 0)
              {
                sign = '-';   // -
                tmpr = -tmpr; // Make tmpr positive for consistent handling
              }

              int hundredsDigit = '0';
              if (tmpr >= 100)
              {
                hundredsDigit = '1';
                tmpr = tmpr % 100; // Extract the remaining two digits.
              }

              char tensDigit, unitsDigit;

              tensDigit = (tmpr / 10) + '0';
              unitsDigit = (tmpr % 10) + '0';

              char newDataTransmit[7];

              newDataTransmit[0] = 'I';
              newDataTransmit[1] = 'D';
              newDataTransmit[2] = sign;
              newDataTransmit[3] = hundredsDigit;
              newDataTransmit[4] = tensDigit;
              newDataTransmit[5] = unitsDigit;
              newDataTransmit[6] = 10;

              amountCharSent = sizeof(newDataTransmit);

              for (int i = 0; i < amountCharSent; i++)
              {
                transmitData[i] = newDataTransmit[i];
              }

              sendDataNow = 1;
            }
          }
        }
        else if (key == 13) // B
        {
          char newDataTransmit[7] = "ID-000\n";
          amountCharSent = sizeof(newDataTransmit);

          for (int i = 0; i < amountCharSent; i++)
          {
            transmitData[i] = newDataTransmit[i];
          }

          sendDataNow = 1;
        }
        else if (key == 14) // C
        {
          char newDataTransmit[7] = "ID*000\n";
          amountCharSent = sizeof(newDataTransmit);

          for (int i = 0; i < amountCharSent; i++)
          {
            transmitData[i] = newDataTransmit[i];
          }

          sendDataNow = 1;
        }
      }

      if (usart_flag_get(USART0, USART_FLAG_RBNE))
      {
        int receivedData = usart_data_receive(USART0);
        int receivedDataArray[7];

        receivedDataArray[currentReceiveIndex] = receivedData;

        if (receivedData == 10)
        {
          if (receivedDataArray[0] == 'I' &&
              receivedDataArray[1] == 'D' &&
              (receivedDataArray[2] == '+' || receivedDataArray[2] == '-') &&
              isdigit(receivedDataArray[3]) && // Check if it's a digit '0' to '9'
              isdigit(receivedDataArray[4]) &&
              isdigit(receivedDataArray[5]))
          {
            LCD_ShowStr(currentReceiveIndex * 10, currentLine * 15, " OK!", WHITE, TRANSPARENT);
          }
          else
          {
            LCD_ShowStr(currentReceiveIndex * 10, currentLine * 15, " ERR", WHITE, TRANSPARENT);
          }

          currentReceiveIndex = 0;
          currentLine++;
        }
        else
        {
          LCD_ShowChar(currentReceiveIndex * 10, currentLine * 15, receivedData, TRANSPARENT, WHITE);
          currentReceiveIndex++;
        }
      }
    }
  }
}