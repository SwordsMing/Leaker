#ifndef LEAKER_QUITPROMTWND_H_
#define LEAKER_QUITPROMTWND_H_

#include "../stdafx.h"
#include "WidgetWnd.h"
#include "MainWidget.h"

class QuitPromptWnd:public WidgetWnd
{
public:
	explicit QuitPromptWnd(MainWidget * owner):owner_(owner){}
protected:
	void init();
	const char * getWidgetXml()const{return _T("QuitPromptWnd.xml");}
	LPCTSTR GetWindowClassName() const{return _T("QuitPromptWnd");}
	UINT GetClassStyle() const{return 0;}
	void OnFinalMessage(HWND hWnd){delete this;}
private:
	void onSave(TNotifyUI &ms);
	void onCancle(TNotifyUI & msg);
	void onNoSave(TNotifyUI &msg);

	MainWidget * owner_;
};

#endif