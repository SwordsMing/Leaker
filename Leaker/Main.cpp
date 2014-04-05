#include "stdafx.h"
#include "widget/MainWidget.h"
#include "plugin/PluginManager.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath()+_T("skin\\default"));

    HRESULT Hr = ::CoInitialize(NULL);
    if( FAILED(Hr) ) return 0;

	//加载协议解析插件
	PluginManager::instance().registerCPlugins();
	PluginManager::instance().registerLuaPlugins();

    MainWidget* pMain = new MainWidget();
    if( pMain == NULL ) return 0;
	pMain->Create(NULL, _T("Leaker"),UI_WNDSTYLE_FRAME,0L);
    pMain->CenterWindow();
    ::ShowWindow(*pMain, SW_SHOWMAXIMIZED);
    CPaintManagerUI::MessageLoop();

    ::CoUninitialize();
    return 0;
}