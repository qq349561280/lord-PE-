#include"stdafx.h"

/*
重定位基本信息
参数(1)重定位窗口句柄*/
void Pe::relocation(HWND hwnd)
{
	HWND         hList;
	LVCOLUMN     lvColumn;
	memset(&lvColumn, 0, sizeof(LVCOLUMN));

	hList = GetDlgItem(hwnd, lbk_relocation_1);
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT);

	PCHAR N1[] = { "索引", "区段", "RVA", "该重定位大小" };

	for (int i = 0; i < 4; i++)
	{
		lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
		lvColumn.cx = 100;
		lvColumn.pszText = N1[i];
		SendDlgItemMessage(hwnd, lbk_relocation_1, LVM_INSERTCOLUMN, i, (LPARAM)&lvColumn);
	}

	PCHAR N2[] = { "索引", "RVA", "FOA", "欲重定位地址" };

	hList = GetDlgItem(hwnd, lbk_relocation_2);
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT);

	for (int i = 0; i < 4; i++)
	{
		lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
		lvColumn.cx = 100;
		lvColumn.pszText = N2[i];
		SendDlgItemMessage(hwnd, lbk_relocation_2, LVM_INSERTCOLUMN, i, (LPARAM)&lvColumn);
	}

	//1信息
	char szBuffer[128];
	LVITEMA lvItem;

	DWORD dwBASEfoa = RVA_To_FOA(pOption->DataDirectory[5].VirtualAddress);
	PIMAGE_BASE_RELOCATION pBASE = (PIMAGE_BASE_RELOCATION)(((DWORD)_FileBuffer) + dwBASEfoa);

	int i = 0; int s = 0;
	while (pBASE->VirtualAddress != 0 && pBASE->SizeOfBlock != 0)//判断是否为结束标记
	{
		memset(&lvItem, 0, sizeof(LVITEMA));
		lvItem.mask = LVIF_TEXT;
		lvItem.iItem = i;
		lvItem.pszText = szBuffer;

		StringCchPrintfA(szBuffer, 128, "%d", i + 1);
		SendDlgItemMessageA(hwnd, lbk_relocation_1, LVM_INSERTITEMA, 0, (LPARAM)&lvItem);

		int yyyyy = RVA_To_FOA(pBASE->VirtualAddress);

		if (pBASE->VirtualAddress < pSection[s + 1].VirtualAddress)
		{
			StringCchPrintfA(szBuffer, 128, "%s", pSection[s].Name);
			lvItem.iSubItem = 1;
			SendDlgItemMessageA(hwnd, lbk_relocation_1, LVM_SETITEMA, 0, (LPARAM)&lvItem);
		}
		else
		{
			while (pBASE->VirtualAddress > pSection[s].VirtualAddress && pBASE->VirtualAddress <= pSection[s + 1].VirtualAddress)
			{
				s++;
			}
			StringCchPrintfA(szBuffer, 128, "%s", pSection[s].Name);
			lvItem.iSubItem = 1;
			SendDlgItemMessageA(hwnd, lbk_relocation_1, LVM_SETITEMA, 0, (LPARAM)&lvItem);

		}

		StringCchPrintfA(szBuffer, 128, "0x%08X", pBASE->VirtualAddress);
		lvItem.iSubItem = 2;
		SendDlgItemMessageA(hwnd, lbk_relocation_1, LVM_SETITEMA, 0, (LPARAM)&lvItem);

		StringCchPrintfA(szBuffer, 128, "0x%X", pBASE->SizeOfBlock);
		lvItem.iSubItem = 3;
		SendDlgItemMessageA(hwnd, lbk_relocation_1, LVM_SETITEMA, 0, (LPARAM)&lvItem);

		pBASE = (PIMAGE_BASE_RELOCATION)((DWORD)pBASE + pBASE->SizeOfBlock);
		i++;
	}

}

