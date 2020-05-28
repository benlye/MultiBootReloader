/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "BootReloader.h"

/* Initializes the GPIO pins for inputs, outputs, and USARTs */
static void GPIO_Init()
{
	// Structure for configuring GPIO pins
	GPIO_InitTypeDef GPIO_InitStruct;

	// Enable the clocks
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// Configure PA1 and PA2 as outputs (red LED is PA1; green LED is PA2)
	GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/*
 * Handle for Timer 2
*/
static TIM_HandleTypeDef hTIM2;

/*
 * Initializes Timer 2
  * Frequency is 30Hz: 72000000 / ((239+1) * (9999+1)) = 30
 */
static void Timer_Init()
{
	__HAL_RCC_TIM2_CLK_ENABLE();	// Enable the clock

	hTIM2.Instance = TIM2;
	hTIM2.Init.Prescaler = 9999;
	hTIM2.Init.CounterMode = TIM_COUNTERMODE_UP;
	hTIM2.Init.Period = 239;
	hTIM2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hTIM2.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&hTIM2);

}

/*
 * Timer 2 interrupt handler override
 * Update interrupts are used to flash the red LED when the bootloader is active.
 */
extern "C" void TIM2_IRQHandler()
{
	HAL_TIM_IRQHandler(&hTIM2);
}

/*
 * Timer 2 update interrupt callback handler
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	// Toggle the LED pin
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
}

/*
 * Writes the bootloader data to the module's flash memory
 */
void writeBootloader() {

	uint32_t memAddress;

	for (int i = 0; i < 8192; i += 2)
	{
		memAddress = (FLASH_START + WRITE_OFFSET + i);
		// Check if this is the start of the page; if so we'll erase it
		if (((uint32_t)memAddress & PAGE_END) == 0)
		{
			// Clear the flash flags
			__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR);

			// Structure for erasing flash pages
			FLASH_EraseInitTypeDef EraseInitStruct;
			uint32_t SectorError = 0;

			// Configure the erase
			EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
			EraseInitStruct.PageAddress = (uint32_t)memAddress;
			EraseInitStruct.NbPages = 1;

			// Erase the page
			HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError);
		}

		// Get two bytes of data
		uint16_t data = bootloaderData[i + 1] << 8 | bootloaderData[i];

		// Write the data
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (uint32_t)memAddress, data);
	}
}

/*
 * Deletes the first page of application code so that we don't get into a re-flashing loop.
 * Ensures bootreloader application code can only execute once.
 */
void selfTerminate() 
{
	uint32_t memAddress = APPLICATION_START;

	// Clear the flash flags
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR);

	// Structure for erasing flash pages
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t SectorError = 0;

	// Configure the erase
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = (uint32_t)memAddress;
	EraseInitStruct.NbPages = 1;

	// Erase the page
	HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError);
}

// The setup() function runs once each time the micro-controller starts
void setup()
{
	// Initialize the STM32 HAL
	HAL_Init();

	// Initialize the timer
	Timer_Init();

	// Initialize the GPIO pins
	GPIO_Init();

	// Turn the LED on and wait 2s
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_Delay(2000);
}

// Add the main program code into the continuous loop() function
void loop()
{
	// Start blinking the LED really fast so the user knows we're doing scary stuff
	HAL_TIM_Base_Start_IT(&hTIM2);
	HAL_Delay(1000);

	// Unlock the flash
	HAL_FLASH_Unlock();

	// Rewrite the bootloader
	writeBootloader();
		
	// Stop blinking the LED
	HAL_TIM_Base_Stop(&hTIM2);

	// Turn the LED on for 1s to show we finished writing
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_Delay(1000);

	// Turn the LED off to show we're done
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

	// Delete ourself so we can't run again
	selfTerminate();

	// Should never get here
	while (1) {};
}
