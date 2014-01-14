/*

    封装Duilib窗口类和控件事件通知机制
    created by zc in 2013/12/25

*/
#ifndef LEAKER_WIDGETWND_H_
#define LEAKER_WIDGETWND_H_


#include "../stdafx.h"
#include <boost/function.hpp>
#include <map>
#include <utility>
#include <string>

using std::string;
using std::multimap;
using std::pair;

class WidgetWnd: public CWindowWnd,public INotifyUI
{
public:
	void Notify(TNotifyUI& msg);
	virtual void onCloseBtn(TNotifyUI&msg);
	void onMaxBtn(TNotifyUI& msg);
	void onMinBtn(TNotifyUI& msg);
	void onRestoreBtn(TNotifyUI & msg);
	CPaintManagerUI paintManager_;
protected:
	//控件事件回调函数对象
	typedef boost::function<void(TNotifyUI&)> eventCallback;
	typedef pair<string,eventCallback> controlCallback;


	virtual void init(){};
	LRESULT onCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT onSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT	onGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam,LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam,LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam,LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam,LPARAM lParam, BOOL& bHandled);;
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam,LPARAM lParam, BOOL& bHandled);;
	virtual const char * getWidgetXml()const=0;
	void registerEvent(const string & eventType,const string & controlName,const eventCallback & cb);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	multimap<string,controlCallback> registertedEvent_;
};

#endif