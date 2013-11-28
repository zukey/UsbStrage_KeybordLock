// KeyboardDisabler.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "KeyboardDisabler.h"

//-----------------------------------------------------------------------------------
// �����g�p�֐��錾
//-----------------------------------------------------------------------------------
void HookStart();
void HookEnd();
LRESULT CALLBACK HookProc(int nCode, WPARAM wp, LPARAM lp);

//-----------------------------------------------------------------------------------
// �O���g�p�ϐ�
//-----------------------------------------------------------------------------------
extern HINSTANCE hInstance;		// DLL�C���X�^���X(dllmain�Őݒ�)

//-----------------------------------------------------------------------------------
// �����g�p�ϐ�
//-----------------------------------------------------------------------------------
HHOOK hHook = NULL;				// �t�b�N�n���h��


//-----------------------------------------------------------------------------------
// CKeyboardDisabler
//-----------------------------------------------------------------------------------
// ����́A�G�N�X�|�[�g���ꂽ�N���X�̃R���X�g���N�^�[�ł��B
// �N���X��`�Ɋւ��Ă� KeyboardDisabler.h ���Q�Ƃ��Ă��������B
CKeyboardDisabler::CKeyboardDisabler()
{
	return;
}

/// <summary>�f�X�g���N�^</summary>
CKeyboardDisabler::~CKeyboardDisabler(void)
{
	End();
}

/// <summary>�L�[�{�[�h�̓��͖��������J�n���܂��B</summary>
void CKeyboardDisabler::Start()
{
	HookStart();
}

/// <summary>�L�[�{�[�h�̓��͖��������I�����܂��B</summary>
void CKeyboardDisabler::End()
{
	HookEnd();
}

/// <summary>�L�[�{�[�h�̓��͖��������s���Ă��邩�𔻒肵�܂��B</summary>
bool CKeyboardDisabler::IsDisabled()
{
	if (hHook != NULL) { return true; }
	return false;
}



//-----------------------------------------------------------------------------------
// �����g�p�֐�
//-----------------------------------------------------------------------------------

/// <summary>�L�[�{�[�h�̓��͖��������J�n���܂��B</summary>
void HookStart()
{
	if (hHook != NULL) { return; }

	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, hInstance, 0);
}

/// <summary>�L�[�{�[�h�̓��͖��������I�����܂��B</summary>
void HookEnd()
{
	if (hHook != NULL)
	{
		UnhookWindowsHookEx(hHook);
	}
	hHook = NULL;
}

/// <summary>�t�b�N�n���h��</summary>
/// <param name="nCode">�t�b�N�R�[�h</param>
/// <param name="wp">���b�Z�[�W���ʎq</param>
/// <param name="lp">���b�Z�[�W�f�[�^</param>
LRESULT CALLBACK HookProc(int nCode, WPARAM wp, LPARAM lp)
{
	if (nCode != HC_ACTION) { return CallNextHookEx(hHook, nCode, wp, lp); }

	// ���ׂẴL�[���͂𖳌���
	return TRUE;
}

