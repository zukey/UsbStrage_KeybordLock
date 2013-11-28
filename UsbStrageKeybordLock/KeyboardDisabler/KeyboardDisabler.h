// 以下の ifdef ブロックは DLL からのエクスポートを容易にするマクロを作成するための 
// 一般的な方法です。この DLL 内のすべてのファイルは、コマンド ラインで定義された KEYBOARDDISABLER_EXPORTS
// シンボルを使用してコンパイルされます。このシンボルは、この DLL を使用するプロジェクトでは定義できません。
// ソースファイルがこのファイルを含んでいる他のプロジェクトは、 
// KEYBOARDDISABLER_API 関数を DLL からインポートされたと見なすのに対し、この DLL は、このマクロで定義された
// シンボルをエクスポートされたと見なします。
#ifdef KEYBOARDDISABLER_EXPORTS
#define KEYBOARDDISABLER_API __declspec(dllexport)
#else
#define KEYBOARDDISABLER_API __declspec(dllimport)
#endif

// このクラスは KeyboardDisabler.dll からエクスポートされました。
// このクラスを使用するためにはメッセージループが必要です。
// コンソールアプリケーションでは動作しませんのでご注意ください。
class KEYBOARDDISABLER_API CKeyboardDisabler {
public:
	CKeyboardDisabler(void);
	~CKeyboardDisabler(void);

	void Start();
	void End();
	bool IsDisabled();
};

