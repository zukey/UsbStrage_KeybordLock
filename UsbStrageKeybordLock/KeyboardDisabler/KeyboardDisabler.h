// �ȉ��� ifdef �u���b�N�� DLL ����̃G�N�X�|�[�g��e�Ղɂ���}�N�����쐬���邽�߂� 
// ��ʓI�ȕ��@�ł��B���� DLL ���̂��ׂẴt�@�C���́A�R�}���h ���C���Œ�`���ꂽ KEYBOARDDISABLER_EXPORTS
// �V���{�����g�p���ăR���p�C������܂��B���̃V���{���́A���� DLL ���g�p����v���W�F�N�g�ł͒�`�ł��܂���B
// �\�[�X�t�@�C�������̃t�@�C�����܂�ł��鑼�̃v���W�F�N�g�́A 
// KEYBOARDDISABLER_API �֐��� DLL ����C���|�[�g���ꂽ�ƌ��Ȃ��̂ɑ΂��A���� DLL �́A���̃}�N���Œ�`���ꂽ
// �V���{�����G�N�X�|�[�g���ꂽ�ƌ��Ȃ��܂��B
#ifdef KEYBOARDDISABLER_EXPORTS
#define KEYBOARDDISABLER_API __declspec(dllexport)
#else
#define KEYBOARDDISABLER_API __declspec(dllimport)
#endif

// ���̃N���X�� KeyboardDisabler.dll ����G�N�X�|�[�g����܂����B
// ���̃N���X���g�p���邽�߂ɂ̓��b�Z�[�W���[�v���K�v�ł��B
// �R���\�[���A�v���P�[�V�����ł͓��삵�܂���̂ł����ӂ��������B
class KEYBOARDDISABLER_API CKeyboardDisabler {
public:
	CKeyboardDisabler(void);
	~CKeyboardDisabler(void);

	void Start();
	void End();
	bool IsDisabled();
};

