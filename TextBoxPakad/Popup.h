#pragma once
#include "Panel.h"
#include "Button.h"


#undef MessageBox

typedef enum PopupResult {
	OK_CLICKED, CANCEL_CLICKED
} PopupResult;

typedef struct PopupEvent {
	virtual void PopupClicked(PopupResult res) = 0;
} PopupEvent;

class MessageBox :
	public Panel, public MouseListener
{
	Button okbtn; 
	Button cancelbtn;
	Label textLbl;
	Label titleLbl;
	PopupEvent *event;
public:
	MessageBox(int height, int width, PopupEvent *cb);
	void SetTitle(string title);
	void SetText(string text);
	void MousePressed(IControl &element, int x, int y, bool isLeft);
	~MessageBox();
};

