#include "Paint.h"

//should've done it in a stack to avoid the function loops, but I didn't really have the time to make classes for each state
void Paint::menuState() {
	system("cls");
	//cin.clear;
	if (saved) {
		saved = false;
		cout << "Saved!" << endl;
	} else if (againMenu) {
		cout << "Try again!" << endl;
		againMenu = false;
	} else if (welcome) {
		cout << "Hi! Welcome to My Paint!" << endl;
		welcome = false;
	}
	cout 
		<< "1. Draw" << endl
		<< "2. Load" << endl
		<< "3. Save" << endl
		<< "4. Help" << endl
		<< "5. Quit" << endl;

	cin >> choice;
	cout << endl;

	if (choice > 5 || choice < 1) { //check for dumb input
		choice = 0;
		againMenu = true;
		menuState();
	} 
	
	if (choice == 1) {
		if (!SetConsoleCursorInfo(hOut, &consoleCursor))
			cout << "SetConsoleCursorInfo() did not work!\nError: " << GetLastError() << endl;

		if (consoleCursor.bVisible) {
			whyUGottaResize();
		}

		system("cls");

		if (!SetConsoleMode(hIn, ENABLE_EXTENDED_FLAGS))
			cout << "SetConsoleMode() did not work!\nError: " << GetLastError() << endl;

		save2file("temp");
		loadFromFile("temp");
		run();

	} else if (choice == 2) {
		cout << "Write the path:\n    ";
		cin >> path;
		system("cls");
		if (!SetConsoleMode(hIn, ENABLE_EXTENDED_FLAGS))
			cout << "SetConsoleMode() did not work!\nError: " << GetLastError() << endl;

		loadFromFile(path);
		run();
	} else if (choice == 3) {
		cout << "Write the name of the file:\n    ";
		cin >> path;
		save2file(path);
		saved = true;
		menuState();
	} else if (choice == 4) {
		system("cls");
		cout 
			<< "Help:" << endl
			<< "Esc - menu" << endl
			<< "Q   - red" << endl
			<< "W   - green" << endl
			<< "E   - blue" << endl
			<< "Shift + color key - turn off color" << endl << endl
			<< "DO NOT RESIZE THE WINDOW" << endl << endl;
		system("pause");
		menuState();
	} else if (choice == 5) {
		exit(0);
	}
}

void Paint::whyUGottaResize() {
	if (!SetConsoleMode(hIn, normalMode))
		cout << "SetConsoleMode() did not work!\nError: " << GetLastError() << endl;

	save2file("temp");
	while (Iunderstand != "understood" && Iunderstand != "\"understood\"") {
		cout << "DO NOT RESIZE THE CONSOLE!" << endl << "Write: \"understood\"" << endl;
		cin >> Iunderstand;
		system("cls");
		run();
	}
	Iunderstand = "I don't understand";
	system("cls");
	consoleSetup();

	if (!SetConsoleMode(hIn, ENABLE_EXTENDED_FLAGS))
		cout << "SetConsoleMode() did not work!\nError: " << GetLastError() << endl;

	loadFromFile("temp");
}

void Paint::consoleSetup() {
	SetConsoleTitle("Homemade paint!");
	//getting screen and display resolution;
	RECT resolution;	//for the scaled resolution of the display
	DEVMODE x;			//for resolution of the display
	float scaledR, absR;

	GetWindowRect(GetDesktopWindow(), &resolution);
	scaledR = (float)resolution.right;

	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &x);
	absR = (float)x.dmPelsWidth; //thanks the god of informatics I CANNOT BELIEVE IM CRYING AFTER HOURS OF WORK I"VE FOUND THAT 1 FUNCTION THAT HELPS

	screenScale = absR / scaledR;

	//setting window height and stuff
	wndRect.left = 100;
	wndRect.top = 100;
	wndRect.right = 512 + 16;	//calibrated for my pc
	wndRect.bottom = 512 + 36;  //might need to get it from the stuffs
	MoveWindow(hwnd, wndRect.left, wndRect.top, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, true);

	if (!GetWindowRect(hwnd, &wndRect))
		cout << "GetWindowRect() did not work!\nError: " << GetLastError() << endl;

	//disabling blinking cursor
	CONSOLE_CURSOR_INFO consoleCursor;
	consoleCursor.bVisible = false; consoleCursor.dwSize = 20;
	if (!SetConsoleCursorInfo(hOut, &consoleCursor))
		cout << "SetConsoleCursorInfo() did not work!\nError: " << GetLastError() << endl;


	//setting buffer size (disable vertical bar)
	COORD newBufferSize;

	GetConsoleScreenBufferInfo(hOut, &bufferInfo);
	newBufferSize.X = bufferInfo.dwSize.X;
	newBufferSize.Y = 25; //#!# might need to get it from the charsize but this works for this exact size


	if (!SetConsoleScreenBufferSize(hOut, newBufferSize))
		cout << "SetConsoleScreenBufferSize() did not work!\nError: " << GetLastError() << endl;
}

