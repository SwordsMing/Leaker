#ifndef LEAKER_MENUANALYZEWND_H_
#define LEAKER_MENUANALYZEWND_H_

#include "../stdafx.h"
#include "WidgetWnd.h"
#include "MainWidget.h"

class MenuAnalyzeWnd: public WidgetWnd,public IMessageFilterUI
{
public:
	MenuAnalyzeWnd(MainWidget * owner):owner_(owner){}
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
protected:
	void init();
	const char * getWidgetXml()const{return _T("Menu_Analyze.xml");}
	LPCTSTR GetWindowClassName() const{return _T("MenuAnalyzeWnd");}
	UINT GetClassStyle() const{return 0;}
	void OnFinalMessage(HWND hWnd){delete this;}
private:
	MainWidget *owner_;
	LRESULT onKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT onKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam);

};

#endif