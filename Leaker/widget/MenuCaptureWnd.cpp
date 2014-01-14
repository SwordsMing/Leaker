#include "MenuCaptureWnd.h"
#include "InterfacesLstWidget.h"
#include <boost/bind.hpp>
#include "../CaptureCore.h"

void MenuCaptureWnd::init(){
	
	paintManager_.AddMessageFilter(this);
	registerEvent(DUI_ITEMCLICK,_T("menu_Interfaces"),boost::bind(&MenuCaptureWnd::onInterfaces,this,_1));
	registerEvent(DUI_ITEMCLICK,_T("menu_Options"),boost::bind(&MenuCaptureWnd::onOptions,this,_1));
	registerEvent(DUI_ITEMCLICK,_T("menu_Start"),boost::bind(&MenuCaptureWnd::onStart,this,_1));
	registerEvent(DUI_ITEMCLICK,_T("menu_Stop"),boost::bind(&MenuCaptureWnd::onStop,this,_1));
	registerEvent(DUI_ITEMSELECT,_T("menu_capture"),boost::bind(&MenuCaptureWnd::onSelectItem,this,_1));
}

void MenuCaptureWnd::onInterfaces(DuiLib::TNotifyUI &msg){

	InterfacesLstWidget *widget = new InterfacesLstWidget(owner_);
	widget->Create(owner_->GetHWND(),_T("InterfacesLstWidget"),UI_WNDSTYLE_DIALOG  ,0L);
	widget->CenterWindow();
	widget->ShowWindow();
	widget->initInterfacesLst();
}

void MenuCaptureWnd::onOptions(DuiLib::TNotifyUI &msg){

}

void MenuCaptureWnd::onStart(TNotifyUI &msg){


}

void MenuCaptureWnd::onStop(TNotifyUI &msg){
	
	CaptureCore::instance().stopCapture();
}

LRESULT MenuCaptureWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled){

	switch(uMsg)
	{
	case WM_KILLFOCUS:
		onKillFocus(uMsg,wParam,lParam);
		bHandled = true;
		break;
	case WM_KEYDOWN:
		onKeyDown(uMsg,wParam,lParam);
		bHandled = true;
		break;
	default:
		bHandled = false;
	}
	return 0L;
}

LRESULT MenuCaptureWnd::onKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	  if(m_hWnd != (HWND) wParam ) PostMessage(WM_CLOSE);
        return 0;
}

LRESULT MenuCaptureWnd::onKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam){
      if( wParam == VK_ESCAPE ) Close();
      return 0;
}

void MenuCaptureWnd::onSelectItem(TNotifyUI& msg){

	Close();
}
