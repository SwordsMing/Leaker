#ifndef LEAKER_MENUVIEWWND_H_
#define LEAKER_MENUVIEWWND_H_

#include "../stdafx.h"
#include "WidgetWnd.h"
#include "MainWidget.h"

class MenuViewWnd: public WidgetWnd,public IMessageFilterUI
{
public:
	MenuViewWnd(MainWidget * owner):owner_(owner){}
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
protected:
	void init();
	const char * getWidgetXml()const{return _T("Menu_View.xml");}
	LPCTSTR GetWindowClassName() const{return _T("MenuViewWnd");}
	UINT GetClassStyle() const{return 0;}
	void OnFinalMessage(HWND hWnd){delete this;}
private:
	MainWidget *owner_;
	LRESULT onKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT onKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam);

};

#endif