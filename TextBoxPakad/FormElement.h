#pragma once

#include <Windows.h>
#include <string>
#include <vector>

using namespace std;

class FormElement
{
protected:
	COORD position;
	vector<COORD> tabPosArr;
public:
	FormElement();
	FormElement(COORD position);

	virtual int width() = 0;
	virtual int height() = 0;
	virtual COORD pos();
	virtual vector<COORD> &tabPositions();
	virtual void print(HANDLE h) = 0;
	virtual bool handle_keys(PCOORD x, char c, int keycode) = 0;
	virtual bool handle_clicks(PCOORD mouse, PCOORD cursor) = 0;
	virtual bool intersects(PCOORD pos);

	virtual ~FormElement() {};
};

