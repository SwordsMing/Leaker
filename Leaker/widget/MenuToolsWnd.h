#ifndef LEAKER_MENUTOOLSWND_H_
#define LEAKER_MENUTOOLSWND_H_

#include "../stdafx.h"
#include "WidgetWnd.h"
#include "MainWidget.h"

class MenuToolsWnd: public WidgetWnd,public IMessageFilterUI
{
public:
	MenuToolsWnd(MainWidget * owner):owner_(owner){}
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
protected:
	void init();
	const char * getWidgetXml()const{return _T("Menu_Tools.xml");}
	LPCTSTR GetWindowClassName() const{return _T("MenuToolsWnd");}
	UINT GetClassStyle() const{return 0;}
	void OnFinalMessage(HWND hWnd){delete this;}
private:
	MainWidget *owner_;
	LRESULT onKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT onKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam);

};

#endif