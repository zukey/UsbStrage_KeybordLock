
// UsbStrageKeybordLockDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once

#include "../KeyboardDisabler/KeyboardDisabler.h"
#include "../UsbStrageDisabler/UsbStrageDisabler.h"


// CUsbStrageKeybordLockDlg �_�C�A���O
class CUsbStrageKeybordLockDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CUsbStrageKeybordLockDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_USBSTRAGEKEYBORDLOCK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnKeyLock();
	afx_msg void OnBnClickedBtnKeyUnlock();
	afx_msg void OnBnClickedBtnUsbLock();
	afx_msg void OnBnClickedBtnUsbUnlock();

private:
	COLORREF	mColorWhite;
	COLORREF	mColorGreen;
	COLORREF	mColorRed;
	CBrush		mBrushWhite;
	CBrush		mBrushGreen;
	CBrush		mBrushRed;
	CKeyboardDisabler	mKeyDisabler;
	CUsbStrageDisabler	mUsbDisabler;
	bool		mUsbLocking;

	void UpdateKeybordLockState();
	void UpdateUsbLockState();
	void UpdateLockState(int targetControlId, bool locked);
	void UpdateUsbHasLockState();
	void UpdateUsbControls(bool needReboot);
	void FuncDeviceChangeMessage(WPARAM wp, LPARAM lp);
	
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
//	afx_msg void OnDevModeChange(LPTSTR lpDeviceName);
};
