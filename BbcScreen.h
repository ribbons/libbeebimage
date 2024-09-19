/*
 * Copyright © 2010-2021 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <stdint.h>

#define BBCSCREEN_PALETTE_SIZE 16    // Size of the BBC palette
#define BBCSCREEN_MAX_MEMSIZE  40960 // Size of screen memory storage

#ifdef __cplusplus
    #include <functional>

    class BbcScreen
    {
    public:
        static const int PALETTE_SIZE = BBCSCREEN_PALETTE_SIZE;
        static const int MAX_MEMSIZE  = BBCSCREEN_MAX_MEMSIZE;

        #undef BBCSCREEN_PALETTE_SIZE
        #undef BBCSCREEN_MAX_MEMSIZE

        typedef std::function<void(int x, int y, uint8_t colour)> DrawPixel;

        explicit BbcScreen(int screenMemSize);
        BbcScreen(int screenMemSize, uint8_t *screenData);

        void setMode(uint8_t mode);
        uint8_t getMode();
        void setScreenByte(int address, uint8_t byte);
        uint8_t getScreenByte(int address);
        int getScreenWidth();
        int getScreenHeight();
        void setColour(uint8_t colour, uint8_t value);
        uint8_t getColour(uint8_t colour);
        void draw(DrawPixel callback);

    private:
        int screenMemSize_;
        int screenWidth_;
        int screenHeight_;
        int blockRows_;
        uint8_t mode_ = 1;
        uint8_t palette_[PALETTE_SIZE];
        uint8_t screenStorage_[MAX_MEMSIZE];

        void init(int screenMemSize);
        void draw04(DrawPixel callback);
        void draw15(DrawPixel callback);
        void draw2(DrawPixel callback);
    };

    extern "C" {
#endif

    typedef struct BbcScreenS *BbcScreenP;
    typedef void (*DrawPixelP)(int x, int y, uint8_t colour);

    BbcScreenP BbcScreen_create(int screenMemSize);
    BbcScreenP BbcScreen_create2(int screenMemSize, uint8_t *screenData);
    void BbcScreen_delete(BbcScreenP screen);
    void BbcScreen_setMode(BbcScreenP screen, uint8_t mode);
    uint8_t BbcScreen_getMode(BbcScreenP screen);
    void BbcScreen_setScreenByte(BbcScreenP screen, int address, uint8_t byte);
    uint8_t BbcScreen_getScreenByte(BbcScreenP screen, int address);
    int BbcScreen_getScreenWidth(BbcScreenP screen);
    int BbcScreen_getScreenHeight(BbcScreenP screen);
    void BbcScreen_setColour(BbcScreenP screen, uint8_t colour, uint8_t value);
    uint8_t BbcScreen_getColour(BbcScreenP screen, uint8_t colour);
    void BbcScreen_draw(BbcScreenP screen, DrawPixelP callback);

#ifdef __cplusplus
    }
#endif
