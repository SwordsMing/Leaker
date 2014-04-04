#include "CommonFileDlg.h"

const char CommonFileDlg::filterBuf_[] = "pcap file(*.pcap)\0*.pcap\0";

void CommonFileDlg::init(){

	::ZeroMemory(&fileNameInfo_,sizeof(fileNameInfo_));
	::ZeroMemory(buf_,sizeof(buf_));

	fileNameInfo_.lStructSize = sizeof(fileNameInfo_);
	fileNameInfo_.lpstrFile = buf_;
	fileNameInfo_.hwndOwner = hWnd_;
	fileNameInfo_.nMaxFile = sizeof(buf_);
	fileNameInfo_.nFilterIndex    = 1;  
    fileNameInfo_.lpstrFileTitle  = (LPSTR)filterBuf_; 
    fileNameInfo_.nMaxFileTitle   = 0;  
    fileNameInfo_.lpstrInitialDir = NULL;  
    fileNameInfo_.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; 
}

std::string CommonFileDlg::getOpenFileName(){

	init();
	if(::GetOpenFileName(&fileNameInfo_)){
		return std::string(buf_);
	}
	return std::string("");
}

std::string CommonFileDlg::getSaveFileName(){

	init();
	if(::GetSaveFileName(&fileNameInfo_)){
		return std::string(buf_);
	}
	return std::string("");
}