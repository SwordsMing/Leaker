#ifndef LEAKER_MENUEDITWND_H_
#define LEAKER_MENUEDITWND_H_

#include "../stdafx.h"
#include "WidgetWnd.h"
#include "MainWidget.h"

class MenuEditWnd: public WidgetWnd,public IMessageFilterUI
{
public:
	MenuEditWnd(MainWidget * owner):owner_(owner){}
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
protected:
	void init();
	const char * getWidgetXml()const{return _T("Menu_Edit.xml");}
	LPCTSTR GetWindowClassName() const{return _T("MenuEditWnd");}
	UINT GetClassStyle() const{return 0;}
	void OnFinalMessage(HWND hWnd){delete this;}
private:
	MainWidget *owner_;
	LRESULT onKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT onKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam);

};

#endif