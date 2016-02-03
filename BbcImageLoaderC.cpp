/*
 * This file is part of libbeebimage.
 * Copyright © 2016 by the authors - see the AUTHORS file for details.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>

#include "BbcImageLoader.h"

void PrintException();

BbcScreenP BbcImageLoader_LoadAuto(uint8_t *data, int size)
{
    try
    {
        BbcImageLoader loader(data, size);
        return reinterpret_cast<BbcScreenP>(loader.LoadAuto());
    }
    catch(...)
    {
        PrintException();
        std::terminate();
    }
}

BbcScreenP BbcImageLoader_LoadMemDump(uint8_t *data, int size)
{
    try
    {
        BbcImageLoader loader(data, size);
        return reinterpret_cast<BbcScreenP>(loader.LoadMemDump());
    }
    catch(...)
    {
        PrintException();
        std::terminate();
    }
}

BbcScreenP BbcImageLoader_LoadLdPic(uint8_t *data, int size)
{
    try
    {
        BbcImageLoader loader(data, size);
        return reinterpret_cast<BbcScreenP>(loader.LoadLdPic());
    }
    catch(...)
    {
        PrintException();
        std::terminate();
    }
}
