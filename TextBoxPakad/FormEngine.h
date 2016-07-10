#pragma once
#include <Windows.h>
#include <vector>
#include "IControl.h"
#include "RadioList.h"
#include "TextBox.h"
#include "Checklist.h"
#include "ComboBox.h"
#include "Popup.h"
#include <algorithm>

class EventEngine
{
	IControl *element;
	//PopupEvent *event;
	bool needsRefresh = true;
public:
	EventEngine();
	//void addTextBox(int x = 0, int y = 0, int len = 30);
	//void addRadioGroup(vector<string> options, int x, int y);
	//void addCheckBoxGroup(vector<string> options, int x, int y);
	//void addComboBox(vector<string> options, int x, int y);
	void Run(IControl *);
	//void EventEngine::PopupClicked(PopupResult res);
	void focusOnNextElement(PCOORD pos, HANDLE oh);
	bool handleKeys(PCOORD cor, char c, int keycode, HANDLE oh);
	bool *calculateElementMap(int width, int height);
	void clear_screen(HANDLE oh);
	bool handleMouse(PCOORD mouse, PCOORD cursor, HANDLE oh);
	void redraw(HANDLE oh, CONSOLE_SCREEN_BUFFER_INFO info);
	void validate_views(HANDLE oh);
	~EventEngine();
};

