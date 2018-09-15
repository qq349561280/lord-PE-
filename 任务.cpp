
#include"stdafx.h"
#include <TLHELP32.H> 

/*�����б��
����(1)���񴰿ھ��*/
void process::InitListview(HWND hwnd)
{
	LV_COLUMN lv;
	HWND hListProcess;
	//��ʼ��								
	memset(&lv, 0, sizeof(LV_COLUMN));
	//��ȡ���								
	hListProcess = GetDlgItem(hwnd, lbk_process);
	//������ʾ����ѡ��								
	SendMessage(hListProcess, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	//��һ��								
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lv.pszText = TEXT("����");				//�б���				
	lv.cx = 173;							//�п�
	lv.iSubItem = 0;
			
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 0, (DWORD)&lv);
	//�ڶ���								
	lv.pszText = TEXT("PID");
	lv.cx = 75;
	lv.iSubItem = 1;
								
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 1, (DWORD)&lv);
	//������								
	lv.pszText = TEXT("�����ַ");
	lv.cx = 90;
	lv.iSubItem = 2;
	ListView_InsertColumn(hListProcess, 2, &lv);
	//������								
	lv.pszText = TEXT("�����С");
	lv.cx = 90;
	lv.iSubItem = 3;
	ListView_InsertColumn(hListProcess, 3, &lv);

	////////////////////////////////ģ��

	//��ʼ��								
	memset(&lv, 0, sizeof(LV_COLUMN));
	//��ȡ���								
	hListProcess = GetDlgItem(hwnd, lbk_modules);
	//������ʾ����ѡ��								
	SendMessage(hListProcess, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	//��һ��								
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lv.pszText = TEXT("ģ��·��");			//�б���				
	lv.cx = 285;							//�п�
	lv.iSubItem = 0;
								
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 0, (DWORD)&lv);
	//�ڶ���								
	lv.pszText = TEXT("�����С");
	lv.cx = 71;
	lv.iSubItem = 1;

	SendMessage(hListProcess, LVM_INSERTCOLUMN, 1, (DWORD)&lv);
	//������								
	lv.pszText = TEXT("�����ַ");
	lv.cx = 71;
	lv.iSubItem = 2;
	
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 1, (DWORD)&lv);


}

/*��������
����(1)���񴰿ھ��*/
void process::EnunProcess(HWND hwnd)
{
	//ˢ��
	HWND hList = GetDlgItem(hwnd, lbk_process);
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT);
	SendMessage(hList, LVM_DELETEALLITEMS, 0, 0);

	TCHAR szTemp[20];
	LV_ITEM vitem;
	PROCESSENTRY32 process;
	MODULEENTRY32 me;

	process.dwSize = sizeof(process);
	me.dwSize = sizeof(me);

	//����
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//��ʼ��							
	memset(&vitem, 0, sizeof(LV_ITEM));
	vitem.mask = LVIF_TEXT;

	if (hProcessSnap == INVALID_HANDLE_VALUE)return;

	BOOL bMore = ::Process32First(hProcessSnap, &process);
	int i = 0;
	while (bMore)
	{

		vitem.pszText = process.szExeFile;
		vitem.iItem = i;
		vitem.iSubItem = 0;
		SendMessage(hList, LVM_INSERTITEM, 0, (DWORD)&vitem);

		sprintf(szTemp, "%u", process.th32ProcessID);
		ListView_SetItemText(hList, i, 1, szTemp);
		bMore = ::Process32Next(hProcessSnap, &process);

		HANDLE hModule = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process.th32ProcessID);
		if (hModule != INVALID_HANDLE_VALUE)
		{

			if (Module32First(hModule, &me))
			{
				sprintf(szTemp, "%p", me.modBaseAddr);
				ListView_SetItemText(hList, i, 2, szTemp);
				sprintf(szTemp, "%08X", me.modBaseSize);
				ListView_SetItemText(hList, i, 3, szTemp);
			}
		}
		else
		{
			sprintf(szTemp, "%p", 00000000);
			ListView_SetItemText(hList, i, 2, szTemp);
			sprintf(szTemp, "%08X", 00000000);
			ListView_SetItemText(hList, i, 3, szTemp);
		}

		i++;
	}

	// ���snapshot����
	::CloseHandle(hProcessSnap);
}

/*����ģ��
����(1)���񴰿ھ��
����(2)����*/
void process::EnumModules(HWND hwnd, int index)
{
	//ˢ��
	HWND hList = GetDlgItem(hwnd, lbk_modules);
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT);
	SendMessage(hList, LVM_DELETEALLITEMS, 0, 0);

	TCHAR sz[128];
	LVITEM lv;
	DWORD Pid;

	memset(&lv, 0, sizeof(LVITEM));
	memset(sz, 0, 128);
	lv.mask = LVIF_TEXT;
	lv.iSubItem = 1;
	lv.pszText = sz;
	lv.cchTextMax = 128;
	SendDlgItemMessage(hwnd, lbk_process, LVM_GETITEMTEXT, index, (LPARAM)&lv);

	Pid = _ttoi(lv.pszText);
	HANDLE hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Pid);
	
	if ((HANDLE)-1 == hSnapShot)return;

	MODULEENTRY32 mk; 
	mk.dwSize = sizeof(mk);
	Module32First(hSnapShot, &mk);

	int i = 0;

	do
	{
		memset(&lv, 0, sizeof(LVITEM));
		lv.mask = LVIF_TEXT;
		lv.iItem = i;
		lv.pszText = sz;

		StringCchPrintf(sz, 128, TEXT("%s"), mk.szModule);
		SendDlgItemMessage(hwnd, lbk_modules, LVM_INSERTITEM, 0, (LPARAM)&lv);

		StringCchPrintf(sz, 128, TEXT("%p"), mk.modBaseAddr);
		lv.iSubItem = 1;
		SendDlgItemMessage(hwnd, lbk_modules, LVM_SETITEM, 0, (LPARAM)&lv);

		StringCchPrintf(sz, 128, TEXT("%08X"), mk.modBaseSize);
		lv.iSubItem = 2;
		SendDlgItemMessage(hwnd, lbk_modules, LVM_SETITEM, 0, (LPARAM)&lv);
		i++;
	} while (Module32Next(hSnapShot, &mk));

}


