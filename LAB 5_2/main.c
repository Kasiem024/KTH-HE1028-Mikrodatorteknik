#include "gd32vf103.h"
#include "drivers.h"
#include "adc.h"
#include "lcd.h"
#include "usart.h"

#include <string.h>

#define TRUE 1
#define FALSE 0

int main(void)
{
  int key, currentSendIndex = 0, transmitDataNow = FALSE, currentLine = 0, receivedDataArray[7], currentReceiveIndex = 0;
  char transmitData[7];

  t5omsi();                // Initialize timer5 1kHz
  colinit();               // Initialize column toolbox
  l88init();               // Initialize 8*8 led toolbox
  keyinit();               // Initialize keyboard toolbox
  ADC3powerUpInit(1);      // Initialize ADC0, Ch3
  Lcd_SetType(LCD_NORMAL); // or use LCD_INVERTED!
  Lcd_Init();
  LCD_Clear(BLACK);
  u0init(TRUE);                    // Initialize USART0 toolbox
  eclic_global_interrupt_enable(); // !!! INTERRUPT ENABLED !!!

  while (TRUE)
  {
    LCD_WR_Queue(); // Manage LCD com queue!
    u0_TX_Queue();  // Manage U(S)ART TX Queue!

    if (t5expq())
    {
      l88row(colset()); // ...8*8LED and Keyboard

      if (transmitDataNow == TRUE) // If there is data to transmit
      {
        usart_data_transmit(USART0, transmitData[currentSendIndex]); // Transmit one char at a time
        currentSendIndex++;                                          // Go to next char to transmit

        if (currentSendIndex == sizeof(transmitData)) // If we have sent the whole word
        {
          transmitDataNow = FALSE; // Reset
          currentSendIndex = 0;    // Reset
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
            {
              int tmpr = adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_0); // Get current temprature

              adc_flag_clear(ADC0, ADC_FLAG_EOC);
              adc_flag_clear(ADC0, ADC_FLAG_EOIC);

              tmpr = ((0x680 - tmpr) / 5) + 25; // Convert tmpr to a normal value

              int sign = '+'; // Default
              if (tmpr < 0)
              {
                sign = '-';
                tmpr = -tmpr; // Make tmpr positive if it's negative
              }

              int hundredsDigit = '0'; // Default
              if (tmpr >= 100)
              {
                hundredsDigit = '1';
                tmpr = tmpr % 100; // Extract the remaining two digits
              }

              char tensDigit, unitsDigit;

              tensDigit = (tmpr / 10) + '0';  // Seperate into their own char
              unitsDigit = (tmpr % 10) + '0'; // Seperate into their own char

              transmitData[0] = 'I';
              transmitData[1] = 'D';
              transmitData[2] = sign;
              transmitData[3] = hundredsDigit;
              transmitData[4] = tensDigit;
              transmitData[5] = unitsDigit;
              transmitData[6] = 10;

              transmitDataNow = TRUE; // Ready to transmit data
            }
          }
        }
        else if (key == 13) // B
        {
          strcpy(transmitData, "ID-000\n");
          transmitDataNow = TRUE;
        }
        else if (key == 14) // C
        {
          strcpy(transmitData, "ID*000\n");
          transmitDataNow = TRUE;
        }
      }

      if (usart_flag_get(USART0, USART_FLAG_RBNE))
      {
        int receivedData = usart_data_receive(USART0); // Receive one char at a time

        receivedDataArray[currentReceiveIndex] = receivedData; // Put each char of the word in the correct position

        if (receivedData == 10) // If whole word has been received, \n
        {
          if (receivedDataArray[0] == 'I' &&
              receivedDataArray[1] == 'D' &&
              (receivedDataArray[2] == '+' || receivedDataArray[2] == '-') &&
              isdigit(receivedDataArray[3]) && // Check if it's a digit '0' to '9'
              isdigit(receivedDataArray[4]) &&
              isdigit(receivedDataArray[5]))
          { // If A or B has been pressed
            LCD_ShowStr(currentReceiveIndex * 10, currentLine * 15, " OK!", WHITE, TRANSPARENT);
          }
          else
          {
            LCD_ShowStr(currentReceiveIndex * 10, currentLine * 15, " ERR", WHITE, TRANSPARENT);
          }

          currentReceiveIndex = 0; // Reset
          currentLine++;           // Move down screen
        }
        else
        {
          LCD_ShowChar(currentReceiveIndex * 10, currentLine * 15, receivedData, TRANSPARENT, WHITE); // Print each char to screen
          currentReceiveIndex++;                                                                      // Go to next char and move right on screen
        }
      }
    }
  }
}