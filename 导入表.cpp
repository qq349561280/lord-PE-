#include"stdafx.h"


/*
导入表基本信息
参数(1) 导入表窗口句柄*/
void Pe::import(HWND hwnd)
{
	//列表框1
	HWND         hList;
	LVCOLUMN     lvColumn;
	memset(&lvColumn, 0, sizeof(LVCOLUMN));

	hList = GetDlgItem(hwnd, lbk_Descriptor_1);
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT);

	PCHAR N1[] = { "DLL名称", "->INT_RVA", "时间戳", "Name", "->IAT_RVA" };
	for (int i = 0; i < 5; i++)
	{
		lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
		lvColumn.cx = 118;
		lvColumn.pszText = N1[i];
		SendDlgItemMessage(hwnd, lbk_Descriptor_1, LVM_INSERTCOLUMN, i, (LPARAM)&lvColumn);
	}

	//列表框2
	memset(&lvColumn, 0, sizeof(LVCOLUMN));

	hList = GetDlgItem(hwnd, lbk_Descriptor_2);
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT);

	PCHAR N2[] = { "索引号", "RVA", "THUNK值", "名称" };

	for (int i = 0; i < 4; i++)
	{
		lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
		lvColumn.cx = 143;
		lvColumn.pszText = N2[i];
		SendDlgItemMessage(hwnd, lbk_Descriptor_2, LVM_INSERTCOLUMN, i, (LPARAM)&lvColumn);
	}

	//信息1
	DWORD dwIMPORTfoa = (DWORD)RVA_To_FOA(pOption->DataDirectory[1].VirtualAddress);
	PIMAGE_IMPORT_DESCRIPTOR pID = (PIMAGE_IMPORT_DESCRIPTOR)(((DWORD)_FileBuffer) + dwIMPORTfoa);


	//HWND hList;
	LVITEMA lvItem;
	char szBuffer[128];


	hList = GetDlgItem(hwnd, lbk_Descriptor_1);
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT);

	size_t i = 0;
	while (0 != pID->Name)
	{
		memset(&lvItem, 0, sizeof(LVITEMA));
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = i;
		lvItem.pszText = szBuffer;
		//dll
		StringCchPrintfA(szBuffer, 128, "%s", (char*)((DWORD)_FileBuffer + RVA_To_FOA(pID->Name)));
		SendDlgItemMessageA(hwnd, lbk_Descriptor_1, LVM_INSERTITEMA, 0, (LPARAM)&lvItem);
		//InT
		StringCchPrintfA(szBuffer, 128, "%08X", pID->OriginalFirstThunk);
		lvItem.iSubItem = 1;
		SendDlgItemMessageA(hwnd, lbk_Descriptor_1, LVM_SETITEMA, 0, (LPARAM)&lvItem);
		//时间
		StringCchPrintfA(szBuffer, 128, "%08X", pID->TimeDateStamp);
		lvItem.iSubItem = 2;
		SendDlgItemMessageA(hwnd, lbk_Descriptor_1, LVM_SETITEMA, 0, (LPARAM)&lvItem);
		//Name
		StringCchPrintfA(szBuffer, 128, "%08X", pID->Name);
		lvItem.iSubItem = 3;
		SendDlgItemMessageA(hwnd, lbk_Descriptor_1, LVM_SETITEMA, 0, (LPARAM)&lvItem);
		//Iat
		StringCchPrintfA(szBuffer, 128, "%08X", pID->FirstThunk);
		lvItem.iSubItem = 4;
		SendDlgItemMessageA(hwnd, lbk_Descriptor_1, LVM_SETITEMA, 0, (LPARAM)&lvItem);

		pID++;
		i++;
	}



}


