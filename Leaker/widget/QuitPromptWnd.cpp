#include "QuitPromptWnd.h"
#include <boost/bind.hpp>
#include "../CaptureCore.h"

void QuitPromptWnd::init(){
	
	registerEvent(DUI_CLICK,_T("save"),boost::bind(&QuitPromptWnd::onSave,this,_1));
	registerEvent(DUI_CLICK,_T("cancle"),boost::bind(&QuitPromptWnd::onCancle,this,_1));
	registerEvent(DUI_CLICK,_T("nosave"),boost::bind(&QuitPromptWnd::onNoSave,this,_1));
}

void QuitPromptWnd::onCancle(DuiLib::TNotifyUI &msg){

	Close();
}

void QuitPromptWnd::onNoSave(DuiLib::TNotifyUI &msg){
	
	Close();
	CaptureCore::instance().stopCapture();
	owner_->quit();
}

void QuitPromptWnd::onSave(DuiLib::TNotifyUI &ms){

	owner_->saveToDumpFile();
	Close();
	owner_->quit();
}