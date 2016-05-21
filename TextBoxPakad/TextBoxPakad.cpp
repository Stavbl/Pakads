#include <iostream>
#include <Windows.h>
#include "TextBox.h"
#include "FormEngine.h"
#include "RadioGroup.h"
#include "ComboBox.h"
#include "TextBoxPakad.h"
#include "Label.h"
#include "Button.h"
#include "NumericBox.h"
#include "Panel.h"

using namespace std;

struct PrintListener : public MouseListener {
	int times = 0;
	COORD pos;
	TextBox tb;
	PrintListener() : tb(20, 20, 10) {
		tb.set_text("Button");
		pos.X = 0;
		pos.Y = 0;
		tb.set_background_color(PURPLE);
	}
	void MousePressed(FormElement &element, int x, int y, bool is_left) {
		pos.X += 1;
		pos.Y += 1;
		tb.set_position(pos);
		element.set_background_color(times % 2 == 0 ? PURPLE : BLUE);
	}
};

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
	Panel ch = Panel(30, 12, 10, 3);
	ch.set_border(DOUBLE_LINE);
	ch.set_foreground_color(GREEN);
	engine.addElement(&ch);

	Label lbl = Label(8, 2, 2, "Label");
	/*vector<string> opts;
	opts.push_back("One");
	opts.push_back("Stav Blyman");
	opts.push_back("Three");
	RadioGroup rg = RadioGroup(3, 20, opts, 2, 6);*/
	Panel p2 = Panel(15, 4, 12, 6);
	p2.set_background_color(WHITE);
	p2.set_border(SINGLE_LINE);
	p2.set_foreground_color(BLACK);
	NumericBox nb = NumericBox(6, 0, 100, 4, 3);
	p2.addElement(&nb);
	ch.addElement(&p2);
	ch.addElement(&lbl);
	//ch.addElement(&rg);
	ch.addElement(&nb);

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
		engine.validate_views(oh);
	}
	 
	return 0;
}