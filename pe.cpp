
#include "stdafx.h"


/*	
读取文件数据到缓冲区
参数(1)文件路径
返回值:文件大小	*/
DWORD Pe::OpenFile(IN LPSTR FileName)
{
	if (!FileName)
	{
		MessageBox(0,"无效文件路径",0,MB_OK);
		return FALSE;
	}
	
	DWORD fileSize;		//文件大小

	//打开文件
	FILE* pFile = fopen((LPSTR)FileName, "rb");
	if (!pFile)
	{
		MessageBox(0, "无法打开EXE文件!\n", 0, MB_OK);
		return FALSE;
	}

	//读取文件大小
	fseek(pFile, 0, SEEK_END);
	fileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	//分配缓冲区
	_FileBuffer = new BYTE [fileSize];
	if (!_FileBuffer)
	{
		MessageBox(0, "分配空间失败", 0, MB_OK);
		fclose(pFile);
		return NULL;
	}

	//将文件数据读取到缓冲区
	size_t n = fread(_FileBuffer, fileSize, 1, pFile);
	if (!n)
	{
		MessageBox(0, "读取数据失败", 0, MB_OK);
		delete[] _FileBuffer;
		fclose(pFile);
		return NULL;
	}

	if (*(PWORD)_FileBuffer!=0x5A4D)
	{
		
		delete[] _FileBuffer;
		fclose(pFile);
		return NULL;
	}

	pDos = (PIMAGE_DOS_HEADER)_FileBuffer;
	pNT = (PIMAGE_NT_HEADERS)((DWORD)_FileBuffer + pDos->e_lfanew);
	pPE = (PIMAGE_FILE_HEADER)(((DWORD)pNT) + 4);
	pOption = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pPE + sizeof(IMAGE_FILE_HEADER));
	pSection = (PIMAGE_SECTION_HEADER)(((DWORD)pOption) + pPE->SizeOfOptionalHeader);
	return fileSize;
}

/*
PE头基本信息
参数(1) main窗口句柄	*/
void Pe::PELook(HWND hwnd)
{
	if (*(PWORD)_FileBuffer != 0x5a4d)
	{
		MessageBox(0, "不是有效的MZ标记", 0, MB_OK);
		return;
	}
		

	TCHAR szBuffer[10];
	//DOS
	sprintf((PCHAR)szBuffer, "%04X\n", pDos->e_magic);
	SendDlgItemMessage(hwnd, bjk_e_magic, WM_SETTEXT, 0, (DWORD)szBuffer);

	sprintf((PCHAR)szBuffer, "%04X\n", pDos->e_lfanew);
	SendDlgItemMessage(hwnd, bjk_e_lfanew, WM_SETTEXT, 0, (DWORD)szBuffer);

	//FILE
	sprintf((PCHAR)szBuffer, "%04X\n", pPE->Machine);
	SendDlgItemMessage(hwnd, bjk_Machine, WM_SETTEXT, 0, (DWORD)szBuffer);

	sprintf((PCHAR)szBuffer, "%04X\n", pPE->NumberOfSections);
	SendDlgItemMessage(hwnd, bjk_NumberOfSections, WM_SETTEXT, 0, (DWORD)szBuffer);

	sprintf((PCHAR)szBuffer, "%08X\n", pPE->TimeDateStamp);
	SendDlgItemMessage(hwnd, bjk_TimeDateStamp, WM_SETTEXT, 0, (DWORD)szBuffer);

	sprintf((PCHAR)szBuffer, "%04X\n", pPE->SizeOfOptionalHeader);
	SendDlgItemMessage(hwnd, bjk_SizeOfOptionalHeader, WM_SETTEXT, 0, (DWORD)szBuffer);

	sprintf((PCHAR)szBuffer, "%04X\n", pPE->Characteristics);
	SendDlgItemMessage(hwnd, bjk_Charactersitics, WM_SETTEXT, 0, (DWORD)szBuffer);

	//OPTIONAL
	sprintf((PCHAR)szBuffer, "%04X\n", pOption->Magic);
	SendDlgItemMessage(hwnd, bjk_Magic, WM_SETTEXT, 0, (DWORD)szBuffer);

	sprintf((PCHAR)szBuffer, "%08X\n", pOption->AddressOfEntryPoint);
	SendDlgItemMessage(hwnd, bjk_AddressOfEntryPoint, WM_SETTEXT, 0, (DWORD)szBuffer);

	sprintf((PCHAR)szBuffer, "%08X\n", pOption->ImageBase);
	SendDlgItemMessage(hwnd, bjk_ImageBase, WM_SETTEXT, 0, (DWORD)szBuffer);

	sprintf((PCHAR)szBuffer, "%08X\n", pOption->SectionAlignment);
	SendDlgItemMessage(hwnd, bjk_SectionAlignment, WM_SETTEXT, 0, (DWORD)szBuffer);

	sprintf((PCHAR)szBuffer, "%08X\n", pOption->FileAlignment);
	SendDlgItemMessage(hwnd, bjk_FileAlignment, WM_SETTEXT, 0, (DWORD)szBuffer);

	sprintf((PCHAR)szBuffer, "%08X\n", pOption->SizeOfImage);
	SendDlgItemMessage(hwnd, bjk_SizeOfImage, WM_SETTEXT, 0, (DWORD)szBuffer);

	sprintf((PCHAR)szBuffer, "%08X\n", pOption->SizeOfHeaders);
	SendDlgItemMessage(hwnd, bjk_SizeOfHeaders, WM_SETTEXT, 0, (DWORD)szBuffer);

	sprintf((PCHAR)szBuffer, "%04X\n", pOption->NumberOfRvaAndSizes);
	SendDlgItemMessage(hwnd, bjk_NumberOfRvaASizes, WM_SETTEXT, 0, (DWORD)szBuffer);


}

