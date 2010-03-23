#pragma once

#define BBC_WIDTH0 640          // width of BBC MODE0 screen
#define BBC_WIDTH1 320          //       "      MODE1   "   
#define BBC_WIDTH2 160          //       "      MODE2   "   
#define BBC_WIDTH4 320          //       "      MODE4   "   
#define BBC_WIDTH5 160          //       "      MODE5   "    
#define BBC_XBLKS0 80			// Horizontal blocks on MODE 0 screen
#define BBC_XBLKS1 80			//           "          MODE 1   "   
#define BBC_XBLKS2 80			//           "          MODE 2   "   
#define BBC_XBLKS4 40			//           "          MODE 4   "   
#define BBC_XBLKS5 40			//           "          MODE 5   "   
#define BLOCK_BYTES 8           // number bytes in a block
#define PALETTE_SIZE 16         // Size of the BBC palette
#define DEFAULT_MODE 1          // Default screen mode

class BbcScreen
{
private:
	int screenMemSize_;
	int screenWidth_;
	int screenHeight_;
	int blockRows_;
	unsigned char mode_;
	unsigned char palette_[PALETTE_SIZE];
	unsigned char* screenStorage_;
	HBITMAP bitmap_;

	void genBitmap04(HDC bitmapDC);
	void genBitmap15(HDC bitmapDC);
	void genBitmap2(HDC bitmapDC);
	COLORREF convColour(int bbcColour);

public:
	BbcScreen(int screenMemSize);
	~BbcScreen();
	void setMode(int mode);
	unsigned char getMode();
	void setScreenByte(int address, unsigned char byte);
	int getScreenWidth();
	int getScreenHeight();
	void setColour(unsigned char colour, unsigned char value);
	unsigned char getColour(unsigned char colour);
	void generateBitmap(HWND hWnd);
	HBITMAP getBitmap();
};