/*
导入表指向信息
参数(1)导入表窗口句柄	参数(2)行数*/
void Pe::import(HWND hwnd, int index)
{
	DWORD dwIMPORTfoa = (DWORD)RVA_To_FOA(pOption->DataDirectory[1].VirtualAddress);

	PIMAGE_IMPORT_BY_NAME pIBN = NULL;
	HWND hList;
	LVITEMA lvItem;
	char szBuffer[128];

	hList = GetDlgItem(hwnd, lbk_Descriptor_2);

	//清空
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT);
	SendMessage(hList, LVM_DELETEALLITEMS, 0, 0);

	PIMAGE_IMPORT_DESCRIPTOR  pID = (PIMAGE_IMPORT_DESCRIPTOR)(((DWORD)_FileBuffer) + dwIMPORTfoa) + index;
	PIMAGE_THUNK_DATA  pTD;

	DWORD d = (DWORD)pOption->DataDirectory;

	

	DWORD z = SendMessage(GetDlgItem(hwnd, fxk_IAT), BM_GETCHECK, 0, 0);

	
	if (z==0)
		pTD = (PIMAGE_THUNK_DATA)((DWORD)_FileBuffer + RVA_To_FOA(pID->OriginalFirstThunk));
	else
		pTD = (PIMAGE_THUNK_DATA)((DWORD)_FileBuffer + RVA_To_FOA(pID->FirstThunk));

	
	int i = 0;
	while (0 != pTD->u1.AddressOfData)
	{
		memset(&lvItem, 0, sizeof(LVITEMA));
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = i;
		lvItem.pszText = szBuffer;

		if (*(PDWORD)pTD & 0x80000000)
		{
			StringCchPrintfA(szBuffer, 128, "0x%04X", *(PDWORD)pTD & 0x0fff);
			SendDlgItemMessageA(hwnd, lbk_Descriptor_2, LVM_INSERTITEMA, 0, (LPARAM)&lvItem);

			if (z == 0)
			{
				StringCchPrintfA(szBuffer, 128, "0x%08X", RVA_To_FOA(pID->OriginalFirstThunk));
				lvItem.iSubItem = 1;
				SendDlgItemMessageA(hwnd, lbk_Descriptor_2, LVM_SETITEMA, 0, (LPARAM)&lvItem);
			}
			else
			{
				StringCchPrintfA(szBuffer, 128, "0x%08X", RVA_To_FOA(pID->FirstThunk));
				lvItem.iSubItem = 1;
				SendDlgItemMessageA(hwnd, lbk_Descriptor_2, LVM_SETITEMA, 0, (LPARAM)&lvItem);
			}

			StringCchPrintfA(szBuffer, 128, "0x%08X", *(PDWORD)pTD);
			lvItem.iSubItem = 2;
			SendDlgItemMessageA(hwnd, lbk_Descriptor_2, LVM_SETITEMA, 0, (LPARAM)&lvItem);

			StringCchPrintfA(szBuffer, 128, "-");
			lvItem.iSubItem = 3;
			SendDlgItemMessageA(hwnd, lbk_Descriptor_2, LVM_SETITEMA, 0, (LPARAM)&lvItem);
		}
		else
		{

			pIBN = (PIMAGE_IMPORT_BY_NAME)((DWORD)_FileBuffer + RVA_To_FOA(*(PDWORD)pTD));

			StringCchPrintfA(szBuffer, 128, "0x%04X", pIBN->Hint);
			SendDlgItemMessageA(hwnd, lbk_Descriptor_2, LVM_INSERTITEMA, 0, (LPARAM)&lvItem);

			if (z == 0)
			{
				StringCchPrintfA(szBuffer, 128, "0x%08X", RVA_To_FOA(pID->OriginalFirstThunk));//int
				lvItem.iSubItem = 1;
				SendDlgItemMessageA(hwnd, lbk_Descriptor_2, LVM_SETITEMA, 0, (LPARAM)&lvItem);
			}
			else
			{
				StringCchPrintfA(szBuffer, 128, "0x%08X", RVA_To_FOA(pID->FirstThunk));//iat
				lvItem.iSubItem = 1;
				SendDlgItemMessageA(hwnd, lbk_Descriptor_2, LVM_SETITEMA, 0, (LPARAM)&lvItem);
			}

			StringCchPrintfA(szBuffer, 128, "0x%08X", *(PDWORD)pTD);
			lvItem.iSubItem = 2;
			SendDlgItemMessageA(hwnd, lbk_Descriptor_2, LVM_SETITEMA, 0, (LPARAM)&lvItem);

			StringCchPrintfA(szBuffer, 128, "%s", pIBN->Name);
			lvItem.iSubItem = 3;
			SendDlgItemMessageA(hwnd, lbk_Descriptor_2, LVM_SETITEMA, 0, (LPARAM)&lvItem);
		}
		i++;
		pTD++;
	}


}


