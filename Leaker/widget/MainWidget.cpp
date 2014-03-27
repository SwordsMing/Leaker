#include "../stdafx.h"
#include "MainWidget.h"
#include "InterfacesLstWidget.h"
#include <boost/bind.hpp>
#include "../AdaptersInfo.h"
#include "../CaptureCore.h"
#include "../Message.h"
#include "../CaptureCore.h"
#include "Menu.h"
#include "../ControlExt/UIWebBrowser.h"
#include "../utility/CommonFileDlg.h"
#include "../utility/UtilityFun.h"
#include "QuitPromptWnd.h"
#include <ExDisp.h>
#include <mshtml.h>

MainWidget::MainWidget():
	curWnd_(NULL),
	beforeWnd_(NULL),
	active_(false),
	start_(false)
{
}

MainWidget::~MainWidget(){

}


void MainWidget::init(){
	
	//mainPannelZoneUI_ = static_cast<CHorizontalLayoutUI*>(paintManager_.FindControl(_T("mainPannelZone")));

	registerEvent(DUI_CLICK,_T("minBtn"),boost::bind(&MainWidget::onMinBtn,this,_1));
	registerEvent(DUI_CLICK,_T("maxBtn"),boost::bind(&MainWidget::onMaxBtn,this,_1));
	registerEvent(DUI_CLICK,_T("restoreBtn"),boost::bind(&MainWidget::onRestoreBtn,this,_1));

	//registerEvent(DUI_ITEMCLICK,_T("interfaceList"),boost::bind(&MainWidget::onInterfaceList,this,_1));
	registerEvent(DUI_ITEMCLICK,_T("startCapture"),boost::bind(&MainWidget::onStartCapture,this,_1));
	registerEvent(DUI_ITEMSELECT,_T("mainCaptureLst"),boost::bind(&ShowCore::onSelectItem,&showCore_,_1));
	registerEvent(DUI_ITEMSELECT,_T("mainCaptureTree"),boost::bind(&ShowCore::onItemSelect,&showCore_,_1));

	//菜单响应
	registerEvent(DUI_CLICK,_T("Files"),boost::bind(&MainWidget::showMenuWnd,this,_1));
	registerEvent(DUI_MOUSEENTER,_T("Files"),boost::bind(&MainWidget::mouseEnter,this,_1));
	registerEvent(DUI_CLICK,_T("Edit"),boost::bind(&MainWidget::showMenuWnd,this,_1));
	registerEvent(DUI_MOUSEENTER,_T("Edit"),boost::bind(&MainWidget::mouseEnter,this,_1));
	registerEvent(DUI_CLICK,_T("View"),boost::bind(&MainWidget::showMenuWnd,this,_1));
	registerEvent(DUI_MOUSEENTER,_T("View"),boost::bind(&MainWidget::mouseEnter,this,_1));
	registerEvent(DUI_CLICK,_T("Go"),boost::bind(&MainWidget::showMenuWnd,this,_1));
	registerEvent(DUI_MOUSEENTER,_T("Go"),boost::bind(&MainWidget::mouseEnter,this,_1));
	registerEvent(DUI_CLICK,_T("Capture"),boost::bind(&MainWidget::showMenuWnd,this,_1));
	registerEvent(DUI_MOUSEENTER,_T("Capture"),boost::bind(&MainWidget::mouseEnter,this,_1));
	registerEvent(DUI_CLICK,_T("Analyze"),boost::bind(&MainWidget::showMenuWnd,this,_1));
	registerEvent(DUI_MOUSEENTER,_T("Analyze"),boost::bind(&MainWidget::mouseEnter,this,_1));
	registerEvent(DUI_CLICK,_T("Statistics"),boost::bind(&MainWidget::showMenuWnd,this,_1));
	registerEvent(DUI_MOUSEENTER,_T("Statistics"),boost::bind(&MainWidget::mouseEnter,this,_1));
	registerEvent(DUI_CLICK,_T("Telephony"),boost::bind(&MainWidget::showMenuWnd,this,_1));
	registerEvent(DUI_MOUSEENTER,_T("Telephony"),boost::bind(&MainWidget::mouseEnter,this,_1));
	registerEvent(DUI_CLICK,_T("Tools"),boost::bind(&MainWidget::showMenuWnd,this,_1));
	registerEvent(DUI_MOUSEENTER,_T("Tools"),boost::bind(&MainWidget::mouseEnter,this,_1));
	registerEvent(DUI_CLICK,_T("Internals"),boost::bind(&MainWidget::showMenuWnd,this,_1));
	registerEvent(DUI_MOUSEENTER,_T("Internals"),boost::bind(&MainWidget::mouseEnter,this,_1));
	registerEvent(DUI_CLICK,_T("Help"),boost::bind(&MainWidget::showMenuWnd,this,_1));
	registerEvent(DUI_MOUSEENTER,_T("Help"),boost::bind(&MainWidget::mouseEnter,this,_1));
	//end
	registerEvent(DUI_CLICK,_T("Apply"),boost::bind(&MainWidget::onApplyFilter,this,_1));
	initMainPannelInterfaces();

	showCore_.setMainWnd(this);
	showCore_.setLstUI(static_cast<CListUI*>(paintManager_.FindControl(_T("mainCaptureLst"))));
	showCore_.setTreeViewUI(static_cast<CTreeViewUI*>(paintManager_.FindControl(_T("mainCaptureTree"))));
	showCore_.setHexViewUI(static_cast<CRichEditUI*>(paintManager_.FindControl(_T("hexShow"))));
	paintManager_.AddMessageFilter(this);

	DragAcceptFiles(GetHWND(),TRUE);

	 CWebBrowserUI* pWebBrowser = static_cast<CWebBrowserUI*>(paintManager_.FindControl(_T("HackerWnd")));
	 pWebBrowser->DoCreateControl();
     if( pWebBrowser ) {
		 std::string path = utility::getResourceDir() + _T("skin\\hacker.html");
         pWebBrowser->Navigate2(path.c_str()); 
     }
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

	if(start_){
		QuitPromptWnd * widget = new QuitPromptWnd(this);
		widget->Create(m_hWnd,_T("QuitPromptWidget"),UI_WNDSTYLE_DIALOG,0L);
		widget->CenterWindow();
		widget->ShowModal();
	}
	else{
		quit();
	}
}

