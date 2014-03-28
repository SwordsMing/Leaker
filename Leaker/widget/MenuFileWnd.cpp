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

	/*MainWidget * tmp = owner_;
	CommonFileDlg dlg(tmp->GetHWND());
	std::string fileName = dlg.getOpenFileName();
	if(fileName!= _T(" ")){
		tmp->openDumpFile(fileName.c_str());
	}*/
	::SendMessage(owner_->GetHWND(),WM_OPENFILE,0,0);
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