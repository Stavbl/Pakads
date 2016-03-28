#include <iostream>
#include <Windows.h>
#include "TextBox.h"
#include "FormEngine.h"
#include "RadioGroup.h"
#include "ComboBox.h"
#include "TextBoxPakad.h"
#include "Label.h"

int main() {
	_INPUT_RECORD  c;
	CONSOLE_SCREEN_BUFFER_INFO  ci;
	DWORD read = 0;
	HANDLE ih = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(ih, &mode);
	SetConsoleMode(ih, mode & (~ENABLE_ECHO_INPUT) | (ENABLE_MOUSE_INPUT)& (~ENABLE_LINE_INPUT) & (~ENABLE_PROCESSED_OUTPUT) & (~ENABLE_QUICK_EDIT_MODE) | (ENABLE_EXTENDED_FLAGS) & (~ENABLE_INSERT_MODE) | ENABLE_WINDOW_INPUT);
	HANDLE oh = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD style = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(ih, style);

	FormEngine engine;

	engine.addTextBox(0, 0);
	engine.addTextBox(0, 3, 25);
	engine.addTextBox(45, 0, 15);
	
	vector<string> demoOpts;
	demoOpts.push_back("Hello");
	demoOpts.push_back("World");
	demoOpts.push_back("Motec");

	engine.addRadioGroup(demoOpts, 0, 10);

	vector<string> foodOpts;
	foodOpts.push_back("Pizza");
	foodOpts.push_back("Hamburger");
	foodOpts.push_back("Sushi");
	foodOpts.push_back("Pasta");
	foodOpts.push_back("Poutine");

	engine.addRadioGroup(foodOpts, 20, 10);

	vector<string> cityOpts;
	cityOpts.push_back("Tel Aviv");
	cityOpts.push_back("New York");
	cityOpts.push_back("Rio");
	cityOpts.push_back("Berlin");
	cityOpts.push_back("Amsterdam");

	engine.addComboBox(cityOpts, 60, 8);

	vector<string> colorsOpts;
	colorsOpts.push_back("Blue");
	colorsOpts.push_back("Green");
	colorsOpts.push_back("Yellow");
	colorsOpts.push_back("Black");
	colorsOpts.push_back("Orange");
	colorsOpts.push_back("Aquamarine");
	colorsOpts.push_back("Black");
	colorsOpts.push_back("Pink");

	engine.addCheckBoxGroup(colorsOpts, 45, 10);

	Label lbl = Label(0, 1, "Label 1");
	Label lbl2 = Label(0, 2, "Label 2");

	engine.addElement((FormElement*)&lbl);
	engine.addElement((FormElement*)&lbl2);

	engine.redraw(oh);
	int keyCode;
	while (1) {
		ReadConsoleInput(ih, &c, 1, &read);
		if (read == 1) {
			if (c.EventType == KEY_EVENT && c.Event.KeyEvent.bKeyDown) {
				keyCode = c.Event.KeyEvent.wVirtualKeyCode;
				GetConsoleScreenBufferInfo(oh, &ci);
				if (engine.handleKeys(&ci.dwCursorPosition, c.Event.KeyEvent.uChar.AsciiChar, keyCode, oh)) {
				} else if (keyCode == 37) {
					ci.dwCursorPosition.X = (ci.dwCursorPosition.X <= 0) ? 0 : ci.dwCursorPosition.X - 1;
				} else if (keyCode == 39) {
					ci.dwCursorPosition.X = (ci.dwCursorPosition.X >= ci.dwMaximumWindowSize.X) ? 0 : ci.dwCursorPosition.X + 1;
				} else if (keyCode == 38) {
					ci.dwCursorPosition.Y = (ci.dwCursorPosition.Y <= 0) ? 0 : ci.dwCursorPosition.Y - 1;
				} else if (keyCode == 40) {
					ci.dwCursorPosition.Y = (ci.dwCursorPosition.Y >= ci.dwMaximumWindowSize.Y) ? 0 : ci.dwCursorPosition.Y + 1;
				}
				SetConsoleCursorPosition(oh, ci.dwCursorPosition);
			}
			else if (c.EventType == MOUSE_EVENT && c.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
				GetConsoleScreenBufferInfo(oh, &ci);
				if (engine.handleMouse(&c.Event.MouseEvent.dwMousePosition, &ci.dwCursorPosition, oh)) {
				}
				SetConsoleCursorPosition(oh, ci.dwCursorPosition);
			}
		}
	}
	 
	return 0;
}