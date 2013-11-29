// �ȉ��� ifdef �u���b�N�� DLL ����̃G�N�X�|�[�g��e�Ղɂ���}�N�����쐬���邽�߂� 
// ��ʓI�ȕ��@�ł��B���� DLL ���̂��ׂẴt�@�C���́A�R�}���h ���C���Œ�`���ꂽ USBSTRAGEDISABLER_EXPORTS
// �V���{�����g�p���ăR���p�C������܂��B���̃V���{���́A���� DLL ���g�p����v���W�F�N�g�ł͒�`�ł��܂���B
// �\�[�X�t�@�C�������̃t�@�C�����܂�ł��鑼�̃v���W�F�N�g�́A 
// USBSTRAGEDISABLER_API �֐��� DLL ����C���|�[�g���ꂽ�ƌ��Ȃ��̂ɑ΂��A���� DLL �́A���̃}�N���Œ�`���ꂽ
// �V���{�����G�N�X�|�[�g���ꂽ�ƌ��Ȃ��܂��B
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


// ���̃N���X�� UsbStrageDisabler.dll ����G�N�X�|�[�g����܂����B
class USBSTRAGEDISABLER_API CUsbStrageDisabler {
public:
	CUsbStrageDisabler(void);
	~CUsbStrageDisabler(void);

	// �w�肳�ꂽ�f�o�C�X�𖳌������܂��B
	bool DisableDevice(GUID target, bool* needReboot);

	// ���ݐڑ�����Ă���USB�X�g���[�W�f�o�C�X�𖳌������܂��B
	bool DisablePresentDevice(bool* needReboot);

	// ���̃C���X�^���X�������������f�o�C�X�����Z�b�g(�L����)���܂��B
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

