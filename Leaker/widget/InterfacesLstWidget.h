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
	void onChooseBtn(TNotifyUI&);      //��ѡ��ť��ѡ�д��򿪵������������ӿ�
protected:
	void init();
	const char * getWidgetXml()const;
	LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;
	void OnFinalMessage(HWND hWnd){delete this;}
private:
	CListUI  * interfacesLstUI_;
	MainWidget * mainWidget_;
	CStdString curInterface_;  //��ǰѡ�еĽӿ�
	std::vector<COptionUI*> optionGroup_;
	CStdString getChoosedInterface()const;
};

#endif