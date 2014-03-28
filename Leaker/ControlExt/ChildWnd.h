/*

	为Duilib提供真正的了窗口控件,
	2014/03/20

*/
#ifndef LEAKER_CHILDWND_H_
#define LEAKER_CHILDWND_H_

#include "../stdafx.h"
#include <assert.h>

class ChildWnd: public CControlUI
{
public:
	
	ChildWnd():hWnd_(NULL){}

	void attach(HWND hwnd){
		assert(hwnd!= NULL);
		hWnd_ = hwnd;
	}

	void setVisible(bool show){
		if(show){
			CControlUI::SetVisible(true);
			::ShowWindow(hWnd_,SW_SHOW);
		}else{
			CControlUI::SetVisible(false);
			::ShowWindow(hWnd_,SW_HIDE);
		}
	}

	HWND detach(){
			HWND tmp = hWnd_;
			hWnd_ = NULL;
			return tmp;
		}
	HWND getHwnd()const{
		return hWnd_;
	}
    operator HWND()const{
		return hWnd_;
	}
protected:
	HWND hWnd_;
};

#endif