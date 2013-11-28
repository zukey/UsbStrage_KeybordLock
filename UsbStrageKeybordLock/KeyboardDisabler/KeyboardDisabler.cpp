// KeyboardDisabler.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "KeyboardDisabler.h"

//-----------------------------------------------------------------------------------
// 内部使用関数宣言
//-----------------------------------------------------------------------------------
void HookStart();
void HookEnd();
LRESULT CALLBACK HookProc(int nCode, WPARAM wp, LPARAM lp);

//-----------------------------------------------------------------------------------
// 外部使用変数
//-----------------------------------------------------------------------------------
extern HINSTANCE hInstance;		// DLLインスタンス(dllmainで設定)

//-----------------------------------------------------------------------------------
// 内部使用変数
//-----------------------------------------------------------------------------------
HHOOK hHook = NULL;				// フックハンドル


//-----------------------------------------------------------------------------------
// CKeyboardDisabler
//-----------------------------------------------------------------------------------
// これは、エクスポートされたクラスのコンストラクターです。
// クラス定義に関しては KeyboardDisabler.h を参照してください。
CKeyboardDisabler::CKeyboardDisabler()
{
	return;
}

/// <summary>デストラクタ</summary>
CKeyboardDisabler::~CKeyboardDisabler(void)
{
	End();
}

/// <summary>キーボードの入力無効化を開始します。</summary>
void CKeyboardDisabler::Start()
{
	HookStart();
}

/// <summary>キーボードの入力無効化を終了します。</summary>
void CKeyboardDisabler::End()
{
	HookEnd();
}

/// <summary>キーボードの入力無効化が行われているかを判定します。</summary>
bool CKeyboardDisabler::IsDisabled()
{
	if (hHook != NULL) { return true; }
	return false;
}



//-----------------------------------------------------------------------------------
// 内部使用関数
//-----------------------------------------------------------------------------------

/// <summary>キーボードの入力無効化を開始します。</summary>
void HookStart()
{
	if (hHook != NULL) { return; }

	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, hInstance, 0);
}

/// <summary>キーボードの入力無効化を終了します。</summary>
void HookEnd()
{
	if (hHook != NULL)
	{
		UnhookWindowsHookEx(hHook);
	}
	hHook = NULL;
}

/// <summary>フックハンドラ</summary>
/// <param name="nCode">フックコード</param>
/// <param name="wp">メッセージ識別子</param>
/// <param name="lp">メッセージデータ</param>
LRESULT CALLBACK HookProc(int nCode, WPARAM wp, LPARAM lp)
{
	if (nCode != HC_ACTION) { return CallNextHookEx(hHook, nCode, wp, lp); }

	// すべてのキー入力を無効化
	return TRUE;
}