/*导入表注入
参数(1)文件大小
参数(2)被注入程序的完整路径
参数(2)欲注入的dll名称
参数(3)dll的导出函数名称*/
void Pe::import_InjectDll(DWORD filesize,  PCHAR DllName, PCHAR FunctionName)
{
	//需要的空间
	DWORD Newsize = sizeof(IMAGE_IMPORT_DESCRIPTOR)+
					sizeof(IMAGE_THUNK_DATA)+
					(sizeof(DllName)+1) + 
					(sizeof(FunctionName)+1 + 2) + 
					pOption->DataDirectory[1].Size;

	//新增节
	PBYTE buffer;	//新增后的缓冲区
	
	/*不知为什么新增不写0x1000 易语言程序无法运行 知道的大牛解答下感谢!*/

	DWORD size = Section(".yun", 0x1000, filesize, (PVOID*)&buffer);

	//新增后的指针
	PIMAGE_DOS_HEADER			iDos =		(PIMAGE_DOS_HEADER)			buffer;
	PIMAGE_NT_HEADERS			iNT =		(PIMAGE_NT_HEADERS)			((DWORD)buffer + iDos->e_lfanew);
	PIMAGE_FILE_HEADER			iPE =		(PIMAGE_FILE_HEADER)		(((DWORD)iNT) + 4);
	PIMAGE_OPTIONAL_HEADER32	iOption =	(PIMAGE_OPTIONAL_HEADER32)	((DWORD)iPE + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_SECTION_HEADER		iSection =	(PIMAGE_SECTION_HEADER)		(((DWORD)iOption) + iPE->SizeOfOptionalHeader);

	DWORD s = pPE->NumberOfSections - 1;//最后一个节
	DWORD IMPORTfoa = (DWORD)(RVA_To_FOA(iOption->DataDirectory[1].VirtualAddress));

	PIMAGE_IMPORT_DESCRIPTOR pIMPORT = (PIMAGE_IMPORT_DESCRIPTOR)(((DWORD)buffer) + IMPORTfoa);


	PBYTE Newsection = ((DWORD)iSection[s].PointerToRawData + buffer);
	int i = 0;//记录导入表个数

	//整个导入表结构拷贝到新的节里,追加一个导入表结构
	while (pIMPORT->FirstThunk != 0)
	{
		memcpy(Newsection, pIMPORT, sizeof(IMAGE_IMPORT_DESCRIPTOR));
		pIMPORT++;
		Newsection += sizeof(IMAGE_IMPORT_DESCRIPTOR);
		i++;
	}
	memcpy(Newsection, (Newsection - sizeof(IMAGE_IMPORT_DESCRIPTOR)), sizeof(IMAGE_IMPORT_DESCRIPTOR));

	DWORD dwDelt = iSection[s].VirtualAddress - iSection[s].PointerToRawData;

	//避免导入不需要的表
	PIMAGE_THUNK_DATA ThunkData = (PIMAGE_THUNK_DATA)(Newsection + sizeof(IMAGE_IMPORT_DESCRIPTOR)* 2);

	//导入dll名称
	PBYTE pszDllNamePosition = (PBYTE)(ThunkData + 2);		//前两个字节==序号
	memcpy(pszDllNamePosition, DllName, strlen(DllName));
	pszDllNamePosition[strlen(DllName)] = 0;


	PIMAGE_IMPORT_BY_NAME pImgImportByName = (PIMAGE_IMPORT_BY_NAME)(pszDllNamePosition + strlen(DllName) + 1);

	//初始化THUNK
	ThunkData->u1.Ordinal = dwDelt + (DWORD)pImgImportByName - (DWORD)buffer;


	pImgImportByName->Hint = 1;
	memcpy(pImgImportByName->Name, FunctionName, strlen(FunctionName));
	pImgImportByName->Name[strlen(FunctionName)] = 0;

	//设置导入表
	((PIMAGE_IMPORT_DESCRIPTOR)Newsection)->OriginalFirstThunk = dwDelt + (DWORD)ThunkData - (DWORD)buffer;
	((PIMAGE_IMPORT_DESCRIPTOR)Newsection)->FirstThunk = dwDelt + (DWORD)ThunkData - (DWORD)buffer;
	((PIMAGE_IMPORT_DESCRIPTOR)Newsection)->Name = dwDelt + (DWORD)pszDllNamePosition - (DWORD)buffer;


	//改变pOption->DataDirectory[1]的信息
	iOption->DataDirectory[1].VirtualAddress = pSection[s].VirtualAddress;
	iOption->DataDirectory[1].Size = (i + 1) * sizeof(IMAGE_IMPORT_DESCRIPTOR);


	//写出新文件
	NewFile("C:/Users/Administrator/Desktop/注入成功.exe", buffer, size);
	

}