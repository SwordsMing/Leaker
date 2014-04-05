#include "ShowCore.h"
#include <cassert>
#include <boost/bind.hpp>
#include "CaptureCore.h"

ShowCore::ShowCore()
	:start_(false),
	helperStart_(false),
	lstUI_(NULL),
	treeViewUI_(NULL),
	showThread_(NULL),
	helperThread_(NULL),
	curPkt_(NULL),
	mainWnd_(NULL),
	hexViewUI_(NULL)
{
}

ShowCore::~ShowCore(){

}

void ShowCore::startShow(){
	
	assert(!start_ && !helperStart_);
	assertInit();
	start_ = true;
	helperStart_ = true;
	lstUI_->SetTextCallback(this);   //设置回调对象
	showThread_ = new Thread(boost::bind(&ShowCore::showFunc,this),_T("ShowThread"));
	helperThread_ = new Thread(boost::bind(&ShowCore::helperFunc,this),_T("helperThread"));
	showThread_->startThread();
	helperThread_->startThread();
}

void ShowCore::assertInit()const{

	assert(lstUI_!= NULL && treeViewUI_!= NULL && hexViewUI_!= NULL);
}

void ShowCore::stopShow(){

	assert(start_ && helperStart_);
	start_ = false;
	helperStart_ = false;
}

void ShowCore::showFunc(){
	
	assert(start_);
	while (start_)
	{
		boost::mutex::scoped_lock lock(showMutex_);
		while (infoBuf_.empty()){

			showCondition_.wait(lock);
		}
		if(!infoBuf_.empty()){

			const PacketInfo * pkt = infoBuf_.front();
			infoBuf_.pop_front();
			show(pkt);
		}
	}
}

void ShowCore::helperFunc(){

	while (helperStart_)
	{
		boost::mutex::scoped_lock lock(helperMutex_);
		while (helperBuf_.empty()){

			helperCondition_.wait(lock);
		}
		if(!helperBuf_.empty()){

			const PacketInfo * pkt = helperBuf_.front();
			helperBuf_.pop_front();
			showDetail(pkt);
		}
	}

}

void ShowCore::add(const PacketInfo *pkt){

	boost::mutex::scoped_lock lock(showMutex_);
	infoBuf_.push_back(pkt);
	showCondition_.notify_one();
}

void ShowCore::addShowDetail(const PacketInfo * pkt){
	
#if 0
	TRACE("start addShowDetail.....\n");
	boost::mutex::scoped_lock lock(helperMutex_);
	helperBuf_.push_back(pkt);
	helperCondition_.notify_one();
#endif
	showDetail(pkt);
}

void ShowCore::show(const PacketInfo * pkt){

	 CListTextElementUI* pLstElement = new CListTextElementUI;
     pLstElement->SetTag(reinterpret_cast<UINT_PTR>(pkt));
	 lstUI_->Add(pLstElement);
	 if(pkt->no_== 0)
	 {
		addShowDetail(pkt);
	 }
}

LPCTSTR ShowCore::GetItemText(DuiLib::CControlUI *pControl, int iIndex, int iSubItem){
	
	curPkt_ = reinterpret_cast<const PacketInfo*>(pControl->GetTag());
	switch(iSubItem)
	{
	case 0:
		return curPkt_->columnInfo_.no_.c_str();
	case 1:
		return curPkt_->columnInfo_.time_.c_str();
	case 2:
		return curPkt_->columnInfo_.source_.c_str();
	case 3:
		return curPkt_->columnInfo_.destination_.c_str();
	case 4:
		return curPkt_->columnInfo_.protocal_.c_str();
	case 5:
		return curPkt_->columnInfo_.length_.c_str();
	case 6:
		return curPkt_->columnInfo_.opcode_.c_str();
	case 7:
		return curPkt_->columnInfo_.info_.c_str();
	}
	return NULL;
}

void ShowCore::showDetail(const PacketInfo* pkt){

	    treeViewUI_->RemoveAll();
		hexViewUI_->Clear();
		showTreeInfo(pkt);
		showHexInfo(pkt);
}

void ShowCore::onSelectItem(TNotifyUI& notify){
	
	unsigned int index = notify.wParam;   //选中行
	assert(index>= 0);
	const PacketInfo * pkt = CaptureCore::instance().getPkt(index);
	addShowDetail(pkt);
}

void ShowCore::showTreeInfo(const PacketInfo *pkt){
	
	int no = pkt->detailInfo_.size();
	for (int i= 0; i< no; i++){
		addTreeInfo(pkt->detailInfo_[i]);
	}
}

