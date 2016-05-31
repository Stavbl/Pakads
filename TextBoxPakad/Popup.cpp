#include "Popup.h"
#include <iostream>

using namespace std;



MessageBox::MessageBox(int height, int width, PopupEvent *cb) : Panel(height, width), okbtn(width / 2), cancelbtn(width / 2), textLbl(width, NULL, 0, (height - 1) / 2), titleLbl(width, NULL, 0, 0)
{
	event = cb;
	HANDLE oh = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO ci;
	GetConsoleScreenBufferInfo(oh, &ci);
	int wheight = ci.srWindow.Bottom - ci.srWindow.Top + 1;
	int wwidth = ci.srWindow.Right - ci.srWindow.Left + 1;
	this->position.X = (wwidth - width) / 2;
	this->position.Y = (wheight - height) / 2;
	COORD firstBtnPos;
	firstBtnPos.X = 0;
	firstBtnPos.Y = height - okbtn.height();
	okbtn.set_position(firstBtnPos);
	okbtn.SetValue("OK");
	COORD secondBtnPos;
	secondBtnPos.X = (width+1)/2;
	secondBtnPos.Y = height - okbtn.height();
	cancelbtn.set_position(secondBtnPos);
	cancelbtn.SetValue("Cancel");
	okbtn.add_listener(this);
	okbtn.SetBorder(SINGLE_LINE);
	cancelbtn.SetBorder(SINGLE_LINE);
	cancelbtn.add_listener(this);
	SetBorder(SINGLE_LINE);
	AddControl(&okbtn, okbtn.pos().X, okbtn.pos().Y);
	AddControl(&cancelbtn, cancelbtn.pos().X, cancelbtn.pos().Y);
	AddControl(&textLbl, textLbl.pos().X, textLbl.pos().Y);
	AddControl(&titleLbl, titleLbl.pos().X, titleLbl.pos().Y);
}

void MessageBox::SetTitle(string title)
{
	titleLbl.SetValue(title);
}

void MessageBox::SetText(string text)
{
	textLbl.SetValue(text);
}

void MessageBox::MousePressed(IControl & element, int x, int y, bool isLeft)
{
	if (&element == &okbtn) {
		event->PopupClicked(OK_CLICKED);
	}
	else {
		event->PopupClicked(CANCEL_CLICKED);
	}
}

MessageBox::~MessageBox()
{
}
