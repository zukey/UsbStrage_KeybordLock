
// UsbStrageKeybordLockDlg.h : ヘッダー ファイル
//

#pragma once

#include "../KeyboardDisabler/KeyboardDisabler.h"
#include "../UsbStrageDisabler/UsbStrageDisabler.h"


// CUsbStrageKeybordLockDlg ダイアログ
class CUsbStrageKeybordLockDlg : public CDialogEx
{
// コンストラクション
public:
	CUsbStrageKeybordLockDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_USBSTRAGEKEYBORDLOCK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
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
