// UsbStrageDisabler.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "UsbStrageDisabler.h"

// DDK or WDK���C���X�g�[������Ă��include�t�@�C�����邩��
#define FILE_DEVICE_MASS_STORAGE	0x0000002d
#define FILE_DEVICE_DISK			0x00000007
#define FILE_DEVICE_CD_ROM			0x00000002
#define FILE_DEVICE_DVD				0x00000033
#define FILE_DEVICE_SMARTCARD		0x00000031

#pragma comment(lib,"setupapi.lib")

using namespace std;

// ����́A�G�N�X�|�[�g���ꂽ�N���X�̃R���X�g���N�^�[�ł��B
// �N���X��`�Ɋւ��Ă� UsbStrageDisabler.h ���Q�Ƃ��Ă��������B
CUsbStrageDisabler::CUsbStrageDisabler()
{
	_guidList = new std::vector<GUID>();
}

/// <summary>�f�X�g���N�^</summary>
CUsbStrageDisabler::~CUsbStrageDisabler(void)
{
	OutputDebugString(TEXT("UsbInvalider Destructor\n"));
	ResetDisabledDevice();

	// void*�̂܂܂��ƃ��[�N����̂œK�؂Ȍ^�ɃL���X�g���Ƃ�
	vector<GUID>* guidList = (vector<GUID>*)_guidList;
	delete guidList;
	_guidList = NULL;
}

// �w�肳�ꂽUSB�X�g���[�W�f�o�C�X�𖳌������܂��B
bool CUsbStrageDisabler::DisableDevice(GUID target)
{
	return DisableDevice(&target);
}

// ���ݐڑ�����Ă���USB�X�g���[�W�f�o�C�X�𖳌������܂��B
bool CUsbStrageDisabler::DisablePresentDevice()
{
	return DisableDevice(NULL);
}

// ���̃C���X�^���X�������������f�o�C�X�����Z�b�g(�L����)���܂��B
void CUsbStrageDisabler::ResetDisabledDevice()
{
	vector<GUID>* guidList = (vector<GUID>*)_guidList;
	if (guidList->empty()) { return; }

	int count = guidList->size();
	for (int i=0 ; i<count ; i++)
	{
		GUID guid = guidList->at(i);

		// �L���ɂ���
		EnableDevice(guid);

		// ���u�[�g���K�v���𔻒�
	}
}

bool CUsbStrageDisabler::HasDisabledItem()
{
	vector<GUID>* guidList = (vector<GUID>*)_guidList;
	if (guidList->empty()) { return false; }

	return guidList->size() > 0;
}


bool CUsbStrageDisabler::IsUsbStrage(HDEVINFO devInfo, SP_DEVINFO_DATA& devData)
{
	DWORD deviceType;
	BOOL rslt = SetupDiGetDeviceRegistryProperty(devInfo, &devData, SPDRP_DEVTYPE, NULL, (BYTE*)&deviceType, sizeof(deviceType), NULL);
	if (!rslt)
	{
		return false;
	}

	switch (deviceType)
	{
	case FILE_DEVICE_MASS_STORAGE:
		return true;
	}

	return false;
}

bool CUsbStrageDisabler::DisableDevice(GUID* target)
{
	// ��
	HDEVINFO devInfo = SetupDiGetClassDevsEx(target, TEXT("USB"), NULL, DIGCF_PRESENT, NULL, NULL, NULL);
	if (devInfo == 0) { return false; }

	SP_DEVINFO_DATA devData;
	memset(&devData, 0, sizeof(devData));
	devData.cbSize = sizeof(SP_DEVINFO_DATA);

	bool doDisable = false;
	for (int i = 0; SetupDiEnumDeviceInfo(devInfo, i, &devData); i++)
	{
		// �X�g���[�W�f�o�C�X���𔻒�
		if (!IsUsbStrage(devInfo, devData)){ continue; }

		// ���Y�f�o�C�X�𖳌���
		doDisable = true;
		DisableDevice(devInfo, devData);

		// ����������GUID�����X�g�ɒǉ�
		AddDisabledList(devData.ClassGuid);

		// ���u�[�g���K�v���𔻒�
	}

	// ���\�[�X�J��
	SetupDiDestroyDeviceInfoList(devInfo);

	return doDisable;
}

bool CUsbStrageDisabler::DisableDevice(HDEVINFO devInfo, SP_DEVINFO_DATA& devData)
{
	SP_PROPCHANGE_PARAMS param;
	memset(&param, 0, sizeof(param));
	param.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
	param.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
	param.StateChange = DICS_DISABLE;
	param.Scope = DICS_FLAG_CONFIGSPECIFIC;
	param.HwProfile = 0;

	if (!SetupDiSetClassInstallParams(devInfo, &devData, &param.ClassInstallHeader, sizeof(param))
		|| !SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, devInfo, &devData))
	{
		//TRACE(_T("DisableDevice : RegistFailed Error[%d]\n"), GetLastError());
		return false;
	}
	//TRACE(_T("DisableDevice : Regist Success \n"));

	return true;
}

bool CUsbStrageDisabler::EnableDevice(GUID target)
{
	HDEVINFO devInfo = SetupDiGetClassDevsEx(&target, L"USB", NULL, DIGCF_PRESENT, NULL, NULL, NULL);
	if (devInfo == 0) { return false; }

	SP_DEVINFO_DATA devData;
	memset(&devData, 0, sizeof(devData));
	devData.cbSize = sizeof(SP_DEVINFO_DATA);

	SP_PROPCHANGE_PARAMS param;
	for (int i = 0; SetupDiEnumDeviceInfo(devInfo, i, &devData); i++)
	{
		// ��x�O���[�o���X�R�[�v�œo�^����
		memset(&param, 0, sizeof(param));
		param.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
		param.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
		param.StateChange = DICS_ENABLE;
		param.Scope = DICS_FLAG_GLOBAL;
		param.HwProfile = 0;

		SetupDiSetClassInstallParams(devInfo, &devData, &param.ClassInstallHeader, sizeof(param));
		SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, devInfo, &devData);

		// ConfigSpecific�œo�^
		param.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
		param.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
		param.StateChange = DICS_ENABLE;
		param.Scope = DICS_FLAG_CONFIGSPECIFIC;
		param.HwProfile = 0;

		if (!SetupDiSetClassInstallParams(devInfo, &devData, &param.ClassInstallHeader, sizeof(param))
			|| !SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, devInfo, &devData))
		{
			return false;
		}
	}

	return true;
}

void CUsbStrageDisabler::AddDisabledList(GUID& target)
{
	vector<GUID>* guidList = (vector<GUID>*)_guidList;

	vector<GUID>::iterator it = guidList->begin();
	while(it != guidList->end())
	{
		if (*it == target) { return; }
		it++;
	}

	guidList->push_back(target);
}

