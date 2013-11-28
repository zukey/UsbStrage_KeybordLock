
// UsbStrageKeybordLock.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CUsbStrageKeybordLockApp:
// このクラスの実装については、UsbStrageKeybordLock.cpp を参照してください。
//

class CUsbStrageKeybordLockApp : public CWinApp
{
public:
	CUsbStrageKeybordLockApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CUsbStrageKeybordLockApp theApp;