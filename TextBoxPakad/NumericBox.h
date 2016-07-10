#pragma once
#include <Windows.h>
#include "IControl.h"

class NumericBox : public IControl
{
private:
	int size;
	int minVal, maxVal, val = 0;
public:
	NumericBox(int height, int width, int min, int max, int x = 0, int y = 0);
	int width();
	int height();
	COORD pos();
	void print(HANDLE h, COORD cursor, COORD window);
	bool handle_keys(PCOORD x, COORD window, char c, int keycode);
	bool handle_clicks(PCOORD mouse, COORD window, PCOORD cursor);
	void SetValue(int val);
	int GetValue();
	void updateView(COORD cursor);
	~NumericBox();
};
