#ifndef LEAKER_MENUFILEWND_H_
#define LEAKER_MENUFILEWND_H_

#include "../stdafx.h"
#include "WidgetWnd.h"
#include "MainWidget.h"

class MenuFileWnd: public WidgetWnd,public IMessageFilterUI
{
public:
	MenuFileWnd(MainWidget * owner):owner_(owner){}
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
protected:
	void init();
	const char * getWidgetXml()const{return _T("Menu_Files.xml");}
	LPCTSTR GetWindowClassName() const{return _T("MenuFileWnd");}
	UINT GetClassStyle() const{return 0;}
	void OnFinalMessage(HWND hWnd){delete this;}
private:
	MainWidget *owner_;
	LRESULT onKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT onKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//œÏ”¶
	void onOpenFile(TNotifyUI & msg);
};

#endif