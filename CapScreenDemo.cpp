// dui-demo.cpp : main source file
//

#include "stdafx.h"
#include "MainDlg.h"

#include "SSnapshotCtrl.h"
#include "CEdit9527.h"
//从PE文件加载，注意从文件加载路径位置
#define RES_TYPE 1
// #define RES_TYPE 1  //从PE资源中加载UI资源

#ifdef _DEBUG
#define SYS_NAMED_RESOURCE _T("soui-sys-resourced.dll")
#else
#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")
#endif

#pragma data_seg(".Shared_CapScreen")        // 声明共享数据段，并命名该数据段
HWND hMainWnd = NULL;       // 必须在定义的同时进行初始化!!!!
#pragma data_seg()
#pragma comment(linker,"/section:.Shared_CapScreen,rws")  


//定义唯一的一个R,UIRES对象,ROBJ_IN_CPP是resource.h中定义的宏。
ROBJ_IN_CPP

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{
	//已经启动进程
	if (IsWindow(hMainWnd))
	{
		//MessageBox(hMainWnd, L"禁止多实例运行", L"", MB_OK|MB_ICONERROR);
		SendMessage(hMainWnd, WM_STARTSNAPSHOT,NULL,NULL);
		return 0;
	}

	HRESULT hRes = OleInitialize(NULL);
	SASSERT(SUCCEEDED(hRes));
	int nRet = 0;

	SComMgr *pComMgr = new SComMgr;

	//将程序的运行路径修改到项目所在目录所在的目录
	TCHAR szCurrentDir[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szCurrentDir, sizeof(szCurrentDir));
	LPTSTR lpInsertPos = _tcsrchr(szCurrentDir, _T('\\'));
	_tcscpy(lpInsertPos + 1, _T("..\\"));
	SetCurrentDirectory(szCurrentDir);
	{
		BOOL bLoaded = FALSE;
		CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
		CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
		bLoaded = pComMgr->CreateRender_GDI((IObjRef**)&pRenderFactory);
		SASSERT_FMT(bLoaded, _T("load interface [render] failed!"));
		bLoaded = pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
		SASSERT_FMT(bLoaded, _T("load interface [%s] failed!"), _T("imgdecoder"));

		pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
		SApplication *theApp = new SApplication(pRenderFactory, hInstance);
		//从DLL加载系统资源
		{
			HMODULE hModSysRes = LoadLibrary(SYS_NAMED_RESOURCE);
			SASSERT(hModSysRes);
			CAutoRefPtr<IResProvider> sysResProvider;
			CreateResProvider(RES_PE, (IObjRef**)&sysResProvider);
			sysResProvider->Init((WPARAM)hModSysRes, 0);
			theApp->LoadSystemNamedResource(sysResProvider);
		}
		//load app resource.
		{
			CAutoRefPtr<IResProvider>   pResProvider;
			CreateResProvider(RES_PE, (IObjRef**)&pResProvider);
			pResProvider->Init((WPARAM)hInstance, 0);
			theApp->InitXmlNamedID(namedXmlID, ARRAYSIZE(namedXmlID), TRUE);
			theApp->AddResProvider(pResProvider);
		}
		theApp->RegisterWindowClass<SSnapshotCtrl>();
		theApp->RegisterWindowClass<CEdit9527>();
		// BLOCK: Run application
		{
			CMainDlg dlgMain;
			dlgMain.Create(GetActiveWindow());

			hMainWnd = dlgMain.GetNative()->m_hWnd;

			dlgMain.SendMessage(WM_INITDIALOG);
			dlgMain.CenterWindow(dlgMain.m_hWnd);
			dlgMain.ShowWindow(SW_SHOWNORMAL);
			nRet = theApp->Run(dlgMain.m_hWnd);
	}

		delete theApp;
}

	delete pComMgr;

	OleUninitialize();
	return nRet;
}