void Paint::run() {
	while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000)) { //while esc isn't pressed
		update();
		//might've done a loop that would've timed the input, but whatever
		updateColor();
	}

	if (!SetConsoleMode(hIn, normalMode))
		cout << "SetConsoleMode() did not work!\nError: " << GetLastError() << endl;

	menuState();
}

void Paint::update() {
	GetCursorPos(&mouse);
	GetWindowRect(hwnd, &wndRect);
	GetConsoleCursorInfo(hOut, &consoleCursor);
	if (consoleCursor.bVisible) {
		whyUGottaResize();
	}

	mouse.x = (mouse.x - wndRect.left - 8) * screenScale;
	mouse.y = (mouse.y - wndRect.top - 32) * screenScale;
	//mouse cursor size is 8x32

	if (mouse.x < 512 && //if mouse in the window
		mouse.x > 0 &&
		mouse.y > 0 &&
		mouse.y < 512 &&
		keyPressed(VK_LBUTTON)) {
		drawPixel(mouse.x, mouse.y);
	}
}

void Paint::updateColor() {
	//pressing Q
	if (GetAsyncKeyState(0x51) & 0x8000) {			//Q
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {	//shift
			r = 0;
		} else {
			r = 255;
		}
	}

	//pressing W
	if (GetAsyncKeyState(0x57) & 0x8000) {				//W
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {		//shift
			g = 0;

		} else {
			g = 255;
		}
	}

	//pressing E
	if (GetAsyncKeyState(0x45) & 0x8000) {				//E
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {		//shift
			b = 0;
		} else {
			b = 255;
		}
	}

	//updating color properly
	color = RGB(r, g, b);
}

void Paint::drawPixel(const long& x, const long& y) {
	SetPixel(device, x, y, color);
	pixArray[mouse.x][mouse.y][0] = b;
	pixArray[mouse.x][mouse.y][1] = g;
	pixArray[mouse.x][mouse.y][2] = r;
}

void Paint::save2file(string path) {
	char header[54] = {
		0x42, 0x4d,				//file magic number
		0x36, 0x00, 0x0c, 0x00,	//file size (with header)			#!# 36 + pix ** 2 * 3 + padding * height
		0x00, 0x00, 0x00, 0x00,	//firmware stuff
		0x36, 0x00, 0x00, 0x00, //pixel array position - after header
		0x28, 0x00, 0x00, 0x00,	//dpi header length
		0x00, 0x02, 0x00, 0x00,	//width px		- 512					#!# pix
		0x00, 0x02, 0x00, 0x00,	//height px		- 512					#!# pix
		0x01, 0x00,				//color plane - 1
		0x18, 0x00,				//bits per pixel - 24
		0x00, 0x00, 0x00, 0x00, //compression - none
		0x00, 0x00, 0x0c, 0x00, //img size in bytes (no header)			#!# pix ** 2 * 3 + padding * height
		0x00, 0x00, 0x00, 0x00, //vertical resolution	- default
		0x00, 0x00, 0x00, 0x00, //horizontal resolution - default
		0x00, 0x00, 0x00, 0x00, //color in plane
		0x00, 0x00, 0x00, 0x00 //impClr (?) idk what it is
		//pixel array
	};

	ofstream bmp(path, ios::binary | ios::out);
	bmp.write(header, sizeof(header));

	for (int y = 512; y > 0; y--) {
		for (int x = 0; x < 512; x++) {
			bmp.write(pixArray[x][y], 3);
		}
	}

	bmp.close();
}

void Paint::loadFromFile(string path) {
	ifstream bmp(path);
	unsigned char ch;
	int counter{};
	int y = 512;
	int x{};

	//skipping header
	while (counter < 0x36) {
		++counter;
		bmp >> noskipws;
	}

	for (int y = 512; y > 0; y--) {
		for (int x = 0; x < 512; x++) {
			for (char bit = 0; bit < 3; bit++) {
				bmp >> noskipws >> ch;
				pixArray[x][y][bit] = ch;
			}
		}
	}

	for (int y = 0; y < 512; y++) {
		for (int x = 0; x < 512; x++) {
			SetPixel(device, x, y, RGB(pixArray[x][y][2], pixArray[x][y][1], pixArray[x][y][0]));
		}
	}
}

bool Paint::keyPressed(int key) {
	if ((GetAsyncKeyState(key) & 0x8000)) {
		return true;
	}
	return false;
}

Paint::Paint() {
	hwnd = GetConsoleWindow();
	device = GetDC(hwnd);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);	//output handle
	hIn = GetStdHandle(STD_INPUT_HANDLE);

	if (!GetConsoleMode(hIn, &normalMode))
		cout << "GetConsoleMode() did not work!\nError: " << GetLastError() << endl;

	choice = 0;

	againMenu = 0;
	saved = 0;
	welcome = 1;
	Iunderstand = "I do not understand";

	r = 255;
	g = 255;
	b = 255;

	color = RGB(r, g, b);

	consoleSetup();
	menuState();
}

Paint::~Paint() {
	remove("temp");
}
