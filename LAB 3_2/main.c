#include "gd32vf103.h"
#include "drivers.h"
#include "adc.h"
#include "pwm.h"

int main(void)
{
  int alive = 1, playerPosition = 0, currentObstacle = 0, points = 0, ms = 0, potentiometerValue;
  int obstacles[50] = {192, 192, 193, 192, 4, 4, 0, 128, 159, 17, 21, 17, 31, 0, 8, 0, 194, 194, 192, 192, 0, 3, 3, 35, 35, 8, 8, 1, 1, 248, 136, 168, 136, 248, 64, 64, 0, 4, 19, 19, 3, 0, 0, 192, 192, 193, 192, 4, 4};

  t5omsi();           // Initialize timer5 1kHz
  colinit();          // Initialize column toolbox
  l88init();          // Initialize 8*8 led toolbox
  keyinit();          // Initialize keyboard toolbox
  ADC3powerUpInit(0); // Initialize ADC0, Ch3

  while (1) // Infinite loop
  {
    if (adc_flag_get(ADC0, ADC_FLAG_EOC) == SET) // If ADC is done
    {
      potentiometerValue = adc_regular_data_read(ADC0); // Get value of potentiometer
      adc_flag_clear(ADC0, ADC_FLAG_EOC);               // ......clear IF
    }

    if (t5expq())
    {                   // Manage periodic tasks
      l88row(colset()); // ...8*8LED and Keyboard
      ms++;             // One millisecond

      playerPosition = 7 - ((potentiometerValue * 8) / 4096); // Update the position of the player based on the potentiometer value
      l88mem(7, 1 << playerPosition);                         // Display the position of the player

      if (ms == (500 - (points * 2))) // If half a second has passed minus the score of the player, gets faster as the player gets more points
      {
        ms = 0; // Reset timer

        if (alive) // If the player is alive
        {
          for (int counter = 0; counter < 7; counter++)
          {
            l88mem(counter, obstacles[currentObstacle + 7 - counter]); // Go through each row of the display and update it with the next obstacle
          }

          currentObstacle++; // Move to the next obstacle
          points++;          // Increase player points
        }

        if ((obstacles[currentObstacle] & (1 << (playerPosition))) > 0) // If the position of the player overlaps with an obstacle
        {
          alive = 0; // Set the player to dead

          for (int counter = 0; counter < 7; counter++)
          {
            l88mem(counter, 0); // Go through each row of the display and clear it
          }

          l88mem(3, points); // Display the player's score
        }

        if (currentObstacle == 43) // If the last obstacle has been displayed
        {
          currentObstacle = 0; // Reset the obstacles
        }
      }

      adc_software_trigger_enable(ADC0, // Trigger another ADC conversion!
                                  ADC_REGULAR_CHANNEL);
    }
  }
}