#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <string>
#include <locale>
#include <codecvt>
#include <fstream>

using namespace std;
string unicode2ansi(const std::wstring& wstr)
{
	int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], -1, NULL, 0, NULL, NULL);
	string strTo(size_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

wstring ansi2unicode(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), NULL, 0);
	wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
int main(int argc, char* argv[])
{
	locale loc(locale(), new codecvt_utf16<wchar_t>);
	string name;
	cout << "Input file name:";
	cin >> name;
	name = "Inputs//" + name;
	if (argc > 1 && (string(argv[1]) == "-a")) {
		ifstream file(name);
		basic_ofstream<wchar_t> ofs("out_Unicode.txt");
		ofs.imbue(loc);
		ofs << (wchar_t)0xfeff;
		string str;
		while (getline(file, str))
		{
			ofs << ansi2unicode(str);
		}
	}
	else {
		wifstream file(name);
		file.imbue(loc);
		file.seekg(2);
		ofstream ofs("out_ANSI.txt");
		wstring str;
		while (getline(file, str))
		{
			ofs << unicode2ansi(str);
		}
	}
	cout << "Done!" << endl;
	return 0;
}

/*
	#include <stdio.h>
#include <Windows.h>
#include <locale.h>


void get_information();
void generate_error();

int main(int argc, char* argv[])
{
	if (argv[1] != NULL) {
		if (strcmp(argv[1], "-s") == 0)
		{
			get_information();
		}
		if (strcmp(argv[1], "-e") == 0)
		{
			generate_error();
		}
	}
	else
	{
		printf("Print parameter and try again!\n");
	}
	system("pause");
	return 0;
}

void generate_error() {
	MEMORYSTATUS memory_info;
	GlobalMemoryStatus(&memory_info);

	if (LocalAlloc(LPTR, memory_info.dwTotalVirtual) == NULL) {
		LPVOID error_message;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&error_message,
			0, NULL);
		printf("System description error:\:\n%s", error_message);
	}
	return;
}
void get_information() {
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	printf("System information: \n");
	printf("Oem Id: %u\n", system_info.dwOemId);
	printf("Processor architecture: %u\n", system_info.wProcessorArchitecture);
	printf("Page size: %u\n", system_info.dwPageSize);
	printf("Minimum application address: %lx\n", system_info.lpMinimumApplicationAddress);
	printf("Maximum application address: %lx\n", system_info.lpMaximumApplicationAddress);
	printf("Active processor mask: %u\n", system_info.dwActiveProcessorMask);
	printf("Number of processors: %u\n", system_info.dwNumberOfProcessors);
	printf("Processor type: %u\n", system_info.dwProcessorType);
}

*/