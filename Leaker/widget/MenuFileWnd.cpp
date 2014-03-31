#include "MenuFileWnd.h"
#include "../utility/CommonFileDlg.h"
#include <boost/bind.hpp>
#include <string>
#include "../Message.h"

void MenuFileWnd::init(){
	
	paintManager_.AddMessageFilter(this);
	registerEvent(DUI_ITEMCLICK,_T("menu_0_0"),boost::bind(&MenuFileWnd::onOpenFile,this,_1));


}


LRESULT MenuFileWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled){

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

void MenuFileWnd::onOpenFile(TNotifyUI & msg){

	//此处不能用SendMessage同步发送消息，否则会引发程序崩溃
	//具体原因是在弹出“对话框窗口”时，原菜单窗口因失去焦点而导致
	//"delete this，再对这个类的任何函数调用操作会导致bug
	::PostMessage(owner_->GetHWND(),WM_OPENFILE,0,0);
}

LRESULT MenuFileWnd::onKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(m_hWnd != (HWND) wParam ){
		owner_->disableMenuActive();
		PostMessage(WM_CLOSE);
	}
    return 0;
}

LRESULT MenuFileWnd::onKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam){
	if( wParam == VK_ESCAPE ){
		owner_->disableMenuActive();
		Close();
	}
     return 0;
}