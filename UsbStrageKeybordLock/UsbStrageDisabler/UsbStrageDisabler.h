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
	bool DisableDevice(GUID target);

	// 現在接続されているUSBストレージデバイスを無効化します。
	bool DisablePresentDevice();

	// このインスタンスが無効化したデバイスをリセット(有効化)します。
	void ResetDisabledDevice();

	bool HasDisabledItem();

private:
	void* _guidList;
	bool IsUsbStrage(HDEVINFO devInfo, SP_DEVINFO_DATA& devData);
	bool DisableDevice(GUID* target);
	bool DisableDevice(HDEVINFO devInfo, SP_DEVINFO_DATA& devData);
	bool EnableDevice(GUID target);
	void AddDisabledList(GUID& target);
};

extern USBSTRAGEDISABLER_API int nUsbStrageDisabler;

USBSTRAGEDISABLER_API int fnUsbStrageDisabler(void);
