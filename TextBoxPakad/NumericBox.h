#pragma once
#include <Windows.h>
#include "FormElement.h"

class NumericBox : public FormElement
{
private:
	int size;
	int minVal, maxVal, val = 0;
public:
	NumericBox(int width, int min, int max, int x = 0, int y = 0);
	int width();
	int height();
	COORD pos();
	void print(HANDLE h, COORD cursor, COORD window);
	bool handle_keys(PCOORD x, COORD window, char c, int keycode);
	bool handle_clicks(PCOORD mouse, COORD window, PCOORD cursor);
	void set_value(int val);
	int get_value();
};
