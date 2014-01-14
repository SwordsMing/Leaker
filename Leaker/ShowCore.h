#ifndef LEAKER_SHOWCORE_H_
#define LEAKER_SHOWCORE_H_

#include "stdafx.h"
#include <boost/thread.hpp>
#include "utility/Thread.h"
#include <deque>
#include <string>
#include "Protocal.h"
#include "ShowCore.h"
#include "ControlExt/TreeView.h"
#include "widget/WidgetWnd.h"
#include <utility>

class ShowCore: public IListCallbackUI
{
public:
	ShowCore();
	~ShowCore();
	void startShow();
	void stopShow();
	void setMainWnd(WidgetWnd * wnd){mainWnd_ = wnd;}
	void add(const PacketInfo* pkt);
	void addShowDetail(const PacketInfo * pkt);
	void setLstUI(CListUI * lst){lstUI_ = lst;}
	void setTreeViewUI(CTreeViewUI* tree){treeViewUI_ = tree;}
	void setHexViewUI(CRichEditUI* view){hexViewUI_ = view;}
	void showFunc();
	void helperFunc();
	//回调接口，用于显示列表信息
	 LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem);
	 void onSelectItem(TNotifyUI& notify);
	 void onItemSelect(TNotifyUI& msg); //响应树形控件
private:

	typedef std::pair<CTreeViewUI::Node*,HITEM> TreeNode;
	WidgetWnd * mainWnd_;
	bool start_;
	bool helperStart_;
	ShowCore * showCore_;       //显示协议信息对象
	Thread *   showThread_;     //此线程单独处理列表框信息的显示
	Thread *   helperThread_;   //此线程处理树形控件……的显示
	CListUI * lstUI_;
	CTreeViewUI * treeViewUI_;
	CRichEditUI * hexViewUI_;
	const PacketInfo * curPkt_;      //当前显示信息
	boost::mutex showMutex_;
	boost::condition_variable_any showCondition_;
	boost::mutex helperMutex_;
	boost::condition_variable_any helperCondition_;
	std::deque<const PacketInfo*> infoBuf_;
	std::deque<const PacketInfo*> helperBuf_;
	void show(const PacketInfo * pkt);
	void showDetail(const PacketInfo* pkt);
	void assertInit()const;
	void showTreeInfo(const PacketInfo * pkt);
	void showHexInfo(const PacketInfo * pkt);
	void addTreeInfo(const RootedTree<std::string> * info);
};


#endif