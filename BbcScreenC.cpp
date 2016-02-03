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

#include "BbcScreen.h"

void PrintException()
{
    try
    {
        // Re-throw the exception so we can handle it here
        throw;
    }
    catch(const std::exception& exp)
    {
        std::cerr << exp.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "A non-standard exception was thrown" << std::endl;
    }
}

BbcScreenP BbcScreen_create(int screenMemSize)
{
    try
    {
        return reinterpret_cast<BbcScreenP>(new BbcScreen(screenMemSize));
    }
    catch(...)
    {
        PrintException();
        std::terminate();
    }
}

void BbcScreen_delete(BbcScreenP screen)
{
    try
    {
        delete reinterpret_cast<BbcScreen*>(screen);
    }
    catch(...)
    {
        PrintException();
        std::terminate();
    }
}

void BbcScreen_setMode(BbcScreenP screen, uint8_t mode)
{
    try
    {
        BbcScreen *screenPtr = reinterpret_cast<BbcScreen*>(screen);
        screenPtr->setMode(mode);
    }
    catch(...)
    {
        PrintException();
        std::terminate();
    }
}

uint8_t BbcScreen_getMode(BbcScreenP screen)
{
    try
    {
        BbcScreen *screenPtr = reinterpret_cast<BbcScreen*>(screen);
        return screenPtr->getMode();
    }
    catch(...)
    {
        PrintException();
        std::terminate();
    }
}

void BbcScreen_setScreenByte(BbcScreenP screen, int address, uint8_t byte)
{
    try
    {
        BbcScreen *screenPtr = reinterpret_cast<BbcScreen*>(screen);
        return screenPtr->setScreenByte(address, byte);
    }
    catch(...)
    {
        PrintException();
        std::terminate();
    }
}

int BbcScreen_getScreenWidth(BbcScreenP screen)
{
    try
    {
        BbcScreen *screenPtr = reinterpret_cast<BbcScreen*>(screen);
        return screenPtr->getScreenWidth();
    }
    catch(...)
    {
        PrintException();
        std::terminate();
    }
}

int BbcScreen_getScreenHeight(BbcScreenP screen)
{
    try
    {
        BbcScreen *screenPtr = reinterpret_cast<BbcScreen*>(screen);
        return screenPtr->getScreenHeight();
    }
    catch(...)
    {
        PrintException();
        std::terminate();
    }
}

void BbcScreen_setColour(BbcScreenP screen, uint8_t colour, uint8_t value)
{
    try
    {
        BbcScreen *screenPtr = reinterpret_cast<BbcScreen*>(screen);
        return screenPtr->setColour(colour, value);
    }
    catch(...)
    {
        PrintException();
        std::terminate();
    }
}

uint8_t BbcScreen_getColour(BbcScreenP screen, uint8_t colour)
{
    try
    {
        BbcScreen *screenPtr = reinterpret_cast<BbcScreen*>(screen);
        return screenPtr->getColour(colour);
    }
    catch(...)
    {
        PrintException();
        std::terminate();
    }
}

void BbcScreen_draw(BbcScreenP screen, DrawPixelP callback)
{
    try
    {
        BbcScreen *screenPtr = reinterpret_cast<BbcScreen*>(screen);
        return screenPtr->draw(static_cast<BbcScreen::DrawPixel>(callback));
    }
    catch(...)
    {
        PrintException();
        std::terminate();
    }
}
