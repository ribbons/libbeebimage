/*
 * This file is part of libbeebimage.
 * Copyright © 2003-2016 by the authors - see the AUTHORS file for details.
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

#pragma once

#include <stdint.h>

#include "BbcScreen.h"

#ifdef __cplusplus
    class BbcImageLoader
    {
    public:
        BbcImageLoader(uint8_t *data, int size);
        BbcScreen* LoadAuto();
        BbcScreen* LoadMemDump();
        BbcScreen* LoadLdPic();
        BbcScreen* LoadScrLoad();

    private:
        bool getBit(uint8_t *bit);
        bool getBits(int numBits, uint8_t *bits);

        uint8_t *data;
        int size;
        int pos = 0;

        uint8_t bitStore;
        int bitsLeft = 0;
    };

    extern "C" {
#endif

    BbcScreenP BbcImageLoader_LoadAuto(uint8_t *data, int size);
    BbcScreenP BbcImageLoader_LoadMemDump(uint8_t *data, int size);
    BbcScreenP BbcImageLoader_LoadLdPic(uint8_t *data, int size);
    BbcScreenP BbcImageLoader_LoadScrLoad(uint8_t *data, int size);

#ifdef __cplusplus
    }
#endif
