#include "MenuTelephonyWnd.h"


void MenuTelephonyWnd::init(){
	
	paintManager_.AddMessageFilter(this);

}


LRESULT MenuTelephonyWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled){

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

LRESULT MenuTelephonyWnd::onKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam){
	
	if(m_hWnd != (HWND) wParam ){
		owner_->disableMenuActive();
		PostMessage(WM_CLOSE);
	}
    return 0;
}

LRESULT MenuTelephonyWnd::onKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam){
	if( wParam == VK_ESCAPE ){
		owner_->disableMenuActive();
		Close();
	}
    return 0;
}