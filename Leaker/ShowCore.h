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
	//�ص��ӿڣ�������ʾ�б���Ϣ
	 LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem);
	 void onSelectItem(TNotifyUI& notify);
	 void onItemSelect(TNotifyUI& msg); //��Ӧ���οؼ�
private:

	typedef std::pair<CTreeViewUI::Node*,HITEM> TreeNode;
	WidgetWnd * mainWnd_;
	bool start_;
	bool helperStart_;
	ShowCore * showCore_;       //��ʾЭ����Ϣ����
	Thread *   showThread_;     //���̵߳��������б����Ϣ����ʾ
	Thread *   helperThread_;   //���̴߳������οؼ���������ʾ
	CListUI * lstUI_;
	CTreeViewUI * treeViewUI_;
	CRichEditUI * hexViewUI_;
	const PacketInfo * curPkt_;      //��ǰ��ʾ��Ϣ
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