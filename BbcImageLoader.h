/*
 * Copyright © 2010-2021 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
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

        uint8_t bitStore = 0;
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
