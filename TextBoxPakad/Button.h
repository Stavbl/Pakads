#pragma once
#include <Windows.h>
#include "Label.h"

typedef struct MouseListener {
	virtual void MousePressed(IControl &element, int x, int y, bool isLeft) = 0;
} MouseListener;

class Button : public Label
{
private:
	char *buffer;
	vector<MouseListener*> events;
	int size;
public:
	Button(int width, int x = 0, int y = 0, char *txt = NULL);
	bool handle_clicks(PCOORD mouse, COORD window, PCOORD cursor);
	void add_listener(MouseListener *listener);
};