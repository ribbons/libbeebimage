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

#include <cstring>
#include <stdexcept>

#include "BbcScreen.h"

const int BLOCK_BYTES  = 8;     // Number bytes in a block
const int WIDTH0       = 640;   // Width of BBC MODE 0 screen
const int WIDTH14      = 320;   // Width of BBC MODE 1 & 4 screens
const int WIDTH25      = 160;   // Width of BBC MODE 2 & 5 screens
const int XBLKS012     = 80;    // Horizontal blocks in MODE 0, 1 & 2 screens
const int XBLKS45      = 40;    // Horizontal blocks in MODE 4 & 5 screens

void BbcScreen::init(int screenMemSize)
{
    if(screenMemSize > MAX_MEMSIZE)
    {
        throw std::invalid_argument("Requested screen memory size too large");
    }

    screenMemSize_ = screenMemSize;
    setMode(mode_);
}

BbcScreen::BbcScreen(int screenMemSize)
{
    init(screenMemSize);

    // Initialise the screen memory to all zeros (to match BBC)
    for(int init = 0; init < screenMemSize; init++)
    {
        screenStorage_[init] = 0;
    }
}

BbcScreen::BbcScreen(int screenMemSize, uint8_t *screenData)
{
    init(screenMemSize);
    memcpy(screenStorage_, screenData, screenMemSize);
}

void BbcScreen::setMode(uint8_t mode)
{
    mode = mode % 8;

    if(mode == 3 || mode == 6 || mode == 7)
    {
        throw std::invalid_argument("modes 3, 6 and 7 are not supported");
    }

    mode_ = mode;
    int blocksInFile = screenMemSize_ / BLOCK_BYTES;

    switch(mode_)
    {
        case 0:
            screenWidth_ = WIDTH0;
            blockRows_ = blocksInFile / XBLKS012;
            break;
        case 1:
            screenWidth_ = WIDTH14;
            blockRows_ = blocksInFile / XBLKS012;
            break;
        case 2:
            screenWidth_ = WIDTH25;
            blockRows_ = blocksInFile / XBLKS012;
            break;
        case 4:
            screenWidth_ = WIDTH14;
            blockRows_ = blocksInFile / XBLKS45;
            break;
        case 5:
            screenWidth_ = WIDTH25;
            blockRows_ = blocksInFile / XBLKS45;
            break;
    }

    if(blockRows_ == 0) {
        // The screen memory doesn't have even one row of blocks
        throw std::runtime_error("Allocated screen too small");
    }

    // Calculate how tall the image is based on the number of rows of blocks.
    screenHeight_ = blockRows_ * BLOCK_BYTES;

    // Set the default colour mappings for the mode
    switch(mode)
    {
        case 0:
        case 4:
            palette_[0] = 0;
            palette_[1] = 7;
            break;
        case 1:
        case 5:
            palette_[0] = 0;
            palette_[1] = 1;
            palette_[2] = 3;
            palette_[3] = 7;
            break;
        case 2:
            for(uint8_t i = 0; i < PALETTE_SIZE; i++)
            {
                palette_[i] = i;
            }
            break;
    }
}

uint8_t BbcScreen::getMode()
{
    return mode_;
}

void BbcScreen::setScreenByte(int address, uint8_t byte)
{
    if(address >= screenMemSize_)
    {
        throw std::invalid_argument("address out of bounds");
    }

    screenStorage_[address] = byte;
}

uint8_t BbcScreen::getScreenByte(int address)
{
    if(address >= screenMemSize_)
    {
        throw std::invalid_argument("address out of bounds");
    }

    return screenStorage_[address];
}

int BbcScreen::getScreenWidth()
{
    return screenWidth_;
}

int BbcScreen::getScreenHeight()
{
    return screenHeight_;
}

void BbcScreen::setColour(uint8_t colour, uint8_t value)
{
    if(colour >= PALETTE_SIZE)
    {
        throw std::invalid_argument("Colour out of range");
    }

    if(value >= PALETTE_SIZE)
    {
        throw std::invalid_argument("Value out of range");
    }

    palette_[colour] = value;
}

uint8_t BbcScreen::getColour(uint8_t colour)
{
    if(colour >= PALETTE_SIZE)
    {
        throw std::invalid_argument("Colour out of range");
    }

    return palette_[colour];
}

void BbcScreen::draw(DrawPixel callback)
{
    switch(mode_)
    {
        case 0:
        case 4:
            draw04(callback);
            break;
        case 1:
        case 5:
            draw15(callback);
            break;
        case 2:
            draw2(callback);
            break;
    }
}

// MODE 0 or MODE 4 picture
void BbcScreen::draw04(DrawPixel callback)
{
   int bit, i, j, k;
   uint8_t thisByte;
   uint8_t index;
   int nX = 0;
   int nY = 0;
   int nBlocks = XBLKS012;

   if(mode_ == 4) {
      nBlocks = XBLKS45;
   }

   int address = 0;

   for(k = 0; k < blockRows_; k++)
   {
       for(j = 0; j < nBlocks; j++)
       {
           for(i = 0; i < BLOCK_BYTES; i++)
           {
               thisByte = screenStorage_[address];

               for(bit = 0; bit < 8; bit++)
               {
                   index = thisByte & 1;
                   callback((nX+7)-bit, nY+i, getColour(index));
                   thisByte = thisByte >> 1;
               }

               address++;
           }

           nX = nX + 8;
       }

       nX = 0;
       nY = nY + 8;
   }
}

// MODE 1 or MODE 5 picture
void BbcScreen::draw15(DrawPixel callback)
{
   int i, j, k;
   uint8_t thisByte;
   uint8_t index;
   int nX = 0;
   int nY = 0;
   int nBlocks = XBLKS012;

   if(mode_ == 5) {
      nBlocks = XBLKS45;
   }

   int address = 0;

   for(k = 0; k < blockRows_; k++)
   {
       for(j = 0; j < nBlocks; j++)
       {
           for(i = 0; i < BLOCK_BYTES; i++)
           {
               thisByte = screenStorage_[address];

               index = ((thisByte >> 6) & 2) | ((thisByte >> 3) & 1);
               callback(nX, nY+i, getColour(index));

               index = ((thisByte >> 5) & 2) | ((thisByte >> 2) & 1);
               callback(nX+1, nY+i, getColour(index));

               index = ((thisByte >> 4) & 2) | ((thisByte >> 1) & 1);
               callback(nX+2, nY+i, getColour(index));

               index = ((thisByte >> 3) & 2) | (thisByte & 1);
               callback(nX+3, nY+i, getColour(index));

               address++;
           }

           nX = nX + 4;
       }
       nX = 0;
       nY = nY + 8;
   }
}

// MODE 2 picture
void BbcScreen::draw2(DrawPixel callback)
{
   int i, j, k;
   uint8_t thisByte;
   uint8_t index;
   int nX = 0;
   int nY = 0;

   int address = 0;

   for(k = 0; k < blockRows_; k++) {
      for(j = 0; j < XBLKS012; j++) {
         for(i = 0; i < BLOCK_BYTES; i++) {
            thisByte = screenStorage_[address];

            index = ((thisByte >> 4) & 8) | ((thisByte >> 3) & 4) | ((thisByte >> 2) & 2) | ((thisByte >> 1) & 1);
            callback(nX, nY+i, getColour(index));

            index = ((thisByte >> 3) & 8) | ((thisByte >> 2) & 4) | ((thisByte >> 1) & 2) | (thisByte  & 1);
            callback(nX+1, nY+i, getColour(index));

            address++;
         }
         nX = nX + 2;
      }
      nX = 0;
      nY = nY + 8;
   }
}
