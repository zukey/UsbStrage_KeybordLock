// UsbStrageDisabler.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "UsbStrageDisabler.h"

// DDK or WDKがインストールされてればincludeファイルあるかも
#define FILE_DEVICE_MASS_STORAGE	0x0000002d
#define FILE_DEVICE_DISK			0x00000007
#define FILE_DEVICE_CD_ROM			0x00000002
#define FILE_DEVICE_DVD				0x00000033
#define FILE_DEVICE_SMARTCARD		0x00000031

#pragma comment(lib,"setupapi.lib")

using namespace std;

// これは、エクスポートされたクラスのコンストラクターです。
// クラス定義に関しては UsbStrageDisabler.h を参照してください。
CUsbStrageDisabler::CUsbStrageDisabler()
{
	_guidList = new std::vector<GUID>();
}

/// <summary>デストラクタ</summary>
CUsbStrageDisabler::~CUsbStrageDisabler(void)
{
	OutputDebugString(TEXT("UsbInvalider Destructor\n"));
	ResetDisabledDevice(NULL);

	// void*のままだとリークするので適切な型にキャストしとく
	vector<GUID>* guidList = (vector<GUID>*)_guidList;
	delete guidList;
	_guidList = NULL;
}

// 指定されたUSBストレージデバイスを無効化します。
bool CUsbStrageDisabler::DisableDevice(GUID target, bool* needReboot)
{
	return DisableDevice(&target, needReboot);
}

// 現在接続されているUSBストレージデバイスを無効化します。
bool CUsbStrageDisabler::DisablePresentDevice(bool* needReboot)
{
	return DisableDevice(NULL, needReboot);
}

// このインスタンスが無効化したデバイスをリセット(有効化)します。
void CUsbStrageDisabler::ResetDisabledDevice(bool* needReboot)
{
	if (needReboot != NULL) { *needReboot = false; }

	vector<GUID>* guidList = (vector<GUID>*)_guidList;
	if (guidList->empty()) { return; }

	int count = guidList->size();
	for (int i=0 ; i<count ; i++)
	{
		GUID guid = guidList->at(i);

		// 有効にする
		bool needrb;
		EnableDevice(guid, &needrb);

		// リブート要否フラグ更新
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

/// <summary></summary>
/// <param name="target">ターゲットとなるGUIDを指定します。現在接続されている全てのデバイスを対象とする場合はNULLを指定します。</param>
/// <returns>操作した結果、再起動が必要となる場合はtrueを返します。</returns>
bool CUsbStrageDisabler::DisableDevice(GUID* target, bool* needReboot)
{
	if (needReboot != NULL) { *needReboot = false; }

	// 列挙
	HDEVINFO devInfo = SetupDiGetClassDevsEx(target, TEXT("USB"), NULL, DIGCF_PRESENT, NULL, NULL, NULL);
	if (devInfo == 0) { return false; }

	SP_DEVINFO_DATA devData;
	memset(&devData, 0, sizeof(devData));
	devData.cbSize = sizeof(SP_DEVINFO_DATA);

	bool doDisable = false;
	for (int i = 0; SetupDiEnumDeviceInfo(devInfo, i, &devData); i++)
	{
		// ストレージデバイスかを判定
		if (!IsUsbStrage(devInfo, devData)){ continue; }

		// 当該デバイスを無効化
		doDisable = true;
		DisableDevice(devInfo, devData);

		// 無効化したGUIDをリストに追加
		AddDisabledList(devData.ClassGuid);

		if (needReboot != NULL)
		{
			// リブートが必要かを判定
			if (NeedReboot(devInfo, devData)) { *needReboot = true; }
		}
	}

	// リソース開放
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

bool CUsbStrageDisabler::EnableDevice(GUID& target, bool* needReboot)
{
	if (needReboot != NULL) { *needReboot = false; }

	HDEVINFO devInfo = SetupDiGetClassDevsEx(&target, TEXT("USB"), NULL, DIGCF_PRESENT, NULL, NULL, NULL);
	if (devInfo == 0) { return false; }

	SP_DEVINFO_DATA devData;
	memset(&devData, 0, sizeof(devData));
	devData.cbSize = sizeof(SP_DEVINFO_DATA);

	SP_PROPCHANGE_PARAMS param;
	for (int i = 0; SetupDiEnumDeviceInfo(devInfo, i, &devData); i++)
	{
		// 一度グローバルスコープで登録する
		memset(&param, 0, sizeof(param));
		param.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
		param.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
		param.StateChange = DICS_ENABLE;
		param.Scope = DICS_FLAG_GLOBAL;
		param.HwProfile = 0;

		SetupDiSetClassInstallParams(devInfo, &devData, &param.ClassInstallHeader, sizeof(param));
		SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, devInfo, &devData);

		// ConfigSpecificで登録
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

		if (needReboot != NULL)
		{
			// リブートが必要か判定
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

/// <summary>指定されたデバイスが再起動を要求するかを判定します。</summary>
/// <param name="devInfo">デバイス情報セット</param>
/// <param name="devData">デバイス情報</param>
/// <returns>再起動が必要な場合はtrueを返します。</returns>
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
