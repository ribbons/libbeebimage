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

        BbcScreen(int screenMemSize);
        void setMode(uint8_t mode);
        uint8_t getMode();
        void setScreenByte(int address, uint8_t byte);
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

        void draw04(DrawPixel callback);
        void draw15(DrawPixel callback);
        void draw2(DrawPixel callback);
    };

    extern "C" {
#endif

    typedef struct BbcScreenS *BbcScreenP;
    typedef void (*DrawPixelP)(int x, int y, uint8_t colour);

    BbcScreenP BbcScreen_create(int screenMemSize);
    void BbcScreen_delete(BbcScreenP screen);
    void BbcScreen_setMode(BbcScreenP screen, uint8_t mode);
    uint8_t BbcScreen_getMode(BbcScreenP screen);
    void BbcScreen_setScreenByte(BbcScreenP screen, int address, uint8_t byte);
    int BbcScreen_getScreenWidth(BbcScreenP screen);
    int BbcScreen_getScreenHeight(BbcScreenP screen);
    void BbcScreen_setColour(BbcScreenP screen, uint8_t colour, uint8_t value);
    uint8_t BbcScreen_getColour(BbcScreenP screen, uint8_t colour);
    void BbcScreen_draw(BbcScreenP screen, DrawPixelP callback);

#ifdef __cplusplus
    }
#endif
