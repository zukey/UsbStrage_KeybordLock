
// UsbStrageKeybordLockDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "UsbStrageKeybordLock.h"
#include "UsbStrageKeybordLockDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUsbStrageKeybordLockDlg ダイアログ




CUsbStrageKeybordLockDlg::CUsbStrageKeybordLockDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUsbStrageKeybordLockDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUsbStrageKeybordLockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUsbStrageKeybordLockDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CUsbStrageKeybordLockDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUsbStrageKeybordLockDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_KEY_LOCK, &CUsbStrageKeybordLockDlg::OnBnClickedBtnKeyLock)
	ON_BN_CLICKED(IDC_BTN_KEY_UNLOCK, &CUsbStrageKeybordLockDlg::OnBnClickedBtnKeyUnlock)
	ON_BN_CLICKED(IDC_BTN_USB_LOCK, &CUsbStrageKeybordLockDlg::OnBnClickedBtnUsbLock)
	ON_BN_CLICKED(IDC_BTN_USB_UNLOCK, &CUsbStrageKeybordLockDlg::OnBnClickedBtnUsbUnlock)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CUsbStrageKeybordLockDlg メッセージ ハンドラー

BOOL CUsbStrageKeybordLockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	// 色作成
	mColorWhite = RGB(255,255,255);
	mColorGreen = RGB(0,255,0);
	mColorRed = RGB(255,0,0);

	// スタティックコントロール背景色設定用のブラシ
	mBrushWhite.CreateSolidBrush(mColorWhite);
	mBrushGreen.CreateSolidBrush(mColorGreen);
	mBrushRed.CreateSolidBrush(mColorRed);

	// 初期表示更新
	UpdateKeybordLockState();
	UpdateUsbLockState();

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CUsbStrageKeybordLockDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CUsbStrageKeybordLockDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUsbStrageKeybordLockDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnOK();
}


void CUsbStrageKeybordLockDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}


void CUsbStrageKeybordLockDlg::OnBnClickedBtnKeyLock()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	mKeyDisabler.Start();
	UpdateKeybordLockState();
}


void CUsbStrageKeybordLockDlg::OnBnClickedBtnKeyUnlock()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	mKeyDisabler.End();
	UpdateKeybordLockState();
}


void CUsbStrageKeybordLockDlg::OnBnClickedBtnUsbLock()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	bool needReboot;
	mUsbDisabler.DisablePresentDevice(&needReboot);
	UpdateUsbControls(needReboot);
}


void CUsbStrageKeybordLockDlg::OnBnClickedBtnUsbUnlock()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	bool needReboot;
	mUsbDisabler.ResetDisabledDevice(&needReboot);
	UpdateUsbControls(needReboot);
}


HBRUSH CUsbStrageKeybordLockDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if (nCtlColor != CTLCOLOR_STATIC) { return hbr; }

	int id =pWnd->GetDlgCtrlID();
	COLORREF color = this->mColorWhite;;
	CBrush* brush = &this->mBrushWhite;
	switch (id)
	{
	case IDC_STATIC_KEY:
		if (mKeyDisabler.IsDisabled())
		{
			color = this->mColorRed;
			brush = &this->mBrushRed;
		}
		else
		{
			color = this->mColorGreen;
			brush = &this->mBrushGreen;
		}
		break;

	case IDC_STATIC_USB:
		if (this->mUsbDisabler.HasDisabledItem())
		{
			color = this->mColorRed;
			brush = &this->mBrushRed;
		}
		else
		{
			color = this->mColorGreen;
			brush = &this->mBrushGreen;
		}
		break;

	default:
		return hbr;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	pDC->SetBkColor(color);
	return (HBRUSH)*brush;
}

void CUsbStrageKeybordLockDlg::UpdateKeybordLockState()
{
	UpdateLockState(IDC_STATIC_KEY, this->mKeyDisabler.IsDisabled());
}

void CUsbStrageKeybordLockDlg::UpdateUsbLockState()
{
	UpdateLockState(IDC_STATIC_USB, this->mUsbDisabler.HasDisabledItem());
}

void CUsbStrageKeybordLockDlg::UpdateLockState(int targetControlId, bool locked)
{
	TCHAR* text = _T("非ロック中");
	if (locked)
	{
		text = _T("ロック中");
	}
	this->SetDlgItemTextA(targetControlId ,text);
}

void CUsbStrageKeybordLockDlg::UpdateUsbControls(bool needReboot)
{
	if (needReboot)
	{
		CWnd* pLabel = this->GetDlgItem(IDC_STATIC_NEEDRB);
		pLabel->ShowWindow(SW_SHOW);
	}
	UpdateUsbLockState();
}

