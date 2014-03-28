#ifndef LEAKER_MENUTELEPHONYWND_H_
#define LEAKER_MENUTELEPHONYWND_H_

#include "../stdafx.h"
#include "WidgetWnd.h"
#include "MainWidget.h"

class MenuTelephonyWnd: public WidgetWnd,public IMessageFilterUI
{
public:
	MenuTelephonyWnd(MainWidget * owner):owner_(owner){}
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
protected:
	void init();
	const char * getWidgetXml()const{return _T("Menu_Telephony.xml");}
	LPCTSTR GetWindowClassName() const{return _T("MenuTelephonyWnd");}
	UINT GetClassStyle() const{return 0;}
	void OnFinalMessage(HWND hWnd){delete this;}
private:
	MainWidget *owner_;
	LRESULT onKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT onKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam);

};

#endif