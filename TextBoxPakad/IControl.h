#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include "View.h"

using namespace std;

class IControl;

typedef enum BorderType { NONE = -1, SINGLE_LINE = 0, DOUBLE_LINE = 1 } BorderType;
typedef struct TabPosition {
	COORD pos; 
	COORD offset = { 0,0 };
	IControl *element;
	bool active = true;
} TabPosition;

DWORD color_to_rgb(ForegroundColor front, BackgroundColor back);
class IControl
{
protected:
	View *view;
	int tabpos = 0;
	COORD position;
	vector<TabPosition> tabPosArr;
	BorderType borderType = NONE;
	ForegroundColor foreground = WHITE;
	ForegroundColor background = BLACK;
	bool view_invalidated = false;
	bool hidden = false;
	bool active = false;
public:
	IControl();
	IControl(COORD position);

	const View *getView();
	virtual void setActive(bool active);
	bool has_border();
	void Show();
	void Hide();
	virtual int width() = 0;
	virtual int height() = 0;
	virtual COORD pos();
	virtual vector<TabPosition> tabPositions();
	virtual void print(HANDLE h, COORD cursor, COORD window) = 0;
	void draw(HANDLE h, COORD cursor, COORD window);
	virtual bool handle_keys(PCOORD x, COORD window, char c, int keycode) = 0;
	virtual bool handle_clicks(PCOORD mouse, COORD window, PCOORD cursor) = 0;
	virtual bool intersects(PCOORD pos, COORD window);
	void SetBorder(BorderType border);
	void print_border(HANDLE h, COORD window);
	void SetForeground(ForegroundColor color);
	void SetBackground(ForegroundColor color);
	bool needs_redraw();
	void set_position(COORD pos);
	virtual void updateView(COORD cursor);
	virtual void updateBorder(int layer);
	virtual ~IControl() {};
};

