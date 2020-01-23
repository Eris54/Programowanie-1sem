#pragma once
#include <iostream>
#include <windows.h>
#include <cmath>
#include <string>
#include <time.h>
#include <fstream>

#include <ShellScalingApi.h>
#pragma comment(lib, "Shcore.lib")

using namespace std;

class Paint {
private:
	//windows shenanigans
	HDC device;
	HWND hwnd;
	RECT wndRect;
	HANDLE hOut;
	HANDLE hIn;
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	CONSOLE_CURSOR_INFO consoleCursor;
	DWORD normalMode;
	float screenScale;

	//variables
	COLORREF color;
	int r : 8, g : 8, b : 8;
	POINT mouse;
	POINT prevPix;
	char pixArray[512][512][3]; //it's at the limit of the stack overflow lol
	short choice;
	string path;
	string Iunderstand;

	//flags
	bool welcome : 1;
	bool saved : 1;
	bool againMenu : 1;

	//functions
	void menuState();
	void whyUGottaResize();

	void consoleSetup();
	void update();
	void updateColor();

	void drawPixel(const long&, const long&);

	void save2file(string);
	void loadFromFile(string);
	
	bool keyPressed(int);

public:	
	void run();

	//masonery
	Paint();
	~Paint();
};