void ShowCore::addTreeInfo(const RootedTree<std::string> * info){
	
	std::deque<TreeNode> deque_;
	HITEM root_ = info->getRoot();
	assert(root_!= NULL);
	TreeNode treeNode_;
	treeNode_.first = NULL;
	treeNode_.second = root_;
	deque_.push_back(treeNode_);
	CTreeViewUI::Node *parent1,*parent2;
	HITEM child,brother;
	while (!deque_.empty()){
		TreeNode front_ = deque_.front();
		deque_.pop_front();
		//CTreeNodeUI * node = createTreeNode(front_.second->t_.c_str());
		//treeViewUI_->Add(node);
		//parent1 = node;
		parent1 = treeViewUI_->AddNode(front_.second->t_.c_str(),front_.first);
		child = front_.second->child_;
		brother = front_.second->brother_;
		while (child!= NULL){
			//parent2 = createTreeNode(child->t_.c_str());
			//parent1->Add(parent2);
			parent2 = treeViewUI_->AddNode(child->t_.c_str(),parent1);
			if(child->child_!= NULL){
				deque_.push_back(std::pair<CTreeViewUI::Node*,HITEM>(parent2,child->child_));
			}
			child = child->brother_;
		}
		while (brother!= NULL){
			//parent2 = createTreeNode(brother->t_.c_str());
			//(front_.first)->Add(parent2);
			parent2 = treeViewUI_->AddNode(brother->t_.c_str(),front_.first);
			if(brother->child_!= NULL){
				deque_.push_back(std::pair<CTreeViewUI::Node*,HITEM>(parent2,brother->child_));
			}
			brother = brother->brother_;
		}

	}
}

//显示数据包十六进信息
void ShowCore::showHexInfo(const PacketInfo *pkt){

	unsigned int nLength = pkt->caplen_;
	const unsigned char * p = pkt->pureData_;
    unsigned char temp[16];
	int row = nLength / 16;
	int left = nLength % 16;
	char info[MAX_PATH] = {0};
	int len;
	int m= 0;
	int i;

	hexViewUI_->SetReadOnly(false);
	hexViewUI_->SetText(_T(" "));
	for (i= 0; i< row; i++)
	{
		memcpy(temp,p+i*16,16);
		sprintf(info,"  %04x    %02x    %02x    %02x    %02x    %02x    %02x    %02x    %02x    %02x    %02x    %02x    %02x    %02x    %02x    %02x    %02x\r\n",i*16,temp[0],\
			temp[1],temp[2],temp[3],temp[4],temp[5],temp[6],temp[7],\
			temp[8],temp[9],temp[10],temp[11],temp[12],temp[13],temp[14],temp[15]);
		if( i == 0)
		{
			hexViewUI_->SetText(info);
		}
		else
		{
			len = hexViewUI_->GetTextLength();
			CHARRANGE range = {len,len};
			hexViewUI_->SetSel(range);
			hexViewUI_->ReplaceSel(info,false);
		}
		memset(info,0,sizeof(info));
	}
	if( left!= 0)
	{
		char buf[15] = {0};
		memcpy(temp,p+16*i,left);
		sprintf(info,_T("  %04x    "),i*16);
		for (int j= 0; j< left; j++){
				sprintf(buf,_T("%02x    "),temp[j]);
				strcat(info,buf);
				memset(buf,0,sizeof(buf));
		}
		len = hexViewUI_->GetTextLength();
		CHARRANGE range = {len,len};
		hexViewUI_->SetSel(range);
		hexViewUI_->ReplaceSel(info,false);
	}
	hexViewUI_->SetReadOnly(true);
}

void ShowCore::onItemSelect(TNotifyUI& msg){
	CControlUI * sender = static_cast<CListUI*>(msg.pSender)->GetItemAt((int)msg.wParam);
	CTreeViewUI::Node* node = (CTreeViewUI::Node*)sender->GetTag();
	POINT pt = { 0 };
    ::GetCursorPos(&pt);
    ::ScreenToClient(mainWnd_->paintManager_.GetPaintWindow(), &pt);
    pt.x -= msg.pSender->GetX();
    SIZE sz = treeViewUI_->GetExpanderSizeX(node);
    if( pt.x >= sz.cx && pt.x < sz.cy ){
		treeViewUI_->SetChildVisible(node, !node->data()._child_visible);
	}
}

#if 0
CTreeNodeUI* ShowCore::createTreeNode(LPCTSTR str){

	 CTreeNodeUI* pNodeTmp = new CTreeNodeUI;
     pNodeTmp->SetItemTextColor(0xFFC8C6CB);
     pNodeTmp->SetItemHotTextColor(0xFFC8C6CB);
     pNodeTmp->SetSelItemTextColor(0xFFC8C6CB);
    pNodeTmp->SetTag(1);
     pNodeTmp->SetItemText(str);  
     pNodeTmp->SetAttribute(_T("height"), _T("22"));
     pNodeTmp->SetAttribute(_T("inset"), _T("7,0,0,0"));
     pNodeTmp->SetAttribute(_T("itemattr"), _T("valign=\"vcenter\" font=\"4\""));
     pNodeTmp->SetAttribute(_T("folderattr"), _T("width=\"0\" float=\"true\""));

	 return pNodeTmp;
}
#endif