/*欲重定位信息
参数(1)导入表窗口句柄	参数(2)行数*/
void Pe::relocation(HWND hwnd, int index)
{
	DWORD dwRVA = 0;
	char szBuffer[128];
	LVITEMA lvItem;

	DWORD dwBASEfoa = RVA_To_FOA(pOption->DataDirectory[5].VirtualAddress);

	HWND hList = GetDlgItem(hwnd, lbk_relocation_2);
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT);
	SendMessage(hList, LVM_DELETEALLITEMS, 0, 0);

	PIMAGE_BASE_RELOCATION pBASE = (PIMAGE_BASE_RELOCATION)(((DWORD)_FileBuffer) + dwBASEfoa);


	if (index != 0)
	{
		for (int i = 0; i < index; i++)
			pBASE = (PIMAGE_BASE_RELOCATION)((DWORD)pBASE + pBASE->SizeOfBlock);
	}

	//"索引","RVA","FOA","欲重定位地址"
	for (int i = 0; i < (pBASE->SizeOfBlock - 8) / 2; i++)
	{
		PWORD pRelocData = (PWORD)((int)pBASE + 8);
		WORD Fourhigh = *(pRelocData + i) >> 12;

		if (Fourhigh == 3)
		{
			dwRVA = pRelocData[i] & 0x0FFF;
			dwRVA += pBASE->VirtualAddress;

			memset(&lvItem, 0, sizeof(LVITEMA));
			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = i;
			lvItem.pszText = szBuffer;

			StringCchPrintfA(szBuffer, 128, "%d", i + 1);
			SendDlgItemMessageA(hwnd, lbk_relocation_2, LVM_INSERTITEMA, 0, (LPARAM)&lvItem);

			StringCchPrintfA(szBuffer, 128, "%X", dwRVA);
			lvItem.iSubItem = 1;
			SendDlgItemMessageA(hwnd, lbk_relocation_2, LVM_SETITEMA, 0, (LPARAM)&lvItem);

			StringCchPrintfA(szBuffer, 128, "%s", " HIGHLOW(3)");
			lvItem.iSubItem = 2;
			SendDlgItemMessageA(hwnd, lbk_relocation_2, LVM_SETITEMA, 0, (LPARAM)&lvItem);

			StringCchPrintfA(szBuffer, 128, "%X", *(PDWORD)((DWORD)RVA_To_FOA(dwRVA) + _FileBuffer));
			lvItem.iSubItem = 3;
			SendDlgItemMessageA(hwnd, lbk_relocation_2, LVM_SETITEMA, 0, (LPARAM)&lvItem);


		}
		else
		{
			StringCchPrintfA(szBuffer, 128, "%d", i + 1);
			SendDlgItemMessageA(hwnd, lbk_relocation_2, LVM_INSERTITEMA, 0, (LPARAM)&lvItem);

			StringCchPrintfA(szBuffer, 128, "%s", " ABSOLUTE(0)");
			lvItem.iSubItem = 2;
			SendDlgItemMessageA(hwnd, lbk_relocation_2, LVM_SETITEMA, 0, (LPARAM)&lvItem);


		}

	}





}

/*模拟系统修复重定位
参数(1)新的Imagebase
参数(2)旧的Imagebase
参数(3)out修复好的缓冲区
*/
void Pe::repair(DWORD NewImagebase, DWORD OudImagebase ,OUT PVOID* newbuffer)
{
	DWORD rep = NewImagebase - OudImagebase;

	DWORD dwfoa = RVA_To_FOA(pOption->DataDirectory[5].VirtualAddress);
	PIMAGE_BASE_RELOCATION pBASE = (PIMAGE_BASE_RELOCATION)(((DWORD)_FileBuffer) + dwfoa);

	DWORD dwRVA = 0;
	PDWORD repaddress = 0;

	while (pBASE->VirtualAddress != 0 && pBASE->SizeOfBlock != 0)//判断是否为结束标记
	{
		PWORD pRelocData = (PWORD)((DWORD)pBASE + 0x8);//重定位指针+va+size=数据开始的指针

		for (int i = 0; i < (pBASE->SizeOfBlock - 8) / 2; i++)//遍历次数=（size-va+se的宽度）/2
		{
			WORD Fourhigh = *(pRelocData + i) >> 12; //取高四位
			if (Fourhigh == 3)
			{
				dwRVA = *(pRelocData + i) & 0x0FFF;//清除高4位
				dwRVA += pBASE->VirtualAddress;

				repaddress = (PDWORD)(RVA_To_FOA((DWORD)dwRVA) + _FileBuffer);
				*repaddress += rep;
			}
		}
		pBASE = (PIMAGE_BASE_RELOCATION)((DWORD)pBASE + pBASE->SizeOfBlock);
	}

	*newbuffer = _FileBuffer;

}