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

void DebugTrace(const TCHAR* format, ...);


// ����́A�G�N�X�|�[�g���ꂽ�N���X�̃R���X�g���N�^�[�ł��B
// �N���X��`�Ɋւ��Ă� UsbStrageDisabler.h ���Q�Ƃ��Ă��������B
CUsbStrageDisabler::CUsbStrageDisabler()
{
	_guidList = new std::vector<GUID>();

	// ����������Ώۃf�o�C�X��GUID���X�g�𐶐�
	_disableTargetGUIDs = new std::vector<GUID>();
	AddDisableTargetGUID(TEXT("DiskDrive"));
	AddDisableTargetGUID(TEXT("CDROM"));
	AddDisableTargetGUID(TEXT("Bluetooth"));
	AddDisableTargetGUID(TEXT("FloppyDisk"));
	AddDisableTargetGUID(TEXT("1394"));
	AddDisableTargetGUID(TEXT("TapeDrive"));
	AddDisableTargetGUID(TEXT("Media"));
}

/// <summary>�f�X�g���N�^</summary>
CUsbStrageDisabler::~CUsbStrageDisabler(void)
{
	OutputDebugString(TEXT("UsbInvalider Destructor\n"));
	ResetDisabledDevice(NULL);

	// void*�̂܂܂��ƃ��[�N����̂œK�؂Ȍ^�ɃL���X�g���Ƃ�
	vector<GUID>* guidList = (vector<GUID>*)_guidList;
	delete guidList;
	_guidList = NULL;

	vector<GUID>* disableTargetList = (vector<GUID>*)_disableTargetGUIDs;
	delete disableTargetList;
	_disableTargetGUIDs = NULL;
}

// �w�肳�ꂽUSB�X�g���[�W�f�o�C�X�𖳌������܂��B
bool CUsbStrageDisabler::DisableDevice(GUID target, bool* needReboot)
{
	return DisableDevice(&target, needReboot);
}

// ���ݐڑ�����Ă���USB�X�g���[�W�f�o�C�X�𖳌������܂��B
bool CUsbStrageDisabler::DisablePresentDevice(bool* needReboot)
{
	return DisableDevice(NULL, needReboot);
}

// ���̃C���X�^���X�������������f�o�C�X�����Z�b�g(�L����)���܂��B
void CUsbStrageDisabler::ResetDisabledDevice(bool* needReboot)
{
	if (needReboot != NULL) { *needReboot = false; }

	vector<GUID>* guidList = (vector<GUID>*)_guidList;
	if (guidList->empty()) { return; }

	int count = guidList->size();
	for (int i=0 ; i<count ; i++)
	{
		GUID guid = guidList->at(i);

		// �L���ɂ���
		bool needrb;
		if (EnableDevice(guid, &needrb))
		{
			RemoveDisabledList(guid);
		}

		// ���u�[�g�v�ۃt���O�X�V
		if (needReboot != NULL)
		{
			if (needrb) { *needReboot = true; }
		}
	}
}

bool CUsbStrageDisabler::HasDisabledItem()
{
	vector<GUID>* guidList = (vector<GUID>*)_guidList;
	if (guidList->empty()) { return false; }

	return guidList->size() > 0;
}

