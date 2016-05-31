#include "Button.h"

Button::Button(int width, int x, int y, char * txt) : Label(width, txt, x, y)
{
}

bool Button::handle_clicks(PCOORD mouse, COORD window, PCOORD cursor)
{
	if (intersects(mouse, window)) {
		for (int i = 0; i < events.size(); i++) {
			events[i]->MousePressed(*this, mouse->X, mouse->Y, true);
		}
		return true;
	}
	return false;
}

void Button::add_listener(MouseListener *listener)
{
	events.push_back(listener);
}


