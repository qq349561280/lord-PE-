#include"stdafx.h"

/*
导出表信息
参数(1) 导出表窗口句柄*/
void Pe::Export(HWND hwnd)
{
	//编辑框
	TCHAR sz[30] = { 0 };
	int OffsetTab[] = { 0x10, 0x0c, 0x14, 0x18, 0x1c, 0x20, 0x24 };//偏移

	DWORD dwExportfoa = RVA_To_FOA(pOption->DataDirectory[0].VirtualAddress);
	PIMAGE_EXPORT_DIRECTORY pEXPORT = (PIMAGE_EXPORT_DIRECTORY)(((DWORD)_FileBuffer) + dwExportfoa);

	sprintf((PCHAR)sz, "%08X\n", dwExportfoa);
	SendDlgItemMessage(hwnd, bjk_exportFoa, WM_SETTEXT, 0, (DWORD)sz);
	for (int i = 0; i < 7; i++)
	{
		sprintf((char*)sz, "%08X", *(DWORD*)((char *)(pEXPORT)+OffsetTab[i]));
		SendDlgItemMessage(hwnd, bjk_Base + i, WM_SETTEXT, 0, (DWORD)sz);
	}
	sprintf((PCHAR)sz, "%s\n", ((DWORD)_FileBuffer) + RVA_To_FOA(pEXPORT->Name));
	SendDlgItemMessage(hwnd, bjk_DllName, WM_SETTEXT, 0, (DWORD)sz);

	//列表框
	LV_COLUMN lv;
	HWND hList;

	memset(&lv, 0, sizeof(LV_COLUMN));
	hList = GetDlgItem(hwnd, lbk_export);

	//设置显示整行选中								
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	PCHAR N[] = { "序号", "RVA", "函数" };
	for (int i = 0; i < 3; i++)
	{
		lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		lv.pszText = N[i];				//列标题				
		lv.cx = 88;					//列宽
		SendMessage(hList, LVM_INSERTCOLUMN, i, (DWORD)&lv);
	}

	//列表框信息
	PIMAGE_EXPORT_DIRECTORY pED = (PIMAGE_EXPORT_DIRECTORY)(((DWORD)_FileBuffer) + dwExportfoa);

	//地址表中的个数
	DWORD dwOfFuntions = pEXPORT->NumberOfFunctions;
	//名称,序号表中的个数
	DWORD dwOfNames = pEXPORT->NumberOfNames;
	//地址表地址
	PDWORD pAddrOfFuntion = (PDWORD)(RVA_To_FOA(pEXPORT->AddressOfFunctions) + _FileBuffer);
	//名称表地址
	PDWORD pAddrOfName = (PDWORD)(RVA_To_FOA(pEXPORT->AddressOfNames) + _FileBuffer);
	//序号表地址
	PWORD pAddrOfOrdial = (PWORD)(RVA_To_FOA(pEXPORT->AddressOfNameOrdinals) + _FileBuffer);
	//base值
	DWORD dwBase = pEXPORT->Base;

	LVITEMA lvItem;
	char szBuffer[128];


	int i = 0;

	for (i = 0; i < dwOfFuntions; i++)
	{
		//根据"序号表"中是否有"地址表的下标值"，
		//来判断是否是名称导出	
		for (int j = 0; j < dwOfNames; j++)
		{
			//判断地址表下标是否在序号表中
			if (i == pAddrOfOrdial[j])
			{
				//因为序号表与名称表的位置对应
				//取出名称表中的名称地址RVA转FOA=基址+偏移（文件缓冲区）
				char* pFunName = (char*)(RVA_To_FOA(pAddrOfName[j]) + _FileBuffer);

				memset(&lvItem, 0, sizeof(LVITEMA));
				lvItem.mask = LVIF_TEXT;
				lvItem.iItem = j;
				lvItem.pszText = szBuffer;

				StringCchPrintfA(szBuffer, 128, "%04X", (i + dwBase));
				SendDlgItemMessageA(hwnd, lbk_export, LVM_INSERTITEMA, 0, (LPARAM)&lvItem);

				StringCchPrintfA(szBuffer, 128, "%08X", pAddrOfFuntion[i]);
				lvItem.iSubItem = 1;
				SendDlgItemMessageA(hwnd, lbk_export, LVM_SETITEMA, 0, (LPARAM)&lvItem);

				StringCchPrintfA(szBuffer, 128, "%s", pFunName);
				lvItem.iSubItem = 2;
				SendDlgItemMessageA(hwnd, lbk_export, LVM_SETITEMA, 0, (LPARAM)&lvItem);

				break;
			}
		}
	}





}
