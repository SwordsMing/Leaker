#include "WidgetWnd.h"
#include <boost/bind.hpp>
//#include "../ControlExt/UITreeView.h"
#include "../ControlExt/treeview.hpp"
#include "../ControlExt/UICheckBox.h"
#include "../ControlExt/UIWebBrowser.h"

LRESULT WidgetWnd::onCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
	paintManager_.Init(m_hWnd);
    CDialogBuilder builder;
    CControlUI* pRoot = builder.Create(getWidgetXml(),(UINT)0,static_cast<IDialogBuilderCallback*>(this), &paintManager_);
    ASSERT(pRoot && "Failed to parse XML");
    paintManager_.AttachDialog(pRoot);
    paintManager_.AddNotifier(this);

	registerEvent(DUI_CLICK,_T("closeBtn"),boost::bind(&WidgetWnd::onCloseBtn,this,_1));

	init();

	return 0L;
}
LRESULT WidgetWnd::onClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled){

	//::DestroyWindow(m_hWnd);
	bHandled = FALSE;
	return 0L;
}

LRESULT WidgetWnd::onDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
	bHandled = FALSE;
	return 0L;
}

LRESULT WidgetWnd::onSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){

		SIZE szRoundCorner = paintManager_.GetRoundCorner();
        if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
            CRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++; rcWnd.bottom++;
            HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }

        bHandled = FALSE;
        return 0;
}

LRESULT WidgetWnd::onGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){

		MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
        CRect rcWork = oMonitor.rcWork;
        LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
        lpMMI->ptMaxPosition.x = rcWork.left;
        lpMMI->ptMaxPosition.y = rcWork.top;
        lpMMI->ptMaxSize.x = rcWork.right - rcWork.left;
        lpMMI->ptMaxSize.y = rcWork.bottom - rcWork.top;
        bHandled = FALSE;
        return 0;
}

LRESULT WidgetWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){

		// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
		if( wParam == SC_CLOSE ) {
			::PostQuitMessage(0L);
			bHandled = TRUE;
			return 0;
		}
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if( ::IsZoomed(*this) != bZoomed ) {
			if( !bZoomed ) {
				CControlUI* pControl = static_cast<CControlUI*>(paintManager_.FindControl(_T("maxBtn")));
				if( pControl ) pControl->SetVisible(false);
				pControl = static_cast<CControlUI*>(paintManager_.FindControl(_T("restoreBtn")));
				if( pControl ) pControl->SetVisible(true);
			}
			else {
				CControlUI* pControl = static_cast<CControlUI*>(paintManager_.FindControl(_T("maxBtn")));
				if( pControl ) pControl->SetVisible(true);
				pControl = static_cast<CControlUI*>(paintManager_.FindControl(_T("restoreBtn")));
				if( pControl ) pControl->SetVisible(false);
			}
		}
		return lRes;
}

LRESULT WidgetWnd::OnNcHitTest(UINT uMsg, WPARAM wParam,LPARAM lParam, BOOL& bHandled){

	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);

// 		if( !::IsZoomed(*this) ) {
// 			RECT rcSizeBox = m_pm.GetSizeBox();
// 			if( pt.y < rcClient.top + rcSizeBox.top ) {
// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
// 				return HTTOP;
// 			}
// 			else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
// 				return HTBOTTOM;
// 			}
// 			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
// 			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
// 		}

		RECT rcCaption = paintManager_.GetCaptionRect();
		if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
				CControlUI* pControl = static_cast<CControlUI*>(paintManager_.FindControl(pt));
				if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
					_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
					_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
					return HTCAPTION;
		}

		return HTCLIENT;
}

LRESULT WidgetWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if( ::IsIconic(*this) ) bHandled = FALSE;
    return (wParam == 0) ? TRUE : FALSE;
}

LRESULT WidgetWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT WidgetWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT WidgetWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam){
	LRESULT lRes = 0;
    BOOL bHandled = TRUE;
	switch( uMsg ){
	   case WM_CREATE: lRes = onCreate(uMsg,wParam,lParam,bHandled); break;
	  // case WM_CLOSE:  lRes = onClose(uMsg,wParam,lParam,bHandled);  break;
	   case WM_DESTROY:lRes = onDestroy(uMsg,wParam,lParam,bHandled); break;
	   case WM_SIZE:   lRes = onSize(uMsg,wParam,lParam,bHandled); break;
	   case WM_NCACTIVATE: lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	   case WM_NCCALCSIZE: lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	   case WM_NCPAINT:    lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	   case WM_NCHITTEST: lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	   case WM_GETMINMAXINFO: lRes = onGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	   case WM_SYSCOMMAND: lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
       default:
            bHandled = FALSE;
    }
    if( bHandled ) return lRes;
    if( paintManager_.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void WidgetWnd::registerEvent(const string & eventType,const string & controlName,const eventCallback & cb){
	controlCallback tmp;
	tmp.first = controlName;
	tmp.second = cb;
	registertedEvent_.insert(pair<string,controlCallback>(eventType,tmp));
}

void WidgetWnd::Notify(TNotifyUI &msg){
	multimap<string,controlCallback>::iterator it_ = registertedEvent_.find(string(msg.sType));
	if(it_!= registertedEvent_.end()){
		int num = registertedEvent_.count(string(msg.sType));
		for (int i= 0; i<num; i++){
			CStdString senderName = msg.pSender->GetName();
			CStdString name = (it_->second).first.c_str();
			if(senderName == name){
				((it_->second).second)(msg);         //回调控件响应函数
				break;
			}
			it_++;
		}
	}
}

CControlUI* WidgetWnd::CreateControl(LPCTSTR pstrClass){
	
	if( _tcscmp(pstrClass, _T("TreeView")) == 0 ) return new CTreeViewUI;
	//else if(_tcscmp(pstrClass,_T("CheckBox"))== 0) return new CCheckBoxUI;
	else if( _tcscmp(pstrClass,_T("WebBrowser")) == 0 )return new CWebBrowserUI;
    return NULL;
}

void WidgetWnd::onCloseBtn(TNotifyUI&msg){

	Close();
}

void WidgetWnd::onMaxBtn(TNotifyUI &msg){
	
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void WidgetWnd::onMinBtn(TNotifyUI &msg){

	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void WidgetWnd::onRestoreBtn(TNotifyUI & msg){
	
	SendMessage(WM_SYSCOMMAND,SC_RESTORE,0);
}
