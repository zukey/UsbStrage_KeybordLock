
// UsbStrageKeybordLockDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "UsbStrageKeybordLock.h"
#include "UsbStrageKeybordLockDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUsbStrageKeybordLockDlg �_�C�A���O




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


// CUsbStrageKeybordLockDlg ���b�Z�[�W �n���h���[

BOOL CUsbStrageKeybordLockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B

	// �F�쐬
	mColorWhite = RGB(255,255,255);
	mColorGreen = RGB(0,255,0);
	mColorRed = RGB(255,0,0);

	// �X�^�e�B�b�N�R���g���[���w�i�F�ݒ�p�̃u���V
	mBrushWhite.CreateSolidBrush(mColorWhite);
	mBrushGreen.CreateSolidBrush(mColorGreen);
	mBrushRed.CreateSolidBrush(mColorRed);

	// �����\���X�V
	UpdateKeybordLockState();
	UpdateUsbLockState();

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CUsbStrageKeybordLockDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CUsbStrageKeybordLockDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUsbStrageKeybordLockDlg::OnBnClickedOk()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDialogEx::OnOK();
}


void CUsbStrageKeybordLockDlg::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDialogEx::OnCancel();
}


void CUsbStrageKeybordLockDlg::OnBnClickedBtnKeyLock()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	mKeyDisabler.Start();
	UpdateKeybordLockState();
}


void CUsbStrageKeybordLockDlg::OnBnClickedBtnKeyUnlock()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	mKeyDisabler.End();
	UpdateKeybordLockState();
}


void CUsbStrageKeybordLockDlg::OnBnClickedBtnUsbLock()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	bool needReboot;
	mUsbDisabler.DisablePresentDevice(&needReboot);
	UpdateUsbControls(needReboot);
}


void CUsbStrageKeybordLockDlg::OnBnClickedBtnUsbUnlock()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	bool needReboot;
	mUsbDisabler.ResetDisabledDevice(&needReboot);
	UpdateUsbControls(needReboot);
}


HBRUSH CUsbStrageKeybordLockDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ������ DC �̑�����ύX���Ă��������B
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

	// TODO:  ����l���g�p�������Ȃ��ꍇ�͕ʂ̃u���V��Ԃ��܂��B
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
	TCHAR* text = _T("�񃍃b�N��");
	if (locked)
	{
		text = _T("���b�N��");
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

