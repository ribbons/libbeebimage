#include "stdafx.h"
#include "BbcScreen.h"
#include <stdexcept>

BbcScreen::BbcScreen(int screenMemSize)
{
	// TODO: Sensible bounds checking
	screenMemSize_ = screenMemSize;
	screenStorage_ = new unsigned char[screenMemSize];

	bitmap_ = NULL;
	setMode(1);
}

void BbcScreen::setMode(int mode)
{
	mode = mode % 8;
	
	if(mode == 3 || mode == 6 || mode == 7)
	{
		throw std::invalid_argument("modes 3, 6 and 7 are not supported");
	}

	mode_ = mode;
	int blocksInFile = screenMemSize_ / BYTES;

	switch(mode_)
	{
		case 0:
			screenWidth_ = BBC_WIDTH0;
			blockRows_ = blocksInFile / BBC_XBLKS0;
			break;
		case 1:
			screenWidth_ = BBC_WIDTH1;
			blockRows_ = blocksInFile / BBC_XBLKS1;
			break;
		case 2:
			screenWidth_ = BBC_WIDTH2;
			blockRows_ = blocksInFile / BBC_XBLKS2;
			break;
		case 4:
			screenWidth_ = BBC_WIDTH4;
			blockRows_ = blocksInFile / BBC_XBLKS4;
			break;
		case 5:
			screenWidth_ = BBC_WIDTH5;
			blockRows_ = blocksInFile / BBC_XBLKS5;
			break;
	}

	if(blockRows_ == 0) {
		// The screen memory doesn't have even one row of blocks
		throw std::runtime_error("Allocated screen too small");
	}

	// Calculate how tall the image is based on the number of rows of blocks.
	screenHeight_ = blockRows_ * BYTES;

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
			for (int i = 0; i < 8; i++) {
				palette_[i] = i;
			}
			break;
	}
}

unsigned char BbcScreen::getMode()
{
	return mode_;
}

void BbcScreen::setScreenBit(int address, unsigned char bit)
{
	if(address >= screenMemSize_)
	{
		throw std::invalid_argument("address out of bounds");
	}

	screenStorage_[address] = bit;
}

int BbcScreen::getScreenWidth()
{
	return screenWidth_;
}

int BbcScreen::getScreenHeight()
{
	return screenHeight_;
}

void BbcScreen::setColour(unsigned char colour, unsigned char value)
{
	if(colour >= PALETTE_SIZE || colour < 0)
	{
		throw std::invalid_argument("Colour out of range");
	}

	if(value >= PALETTE_SIZE || value < 0)
	{
		throw std::invalid_argument("Value out of range");
	}

	palette_[colour] = value;
}

unsigned char BbcScreen::getColour(unsigned char colour)
{
	if(colour >= PALETTE_SIZE || colour < 0)
	{
		throw std::invalid_argument("Colour out of range");
	}

	return palette_[colour];
}

void BbcScreen::generateBitmap(HWND hWnd)
{
	// Clean up the previous bitmap if there is one
	if(bitmap_ != NULL)
	{
		DeleteBitmap(bitmap_);
	}
	
	// Get the handle of the screen DC & create a compatible bitmap
	HDC screenDC = GetDC(hWnd);
	bitmap_ = CreateCompatibleBitmap(screenDC, screenWidth_, screenHeight_);

	// Create a DC for the bitmap & release the screen DC
	HDC bitmapDC = CreateCompatibleDC(screenDC);
	ReleaseDC(hWnd, screenDC);

	// Save handle of the old bitmap & select new bitmap
	HBITMAP oldBitmap = SelectBitmap(bitmapDC, bitmap_);
	
	switch(mode_)
	{
		case 0:
		case 4:
			genBitmap04(bitmapDC);
			break;
		case 1:
		case 5:
			genBitmap15(bitmapDC);
			break;
		case 2:
			genBitmap2(bitmapDC);
			break;
	}

	// Select the previous bitmap & release the DC
	SelectBitmap(bitmapDC, oldBitmap);
	DeleteDC(bitmapDC);
}

