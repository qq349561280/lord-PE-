#include"stdafx.h"

/*
��������Ϣ
����(1) �������ھ��*/
void Pe::Export(HWND hwnd)
{
	//�༭��
	TCHAR sz[30] = { 0 };
	int OffsetTab[] = { 0x10, 0x0c, 0x14, 0x18, 0x1c, 0x20, 0x24 };//ƫ��

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

	//�б��
	LV_COLUMN lv;
	HWND hList;

	memset(&lv, 0, sizeof(LV_COLUMN));
	hList = GetDlgItem(hwnd, lbk_export);

	//������ʾ����ѡ��								
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	PCHAR N[] = { "���", "RVA", "����" };
	for (int i = 0; i < 3; i++)
	{
		lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		lv.pszText = N[i];				//�б���				
		lv.cx = 88;					//�п�
		SendMessage(hList, LVM_INSERTCOLUMN, i, (DWORD)&lv);
	}

	//�б����Ϣ
	PIMAGE_EXPORT_DIRECTORY pED = (PIMAGE_EXPORT_DIRECTORY)(((DWORD)_FileBuffer) + dwExportfoa);

	//��ַ���еĸ���
	DWORD dwOfFuntions = pEXPORT->NumberOfFunctions;
	//����,��ű��еĸ���
	DWORD dwOfNames = pEXPORT->NumberOfNames;
	//��ַ���ַ
	PDWORD pAddrOfFuntion = (PDWORD)(RVA_To_FOA(pEXPORT->AddressOfFunctions) + _FileBuffer);
	//���Ʊ��ַ
	PDWORD pAddrOfName = (PDWORD)(RVA_To_FOA(pEXPORT->AddressOfNames) + _FileBuffer);
	//��ű��ַ
	PWORD pAddrOfOrdial = (PWORD)(RVA_To_FOA(pEXPORT->AddressOfNameOrdinals) + _FileBuffer);
	//baseֵ
	DWORD dwBase = pEXPORT->Base;

	LVITEMA lvItem;
	char szBuffer[128];


	int i = 0;

	for (i = 0; i < dwOfFuntions; i++)
	{
		//����"��ű�"���Ƿ���"��ַ����±�ֵ"��
		//���ж��Ƿ������Ƶ���	
		for (int j = 0; j < dwOfNames; j++)
		{
			//�жϵ�ַ���±��Ƿ�����ű���
			if (i == pAddrOfOrdial[j])
			{
				//��Ϊ��ű������Ʊ��λ�ö�Ӧ
				//ȡ�����Ʊ��е����Ƶ�ַRVAתFOA=��ַ+ƫ�ƣ��ļ���������
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