bool CUsbStrageDisabler::IsUsbStrage(HDEVINFO devInfo, SP_DEVINFO_DATA* devData)
{
	DWORD deviceType = 0;
	DWORD requiredSz = 0;
	DWORD regDataType = 0;
	TCHAR buff[2048];
	TCHAR buf[512];
	memset(buff,0,sizeof(buff));
	if (SetupDiGetDeviceRegistryProperty(devInfo, devData, SPDRP_FRIENDLYNAME, &regDataType, (BYTE*)buff, sizeof(buff), &requiredSz))
	{
		OutputDebugString(buff);
		OutputDebugString(TEXT("\n"));
	}
	//BOOL rslt = SetupDiGetDeviceRegistryProperty(devInfo, devData, SPDRP_DEVTYPE, &regDataType, (BYTE*)&deviceType, sizeof(deviceType), &requiredSz);
	memset(buff,0,sizeof(buff));
	BOOL rslt = SetupDiGetDeviceRegistryProperty(devInfo, devData, SPDRP_CLASS, &regDataType, (BYTE*)buff, sizeof(buff), &requiredSz);
	if (!rslt)
	{
		_sntprintf_s(buf, sizeof(buf), TEXT("DT[%d] ReqSz[%d] Err[%d]\n"), regDataType, requiredSz, GetLastError());
		OutputDebugString(buf);
		return false;
	}
	OutputDebugString(buff);
	OutputDebugString(TEXT("\n"));

	GUID guid = ConvertClassNameToGUID(buff);
	if (MutchDisableTarget(guid))
	{
		return true;
	}

	return false;
}

/// <summary></summary>
/// <param name="target">�^�[�Q�b�g�ƂȂ�GUID���w�肵�܂��B���ݐڑ�����Ă���S�Ẵf�o�C�X��ΏۂƂ���ꍇ��NULL���w�肵�܂��B</param>
/// <returns>���삵�����ʁA�ċN�����K�v�ƂȂ�ꍇ��true��Ԃ��܂��B</returns>
bool CUsbStrageDisabler::DisableDevice(GUID* target, bool* needReboot)
{
	if (needReboot != NULL) { *needReboot = false; }

	// ��
	HDEVINFO devInfo = SetupDiGetClassDevsEx(target, TEXT("USBSTOR"), NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT, NULL, NULL, NULL);
	if (devInfo == 0) { return false; }

	SP_DEVINFO_DATA devData;
	memset(&devData, 0, sizeof(devData));
	devData.cbSize = sizeof(SP_DEVINFO_DATA);

	bool doDisable = false;
	for (int i = 0; SetupDiEnumDeviceInfo(devInfo, i, &devData); i++)
	{
		// �X�g���[�W�f�o�C�X���𔻒�
		if (!IsUsbStrage(devInfo, &devData)){ continue; }

		// ���Y�f�o�C�X�𖳌���
		if (!DisableDevice(devInfo, devData)) { continue; }

		// ����������GUID�����X�g�ɒǉ�
		doDisable = true;
		AddDisabledList(devData.ClassGuid);

		if (needReboot != NULL)
		{
			// ���u�[�g���K�v���𔻒�
			if (NeedReboot(devInfo, devData)) { *needReboot = true; }
		}
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
		|| !SetupDiChangeState(devInfo, &devData))
	{
		DebugTrace(_T("DisableDevice : RegistFailed Error[%d]\n"), GetLastError());
		return false;
	}
	DebugTrace(_T("DisableDevice : Regist Success \n"));

	return true;
}

