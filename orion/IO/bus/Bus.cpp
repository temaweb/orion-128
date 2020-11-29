/*
 * This file is part of the Orion-128 distribution (https://github.com/temaweb/orion-128).
 * Copyright (c) 2020 Artem Okonechnikov.
 *
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

#include "IOBus.hpp"

Space IOBus::getSpace() const
{
    return
    {
        0x0000,
        0xFFFF
    };
}

uint8_t IOBus::read(uint16_t address) const
{
    return getRDevice(address) -> read(address);
}

void IOBus::write(uint16_t address, uint8_t data)
{
    return getWDevice(address) -> write(address, data);
}
