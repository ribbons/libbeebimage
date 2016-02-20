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

#include "BbcImageLoader.h"

const int MEMSIZE012 = 20480; // Screen memory size for modes 0, 1 & 2
const int MEMSIZE45  = 10240; // Screen memory size for modes 4 & 5

BbcImageLoader::BbcImageLoader(uint8_t *data, int size)
{
    if(size <= 0 || size > BbcScreen::MAX_MEMSIZE)
    {
        throw std::invalid_argument("size must be greater than 0 and less than BbcScreen::MAX_MEMSIZE");
    }

    this->data = data;
    this->size = size;
}

BbcScreen* BbcImageLoader::LoadAuto()
{
    BbcScreen *screen;

    // Assume the file is LdPic format, and attempt to load it like that
    if((screen = LoadLdPic()) == NULL)
    {
        // File was not in LdPic format, load it as a memory dump
        screen = LoadMemDump();
    }

    return screen;
}

BbcScreen* BbcImageLoader::LoadMemDump()
{
    return new BbcScreen(this->size, this->data);
}

BbcScreen* BbcImageLoader::LoadLdPic()
{
    uint8_t outValBitSize;
    uint8_t mode;
    uint8_t colMapping;
    uint8_t stepSize;
    uint8_t repCountBits;
    uint8_t readMode;
    uint8_t repeatCount;
    uint8_t valToRepeat;

    this->pos = 0;

    // Read the number of bits to read for each image byte
    if(!getBits(8, &outValBitSize))
    {
        return NULL;
    }

    if(outValBitSize == 0 || outValBitSize > 8)
    {
        // As this is the number of bits to read, it can't be zero, and it can't be
        // larger than eight, as that would overflow the single byte storage.
        return NULL;
    }

    // Read the mode from the file
    if(!getBits(8, &mode))
    {
        return NULL;
    }

    int memSize;

    switch(mode % 8)
    {
        case 0:
        case 1:
        case 2:
            memSize = MEMSIZE012;
            break;
        case 4:
        case 5:
            memSize = MEMSIZE45;
            break;
        default:
            // Modes 3, 6 & 7 are not supported
            return NULL;
    }

    // Initialise a new BbcScreen instance and set the mode
    BbcScreen *screen = new BbcScreen(memSize);
    screen->setMode(mode);

    // Read the colour mappings from the file
    for(int8_t readPal = BbcScreen::PALETTE_SIZE - 1; readPal >= 0; readPal--)
    {
        if(!getBits(4, &colMapping))
        {
            delete screen;
            return NULL;
        }

        screen->setColour(readPal, colMapping);
    }

    // Read the number of bytes to move forward by after each byte is written to memory
    if(!getBits(8, &stepSize))
    {
        delete screen;
        return NULL;
    }

    if(stepSize == 0)
    {
        // This can't be zero, as only the first pixel would get written to
        delete screen;
        return NULL;
    }

    // Fetch the number of bits to read for each repeat count
    if(!getBits(8, &repCountBits))
    {
        delete screen;
        return NULL;
    }

    if(repCountBits == 0 || repCountBits > 8)
    {
        // As this is the number of bits to read, it can't be zero, and it can't be
        // larger than eight, as that would overflow the single byte storage.
        delete screen;
        return NULL;
    }

    // Start in the highest step position and work backwards
    int address = stepSize - 1;
    int progPos = address;

    for(;;)
    {
        // The next bit of the file shows whether to read just a single
        // value, or to read the number of repeats and a value
        if(!getBit(&readMode))
        {
            // Unexpected end of file
            delete screen;
            return NULL;
        }

        if(readMode == 0)
        {
            // Single value mode - only 1 repeat
            repeatCount = 1;
        }
        else
        {
            // Fetch the number of times the value should be repeated
            if(!getBits(repCountBits, &repeatCount))
            {
                // Unexpected end of file
                delete screen;
                return NULL;
            }

            if(repeatCount == 0)
            {
                // The value must be repeated at least once
                delete screen;
                return NULL;
            }
        }

        // Now fetch the value itself
        if(!getBits(outValBitSize, &valToRepeat))
        {
            // Unexpected end of file
            delete screen;
            return NULL;
        }

        // Output the value(s) to the file
        while(repeatCount > 0)
        {
            screen->setScreenByte(address, valToRepeat);
            address += stepSize;

            // Reached the end of the address space, wrap around and
            // store the previous step's values now
            if(address >= memSize)
            {
                if(progPos == 0)
                {
                    if(repeatCount > 1)
                    {
                        // Repeats are still remaining but the memory is full
                        delete screen;
                        return NULL;
                    }

                    // All of the screen memory has now had data loaded to it
                    return screen;
                }

                // Move back to start filling in the next step of values
                progPos--;
                address = progPos;
            }

            repeatCount--;
        }
    }
}

bool BbcImageLoader::getBit(uint8_t *bit)
{
    if(bitsLeft == 0)
    {
        if(this->pos == this->size)
        {
            // End of data
            return false;
        }

        bitStore = this->data[this->pos++];
        bitsLeft = 8;
    }

    // Fetch the leftmost bit
    *bit = (bitStore & 128) >> 7;

    // Shift the remaining bits across
    bitStore <<= 1;

    // Decrement the bytes left counter
    bitsLeft --;

    return true;
}

bool BbcImageLoader::getBits(int numBits, uint8_t *bits)
{
    *bits = 0;
    uint8_t addBit;

    // Must be between 1 and 8 bits that have been asked for
    if(numBits < 1 || numBits > 8)
    {
        throw std::invalid_argument("numBits must be between 1 and 8");
    }

    for(int bitCount = 0; bitCount < 8; bitCount++)
    {
        // Shift the bits in the byte one place to the right
        *bits >>= 1;

        if(bitCount < numBits)
        {
            if(!getBit(&addBit))
            {
                // End of file
                return false;
            }

            // Insert the returned bit as the msb of the byte
            *bits |= addBit << 7;
        }
    }

    return true;
}

BbcScreen* BbcImageLoader::LoadScrLoad()
{
    this->pos = 0;
    int screenSize = 0;
    int repeatCount;

    for(;;)
    {
        if(++this->pos == this->size)
        {
            // Unexpected EOF
            return NULL;
        }

        repeatCount = this->data[this->pos++];

        if(repeatCount == 0)
        {
            screenSize += 256;
        }
        else
        {
            screenSize += repeatCount;
        }

        if(this->pos == this->size)
        {
            // EOF
            break;
        }
    }

    BbcScreen *screen = new BbcScreen(screenSize);

    uint8_t valToRepeat;
    int address = 0;
    this->pos = 0;

    for(;;)
    {
        valToRepeat = this->data[this->pos++];
        repeatCount = this->data[this->pos++];

        if(repeatCount == 0)
        {
            repeatCount = 256;
        }

        for(int i = 0; i < repeatCount; i++)
        {
            screen->setScreenByte(address++, valToRepeat);
        }

        if(this->pos == this->size)
        {
            break;
        }
    }

    return screen;
}
