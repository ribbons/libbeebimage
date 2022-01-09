/*
 * Copyright © 2016 Matt Robinson
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
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

BbcScreenP BbcScreen_create2(int screenMemSize, uint8_t *screenData)
{
    try
    {
        return reinterpret_cast<BbcScreenP>(new BbcScreen(screenMemSize, screenData));
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
        screenPtr->setScreenByte(address, byte);
    }
    catch(...)
    {
        PrintException();
        std::terminate();
    }
}

uint8_t BbcScreen_getScreenByte(BbcScreenP screen, int address)
{
    try
    {
        BbcScreen *screenPtr = reinterpret_cast<BbcScreen*>(screen);
        return screenPtr->getScreenByte(address);
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
