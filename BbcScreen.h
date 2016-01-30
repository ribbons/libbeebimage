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

#include <functional>

class BbcScreen
{
public:
    static const int PALETTE_SIZE = 16;    // Size of the BBC palette
    static const int MAX_MEMSIZE  = 40960; // Size of screen memory storage

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
