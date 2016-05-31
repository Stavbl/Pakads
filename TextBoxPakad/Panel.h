#pragma once
#include <Windows.h>
#include "IControl.h"

class Panel : public IControl
{
private:
	int size;
	int sizeh;
	vector<IControl *> elements;
public:
	Panel(int height, int width, int x = 0, int y = 0);
	int width();
	int height();
	COORD pos();
	virtual vector<COORD> &tabPositions();
	void print(HANDLE h, COORD cursor, COORD window);
	void AddControl(IControl *elem, int left, int top);
	bool handle_keys(PCOORD x, COORD window, char c, int keycode);
	bool handle_clicks(PCOORD mouse, COORD window, PCOORD cursor);
};
