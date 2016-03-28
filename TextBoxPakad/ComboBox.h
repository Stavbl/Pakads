#pragma once
#include <Windows.h>
#include <string>
#include "FormElement.h"
#include <vector>

using namespace std;

class ComboBox : FormElement
{
private:
	vector<string> buffer;
	vector<COORD> closedTabPos;
	int selected = -1;
	int opened = 0;
	int size;
public:
	ComboBox(int x = 0, int y = 0);
	void addOption(string str);
	int width();
	int height();
	vector<COORD>& tabPositions();
	void print(HANDLE h);
	bool handle_keys(PCOORD x, char c, int keycode);
	bool handle_clicks(PCOORD mouse, PCOORD cursor);
	~ComboBox();
};

