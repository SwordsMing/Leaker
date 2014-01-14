#ifndef LEAKER_MENUCAPTURE_H_
#define LEAKER_MENUCAPTURE_H_

#include "WidgetWnd.h"
#include "../stdafx.h"
#include <boost/noncopyable.hpp>
#include "MainWidget.h"

class MenuCaptureWnd: public WidgetWnd,public IMessageFilterUI
{
public:
	explicit MenuCaptureWnd(MainWidget * owner):owner_(owner){}
	~MenuCaptureWnd(){}
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
protected:
	void init();
	const char * getWidgetXml()const{return _T("Menu_Capture.xml");}
	LPCTSTR GetWindowClassName() const{return _T("MenuCaptureWnd");}
	UINT GetClassStyle() const{return 0;}
	void OnFinalMessage(HWND hWnd){delete this;}
private:
	MainWidget * owner_;
	LRESULT onKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT onKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void onInterfaces(TNotifyUI & msg);
	void onOptions(TNotifyUI & msg);
	void onStart(TNotifyUI & msg);
	void onStop(TNotifyUI &msg);
	void onSelectItem(TNotifyUI &msg);
};

#endif