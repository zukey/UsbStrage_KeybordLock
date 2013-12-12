// UsbStrageLock.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "../UsbStrageDisabler/UsbStrageDisabler.h"

#pragma comment(lib,"UsbStrageDisabler.lib")

using namespace std;

void FuncDisablePresentDevice(CUsbStrageDisabler& disabler);
void FuncResetDisabledDevice(CUsbStrageDisabler& disabler);


int _tmain(int argc, _TCHAR* argv[])
{
	CUsbStrageDisabler disabler;

	int inp;
	bool loop = true;
	while(loop)
	{
		cout << "" << endl;
		cout << "select menu" << endl;
		cout << "1.DisablePresentDevice" << endl;
		cout << "2.ResetDisabledDevice" << endl;
		cout << "9.End" << endl;
		cout << ">" ;

		cin >> inp;

		switch (inp)
		{
		case 1:
			FuncDisablePresentDevice(disabler);
			break;

		case 2:
			FuncResetDisabledDevice(disabler);
			break;
		case 9:
			loop = false;
			break;
		}
	}

	disabler.ResetDisabledDevice(NULL);
	return 0;
}


void FuncDisablePresentDevice(CUsbStrageDisabler& disabler)
{
	bool needRestart;
	bool result;
	result = disabler.DisablePresentDevice(&needRestart);

	cout << "Call DisablePresentDevice" << endl;
	cout << "\tResult[" << result << "] NeedRestart[" << needRestart << "]" << endl;
}

void FuncResetDisabledDevice(CUsbStrageDisabler& disabler)
{
	bool needRestart;
	disabler.ResetDisabledDevice(&needRestart);

	cout << "Call DisablePresentDevice" << endl;
	cout << "\tNeedRestart[" << needRestart << "]" << endl;
}
