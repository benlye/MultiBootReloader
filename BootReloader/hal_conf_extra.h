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

/*
 * Disable the unused STM32 HAL modules - saves space and keeps the bootloader below 8KB
 * See https://github.com/stm32duino/wiki/wiki/HAL-configuration for more information.
 */
#define HAL_ADC_MODULE_DISABLED
#define HAL_I2C_MODULE_DISABLED
#define HAL_RTC_MODULE_DISABLED
#define HAL_SPI_MODULE_DISABLED
#define HAL_DAC_MODULE_DISABLED
#define HAL_ETH_MODULE_DISABLED
#define HAL_SD_MODULE_DISABLED
#define HAL_QSPI_MODULE_DISABLED
#define HAL_EXTI_MODULE_DISABLED
#define HAL_TIM_MODULE_DISABLED
