#ifndef LEAKER_COMMONFILEDLG_H_
#define LEAKER_COMMONFILEDLG_H_

#include <boost/noncopyable.hpp>
#include <windows.h>
#include <CommDlg.h>
#include <string>

class CommonFileDlg:boost::noncopyable
{
public:
	explicit CommonFileDlg(HWND hwnd):hWnd_(hwnd){}
	std::string getOpenFileName();
	std::string getSaveFileName();
private:
	void init();
	OPENFILENAME fileNameInfo_;
	HWND hWnd_;
	char buf_[MAX_PATH];
};



#endif