#pragma once
#include <Windows.h>
#include <string>
#include "IControl.h"
#include <vector>

using namespace std;

class ComboBox : public IControl
{
private:
	vector<string> buffer;
	vector<COORD> closedTabPos;
	int selected = -1;
	int opened = 0;
	int size;
public:
	ComboBox(int width, vector<string> entries, int x = 0, int y = 0);
	void addOption(string str);
	int width();
	int height();
	vector<COORD>& tabPositions();
	void print(HANDLE h, COORD cursor, COORD window);
	bool handle_keys(PCOORD x, COORD window, char c, int keycode);
	bool handle_clicks(PCOORD mouse, COORD window, PCOORD cursor);
	void SetSelectedIndex(size_t i);
    size_t GetSelectedIndex();
	~ComboBox();
};