void MainWidget::quit(){
	PostQuitMessage(0);
}

void MainWidget::initMainPannelInterfaces(){

	/*CListUI * showInterfaceLst_ = static_cast<CListUI*>(paintManager_.FindControl(_T("showInterfaceLst")));
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
	}*/
}

//判断Ctrl键是否按下.
#define IsCTRLPressed()((GetKeyState(VK_CONTROL) & (1<<(sizeof(SHORT)*8-1)))!= 0)  

LRESULT MainWidget::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled){

	bool res = false;
	switch(uMsg)
	{
	case WM_STARTCAPTURE:
		res = true;
		start((const char *)wParam);
		break;
	case WM_DROPFILES:
		res = true;
		dealDragFils((HDROP)wParam);
		break;
	case WM_OPENFILE:
		res = true;
		{CommonFileDlg dlg(GetHWND());
		std::string fileName = dlg.getOpenFileName();
		if(fileName!= _T(" ")){
			openDumpFile(fileName.c_str());
		}}
		break;
	case WM_SAVEFILE:
		res = true;
		break;
	case WM_KEYDOWN:
		res = true;
		if(IsCTRLPressed()){
			if(wParam == 'S' || wParam== 's'){
				//保存文件
				saveToDumpFile();
			}
		}
		break;
	}

	return res;
}

