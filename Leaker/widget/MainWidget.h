#ifndef LEAKER_MAINWIDGET_H_
#define LEAKER_MAINWIDGET_H_

#include "WidgetWnd.h"
#include "../ShowCore.h"

class MainWidget:public WidgetWnd,public IMessageFilterUI
{
public:
	MainWidget(){}
	~MainWidget();
	void onInterfaceList(TNotifyUI&);
	void onStartCapture(TNotifyUI&);
	void onCloseBtn(TNotifyUI&msg);
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	
protected:
	void init();
	const char * getWidgetXml()const;
	LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;
	void OnFinalMessage(HWND hWnd){delete this;}
private:
	void initMainPannelInterfaces();
	ShowCore showCore_;
	void toShowMode();

	//开始抓包分析
	void start(const char * name);

	//菜单响应
	void onFilesBtn(TNotifyUI&);
	void onEditBtn(TNotifyUI&);
	void onViewBtn(TNotifyUI&);
	void onGoBtn(TNotifyUI&);
	void onCaptureBtn(TNotifyUI&);
	void onAnalyzeBtn(TNotifyUI&);
	void onStatisticsBtn(TNotifyUI&);
	void onTelephoonyBtn(TNotifyUI&);
	void onToolsBtn(TNotifyUI&);
	void onInternalsBtn(TNotifyUI&);
	void onHelpBtn(TNotifyUI&);
	//
};

#endif