bool CUsbStrageDisabler::EnableDevice(GUID& target, bool* needReboot)
{
	if (needReboot != NULL) { *needReboot = false; }

	HDEVINFO devInfo = SetupDiGetClassDevsEx(&target, TEXT("USBSTOR"), NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT, NULL, NULL, NULL);
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

		if (!SetupDiSetClassInstallParams(devInfo, &devData, &param.ClassInstallHeader, sizeof(param))
			|| !SetupDiChangeState(devInfo, &devData))
		{
			DebugTrace(TEXT("DICS_ENABLE Error[%d]"), GetLastError());
			return false;
		}

		// ConfigSpecific�X�R�[�v�ł��o�^����
		param.Scope = DICS_FLAG_CONFIGSPECIFIC;
		param.HwProfile = 0;

		if (!SetupDiSetClassInstallParams(devInfo, &devData, &param.ClassInstallHeader, sizeof(param))
			|| !SetupDiChangeState(devInfo, &devData))
		{
			return false;
		}

		if (needReboot != NULL)
		{
			// ���u�[�g���K�v������
			if (NeedReboot(devInfo, devData)) { *needReboot = true; }
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

void CUsbStrageDisabler::RemoveDisabledList(GUID& target)
{
	vector<GUID>* guidList = (vector<GUID>*)_guidList;

	vector<GUID>::iterator it = guidList->begin();
	while(it != guidList->end())
	{
		if (*it != target) { it++; continue; }

		// �Ώۂ��폜
		it = guidList->erase(it);
	}

}

/// <summary>�w�肳�ꂽ�f�o�C�X���ċN����v�����邩�𔻒肵�܂��B</summary>
/// <param name="devInfo">�f�o�C�X���Z�b�g</param>
/// <param name="devData">�f�o�C�X���</param>
/// <returns>�ċN�����K�v�ȏꍇ��true��Ԃ��܂��B</returns>
bool CUsbStrageDisabler::NeedReboot(HDEVINFO devInfo, SP_DEVINFO_DATA& devData)
{
    SP_DEVINSTALL_PARAMS devParams;
    devParams.cbSize = sizeof(devParams);

	if (!SetupDiGetDeviceInstallParams(devInfo, &devData, &devParams))
	{
		return false;
	}

	return ((devParams.Flags & (DI_NEEDRESTART|DI_NEEDREBOOT)) != 0);
}

void CUsbStrageDisabler::AddDisableTargetGUID(const TCHAR* className)
{
	vector<GUID>* disableTargetList = (vector<GUID>*)_disableTargetGUIDs;

	GUID	guid;
	DWORD	requireSize = 0;
	if (SetupDiClassGuidsFromName(className ,&guid ,sizeof(guid) ,&requireSize))
	{
		// �擾�o�����̂��P�����Ȃ炱���OK
		disableTargetList->push_back(guid);
		return;
	}

	int count = requireSize / sizeof(GUID);
	GUID* guids = new GUID[count];
	if (SetupDiClassGuidsFromName(className, guids, sizeof(guids), &requireSize))
	{
		for (int i=0 ; i<count ; i++)
		{
			disableTargetList->push_back(guids[i]);
		}
	}
	else
	{
		OutputDebugString(TEXT("SetupDiClassGuidsFromName Error"));
	}
	delete[] guids;
}

GUID CUsbStrageDisabler::ConvertClassNameToGUID(const TCHAR* className)
{
	GUID	guid;
	memset(&guid, 0, sizeof(guid));
	DWORD	requireSize = 0;
	if (SetupDiClassGuidsFromName(className ,&guid ,sizeof(guid) ,&requireSize))
	{
		// �擾�o�����̂��P�����Ȃ炱���OK
		return guid;
	}

	int count = requireSize / sizeof(GUID);
	GUID* guids = new GUID[count];
	if (SetupDiClassGuidsFromName(className, guids, sizeof(guids), &requireSize))
	{
		OutputDebugString(TEXT("SetupDiClassGuidsFromName GotMultiple"));
		// ������ꂿ�������Ƃ肠�����擪�Ԃ��Ƃ�
		return guids[0];
	}
	else
	{
		OutputDebugString(TEXT("SetupDiClassGuidsFromName Error"));
	}

	// ���Ȃ��������̃f�[�^�Ԃ��Ƃ�
	return guid;
}

bool CUsbStrageDisabler::MutchDisableTarget(GUID& guid)
{
	vector<GUID>* disableTargetList = (vector<GUID>*)_disableTargetGUIDs;

	vector<GUID>::iterator it = disableTargetList->begin();
	while(it != disableTargetList->end())
	{
		if (*it == guid) { return true; }
		it++;
	}
	return false;
}

void DebugTrace(const TCHAR* format, ...)
{
#ifdef _DEBUG
	va_list args;
	TCHAR	buff[2048];
	memset(buff, 0, sizeof(buff));

	va_start(args, format);

	_vstprintf_s(buff, sizeof(buff), format, args);
	OutputDebugString(buff);

	va_end(args);
#endif
}

