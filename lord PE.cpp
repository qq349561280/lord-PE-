#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////										吹水群567806008												/////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

 Pe* pe = new Pe();
 process* Task = new process();
 FileIn* File = new FileIn();


PCHAR filename;
DWORD iItem;				//行数(导入表)
DWORD filesize;				//文件大小



//新增节
DLGPROC CALLBACK An_Newsect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR szname[8]; TCHAR szsize[8];
	DWORD dwsize;

	HWND hname = GetDlgItem(hwnd, bjk_sect_name);
	HWND hsize = GetDlgItem(hwnd, bjk_sect_size);

	switch (uMsg)
	{
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;

	case  WM_INITDIALOG:
		SetDlgItemText(hwnd, bjk_sect_name, TEXT(".yun"));
		SetDlgItemText(hwnd, bjk_sect_size, TEXT("0x1000"));
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case an_Newsect:
			GetWindowText(hname, szname, 8);
			GetWindowText(hsize, szsize, 8);

			sscanf(szsize, "%x", &dwsize);

			PBYTE buffer=NULL;
			//新增节
			DWORD size = pe->Section(szname, dwsize, filesize, (PVOID*)&buffer);
			//存盘
			pe->NewFile("C:/Users/Administrator/Desktop/新增成功.exe", buffer, size);


			break;
		}



	}
	return 0;
}
//节表
DLGPROC CALLBACK An_Section(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hkdb = GetDlgItem(hwnd, bjk_kdj);
	TCHAR tchar[8] = { 0 };
	DWORD dw;
	
	switch (uMsg)
	{
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;

	case  WM_INITDIALOG:
		pe->Section(hwnd);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case an_Newsect:
			DialogBox(NULL, MAKEINTRESOURCE(NewSet), NULL, (DLGPROC)An_Newsect);
			break;

		case an_kdj:
			GetWindowText(hkdb,tchar,8);

			sscanf(tchar, "%X", &dw);

			PBYTE buffer = NULL;
			DWORD size = pe->kdSection(dw, filesize, (PVOID*)&buffer);

			//存盘
			pe->NewFile("C:/Users/Administrator/Desktop/扩大成功.exe", buffer, size);

			break;



		}



	}
	return FALSE;
}

//计算
DLGPROC CALLBACK An_Calculate(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	DWORD dwOffset;
	TCHAR szBuffer[10];
	HWND hEdit = GetDlgItem(hwnd, bjk_RVA);

	switch (uMsg)
	{
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;

	case WM_INITDIALOG:
		SetDlgItemText(hwnd, bjk_RVA, TEXT("0x"));
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case an_Execute:
			GetWindowText(hEdit, szBuffer, 10);
			sscanf(szBuffer, "%x", &dwOffset);

			StringCchPrintf(szBuffer, 128, TEXT("0x%X"), pe->RVA_To_FOA(dwOffset));
			SetDlgItemText(hwnd, bjk_FOA, szBuffer);

			break;


		}
		break;



	}
	return 0;
}
//任务
DLGPROC CALLBACK An_Task(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch (uMsg)
	{
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;

	case WM_INITDIALOG:
		Task->InitListview(hwnd);
		Task->EnunProcess(hwnd);

		break;

	case WM_NOTIFY:

				LPNMLISTVIEW lpnmlv = (LPNMLISTVIEW)lParam;
				NMHDR* pNMHDR = (NMHDR*)lParam;

				if (wParam == lbk_process && pNMHDR->code == NM_CLICK)
					Task->EnumModules(hwnd, lpnmlv->iItem);

				if (wParam == lbk_process && pNMHDR->code == NM_RCLICK)
					Task->EnunProcess(hwnd);

			
		


	}
	return 0;
}
//关于
DLGPROC CALLBACK An_About(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch (uMsg)
	{
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}

	return 0;
}


//修复重定位
DLGPROC CALLBACK An_rep(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	TCHAR NEW[128] = { 0 }; TCHAR OUD[128] = { 0 };
	DWORD dwnew = 0; DWORD dwoud = 0;

	HWND hwnd_new = GetDlgItem(hwnd,bjk_NewImagebuff);
	HWND hwnd_Oud = GetDlgItem(hwnd, bjk_OudImagebuff);

	switch (uMsg)
	{
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;

	case WM_INITDIALOG:
		SetDlgItemText(hwnd, bjk_NewImagebuff, TEXT("0x00600000"));
		SetDlgItemText(hwnd, bjk_OudImagebuff, TEXT("0x00500000"));
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case an_xf:
			GetWindowText(hwnd_new, NEW, 128);
			GetWindowText(hwnd_Oud, OUD, 128);

			sscanf(NEW, "%x", &dwnew);
			sscanf(OUD, "%x", &dwoud);

			PBYTE buff;
			//pe->OpenFile(filename);

			pe->repair(dwnew, dwoud,(PVOID*)&buff);

			pe->NewFile(filename, buff, filesize);

			break;
		}


	}

	return 0;
}
//重定位
DLGPROC CALLBACK An_relocation(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;

	case WM_INITDIALOG:
		pe->relocation(hwnd);
		break;

	case WM_NOTIFY:
		if (LOWORD(wParam) == lbk_relocation_1)
		{
			switch (((LPNMHDR)lParam)->code)
			{
			case LVN_ITEMCHANGED:
				LPNMLISTVIEW lpnmlv = (LPNMLISTVIEW)lParam;
				if (lpnmlv->uNewState)
					pe->relocation(hwnd, lpnmlv->iItem);
				break;
			}
		}break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case an_rep:
			DialogBox(NULL, MAKEINTRESOURCE(rep_ck), NULL, (DLGPROC)An_rep);
			break;
		}


	}
	return 0;
}


