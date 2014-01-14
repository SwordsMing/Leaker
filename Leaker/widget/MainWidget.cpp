#include "../stdafx.h"
#include "MainWidget.h"
#include "InterfacesLstWidget.h"
#include <boost/bind.hpp>
#include "../AdaptersInfo.h"
#include "../CaptureCore.h"
#include "../Message.h"
#include "../CaptureCore.h"
#include "MenuCaptureWnd.h"

MainWidget::~MainWidget(){

}


void MainWidget::init(){
	
	//mainPannelZoneUI_ = static_cast<CHorizontalLayoutUI*>(paintManager_.FindControl(_T("mainPannelZone")));

	registerEvent(DUI_CLICK,_T("minBtn"),boost::bind(&MainWidget::onMinBtn,this,_1));
	registerEvent(DUI_CLICK,_T("maxBtn"),boost::bind(&MainWidget::onMaxBtn,this,_1));
	registerEvent(DUI_CLICK,_T("restoreBtn"),boost::bind(&MainWidget::onRestoreBtn,this,_1));

	registerEvent(DUI_ITEMCLICK,_T("interfaceList"),boost::bind(&MainWidget::onInterfaceList,this,_1));
	registerEvent(DUI_ITEMCLICK,_T("startCapture"),boost::bind(&MainWidget::onStartCapture,this,_1));
	registerEvent(DUI_ITEMSELECT,_T("mainCaptureLst"),boost::bind(&ShowCore::onSelectItem,&showCore_,_1));
	registerEvent(DUI_ITEMSELECT,_T("mainCaptureTree"),boost::bind(&ShowCore::onItemSelect,&showCore_,_1));

	registerEvent(DUI_CLICK,_T("Capture"),boost::bind(&MainWidget::onCaptureBtn,this,_1));
	initMainPannelInterfaces();

	showCore_.setMainWnd(this);
	showCore_.setLstUI(static_cast<CListUI*>(paintManager_.FindControl(_T("mainCaptureLst"))));
	showCore_.setTreeViewUI(static_cast<CTreeViewUI*>(paintManager_.FindControl(_T("mainCaptureTree"))));
	showCore_.setHexViewUI(static_cast<CRichEditUI*>(paintManager_.FindControl(_T("hexShow"))));
	paintManager_.AddMessageFilter(this);
}

LPCTSTR MainWidget::GetWindowClassName() const{
	return _T("MainWidget");
}

const char * MainWidget::getWidgetXml() const{
	return _T("MainWidget.xml");
}

UINT MainWidget::GetClassStyle() const{
	return CS_DBLCLKS;
}

void MainWidget::onInterfaceList(TNotifyUI&){

	InterfacesLstWidget *widget = new InterfacesLstWidget(this);
	widget->Create(m_hWnd,_T("InterfacesLstWidget"),UI_WNDSTYLE_DIALOG  ,0L);
	widget->CenterWindow();
	widget->ShowWindow();
	widget->initInterfacesLst();
}

void MainWidget::onStartCapture(TNotifyUI& msg){

	toShowMode();
}

void MainWidget::onCloseBtn(TNotifyUI&msg){

	PostQuitMessage(0);
}

void MainWidget::initMainPannelInterfaces(){

	CListUI * showInterfaceLst_ = static_cast<CListUI*>(paintManager_.FindControl(_T("showInterfaceLst")));
	const pcap_if_t * pcap_ = AdaptersInfo::instance().getAllDevsInfo();
	for (;pcap_!= NULL; pcap_= pcap_->next)
	{
		CListContainerElementUI * pLstElement = new CListContainerElementUI;
		CHorizontalLayoutUI * pHorElement	  = new CHorizontalLayoutUI;
		CLabelUI * pDevLab = new CLabelUI;

		pDevLab->SetText(pcap_->name);
		pDevLab->SetAttribute(_T("height"),_T("20"));
		pHorElement->SetAttribute(_T("height"),_T("20"));
		pHorElement->Add(pDevLab);
		pLstElement->Add(pHorElement);
		showInterfaceLst_->Add(pLstElement);
	}
}

LRESULT MainWidget::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled){

	bool res = false;
	switch(uMsg)
	{
	case WM_STARTCAPTURE:
		res = true;
		start((const char *)wParam);
		break;
	}

	return res;
}

void MainWidget::toShowMode(){

	CHorizontalLayoutUI *mainPannelZoneUI_ = static_cast<CHorizontalLayoutUI*>(paintManager_.FindControl(_T("mainPannelZone")));
	CVerticalLayoutUI * protocalInfoShowZoneUI_ = static_cast<CVerticalLayoutUI *>(paintManager_.FindControl(_T("protocalInfoShowZone")));
	protocalInfoShowZoneUI_->SetVisible(true);
	mainPannelZoneUI_->SetVisible(false);
}

void MainWidget::start(const char *name){

	toShowMode();
	CEditUI * filter = static_cast<CEditUI*>(paintManager_.FindControl(_T("filter")));
	CaptureCore::instance().setShowCore(&showCore_);
	CaptureCore::instance().setFilter(filter->GetText());
	CaptureCore::instance().startCapture(name);
}

//²Ëµ¥ÏìÓ¦

void MainWidget::onCaptureBtn(DuiLib::TNotifyUI & msg){
	
	RECT rect_ = msg.pSender->GetPos();
	POINT pt_ = {rect_.left,rect_.bottom};
	::ClientToScreen(*this,&pt_);
	MenuCaptureWnd *widget = new MenuCaptureWnd(this);
	widget->Create(m_hWnd,_T("MenuCaptureWnd"),WS_POPUP,WS_EX_TOOLWINDOW);
	widget->ShowWindow();
	::SetWindowPos(*widget,NULL,pt_.x,pt_.y,0,0,SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	::SendMessage(m_hWnd, WM_NCACTIVATE, TRUE, 0L);
}


//

