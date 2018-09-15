#include"stdafx.h"

/*遍历节表
参数(1) 节表窗口句柄*/
void Pe::Section(HWND hwnd)
{
	LV_COLUMN lv;
	HWND hList;

	//初始化								
	memset(&lv, 0, sizeof(LV_COLUMN));

	hList = GetDlgItem(hwnd, lbk_Section);
	//设置显示整行选中								
	SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	PCHAR N[] = { "Name", "VirtualSize", "VirtualAddress", "SizeOfRawData", "PointerToRawData", "Characteristics" };
	for (int i = 0; i < 6; i++)
	{
		lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
		lv.pszText = N[i];				//列标题				
		lv.cx = 80;					//列宽
		SendMessage(hList, LVM_INSERTCOLUMN, i, (DWORD)&lv);
	}

	//遍历节表
	TCHAR szTemp[20];
	LVITEM vitem;
	int OffsetTab[5] = { 0x8, 0xc, 0x10, 0x14, 0x24 };//偏移

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

/*新增节
参数(1) 节表名称
参数(2) 要新增节大小
参数(3)	源文件大小
参数(4) OUT新增后的缓冲区
返回值: 新增后的文件大小*/
int Pe::Section(PCHAR Name,DWORD NewSize,DWORD size,OUT PVOID* Newbuffer)
{

	//修改节数量,内存大小
	pPE->NumberOfSections++;
	pOption->SizeOfImage = pOption->SizeOfImage + NewSize;

	//设置节表
	DWORD S = pPE->NumberOfSections - 1;

	memcpy(pSection[S].Name, Name, 8);

	pSection[S].Misc.VirtualSize =NewSize;
	pSection[S].VirtualAddress = Align(pSection[S - 1].VirtualAddress + pSection[S - 1].SizeOfRawData, pOption->SectionAlignment);
	pSection[S].SizeOfRawData = NewSize;
	pSection[S].PointerToRawData= Align(pSection[S - 1].PointerToRawData + pSection[S - 1].SizeOfRawData, pOption->FileAlignment);
	pSection[S].Characteristics = 0XE0000040;

	//申请内存
	DWORD NEWSize = size + NewSize;	
	PBYTE _NewBuffer = (PBYTE)malloc(NEWSize);
	memset(_NewBuffer, 0, NEWSize);

	//拷贝
	memcpy(_NewBuffer, pDos, size);

	
	*Newbuffer = _NewBuffer;//返回

	_NewBuffer = NULL;
	return NEWSize;
}

/*扩大节
参数(1)要扩大的大小
参数(2)原文件大小
参数(3)OUt 扩大后缓冲区
返回值: 扩大后的文件大小
*/
int Pe::kdSection(DWORD size, DWORD Originalfilesize, OUT PVOID* Newbuffer)
{
	//修改内存镜像
	pOption->SizeOfImage = pOption->SizeOfImage + size;

	DWORD s = pPE->NumberOfSections - 1;
	DWORD N = size+Align(pSection[s].Misc.VirtualSize, pOption->SectionAlignment);

	//修改最后一个节属性
	pSection[s].SizeOfRawData = N;
	pSection[s].Misc.VirtualSize = N;

	//申请内存
	PBYTE buffer= (PBYTE)malloc(Originalfilesize + size);
	memset(buffer, 0, Originalfilesize + size);

	//拷贝
	memcpy(buffer, pDos, Originalfilesize);

	*Newbuffer = buffer;//返回数据

	buffer = NULL;
	return Originalfilesize + size;
}





