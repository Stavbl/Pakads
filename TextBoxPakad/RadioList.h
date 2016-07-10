#pragma once
#include <Windows.h>
#include <string>
#include "IControl.h"
#include <vector>

using namespace std;

class RadioList : public IControl
{
private:
	vector<string> buffer;
	int selected;
	int size;
	int sizeh;
public:
	RadioList(int height, int width, vector<string> entries, int x = 0, int y = 0);
	void addOption(string str);
	int width();
	int height();
	void print(HANDLE h, COORD cursor, COORD window);
	bool handle_keys(PCOORD x, COORD window, char c, int keycode);
	bool handle_clicks(PCOORD mouse, COORD window, PCOORD cursor);
	size_t GetSelectedIndex();
	void SetSelectedIndex(size_t index);
	void ClearSelection();
	void updateView(COORD cursor);
	~RadioList();
};

