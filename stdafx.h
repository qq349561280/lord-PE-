#include "sdk.h"
#include <windows.h>
#include<stdio.h>
#include <tchar.h>
#include "lord PE.h"

#include<strsafe.h>//StringCchPrintf

#include <commctrl.h>			
#pragma comment(lib,"comctl32.lib")//列表框


class Pe
{
public:
	PBYTE	_FileBuffer = NULL;   //文件缓冲区

public:
	PIMAGE_DOS_HEADER        pDos = NULL;
	PIMAGE_NT_HEADERS        pNT = NULL;
	PIMAGE_FILE_HEADER       pPE = NULL;
	PIMAGE_OPTIONAL_HEADER32 pOption = NULL;
	PIMAGE_SECTION_HEADER    pSection = NULL;

public:
	DWORD OpenFile(IN LPSTR FileName);
	void NewFile(PCHAR NewPath, PBYTE _NewBuffer, DWORD NEWSize);
	int RVA_To_FOA(int rva);
	int Align(DWORD dwSize, DWORD dwAlign);

public:
	void PELook(HWND hwnd);
	
	
	
public:
	
	void Section(HWND hwnd);
	int  Section(PCHAR Name,DWORD Newsize,DWORD size,OUT PVOID* Newbuffer);

	int  kdSection(DWORD size, DWORD Originalfilesize, OUT PVOID* Newbuffer);
	

public:
	void Directory(HWND hwnd);

	void Export(HWND hwnd);

	

	void resource(HWND hwnd);

	void relocation(HWND hwnd);
	void relocation(HWND hwnd, int index);
	void repair(DWORD NewImagebase,DWORD OudImagebase,OUT PVOID* newbuffer);

public:
	void import(HWND hwnd);
	void import(HWND hwnd, int index);
	void import_InjectDll(DWORD filesize,  PCHAR DllName, PCHAR FunctionName);

};

//文件操作
class FileIn :Pe
{
public:
	TCHAR  FILE_IN[256];          //输入文件路径

public:
	void FileSelect(HWND hwnd);
	void FileHaul(HWND hwnd, HDROP hDrop);
public:
	void DropFileS(HWND hwnd, WPARAM wParam);
	void An_FileName(HWND hwnd);

};

//任务
class process
{
public:

public:
	void InitListview(HWND hwnd);
	void EnunProcess(HWND hwnd);
	void EnumModules(HWND hListProcess, int index);
};
