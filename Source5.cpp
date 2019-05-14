#include<iostream>
#include<fstream>
#include<Windows.h>
#include <string>
#include <commdlg.h>
#include"resource.h"
using namespace std;
HWND ghDlg = 0;
HINSTANCE ghInst;
OPENFILENAME ofn;
struct nodo {
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
	char doc[100];
	char cedula[100];
	char info[100];
	nodo*sig;
	nodo*ant;
};
nodo*lista = NULL;
nodo*tempaux = NULL;
nodo*tempant = NULL;
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
void leer(nodo *);
void escribir(nodo);
char szFileName[MAX_PATH] = "1.bmp";
char _arch_esp[] = "spc.txt";
int gShow = 0;
bool busqueda = false;
bool busquedap = false;
char fechatempp[100];
bool fdato = false;
bool peditar = false;
int index2;
void openfilename();
void LlenaEspecies(HWND objeto, UINT mensa, char *file);
char*ConvierteFecha(char*Fecha);
void crearnodo(HWND Dlg, static HWND Spc);
BOOL CALLBACK Principal(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK NuevaCita(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK DatosD(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK Agenda(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK EditarC(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK EditarD(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmd, int show)
{
	leer(lista);
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
	static HWND hCbolistp = 0;
	hCbolistp = GetDlgItem(Dlg, IDC_LISTA);
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
		nodo*aux = lista;
		while (aux != NULL) {
			SendDlgItemMessage(Dlg, IDC_LISTA, CB_ADDSTRING, 100, (LPARAM)aux->fecha);
			aux = aux->sig;
		}
		return true;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_LISTA:
		{
			
			int indexP = SendMessage(hCbolistp, CB_GETCURSEL, 0, 0);
			SendMessage(hCbolistp, CB_GETLBTEXT, (WPARAM)indexP, (LPARAM)fechatempp);
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
			{
				busquedap = true;
				DialogBox(ghInst, MAKEINTRESOURCE(IDD_DIALOG4), Dlg, Agenda);
				EndDialog(Dlg, 0);
				DialogBox(ghInst, MAKEINTRESOURCE(IDD_DIALOG1), Dlg, Principal);
			}
			}
			return true;
		}
		
		case ID_NUEVA_CITA:
		{

			DialogBox(ghInst, MAKEINTRESOURCE(IDD_DIALOG2), Dlg, NuevaCita);
			EndDialog(Dlg, 0);
			DialogBox(ghInst, MAKEINTRESOURCE(IDD_DIALOG1), Dlg, Principal);
			return true;
		}
		case ID_INFORMAD:
		{
			DialogBox(ghInst, MAKEINTRESOURCE(IDD_DIALOG3), Dlg, DatosD);
			return true;
		}
		case ID_AGENDA_CITASAGREGADAS:
		{
			if (lista == NULL) {
				MessageBox(0, "No hay citas agregadas", "Aviso", MB_ICONINFORMATION);
			}
			else {
				DialogBox(ghInst, MAKEINTRESOURCE(IDD_DIALOG4), Dlg, Agenda);
				EndDialog(Dlg, 0);
				DialogBox(ghInst, MAKEINTRESOURCE(IDD_DIALOG1), Dlg, Principal);
			}
			return true;
		}
		case ID_ARCHIVO_SALIR:
		{
			escribir(*lista);
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
		SendDlgItemMessage(Dlg, IDC_MASCULINO, BM_SETCHECK, BST_CHECKED, 0);
		LlenaEspecies(hCboSpc, CB_ADDSTRING, _arch_esp);
		
		return true;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			crearnodo(Dlg, hCboSpc);
			if (fdato==false) {
				EndDialog(Dlg, 0);
				fdato = false;
			}

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
	static HWND hCbolist = 0;
	hCbolist = GetDlgItem(Dlg, IDC_LISTA);
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
		
		peditar = false;
		nodo*aux = lista;
		while (aux != NULL) {
			SendDlgItemMessage(Dlg, IDC_LISTA, CB_ADDSTRING, 100, (LPARAM)aux->fecha);
			aux = aux->sig;
		}
		if (busquedap) {
			SendDlgItemMessage(Dlg, IDC_LISTA, CB_SETCURSEL, 0, 0);/////////
			nodo*aux = lista;
			nodo*ant = NULL;
			while (aux != NULL) {
				if (strcmp(aux->fecha, fechatempp) == 0) {
					busqueda = true;
					break;
				}
				ant = aux;
				aux = aux->sig;
			}
			tempaux = aux;
			tempant = ant;
			if (busqueda) {
				SendDlgItemMessage(Dlg, IDC_NOMBRE, WM_SETTEXT, 100, (LPARAM)aux->nombre);
				SendDlgItemMessage(Dlg, IDC_APELLIDO, WM_SETTEXT, 100, (LPARAM)aux->apellido);
				SendDlgItemMessage(Dlg, IDC_TELEFONO, WM_SETTEXT, 100, (LPARAM)aux->telefono);
				SendDlgItemMessage(Dlg, IDC_ESPECIE, WM_SETTEXT, 100, (LPARAM)aux->especie);
				SendDlgItemMessage(Dlg, IDC_MASCOTA, WM_SETTEXT, 100, (LPARAM)aux->mascota);
				SendDlgItemMessage(Dlg, IDC_MOTIVO, WM_SETTEXT, 100, (LPARAM)aux->motivo);
				SendDlgItemMessage(Dlg, IDC_COSTO, WM_SETTEXT, 100, (LPARAM)aux->costo);
				SendDlgItemMessage(Dlg, IDC_HORA, WM_SETTEXT, 100, (LPARAM)aux->hora);
				peditar = true;
				busqueda = false;
				busquedap = false;
			}
		}
		

		return true;
	}
	case WM_COMMAND:
	{

		switch (LOWORD(wParam))
		{
		case IDC_LISTA:
		{
			char fechatemp[100];
			index2 = SendMessage(hCbolist, CB_GETCURSEL, 0, 0);
			SendMessage(hCbolist, CB_GETLBTEXT, (WPARAM)index2, (LPARAM)fechatemp);
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
			{
				nodo*aux = lista;
				nodo*ant=NULL;
				while (aux != NULL) {
					if (strcmp(aux->fecha, fechatemp) == 0) {
						busqueda = true;
						break;
					}
					ant = aux;
					aux = aux->sig;
				}
				tempaux = aux;
				tempant = ant;
				if (busqueda) {
					SendDlgItemMessage(Dlg, IDC_NOMBRE, WM_SETTEXT, 100, (LPARAM)aux->nombre);
					SendDlgItemMessage(Dlg, IDC_APELLIDO, WM_SETTEXT, 100, (LPARAM)aux->apellido);
					SendDlgItemMessage(Dlg, IDC_TELEFONO, WM_SETTEXT, 100, (LPARAM)aux->telefono);
					SendDlgItemMessage(Dlg, IDC_ESPECIE, WM_SETTEXT, 100, (LPARAM)aux->especie);
					SendDlgItemMessage(Dlg, IDC_MASCOTA, WM_SETTEXT, 100, (LPARAM)aux->mascota);
					SendDlgItemMessage(Dlg, IDC_MOTIVO, WM_SETTEXT, 100, (LPARAM)aux->motivo);
					SendDlgItemMessage(Dlg, IDC_COSTO, WM_SETTEXT, 100, (LPARAM)aux->costo);
					SendDlgItemMessage(Dlg, IDC_HORA, WM_SETTEXT, 100, (LPARAM)aux->hora);
					peditar = true;
					busqueda = false;
				}
			}
			}
			return true;
		}
		case ID_ARCHIVO_ELIMINAR:
		{
			if (!peditar) {
				MessageBox(0, "Seleccione la cita que desea eliminar", "Aviso", MB_ICONINFORMATION);
			}
			else {
				int opc = MessageBox(0, "¿Esta seguro que desea eliminar la cita?", "Aviso", MB_ICONINFORMATION + MB_OKCANCEL);
				if (opc == IDOK) {
					nodo*aux = tempaux;
					nodo*ant = tempant;
					if (aux == lista) {
						lista = lista->sig;
					}
					else {
						ant->sig = aux->sig;
					}
					delete aux;
					EndDialog(Dlg, 0);
					if (lista != NULL) {
						DialogBox(ghInst, MAKEINTRESOURCE(IDD_DIALOG4), Dlg, Agenda);
					}
				}
			}
			return true;
		}
		case ID_ARCHIVO_EDITAR:
		{

			if (!peditar) {
				MessageBox(0, "Seleccione la cita que desea editar", "Aviso", MB_ICONINFORMATION);
			}
			else {
				DialogBox(ghInst, MAKEINTRESOURCE(IDD_DIALOG5), Dlg, EditarC);
				nodo*aux = tempaux;
				nodo*ant = tempant;
				SendDlgItemMessage(Dlg, IDC_NOMBRE, WM_SETTEXT, 100, (LPARAM)aux->nombre);
				SendDlgItemMessage(Dlg, IDC_APELLIDO, WM_SETTEXT, 100, (LPARAM)aux->apellido);
				SendDlgItemMessage(Dlg, IDC_TELEFONO, WM_SETTEXT, 100, (LPARAM)aux->telefono);
				SendDlgItemMessage(Dlg, IDC_MASCOTA, WM_SETTEXT, 100, (LPARAM)aux->mascota);
				SendDlgItemMessage(Dlg, IDC_MOTIVO, WM_SETTEXT, 100, (LPARAM)aux->motivo);
				SendDlgItemMessage(Dlg, IDC_COSTO, WM_SETTEXT, 100, (LPARAM)aux->costo);
				SendDlgItemMessage(Dlg, IDC_HORA, WM_SETTEXT, 100, (LPARAM)aux->hora);
			}
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
		LlenaEspecies(hCboSpc, CB_ADDSTRING, _arch_esp);
		return true;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			nodo*aux = tempaux;
			nodo*ant = tempant;
			SendDlgItemMessage(Dlg, IDC_NOMBRE, WM_GETTEXT, 100, (LPARAM)aux->nombre);
			SendDlgItemMessage(Dlg, IDC_APELLIDO, WM_GETTEXT, 100, (LPARAM)aux->apellido);
			SendDlgItemMessage(Dlg, IDC_TELEFONO, WM_GETTEXT, 100, (LPARAM)aux->telefono);
			SendDlgItemMessage(Dlg, IDC_MASCOTA, WM_GETTEXT, 100, (LPARAM)aux->mascota);
			SendDlgItemMessage(Dlg, IDC_MOTIVO, WM_GETTEXT, 100, (LPARAM)aux->motivo);
			SendDlgItemMessage(Dlg, IDC_COSTO, WM_GETTEXT, 100, (LPARAM)aux->costo);
			int index = SendMessage(hCboSpc, CB_GETCURSEL, 0, 0);
			SendMessage(hCboSpc, CB_GETLBTEXT, (WPARAM)index, (LPARAM)aux->especie);
			SendDlgItemMessage(Dlg, IDC_FECHA, WM_GETTEXT, (WPARAM)100, (LPARAM)aux->fecha);
			ConvierteFecha(aux->fecha);
			SendDlgItemMessage(Dlg, IDC_HORA, WM_GETTEXT, (WPARAM)100, (LPARAM)aux->hora);
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
char*ConvierteFecha(char*Fecha)
{
	char FechaOk[11] = "";

	string cutMonth, FechaOri;
	cutMonth = Fecha; //Guarda en un string el valor de un char
	string Day = cutMonth.substr(0, 2);
	string Month = cutMonth.substr(3, 2);
	string Year = cutMonth.substr(6, 4);

	FechaOri = Year + Month + Day;
	strcat_s(FechaOk, FechaOri.c_str());

	return FechaOk;
}
void crearnodo(HWND Dlg, static HWND Spc) {
	nodo*nuevo = new nodo;
	SendDlgItemMessage(Dlg, IDC_NOMBRE, WM_GETTEXT, 100, (LPARAM)nuevo->nombre);
	SendDlgItemMessage(Dlg, IDC_APELLIDO, WM_GETTEXT, 100, (LPARAM)nuevo->apellido);
	SendDlgItemMessage(Dlg, IDC_TELEFONO, WM_GETTEXT, 100, (LPARAM)nuevo->telefono);
	SendDlgItemMessage(Dlg, IDC_MASCOTA, WM_GETTEXT, 100, (LPARAM)nuevo->mascota);
	SendDlgItemMessage(Dlg, IDC_MOTIVO, WM_GETTEXT, 100, (LPARAM)nuevo->motivo);
	SendDlgItemMessage(Dlg, IDC_COSTO, WM_GETTEXT, 100, (LPARAM)nuevo->costo);
	int index = SendMessage(Spc, CB_GETCURSEL, 0, 0);
	strcpy_s(nuevo->especie, "");
	SendMessage(Spc, CB_GETLBTEXT, (WPARAM)index, (LPARAM)nuevo->especie);
	SendDlgItemMessage(Dlg, IDC_FECHA, WM_GETTEXT, (WPARAM)100, (LPARAM)nuevo->fecha);
	ConvierteFecha(nuevo->fecha);
	SendDlgItemMessage(Dlg, IDC_HORA, WM_GETTEXT, (WPARAM)100, (LPARAM)nuevo->hora);
	//validaciones
	//se rompe al poner acentos
	fdato = false;
	for (int i = 0; i < strlen(nuevo->nombre); i++)
	{
		if (isdigit(nuevo->nombre[i])) {
			fdato = true;
			MessageBox(0, "El nombre solo debe contener caracteres", "Aviso", MB_ICONINFORMATION);
			break;
		}
	}
	for (int i = 0; i < strlen(nuevo->apellido); i++)
	{
		if (isdigit(nuevo->apellido[i])) {
			fdato = true;
			MessageBox(0, "El apellido solo debe contener caracteres", "Aviso", MB_ICONINFORMATION);
			break;
		}
	}
	for (int i = 0; i < strlen(nuevo->mascota); i++)
	{
		if (isdigit(nuevo->mascota[i])) {
			fdato = true;
			MessageBox(0, "El nombre de la mascota solo debe contener caracteres", "Aviso", MB_ICONINFORMATION);
			break;
		}
	}
	for (int i = 0; i < strlen(nuevo->telefono); i++)
	{
		if (isalpha(nuevo->telefono[i]) || (strlen(nuevo->telefono) != 8) && (strlen(nuevo->telefono) != 10) && (strlen(nuevo->telefono) != 12)) {
			fdato = true;
			MessageBox(0, "El teléfono solo debe contener 8,10 o 12 números y ser digitos", "Aviso", MB_ICONINFORMATION);
			break;
		}
	}
	for (int i = 0; i < strlen(nuevo->costo); i++)
	{
		if (isalpha(nuevo->costo[i])) {
			fdato = true;
			MessageBox(0, "El costo solo debe contener digitos", "Aviso", MB_ICONINFORMATION);
			break;
		}
	}
	if ((strcmp(nuevo->nombre, "") == 0) || (strcmp(nuevo->apellido, "") == 0) || (strcmp(nuevo->telefono, "") == 0) || (strcmp(nuevo->mascota, "") == 0)
		|| (strcmp(nuevo->motivo, "") == 0) || (strcmp(nuevo->costo, "") == 0) || (strcmp(nuevo->especie, "") == 0) || (strcmp(nuevo->fecha, "") == 0))
	{
		fdato = true;
		MessageBox(0, "Agregue los datos faltantes", "Aviso", MB_ICONINFORMATION);
	}

	if (fdato == false) {
		fdato = false;
		nuevo->ant = NULL;
		nuevo->sig = NULL;
		if (lista == NULL) {
			lista = nuevo;
		}
		else {
			nodo*aux = lista;
			while (aux->sig != NULL) {
				aux = aux->sig;
			}
			aux->sig = nuevo;
			aux->sig->ant = aux;
		}

	}
}
void escribir(nodo) {
	fstream archivo;
	archivo.open("alumnos.txt", ios::out | ios::binary);
	if (archivo.is_open()) {
		nodo *tempo = lista;
		while (tempo != NULL) {
			archivo.write((char*)tempo, sizeof(nodo));
			tempo = (*tempo).sig;
		}
		archivo.close();
	}
	else {
		MessageBox(0, "No es posible abrir el archivor", "Aviso", MB_ICONINFORMATION);
		exit(1);
	}
}
void leer(nodo *) {
	fstream archivo("alumnos.txt", ios::out | ios::app);
	archivo.close();

	archivo.open("alumnos.txt", ios::in | ios::binary);
	if (archivo.is_open()) {
		nodo *nuevo = new nodo;
		archivo.read((char*)nuevo, sizeof(nodo));

		while (!archivo.eof()) {
			nuevo->sig = NULL;


			nodo *ultimo = lista;
			while (ultimo != NULL && ultimo->sig != NULL) {
				ultimo = ultimo->sig;
			}
			if (ultimo != NULL)
				ultimo->sig = nuevo;
			else
				lista = nuevo;

			nuevo = new nodo;
			archivo.read((char*)nuevo, sizeof(nodo));
		}

		archivo.close();
	}
	else {
		MessageBox(0, "No es posible abrir el archivor", "Aviso", MB_ICONINFORMATION);
		exit(1);
	}
}
