#include "stdafx.h"


/*拖放文件
参数(1)main窗口句柄
参数(2)附加消息*/
void FileIn::FileHaul(HWND hwnd, HDROP hDrop)
{
	memset(&FILE_IN, 0, 256);
	DragQueryFile(hDrop, 0, FILE_IN, sizeof(FILE_IN));
	SetDlgItemText(hwnd, bjk_FileName, FILE_IN);
}

/*拖放文件
参数(1)main窗口句柄
参数(2)附加消息*/
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

/*选择文件
参数(1)main窗口句柄*/
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

/*选择文件
参数(1)main窗口句柄*/
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