void MainWidget::toShowMode(){

	CHorizontalLayoutUI *hack = static_cast<CHorizontalLayoutUI*>(paintManager_.FindControl(_T("HackerWnd")));
	CVerticalLayoutUI * protocalInfoShowZoneUI_ = static_cast<CVerticalLayoutUI *>(paintManager_.FindControl(_T("protocalInfoShowZone")));
	
	//加载静态网业
	 CWebBrowserUI* pWebBrowser = static_cast<CWebBrowserUI*>(paintManager_.FindControl(_T("HackerWnd")));
     if( pWebBrowser ) {
		 std::string path = utility::getResourceDir() + _T("skin\\static.html");
         pWebBrowser->Navigate2(path.c_str()); 
     }	
	hack->SetVisible(false);
	pWebBrowser->SetVisible(false);
	protocalInfoShowZoneUI_->SetVisible(true);
}

void MainWidget::start(const char *name){

	assert(!start_);
	start_ = true;
	toShowMode();
	CEditUI * filter = static_cast<CEditUI*>(paintManager_.FindControl(_T("filter")));
	CaptureCore::instance().setShowCore(&showCore_);
	CaptureCore::instance().setFilter(filter->GetText());
	CaptureCore::instance().startCapture(name);
}

//菜单响应

void MainWidget::showMenuWnd(TNotifyUI & msg){

	if(!active_){
		active_ = true;
		curWnd_ = createMenuWnd(msg);
	}else{
		if(curWnd_!= NULL){
			curWnd_->Close();
		}
		disableMenuActive();
	}
}

WidgetWnd * MainWidget::createMenuWnd(TNotifyUI & msg){
	
	RECT rect = msg.pSender->GetPos();
	CStdString className = msg.pSender->GetName();
	POINT pt_ = {rect.left,rect.bottom};
	::ClientToScreen(*this,&pt_);
	WidgetWnd *widget = NULL; 
	if(className== _T("Files")){
		widget = new MenuFileWnd(this);
	}else if(className==_T("Edit")){
		widget = new MenuEditWnd(this);
	}else if(className==_T("View")){
		widget = new MenuViewWnd(this);
	}else if(className==_T("Go")){
		widget = new MenuGoWnd(this);
	}else if(className==_T("Capture")){
		widget = new MenuCaptureWnd(this);
	}else if(className==_T("Analyze")){
		widget = new MenuAnalyzeWnd(this);
	}else if(className==_T("Statistics")){
		widget = new MenuStatisticsWnd(this);
	}else if(className==_T("Telephony")){
		widget = new MenuTelephonyWnd(this);
	}else if(className==_T("Internals")){
		widget = new MenuInternalsWnd(this);
	}else if(className==_T("Tools")){
		widget = new MenuToolsWnd(this);
	}else{
		//widget = new MenuHelpWnd(this);
	}
	if(widget!= NULL){
		widget->Create(m_hWnd,className,WS_POPUP,WS_EX_TOOLWINDOW);
		widget->ShowWindow();
		::SetWindowPos(*widget,NULL,pt_.x,pt_.y,0,0,SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
		::SendMessage(m_hWnd, WM_NCACTIVATE, TRUE, 0L);
	}
	return widget;
}

void MainWidget::mouseEnter(DuiLib::TNotifyUI & msg){

	if(active_){     
		if(curWnd_!=NULL){
			curWnd_->Close();
			curWnd_= NULL;
		}
		
	}
	createMenuWnd(msg);
}
//

void MainWidget::dealDragFils(HDROP hDrop){

	char path[MAX_PATH+1] = {0};
	::DragQueryFile(hDrop,0,path,sizeof(path));
	openDumpFile(path);
	::DragFinish(hDrop);

}
void MainWidget::openDumpFile(const char *file){
	pcap_t *fp;
	char errbuf[PCAP_ERRBUF_SIZE];
	if ((fp= pcap_open_offline(file,errbuf))==NULL){    
		return ;
	}
	toShowMode();
	CaptureCore::instance().setShowCore(&showCore_);
	CaptureCore::instance().startCapture(fp);
}

void MainWidget::saveToDumpFile(){
	

}
//

void MainWidget::onApplyFilter(TNotifyUI&msg){

	CStdString filter_ = static_cast<CEditUI*>(paintManager_.FindControl("filter"))->GetText();
	CaptureCore::instance().setFilter(filter_);
}