// MODE 0 or MODE 4 picture
void BbcScreen::genBitmap04(HDC bitmapDC)
{
   int bit, i, j, k;
   unsigned char thisByte;
   unsigned int index;
   int nX = 0;
   int nY = 0;
   int nBlocks = BBC_XBLKS0;
   
   if(mode_ == 4) {
      nBlocks = BBC_XBLKS4;
   }

   int address = 0;

   for(k = 0; k < blockRows_; k++)
   {
	   for(j = 0; j < nBlocks; j++)
	   {
		   for(i = 0; i < BYTES; i++)
		   {
			   thisByte = screenStorage_[address];

			   for(bit = 0; bit < 8; bit++)
			   {
				   index = thisByte & 1;
				   SetPixel(bitmapDC, (nX+7)-bit, nY+i, convColour(index));
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
void BbcScreen::genBitmap15(HDC bitmapDC)
{
   int i, j, k;
   unsigned char thisByte;
   unsigned char index;
   int nX = 0;
   int nY = 0;
   int nBlocks = BBC_XBLKS1;

   if(mode_ == 5) {
      nBlocks = BBC_XBLKS5;
   }

   int address = 0;

   for(k = 0; k < blockRows_; k++)
   {
	   for(j = 0; j < nBlocks; j++)
	   {
		   for(i = 0; i < BYTES; i++)
		   {
			   thisByte = screenStorage_[address];

			   index = ((thisByte >> 6) & 2) | ((thisByte >> 3) & 1);
			   SetPixel(bitmapDC, nX, nY+i, convColour(index));

			   index = ((thisByte >> 5) & 2) | ((thisByte >> 2) & 1);
			   SetPixel(bitmapDC, nX+1, nY+i, convColour(index));

			   index = ((thisByte >> 4) & 2) | ((thisByte >> 1) & 1);
			   SetPixel(bitmapDC, nX+2, nY+i, convColour(index));

			   index = ((thisByte >> 3) & 2) | (thisByte & 1);
			   SetPixel(bitmapDC, nX+3, nY+i, convColour(index));

			   address++;
		   }

		   nX = nX + 4;
	   }
	   nX = 0;
       nY = nY + 8;
   }
}

// MODE 2 picture
void BbcScreen::genBitmap2(HDC bitmapDC)
{
   int i, j, k;
   unsigned char thisByte;
   unsigned char index;
   int nX = 0;
   int nY = 0;

   int address = 0;

   for(k = 0; k < blockRows_; k++) {
      for(j = 0; j < BBC_XBLKS2; j++) {
	     for(i = 0; i < BYTES; i++) {
            thisByte = screenStorage_[address];

            index = ((thisByte >> 4) & 8) | ((thisByte >> 3) & 4) | ((thisByte >> 2) & 2) | ((thisByte >> 1) & 1);
            SetPixel(bitmapDC, nX, nY+i, convColour(index));

            index = ((thisByte >> 3) & 8) | ((thisByte >> 2) & 4) | ((thisByte >> 1) & 2) | (thisByte  & 1);
            SetPixel(bitmapDC, nX+1, nY+i, convColour(index));

			address++;
         }
         nX = nX + 2;
      }
      nX = 0;
      nY = nY + 8;
   }
}

COLORREF BbcScreen::convColour(int bbcColour)
{
	if(bbcColour >= PALETTE_SIZE || bbcColour < 0)
	{
		throw std::invalid_argument("Colour out of range");
	}
	
	switch(palette_[bbcColour]) {
		case 0:
			return RGB(0, 0, 0);
		case 1:
			return RGB(255, 0, 0);
		case 2:
			return RGB(0, 255, 0);
		case 3:
			return RGB(255, 255, 0);
		case 4:
			return RGB(0, 0, 255);
		case 5:
			return RGB(255, 0, 255);
		case 6:
			return RGB(0, 255, 255);
		case 7:
			return RGB(255, 255, 255);
		default:
			// Map flashing colours to grey
			return RGB(128, 128, 128);
	}
}

HBITMAP BbcScreen::getBitmap()
{
	return bitmap_;
}