#ifndef LEAKER_MAINWIDGET_H_
#define LEAKER_MAINWIDGET_H_

#include "WidgetWnd.h"
#include "../ShowCore.h"
#include <ShellApi.h>

class MainWidget:public WidgetWnd,public IMessageFilterUI
{
public:
	MainWidget();
	~MainWidget();
	void onInterfaceList(TNotifyUI&);
	void onStartCapture(TNotifyUI&);
	void onCloseBtn(TNotifyUI&msg);
	void quit();
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	void disableMenuActive(){curWnd_ = beforeWnd_ = NULL;active_= false;}

	//分析历史数据堆文件
	void openDumpFile(const char * file);
	void saveToDumpFile();  //保存数据到堆文件
protected:
	void init();
	const char * getWidgetXml()const;
	LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;
	void OnFinalMessage(HWND hWnd){delete this;}
private:
	bool start_;   //是否有数据数据,用于程序退出时检测
	void initMainPannelInterfaces();
	ShowCore showCore_;
	void toShowMode();

	//开始抓包分析
	void start(const char * name);

	//处理拖拽文件
	void dealDragFils(HDROP hDrop);

	//菜单响应
	WidgetWnd * curWnd_;     //前一个激活的菜单窗口
	WidgetWnd * beforeWnd_;
	bool active_;              //是否点击了菜单
	WidgetWnd * createMenuWnd(TNotifyUI&);
	void showMenuWnd(TNotifyUI &);
	void mouseEnter(TNotifyUI &);
	//
	
	//应用过滤器
	void onApplyFilter(TNotifyUI&);
};

#endif