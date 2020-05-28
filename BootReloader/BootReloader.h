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

#ifndef __BOOTRELOADER_H
#define __BOOTRELOADER_H

#define FLASH_START (uint32_t)0x08000000
#define APPLICATION_START (uint32_t)0x08000000		// Should be 0x08002000
#define WRITE_OFFSET (uint32_t)0x10000				// Should be 0x0

#ifdef STM32F103xB
#define EEPROM_START (uint32_t)0x0801F800
#define RAM_SIZE (uint32_t)0x00005000
#define PAGE_END 0x000003FF
#endif
#ifdef STM32F303xC
#define EEPROM_START (uint32_t)0x0803F800
#define RAM_SIZE (uint32_t)0x0000A000
#define PAGE_END 0x000007FF
#endif

#endif /* __BOOTRELOADER_H */
