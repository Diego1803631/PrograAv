#include<iostream>
#include<fstream>
#include<Windows.h>
#include <commdlg.h>
#include"resource.h"
using namespace std;
HWND ghDlg = 0;
HINSTANCE ghInst;
OPENFILENAME ofn;
struct cita {
	char nombre[100];
	char apellido[100];
	char fecha[100];
	char hora[100];
	char telefono[100];
	char especie[100];
	char mascota[100];
	char motivo[100];
	char costo[100];
	bool urgente;
	char doc[100] = "Miguel Ádrian Franco López";
	char cedula[100] = "12345678";
	char info[100] = "Médico Veterinario Zootecnista";
};
cita datos[100];  
char szFileName[MAX_PATH] = "1.bmp";
char _arch_esp[] = "spc.txt";
int gShow = 0;
void openfilename();
void LlenaEspecies(HWND objeto, UINT mensa, char *file);
BOOL CALLBACK Principal(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK NuevaCita(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK DatosD(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK Agenda(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK EditarC(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK EditarD(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmd, int show)
{
	ghInst = hInst;
	gShow = show;
	ghDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1),
		0, Principal);
	ShowWindow(ghDlg, show);
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (GetMessage(&msg, 0, 0, 0))
	{
		if (ghDlg == 0 || !IsDialogMessage(ghDlg, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
BOOL CALLBACK Principal(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	HDC hdcStatic = (HDC)wParam;
	static HBRUSH hbrBkgnd = CreateSolidBrush(RGB(169, 203, 209));
	switch (Mensaje)
	{
	case WM_CTLCOLORDLG:
		return (INT_PTR)hbrBkgnd;
	case WM_CTLCOLORSTATIC:
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(169, 203, 209));
		return (INT_PTR)hbrBkgnd;

	case WM_INITDIALOG:
	{
		return true;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		
		case ID_NUEVA_CITA:
		{
			DialogBox(ghInst, MAKEINTRESOURCE(IDD_DIALOG2), Dlg, NuevaCita);
			return true;
		}
		case ID_INFORMAD:
		{
			DialogBox(ghInst, MAKEINTRESOURCE(IDD_DIALOG3), Dlg, DatosD);
			return true;
		}
		case ID_AGENDA_CITASAGREGADAS:
		{
			DialogBox(ghInst, MAKEINTRESOURCE(IDD_DIALOG4), Dlg, Agenda);
			return true;
		}
		case ID_ARCHIVO_SALIR:
		{
			PostQuitMessage(0);
			return true;
		}
		}
		return true;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return true;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return true;
	}
	}
	return false;
}
BOOL CALLBACK NuevaCita(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	HDC hdcStatic = (HDC)wParam;
	static HBRUSH hbrBkgnd = CreateSolidBrush(RGB(169, 203, 209));
	static HWND hCboSpc = 0;
	hCboSpc = GetDlgItem(Dlg, IDC_COMBO1);
	switch (Mensaje)
	{
	case WM_CTLCOLORDLG:
		return (INT_PTR)hbrBkgnd;
	case WM_CTLCOLORSTATIC:
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(169, 203, 209));
		return (INT_PTR)hbrBkgnd;
	case WM_INITDIALOG:
	{
		
		LlenaEspecies(hCboSpc, CB_ADDSTRING, _arch_esp);
		
		return true;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			SendDlgItemMessage(Dlg, IDC_NOMBRE, WM_GETTEXT, 100, (LPARAM)datos->nombre);
			SendDlgItemMessage(Dlg, IDC_APELLIDO, WM_GETTEXT, 100, (LPARAM)datos->apellido);
			SendDlgItemMessage(Dlg, IDC_TELEFONO, WM_GETTEXT, 100, (LPARAM)datos->telefono);
			SendDlgItemMessage(Dlg, IDC_MASCOTA, WM_GETTEXT, 100, (LPARAM)datos->mascota);
			SendDlgItemMessage(Dlg, IDC_MOTIVO, WM_GETTEXT, 100, (LPARAM)datos->motivo);
			SendDlgItemMessage(Dlg, IDC_COSTO, WM_GETTEXT, 100, (LPARAM)datos->costo);
			int index = SendMessage(hCboSpc, CB_GETCURSEL, 0, 0);			SendMessage(hCboSpc, CB_GETLBTEXT, (WPARAM)index, (LPARAM)datos->especie);
			/*char fechatemp[100] = "";
			SendDlgItemMessage(Dlg, IDC_FECHA, WM_GETTEXT, (WPARAM)100, (LPARAM)fechatemp);
			strcpy_s(datos->fecha, ConvierteFecha(fechatemp));*/
			EndDialog(Dlg, 0);
			return true;
		}
		case IDCANCEL:
		{
			int opc = MessageBox(0, "¿Esta seguro que desea salir?", "Aviso", MB_ICONINFORMATION + MB_OKCANCEL);
			if (opc == IDOK) {
				EndDialog(Dlg, 0);
			}
			return true;
		}

		}
		return true;
	}
	case WM_CLOSE:
	{
		EndDialog(Dlg, 0);
		return true;
	}
	case WM_DESTROY:
	{
		EndDialog(Dlg, 0);
		return true;
	}
	}
	return false;
}
BOOL CALLBACK DatosD(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	HDC hdcStatic = (HDC)wParam;
	static HBRUSH hbrBkgnd = CreateSolidBrush(RGB(169, 203, 209));
	static HBITMAP bmp1, bmp2;
	bmp1 = (HBITMAP)SendDlgItemMessage(Dlg, IDC_IMAGE,
		STM_GETIMAGE, IMAGE_BITMAP, 0);
	bmp2 = (HBITMAP)LoadImage(NULL, szFileName,
		IMAGE_BITMAP, 200,200, LR_LOADFROMFILE);
	
	switch (Mensaje)
	{
	case WM_CTLCOLORDLG:
		return (INT_PTR)hbrBkgnd;
	case WM_CTLCOLORSTATIC:
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(169, 203, 209));
		return (INT_PTR)hbrBkgnd;
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(Dlg, IDC_IMAGE, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);
		SendDlgItemMessage(Dlg, IDC_DOC, WM_SETTEXT, 100, (LPARAM)datos->doc);
		SendDlgItemMessage(Dlg, IDC_CED, WM_SETTEXT, 100, (LPARAM)datos->cedula);
		SendDlgItemMessage(Dlg, IDC_INF, WM_SETTEXT, 100, (LPARAM)datos->info);
		return true;
	}
	case WM_COMMAND:
	{
		
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			EndDialog(Dlg, 0);
			return true;
		}
		case ID_ARCHIVO_EDITD:
		{
			DialogBox(ghInst, MAKEINTRESOURCE(IDD_DIALOG6), Dlg, EditarD);
			SendDlgItemMessage(Dlg, IDC_DOC, WM_SETTEXT, 100, (LPARAM)datos->doc);
			SendDlgItemMessage(Dlg, IDC_CED, WM_SETTEXT, 100, (LPARAM)datos->cedula);
			SendDlgItemMessage(Dlg, IDC_INF, WM_SETTEXT, 100, (LPARAM)datos->info);
			bmp1 = (HBITMAP)SendDlgItemMessage(Dlg, IDC_IMAGE,
				STM_GETIMAGE, IMAGE_BITMAP, 0);
			bmp2 = (HBITMAP)LoadImage(NULL, szFileName,
				IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE);
			return true;
		}
		case IDCANCEL:
		{
			EndDialog(Dlg, 0);
			return true;
		}

		}
		return true;
	}
	case WM_CLOSE:
	{
		EndDialog(Dlg, 0);
		return true;
	}
	case WM_DESTROY:
	{
		EndDialog(Dlg, 0);
		return true;
	}
	}
	return false;
}
BOOL CALLBACK Agenda(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	HDC hdcStatic = (HDC)wParam;
	static HBRUSH hbrBkgnd = CreateSolidBrush(RGB(169, 203, 209));
	switch (Mensaje)
	{
	case WM_CTLCOLORDLG:
		return (INT_PTR)hbrBkgnd;
	case WM_CTLCOLORSTATIC:
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(169, 203, 209));
		return (INT_PTR)hbrBkgnd;
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(Dlg, IDC_NOMBRE, WM_SETTEXT, 100, (LPARAM)datos->nombre);
		SendDlgItemMessage(Dlg, IDC_APELLIDO, WM_SETTEXT, 100, (LPARAM)datos->apellido);
		SendDlgItemMessage(Dlg, IDC_TELEFONO, WM_SETTEXT, 100, (LPARAM)datos->telefono);
		SendDlgItemMessage(Dlg, IDC_ESPECIE, WM_SETTEXT, 100, (LPARAM)datos->especie);
		SendDlgItemMessage(Dlg, IDC_MASCOTA, WM_SETTEXT, 100, (LPARAM)datos->mascota);
		SendDlgItemMessage(Dlg, IDC_MOTIVO, WM_SETTEXT, 100, (LPARAM)datos->motivo);
		SendDlgItemMessage(Dlg, IDC_COSTO, WM_SETTEXT, 100, (LPARAM)datos->costo);

		return true;
	}
	case WM_COMMAND:
	{

		switch (LOWORD(wParam))
		{
		case ID_ARCHIVO_ELIMINAR:
		{
			strcpy_s(datos->nombre, " ");
			strcpy_s(datos->apellido, " ");
			strcpy_s(datos->telefono, " ");
			strcpy_s(datos->especie, " ");
			strcpy_s(datos->mascota, " ");
			strcpy_s(datos->motivo, " ");
			strcpy_s(datos->costo, " ");
			SendDlgItemMessage(Dlg, IDC_NOMBRE, WM_SETTEXT, 100, (LPARAM)datos->nombre);
			SendDlgItemMessage(Dlg, IDC_APELLIDO, WM_SETTEXT, 100, (LPARAM)datos->apellido);
			SendDlgItemMessage(Dlg, IDC_TELEFONO, WM_SETTEXT, 100, (LPARAM)datos->telefono);
			SendDlgItemMessage(Dlg, IDC_ESPECIE, WM_SETTEXT, 100, (LPARAM)datos->especie);
			SendDlgItemMessage(Dlg, IDC_MASCOTA, WM_SETTEXT, 100, (LPARAM)datos->mascota);
			SendDlgItemMessage(Dlg, IDC_MOTIVO, WM_SETTEXT, 100, (LPARAM)datos->motivo);
			SendDlgItemMessage(Dlg, IDC_COSTO, WM_SETTEXT, 100, (LPARAM)datos->costo);
			return true;
		}
		case ID_ARCHIVO_EDITAR:
		{
			DialogBox(ghInst, MAKEINTRESOURCE(IDD_DIALOG5), Dlg, EditarC);
			SendDlgItemMessage(Dlg, IDC_NOMBRE, WM_SETTEXT, 100, (LPARAM)datos->nombre);
			SendDlgItemMessage(Dlg, IDC_APELLIDO, WM_SETTEXT, 100, (LPARAM)datos->apellido);
			SendDlgItemMessage(Dlg, IDC_TELEFONO, WM_SETTEXT, 100, (LPARAM)datos->telefono);
			SendDlgItemMessage(Dlg, IDC_MASCOTA, WM_SETTEXT, 100, (LPARAM)datos->mascota);
			SendDlgItemMessage(Dlg, IDC_MOTIVO, WM_SETTEXT, 100, (LPARAM)datos->motivo);
			SendDlgItemMessage(Dlg, IDC_COSTO, WM_SETTEXT, 100, (LPARAM)datos->costo);
			return true;
		}
		case IDOK:
		{
			EndDialog(Dlg, 0);
			return true;
		}
		case IDCANCEL:
		{
			EndDialog(Dlg, 0);
			return true;
		}

		}
		return true;
	}
	case WM_CLOSE:
	{
		EndDialog(Dlg, 0);
		return true;
	}
	case WM_DESTROY:
	{
		EndDialog(Dlg, 0);
		return true;
	}
	}
	return false;
}
BOOL CALLBACK EditarC(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	static HWND hCboSpc = 0;
	HDC hdcStatic = (HDC)wParam;
	static HBRUSH hbrBkgnd = CreateSolidBrush(RGB(169, 203, 209));
	switch (Mensaje)
	{
	case WM_CTLCOLORDLG:
		return (INT_PTR)hbrBkgnd;
	case WM_CTLCOLORSTATIC:
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(169, 203, 209));
		return (INT_PTR)hbrBkgnd;
	case WM_INITDIALOG:
	{
		hCboSpc = GetDlgItem(Dlg, IDC_ESPECIE);
		return true;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			SendDlgItemMessage(Dlg, IDC_NOMBRE, WM_GETTEXT, 100, (LPARAM)datos->nombre);
			SendDlgItemMessage(Dlg, IDC_APELLIDO, WM_GETTEXT, 100, (LPARAM)datos->apellido);
			SendDlgItemMessage(Dlg, IDC_TELEFONO, WM_GETTEXT, 100, (LPARAM)datos->telefono);
			SendDlgItemMessage(Dlg, IDC_MASCOTA, WM_GETTEXT, 100, (LPARAM)datos->mascota);
			SendDlgItemMessage(Dlg, IDC_MOTIVO, WM_GETTEXT, 100, (LPARAM)datos->motivo);
			SendDlgItemMessage(Dlg, IDC_COSTO, WM_GETTEXT, 100, (LPARAM)datos->costo);
			EndDialog(Dlg, 0);
			return true;
		}

		}
		return true;
	}
	case WM_CLOSE:
	{
		EndDialog(Dlg, 0);
		return true;
	}
	case WM_DESTROY:
	{
		EndDialog(Dlg, 0);
		return true;
	}
	}
	return false;
}
BOOL CALLBACK EditarD(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam) {
	HDC hdcStatic = (HDC)wParam;
	static HBRUSH hbrBkgnd = CreateSolidBrush(RGB(169, 203, 209));
	switch (Mensaje)
	{
	case WM_CTLCOLORDLG:
		return (INT_PTR)hbrBkgnd;
	case WM_CTLCOLORSTATIC:
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(169, 203, 209));
		return (INT_PTR)hbrBkgnd;
	case WM_INITDIALOG:
	{
		return true;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1:
		{
			openfilename();
			if (GetOpenFileName(&ofn) == TRUE) { 
				SetWindowText(GetDlgItem(Dlg, IDC_EDIT1), ofn.lpstrFile);
			}
			return true;
		}
		case IDOK:
		{
			SendDlgItemMessage(Dlg, IDC_DOC, WM_GETTEXT, 100, (LPARAM)datos->doc);
			SendDlgItemMessage(Dlg, IDC_CED, WM_GETTEXT, 100, (LPARAM)datos->cedula);
			SendDlgItemMessage(Dlg, IDC_INF, WM_GETTEXT, 100, (LPARAM)datos->info);
			EndDialog(Dlg, 0);
			return true;
		}

		}
		return true;
	}
	case WM_CLOSE:
	{
		EndDialog(Dlg, 0);
		return true;
	}
	case WM_DESTROY:
	{
		EndDialog(Dlg, 0);
		return true;
	}
	}
	return false;
}
void openfilename() {
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn); 
	ofn.lpstrFilter = "Todos\0*.*\0Archivos Texto\0*.TXT\0Archivos Word (97-2003)\0*.doc\0Archivos Word\0*.docx\0Imagenes jpg\0*.jpg\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "txt";
}
void LlenaEspecies(HWND objeto, UINT mensa, char *file)
{
	ifstream archi;
	char row[30] = "";
	archi.open(file);
	if (archi.is_open())
	{
		archi.getline(row, 30);
		while (!archi.eof())
		{
			SendMessage(objeto, mensa, 0, (LPARAM)row);
			archi.getline(row, 30);
		}

		archi.close();
	}

}

