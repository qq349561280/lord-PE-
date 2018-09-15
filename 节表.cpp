#include"stdafx.h"

/*�����ڱ�
����(1) �ڱ��ھ��*/
void Pe::Section(HWND hwnd)
{
	LV_COLUMN lv;
	HWND hList;

	//��ʼ��								
	memset(&lv, 0, sizeof(LV_COLUMN));

	hList = GetDlgItem(hwnd, lbk_Section);
	//������ʾ����ѡ��								
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	PCHAR N[] = { "Name", "VirtualSize", "VirtualAddress", "SizeOfRawData", "PointerToRawData", "Characteristics" };
	for (int i = 0; i < 6; i++)
	{
		lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		lv.pszText = N[i];				//�б���				
		lv.cx = 80;					//�п�
		SendMessage(hList, LVM_INSERTCOLUMN, i, (DWORD)&lv);
	}

	//�����ڱ�
	TCHAR szTemp[20];
	LVITEM vitem;
	int OffsetTab[5] = { 0x8, 0xc, 0x10, 0x14, 0x24 };//ƫ��

	memset(&vitem, 0, sizeof(LVITEM));
	vitem.mask = LVIF_TEXT;

	for (int i = 0; i<pPE->NumberOfSections; i++)
	{

		vitem.pszText = (LPSTR)pSection[i].Name;
		vitem.iItem = i;
		vitem.iSubItem = 0;
		SendMessage(hList, LVM_INSERTITEM, 0, (DWORD)&vitem);

		for (int j = 0; j<5; j++)
		{
			sprintf((char*)szTemp, "%08X", *(DWORD*)((char *)((pSection)+i) + OffsetTab[j]));
			ListView_SetItemText(hList, i, j + 1, szTemp);
		}
	}

}

/*������
����(1) �ڱ�����
����(2) Ҫ�����ڴ�С
����(3)	Դ�ļ���С
����(4) OUT������Ļ�����
����ֵ: ��������ļ���С*/
int Pe::Section(PCHAR Name,DWORD NewSize,DWORD size,OUT PVOID* Newbuffer)
{

	//�޸Ľ�����,�ڴ��С
	pPE->NumberOfSections++;
	pOption->SizeOfImage = pOption->SizeOfImage + NewSize;

	//���ýڱ�
	DWORD S = pPE->NumberOfSections - 1;

	memcpy(pSection[S].Name, Name, 8);

	pSection[S].Misc.VirtualSize =NewSize;
	pSection[S].VirtualAddress = Align(pSection[S - 1].VirtualAddress + pSection[S - 1].SizeOfRawData, pOption->SectionAlignment);
	pSection[S].SizeOfRawData = NewSize;
	pSection[S].PointerToRawData= Align(pSection[S - 1].PointerToRawData + pSection[S - 1].SizeOfRawData, pOption->FileAlignment);
	pSection[S].Characteristics = 0XE0000040;

	//�����ڴ�
	DWORD NEWSize = size + NewSize;	
	PBYTE _NewBuffer = (PBYTE)malloc(NEWSize);
	memset(_NewBuffer, 0, NEWSize);

	//����
	memcpy(_NewBuffer, pDos, size);

	
	*Newbuffer = _NewBuffer;//����

	_NewBuffer = NULL;
	return NEWSize;
}

/*�����
����(1)Ҫ����Ĵ�С
����(2)ԭ�ļ���С
����(3)OUt ����󻺳���
����ֵ: �������ļ���С
*/
int Pe::kdSection(DWORD size, DWORD Originalfilesize, OUT PVOID* Newbuffer)
{
	//�޸��ڴ澵��
	pOption->SizeOfImage = pOption->SizeOfImage + size;

	DWORD s = pPE->NumberOfSections - 1;
	DWORD N = size+Align(pSection[s].Misc.VirtualSize, pOption->SectionAlignment);

	//�޸����һ��������
	pSection[s].SizeOfRawData = N;
	pSection[s].Misc.VirtualSize = N;

	//�����ڴ�
	PBYTE buffer= (PBYTE)malloc(Originalfilesize + size);
	memset(buffer, 0, Originalfilesize + size);

	//����
	memcpy(buffer, pDos, Originalfilesize);

	*Newbuffer = buffer;//��������

	buffer = NULL;
	return Originalfilesize + size;
}





