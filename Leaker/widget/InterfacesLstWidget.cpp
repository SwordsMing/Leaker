#include "InterfacesLstWidget.h"
#include <boost/bind.hpp>
#include "../AdaptersInfo.h"
#include "MainWidget.h"
#include "../Message.h"

InterfacesLstWidget::InterfacesLstWidget(MainWidget* mainWidget)
	:interfacesLstUI_(NULL),
	mainWidget_(mainWidget)
{

}
void InterfacesLstWidget::init(){

	registerEvent(DUI_CLICK,_T("minBtn"),boost::bind(&InterfacesLstWidget::onMinBtn,this,_1));
	registerEvent(DUI_CLICK,_T("startBtn"),boost::bind(&InterfacesLstWidget::onStartBtn,this,_1));
	registerEvent(DUI_CLICK,_T("stopBtn"),boost::bind(&InterfacesLstWidget::onStopBtn,this,_1));
	registerEvent(DUI_CLICK,_T("optionBtn"),boost::bind(&InterfacesLstWidget::onOptionBtn,this,_1));

	interfacesLstUI_ = static_cast<CListUI*>(paintManager_.FindControl(_T("interfacesInfoLst")));
}

const char * InterfacesLstWidget::getWidgetXml()const{

	return _T("InterfacesLstWidget.xml");
}

LPCTSTR InterfacesLstWidget::GetWindowClassName() const{

	return _T("InterfacesLstWidget");
}

UINT InterfacesLstWidget::GetClassStyle() const{
		
	return CS_DBLCLKS;
}

void InterfacesLstWidget::initInterfacesLst(){

	const pcap_if_t * pcap_ = AdaptersInfo::instance().getAllDevsInfo();
	for (;pcap_!= NULL; pcap_= pcap_->next)
	{
		CListContainerElementUI * pLstElement = new CListContainerElementUI;
		CHorizontalLayoutUI * pHorElement = new CHorizontalLayoutUI;
		COptionUI * pOption		= new COptionUI; pOption->SetManager(&paintManager_,NULL);
		optionGroup_.push_back(pOption);
		pOption->SetGroup(_T("optionInterfaces"));
		CLabelUI * pDevLab		= new CLabelUI;
		CLabelUI * pDesLab		= new CLabelUI;
		CLabelUI * pIpLab		= new CLabelUI;
		CLabelUI * pPackets		= new CLabelUI;
		CLabelUI * pPacketsPerS = new CLabelUI;
		CButtonUI * pDetailBtn	= new CButtonUI;

		//Set Attributes
		//registerEvent(DUI_SELECTCHANGED,_T("option"),boost::bind(&InterfacesLstWidget::onChooseBtn,this,_1));
		pOption->SetUserData(pcap_->name);
		pOption->SetAttribute(_T("padding"),_T("2,10,0,0"));
		pOption->SetAttribute(_T("height"),_T("17"));
		pOption->SetAttribute(_T("width"),_T("17"));
		pOption->SetAttribute(_T("name"),_T("option"));
		pOption->SetAttribute(_T("normalimage"),_T("file='hook_option.png' source='0,0,17,17'"));
		pOption->SetAttribute(_T("selectedimage"),_T("file='hook_option.png' source='0,34,17,51'"));

		pDevLab->SetText(pcap_->name);
		pDevLab->SetAttribute(_T("height"),_T("20"));
		pDevLab->SetAttribute(_T("padding"),_T("0,10,0,0"));
		pDevLab->SetAttribute(_T("textcolor"),_T("0XFFFFFFFF"));

		pDesLab->SetAttribute(_T("width"),_T("100"));
		pDesLab->SetAttribute(_T("height"),_T("20"));
		pDesLab->SetAttribute(_T("padding"),_T("0,10,0,0"));
		pDesLab->SetText(pcap_->description);
		pDesLab->SetAttribute(_T("textcolor"),_T("0XFFFFFFFF"));

		pDetailBtn->SetAttribute(_T("padding"),_T("0,10,0,0"));
		pDetailBtn->SetAttribute(_T("name"),_T("detailsBtn"));
		pDetailBtn->SetText(_T("Details"));
		pDetailBtn->SetAttribute(_T("textcolor"),_T("0XFFFFFFFF"));
		pDetailBtn->SetAttribute(_T("height"),_T("20"));
		pDetailBtn->SetAttribute(_T("width"),_T("60"));
		pDetailBtn->SetAttribute(_T("bkcolor"),_T("#FF363636"));
		pDetailBtn->SetAttribute(_T("bordersize"),_T("1"));
		pDetailBtn->SetAttribute(_T("borderround"),_T("3,3"));

		pHorElement->Add(pOption);
		pHorElement->Add(pDevLab);
		pHorElement->Add(pDesLab);
		pHorElement->Add(pIpLab);
		pHorElement->Add(pPackets);
		pHorElement->Add(pPacketsPerS);
		pHorElement->Add(pDetailBtn);

		pHorElement->SetAttribute(_T("height"),_T("20"));
		pHorElement->SetAttribute(_T("bkcolor"),_T("#FF000000"));
		pLstElement->Add(pHorElement);
		interfacesLstUI_->Add(pLstElement);
	}
}


void InterfacesLstWidget::onStartBtn(TNotifyUI&){

	curInterface_ = getChoosedInterface();
	if(!curInterface_.IsEmpty()){
		::SendMessage(mainWidget_->GetHWND(),WM_STARTCAPTURE,(WPARAM)curInterface_.GetData(),0);  //start to capture
	}
	Close();
}

void InterfacesLstWidget::onStopBtn(TNotifyUI&){}
void InterfacesLstWidget::onOptionBtn(TNotifyUI&){}

void InterfacesLstWidget::onChooseBtn(TNotifyUI& notify){
	
	COptionUI * option = static_cast<COptionUI*>(notify.pSender);
	if(!option->IsSelected()){

		curInterface_.Empty();
	}
	else{
		curInterface_ = notify.pSender->GetUserData();
		std::vector<COptionUI*>::iterator it_;
		for (it_= optionGroup_.begin(); it_!= optionGroup_.end(); it_++){
			if(*it_!= option){
				if(option->IsSelected()){
					option->Selected(false);
				}
			}
		}

	}
}

CStdString InterfacesLstWidget::getChoosedInterface()const{

	std::vector<COptionUI*>::const_iterator  it_ = optionGroup_.begin();
	for (;it_!= optionGroup_.end(); it_++)
	{
		if((*it_)->IsSelected())
		{
			return (*it_)->GetUserData();
		}
	}
	return CStdString();
}