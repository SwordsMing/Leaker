#ifndef LEAKER_INTERFACESLSTWIDGET_H_
#define LEAKER_INTERFACESLSTWIDGET_H_

#include "../stdafx.h"
#include "WidgetWnd.h" 
#include <vector>

class MainWidget;

class InterfacesLstWidget:public WidgetWnd
{
public:
	explicit InterfacesLstWidget(MainWidget*);
	void initInterfacesLst();
	void onStartBtn(TNotifyUI&);
	void onStopBtn(TNotifyUI&);
	void onOptionBtn(TNotifyUI&);
	//void onDetailBtn(TNotifyUI&);
	void onChooseBtn(TNotifyUI&);      //单选按钮，选中待打开的网络适配器接口
protected:
	void init();
	const char * getWidgetXml()const;
	LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;
	void OnFinalMessage(HWND hWnd){delete this;}
private:
	CListUI  * interfacesLstUI_;
	MainWidget * mainWidget_;
	CStdString curInterface_;  //当前选中的接口
	std::vector<COptionUI*> optionGroup_;
	CStdString getChoosedInterface()const;
};

#endif