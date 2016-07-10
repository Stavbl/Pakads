#pragma once
#include <Windows.h>
#include "IControl.h"

class Label : public IControl
{
private:
	char *buffer;
	int size;
public:
	Label(int width, string txt, int x = 0, int y = 0);
	int width();
	int height();
	void updateView(COORD cursor);
	COORD pos();
	void print(HANDLE h, COORD cursor, COORD window);
	bool handle_keys(PCOORD x, COORD window, char c, int keycode);
	bool handle_clicks(PCOORD mouse, COORD window, PCOORD cursor);
	void SetValue(string txt);
	virtual ~Label();
};
