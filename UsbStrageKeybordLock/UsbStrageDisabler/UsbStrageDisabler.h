// 以下の ifdef ブロックは DLL からのエクスポートを容易にするマクロを作成するための 
// 一般的な方法です。この DLL 内のすべてのファイルは、コマンド ラインで定義された USBSTRAGEDISABLER_EXPORTS
// シンボルを使用してコンパイルされます。このシンボルは、この DLL を使用するプロジェクトでは定義できません。
// ソースファイルがこのファイルを含んでいる他のプロジェクトは、 
// USBSTRAGEDISABLER_API 関数を DLL からインポートされたと見なすのに対し、この DLL は、このマクロで定義された
// シンボルをエクスポートされたと見なします。
#ifdef USBSTRAGEDISABLER_EXPORTS
#define USBSTRAGEDISABLER_API __declspec(dllexport)
#else
#define USBSTRAGEDISABLER_API __declspec(dllimport)
#endif

#include <tchar.h>
extern "C"
{
#include <SetupAPI.h>
}


// このクラスは UsbStrageDisabler.dll からエクスポートされました。
class USBSTRAGEDISABLER_API CUsbStrageDisabler {
public:
	CUsbStrageDisabler(void);
	~CUsbStrageDisabler(void);

	// 指定されたデバイスを無効化します。
	bool DisableDevice(GUID target, bool* needReboot);

	// 現在接続されているUSBストレージデバイスを無効化します。
	bool DisablePresentDevice(bool* needReboot);

	// このインスタンスが無効化したデバイスをリセット(有効化)します。
	void ResetDisabledDevice(bool* needReboot);

	bool HasDisabledItem();

private:
	void* _guidList;
	void* _disableTargetGUIDs;
	bool IsUsbStrage(HDEVINFO devInfo, SP_DEVINFO_DATA* devData);
	bool DisableDevice(GUID* target, bool* needReboot);
	bool DisableDevice(HDEVINFO devInfo, SP_DEVINFO_DATA& devData);
	bool EnableDevice(GUID& target, bool* needReboot);
	void AddDisabledList(GUID& target);
	bool NeedReboot(HDEVINFO devInfo, SP_DEVINFO_DATA& devData);
	void AddDisableTargetGUID(const TCHAR* className);
	GUID ConvertClassNameToGUID(const TCHAR* className);
	bool MutchDisableTarget(GUID& guid);
};

