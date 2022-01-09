/*
 * Copyright © 2016 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
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

BbcScreenP BbcImageLoader_LoadScrLoad(uint8_t *data, int size)
{
    try
    {
        BbcImageLoader loader(data, size);
        return reinterpret_cast<BbcScreenP>(loader.LoadScrLoad());
    }
    catch(...)
    {
        PrintException();
        std::terminate();
    }
}