//资源表
DLGPROC CALLBACK An_resource(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;

	case WM_INITDIALOG:
		pe->resource(hwnd);
		
		break;

	}
	return 0;
}


//导入表注入
DLGPROC CALLBACK An_indll(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR szdllname[128] = { 0 }; TCHAR szdchs[128] = {0};

	HWND dllname = GetDlgItem(hwnd, bjk_DllName);
	HWND dchs = GetDlgItem(hwnd, bjk_dchs);

	switch (uMsg)
	{
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	case WM_INITDIALOG:
		//SetDlgItemText(hwnd, bjk_DllName, TEXT("hook.dll"));
		//SetDlgItemText(hwnd, bjk_dchs, TEXT("Init"));
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case an_indll:
			GetWindowText(dllname, szdllname, 128);
			GetWindowText(dchs, szdchs, 128);

			pe->import_InjectDll(filesize,  szdllname, szdchs);

			break;
		}

	}


	return 0;
}
//导入表
DLGPROC CALLBACK An_Import(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;

	case WM_INITDIALOG:
		pe->import(hwnd);
		break;

	case WM_NOTIFY:
		if (LOWORD(wParam) == lbk_Descriptor_1)
		{
			switch (((LPNMHDR)lParam)->code)
			{
			case LVN_ITEMCHANGED:
				LPNMLISTVIEW lpnmlv = (LPNMLISTVIEW)lParam;

				if (lpnmlv->uNewState)
					iItem = lpnmlv->iItem;
					pe->import(hwnd, iItem);

				break;
			}

		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case fxk_IAT:
			pe->import(hwnd, iItem);
			break;
		case an_Import_IN:
			DialogBox(NULL, MAKEINTRESOURCE(InjectDll_ck), NULL, (DLGPROC)An_indll);
			break;
		}break;


	}

	return 0;
}


//导出表
DLGPROC CALLBACK An_export(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;

	case WM_INITDIALOG:
		pe->Export(hwnd);
		break;

	}

	return 0;
}

//目录
DLGPROC CALLBACK An_Directory(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	case  WM_INITDIALOG:
		pe->Directory(hwnd);
		
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case an_export:
			DialogBox(NULL, MAKEINTRESOURCE(export_ck), NULL, (DLGPROC)An_export);
			break;

		case an_import:
			DialogBox(NULL, MAKEINTRESOURCE(Import_ck_0), NULL, (DLGPROC)An_Import);
			break;

		case an_resource:
			DialogBox(NULL, MAKEINTRESOURCE(resource_ck), NULL, (DLGPROC)An_resource);
			break;

		case an_relocation:
			DialogBox(NULL, MAKEINTRESOURCE(relocation_ck), NULL, (DLGPROC)An_relocation);
			break;

		

		}break;

		

	}
	return 0;
}




//主窗口
DLGPROC CALLBACK Main(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{

	case WM_DROPFILES:/*拖动文件*/
		File->DropFileS(hwnd, wParam);

		filename = File->FILE_IN;
		filesize=pe->OpenFile(File->FILE_IN);
		break;
		
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case  WM_INITDIALOG:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i < 1000; i++)
			SetPixel(hdc, i, 100, RGB(255, 0, 0));
		SetDlgItemText(hwnd, bjk_FileName, "将文件拖放到窗口 或 点击选择文件");
		
	}break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case an_FileName:/*选择文件*/
			File->An_FileName(hwnd);

			if (*File->FILE_IN != 0)
			{
				filename = File->FILE_IN;
				filesize = pe->OpenFile(File->FILE_IN);
			}
			break;

		case an_Section:/*节表*/
			DialogBox(NULL, MAKEINTRESOURCE(Section_ck), NULL, (DLGPROC)An_Section);
			break;

		case an_About: /*关于*/
			DialogBox(NULL, MAKEINTRESOURCE(About_ck_0), NULL, (DLGPROC)An_About);
			break;

		case an_CalculatorOff:/*计算*/
			DialogBox(NULL, MAKEINTRESOURCE(Calculate_ck), NULL, (DLGPROC)An_Calculate);
			break;

		case an_Directory:/*目录*/
			DialogBox(NULL, MAKEINTRESOURCE(Directory_ck), NULL,(DLGPROC)An_Directory);
			break;

		case an_Task:/*任务*/
			DialogBox(NULL, MAKEINTRESOURCE(Task_ck), NULL, (DLGPROC)An_Task);
			break;


		}break;


	}
	return FALSE;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

	DialogBox(hInstance, MAKEINTRESOURCE(Main_ck_0), NULL,(DLGPROC) Main);


	return FALSE;
}


