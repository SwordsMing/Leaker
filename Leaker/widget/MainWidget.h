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

	//������ʷ���ݶ��ļ�
	void openDumpFile(const char * file);
	void saveToDumpFile();  //�������ݵ����ļ�
protected:
	void init();
	const char * getWidgetXml()const;
	LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;
	void OnFinalMessage(HWND hWnd){delete this;}
private:
	bool start_;   //�Ƿ�����������,���ڳ����˳�ʱ���
	void initMainPannelInterfaces();
	ShowCore showCore_;
	void toShowMode();

	//��ʼץ������
	void start(const char * name);

	//������ק�ļ�
	void dealDragFils(HDROP hDrop);

	//�˵���Ӧ
	WidgetWnd * curWnd_;     //ǰһ������Ĳ˵�����
	WidgetWnd * beforeWnd_;
	bool active_;              //�Ƿ����˲˵�
	WidgetWnd * createMenuWnd(TNotifyUI&);
	void showMenuWnd(TNotifyUI &);
	void mouseEnter(TNotifyUI &);
	//
	
	//Ӧ�ù�����
	void onApplyFilter(TNotifyUI&);
};

#endif