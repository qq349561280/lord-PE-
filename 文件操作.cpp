#include "stdafx.h"


/*�Ϸ��ļ�
����(1)main���ھ��
����(2)������Ϣ*/
void FileIn::FileHaul(HWND hwnd, HDROP hDrop)
{
	memset(&FILE_IN, 0, 256);
	DragQueryFile(hDrop, 0, FILE_IN, sizeof(FILE_IN));
	SetDlgItemText(hwnd, bjk_FileName, FILE_IN);
}

/*�Ϸ��ļ�
����(1)main���ھ��
����(2)������Ϣ*/
void  FileIn::DropFileS(HWND hwnd, WPARAM wParam)
{
	HDROP hDrop = (HDROP)wParam;
	FileHaul(hwnd, hDrop);

	if (*FILE_IN != 0)
	{
		EnableWindow(GetDlgItem(hwnd, an_Section), TRUE);
		EnableWindow(GetDlgItem(hwnd, an_Directory), TRUE);
		EnableWindow(GetDlgItem(hwnd, an_CalculatorOff), TRUE);
		OpenFile(FILE_IN);
		PELook(hwnd);
	}

}

/*ѡ���ļ�
����(1)main���ھ��*/
void  FileIn::An_FileName(HWND hwnd)
{
	FileSelect(hwnd);

	if (*FILE_IN != 0)
	{
		EnableWindow(GetDlgItem(hwnd, an_Section), TRUE);
		EnableWindow(GetDlgItem(hwnd, an_Directory), TRUE);
		EnableWindow(GetDlgItem(hwnd, an_CalculatorOff), TRUE);

		OpenFile(FILE_IN);
		PELook(hwnd);
	}

}

/*ѡ���ļ�
����(1)main���ھ��*/
void FileIn::FileSelect(HWND hwnd)
{
	OPENFILENAME OpenFile;		

	
	memset(&OpenFile, 0, sizeof(OpenFile));

	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.Flags = OFN_FILEMUSTEXIST;
	OpenFile.hwndOwner = hwnd;
	OpenFile.lpstrFilter = "*.exe;*.dll";
	OpenFile.lpstrFile = FILE_IN;
	OpenFile.nMaxFile = MAX_PATH;

	GetOpenFileName(&OpenFile);



	memset(&FILE_IN, 0, sizeof(FILE_IN));
	SetDlgItemText(hwnd, bjk_FileName, FILE_IN);
}
