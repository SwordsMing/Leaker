#ifndef LEAKER_BUILDERCALLBACKEX_H_
#define LEAKER_BUILDERCALLBACKEX_H_

#include "treeview.h"
#include "childWnd.h"

class CDialogBuilderCallbackEx : public IDialogBuilderCallback
{
public:
    CControlUI* CreateControl(LPCTSTR pstrClass) 
    {
        if( _tcscmp(pstrClass, _T("TreeView")) == 0 ) return new CTreeViewUI;
		if( _tcscmp(pstrClass,_T("ChildWnd")) == 0){
			ChildWnd * pWnd = new ChildWnd;
			HWND hWnd = CreateWindow(_T("#32770"), _T("ChildWndEx"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, m_PaintManager.GetPaintWindow(), (HMENU)0, NULL, NULL);
			pWnd->attach(hWnd);
			return new ChildWnd;
        return NULL;
    }
};


#endif 