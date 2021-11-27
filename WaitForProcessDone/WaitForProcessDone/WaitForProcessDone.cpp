// WaitForProcessDone.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Psapi.h>

int _tmain(int argc, _TCHAR* argv[])
{
	string cmd;
	DWORD pid;
	ofstream fout("wfpd.log", ios::app);
	if (argc < 2)
	{
		cout << "please input the command that will execute while target process finish:";
		getline(cin, cmd);
	}
	else
		cmd = argv[1];
	if (argc < 3)
	{
		cout << "please input the target process ID:";
		cin >> pid;
	}
	else
		pid = atoi(argv[2]);

	auto hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);

	if (NULL == hProcess)
	{
		fout << "can't open the process with ID:" << pid << endl;
		return 1;
	}	

	SYSTEMTIME time;
	if (fout)
	{
		FILETIME tc, te, tk, tu;
		GetProcessTimes(hProcess, &tc, &te, &tk, &tu);

		FileTimeToLocalFileTime(&tc, &tc);
		FileTimeToSystemTime(&tc, &time);
		
		fout<<"PID:"<<pid
			<<"\n start at "
			<< time.wYear << '/' << time.wMonth << '/' << time.wDay <<
			' ' << time.wHour << ':' << time.wMinute << ':' << time.wSecond
			<< endl;
	}

	WaitForSingleObject(hProcess, -1);
	CloseHandle(hProcess);
	if (fout)
	{
		GetLocalTime(&time);
		fout << "PID:" << pid << "\n done at "
			<< time.wYear << '/' << time.wMonth << '/' << time.wDay <<
			' ' << time.wHour << ':' << time.wMinute << ':' << time.wSecond
			<< endl;
	}
	system(cmd.c_str());

	//system("pause");
	return 0;
}