/*
虚拟地址转文件地址
参数(1) 虚拟地址
返回值:文件地址(FOA)*/
int Pe::RVA_To_FOA(int rva)
{

	int offset = 0;//偏移
	int i = 0;     //记录第几个节表

	if (rva<pOption->SizeOfHeaders)
		return rva;

	PIMAGE_SECTION_HEADER ptempSection = pSection;

	for (i = 0; i <= pPE->NumberOfSections; i++, ptempSection++)
	{
		if (rva >= ptempSection->VirtualAddress && 
			rva <= (ptempSection->VirtualAddress + ptempSection->SizeOfRawData))
			break;
	}

	// 超出范围
	if (i > pPE->NumberOfSections)
	{
		return 0;
	}

	// 计算出偏移地址
	offset = rva - ptempSection->VirtualAddress;
	return ptempSection->PointerToRawData + offset;

}

/*计算对齐
参数(1) 实际大小
参数(2) 对齐值
返回值 对齐的大小*/
int Pe::Align(DWORD dwSize, DWORD dwAlign)
{
	if (dwSize % dwAlign != 0)
	{
		return (dwSize / dwAlign + 1)*dwAlign;
	}
	else
	{
		return dwSize;
	}
}

/*写出新文件
参数(1)输出文件路径
参数(2)缓冲区
参数(3)大小*/
void Pe::NewFile(PCHAR NewPath, PBYTE _NewBuffer, DWORD NEWSize)
{
	DWORD ret;
	FILE* pFile = fopen(NewPath, "wb");

	ret = fwrite(_NewBuffer, sizeof(BYTE), NEWSize, pFile);
	if (ret != NEWSize)
	{
		fclose(pFile);
		MessageBox(0, "存盘失败", 0, 0);
		return;
	}
	fclose(pFile);
	MessageBox(0, "存盘完毕", 0, 0);

	
}


/*
16个目录项信息
参数(1) 目录窗口句柄	*/ 
void Pe::Directory(HWND hwnd)
{
	TCHAR szBuffer[10];
	//遍历目录
	for (int i = 0; i < 16; i++)
	{
		sprintf((PCHAR)szBuffer, "%08X\n", RVA_To_FOA(pOption->DataDirectory[i].VirtualAddress));
		SendDlgItemMessage(hwnd, FOA_0 + i, WM_SETTEXT, 0, (DWORD)szBuffer);

		sprintf((PCHAR)szBuffer, "%08X\n", pOption->DataDirectory[i].VirtualAddress);
		SendDlgItemMessage(hwnd, RVA_0 + i, WM_SETTEXT, 0, (DWORD)szBuffer);

		sprintf((PCHAR)szBuffer, "%08X\n", pOption->DataDirectory[i].Size);
		SendDlgItemMessage(hwnd, Size_0 + i, WM_SETTEXT, 0, (DWORD)szBuffer);
	}

	//判断是否激活按钮
	for (int i = 0; i <3; i++)
	{
		if (pOption->DataDirectory[i].VirtualAddress != 0)
			EnableWindow(GetDlgItem(hwnd, an_export + i), TRUE);
	}
	if (pOption->DataDirectory[5].VirtualAddress != 0)
		EnableWindow(GetDlgItem(hwnd, an_relocation), TRUE);
}














