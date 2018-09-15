#include"stdafx.h"

/*
资源信息
参数(1)资源表窗口句柄*/
void Pe::resource(HWND hwnd)
{
	//定位资源表
	DWORD dwRESOURCE = RVA_To_FOA(pOption->DataDirectory[2].VirtualAddress);
	PIMAGE_RESOURCE_DIRECTORY p1 = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)_FileBuffer + dwRESOURCE);

	//资源数量
	DWORD Number = p1->NumberOfIdEntries + p1->NumberOfNamedEntries;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY py1 = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((DWORD)p1 + sizeof(IMAGE_RESOURCE_DIRECTORY));

	TVINSERTSTRUCTW tvs;
	WCHAR szBuffer[128];

	HWND hTree = GetDlgItem(hwnd, s_resource);


	//第一层：遍历资源类型
	for (int i = 0; i < Number; i++)
	{
		tvs.item.mask = TVIF_TEXT;
		tvs.hInsertAfter = TVI_LAST;
		tvs.item.pszText = szBuffer;
		tvs.hParent = NULL;
		switch (py1[i].Name)
		{
		case 0x01:
			StringCchPrintfW(szBuffer, 128, L"光标");
			break;

		case 0x02:
			StringCchPrintfW(szBuffer, 128, L"位图");
			break;

		case 0x03:
			StringCchPrintfW(szBuffer, 128, L"图标");
			break;

		case 0x04:
			StringCchPrintfW(szBuffer, 128, L"菜单");
			break;

		case 0x05:
			StringCchPrintfW(szBuffer, 128, L"对话框");
			break;

		case 0x06:
			StringCchPrintfW(szBuffer, 128, L"字符串");
			break;

		case 0x07:
			StringCchPrintfW(szBuffer, 128, L"字体目录");
			break;

		case 0x08:
			StringCchPrintfW(szBuffer, 128, L"字体");
			break;

		case 0x09:
			StringCchPrintfW(szBuffer, 128, L"加速器");
			break;

		case 0x0A:
			StringCchPrintfW(szBuffer, 128, L"无格式");
			break;

		case 0x0B:
			StringCchPrintfW(szBuffer, 128, L"消息表");
			break;

		case 0x0C:
			StringCchPrintfW(szBuffer, 128, L"光标组");
			break;

		case 0x0E:
			StringCchPrintfW(szBuffer, 128, L"图标组");
			break;

		case 0x10:
			StringCchPrintfW(szBuffer, 128, L"版本信息");
			break;

		case 0x18:
			StringCchPrintfW(szBuffer, 128, L"24");
			break;
		default:
			PIMAGE_RESOURCE_DIR_STRING_U pu = (PIMAGE_RESOURCE_DIR_STRING_U)((DWORD)p1 + py1[i].NameOffset);
			StringCchPrintfW(szBuffer, ((DWORD)pu->Length + 1), L"%s", pu->NameString);
			break;
		}

		HTREEITEM hNode = (HTREEITEM)SendMessageW(hTree, TVM_INSERTITEMW, 0, (LPARAM)&tvs);
		tvs.hParent = hNode;

		PIMAGE_RESOURCE_DIRECTORY p2 = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)p1 + py1[i].OffsetToDirectory);
		DWORD dwNameSize = p2->NumberOfIdEntries + p2->NumberOfNamedEntries;
		PIMAGE_RESOURCE_DIRECTORY_ENTRY py2 = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((DWORD)p2 + sizeof(IMAGE_RESOURCE_DIRECTORY));

		PIMAGE_RESOURCE_DIRECTORY p3 = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)p1 + py2->OffsetToDirectory);
		PIMAGE_RESOURCE_DIRECTORY_ENTRY py3 = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((DWORD)p3 + sizeof(IMAGE_RESOURCE_DIRECTORY));
		PIMAGE_DATA_DIRECTORY pVa = (PIMAGE_DATA_DIRECTORY)((DWORD)p1 + py3->OffsetToDirectory);

		for (DWORD k = 0; k < dwNameSize; k++)
		{
			if (py2[k].NameIsString == 1)
			{
				PIMAGE_RESOURCE_DIR_STRING_U pu = (PIMAGE_RESOURCE_DIR_STRING_U)((DWORD)p1 + py2[k].NameOffset);
				StringCchPrintfW(szBuffer, 128, L"%s  RVA:%08XH   Size:%08XH", pu->NameString, pVa[k].VirtualAddress, pVa[k].Size);
				SendMessageW(hTree, TVM_INSERTITEMW, 0, (LPARAM)&tvs);
			}
			else
			{
				StringCchPrintfW(szBuffer, 128, L"%d  RVA:%08XH   Size:%08XH", py2[k].Name, pVa[k].VirtualAddress, pVa[k].Size);
				SendMessageW(hTree, TVM_INSERTITEMW, 0, (LPARAM)&tvs);
			}


			pVa = (PIMAGE_DATA_DIRECTORY)((DWORD)pVa + sizeof(IMAGE_DATA_DIRECTORY));
		}

	}



}


