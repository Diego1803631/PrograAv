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
int gShow = 0;
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
	char monto[100];
	bool urgente;
	char genero[100];
	char dia[100];
	char mes[100];
	char año[100];
	nodo*sig;
	nodo*ant;
};
nodo*lista = NULL;
nodo*tempaux = NULL;
nodo*tempant = NULL;
struct datos {
	char doc[100];
	char cedula[100];
	char info[100];
	char imagen[MAX_PATH];
};
datos doctor[100];
//variables
int dia, mes, año;
int diatemp, mestemp, añotemp;
char fechatempp[100];
int indexP;
int indexS;
char _arch_esp[] = "spc.txt";
char _arch_doc[] = "doc.txt";
char folder[MAX_PATH] = "";
//variables booleanas
bool busqueda = false;
bool busqueda2 = false;
bool busquedap = false;
bool filtro = false;
bool fdato = false;
bool peditar = false;
//funciones
void openfilename();
void LlenaEspecies(HWND objeto, UINT mensa, char *file);
void LlenaDoctor();
void GuardaDoctor();
char*ConvierteFecha(char*Fecha);
void GuardaFecha(nodo*aux);
void Ordenar();
void crearnodo(HWND Dlg, static HWND Spc);
void leer(nodo *);
void escribir(nodo);
void eliminarnodos();
BOOL CALLBACK Principal(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK NuevaCita(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK DatosD(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK Agenda(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK EditarC(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
BOOL CALLBACK EditarD(HWND Dlg, UINT Mensaje, WPARAM wParam, LPARAM lparam);
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmd, int show)
{
	GetCurrentDirectory(MAX_PATH, folder);
	LlenaDoctor();
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
	static HBITMAP bmp1, bmp2;
	bmp1 = (HBITMAP)SendDlgItemMessage(Dlg, IDC_IMAGE,
		STM_GETIMAGE, IMAGE_BITMAP, 0);
	bmp2 = (HBITMAP)LoadImage(NULL, doctor->imagen,
		IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE);
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
		if (lista != NULL) {
			Ordenar();
		}
		SendDlgItemMessage(Dlg, IDC_IMAGE, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);
		SendDlgItemMessage(Dlg, IDC_DOC, WM_SETTEXT, 100, (LPARAM)doctor->doc);
		SendDlgItemMessage(Dlg, IDC_CED, WM_SETTEXT, 100, (LPARAM)doctor->cedula);
		SendDlgItemMessage(Dlg, IDC_INF, WM_SETTEXT, 100, (LPARAM)doctor->info);
		nodo*aux = lista;
		nodo*aux2 = NULL;
		bool repetido;
		while (aux != NULL) {
			repetido = false;
			aux2 = lista;
			while (aux2 != NULL && aux2 != aux) {
				if (strcmp(aux2->fecha, aux->fecha) == 0) {
					repetido = true;
				}
				aux2 = aux2->sig;
			}
			if (repetido == false) {
				SendDlgItemMessage(Dlg, IDC_LISTA, CB_ADDSTRING, 100, (LPARAM)aux->fecha);
			}
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
			
			indexP = SendDlgItemMessage(Dlg,IDC_LISTA, CB_GETCURSEL, 0, 0);
			SendDlgItemMessage(Dlg,IDC_LISTA, CB_GETLBTEXT, (WPARAM)indexP, (LPARAM)fechatempp);
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
			{
				busquedap = true;
				EndDialog(Dlg, 0);
				ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG4), 0, Agenda);
				ShowWindow(ghDlg, gShow);
			}
			}
			return true;
		}
		
		case ID_NUEVA_CITA:
		{
			EndDialog(Dlg, 0);
			ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG2), 0, NuevaCita);
			ShowWindow(ghDlg, gShow);
			return true;
		}
		case ID_INFORMAD:
		{
			EndDialog(Dlg, 0);
			ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG3), 0, DatosD);
			ShowWindow(ghDlg, gShow);
			return true;
		}
		case ID_AGENDA_CITASAGREGADAS:
		{
			if (lista == NULL) {
				MessageBox(0, "No hay citas agregadas", "Aviso", MB_ICONINFORMATION);
			}
			else {
				EndDialog(Dlg, 0);
				ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG4), 0, Agenda);
				ShowWindow(ghDlg, gShow);
			}
			return true;
		}
		case ID_ARCHIVO_SALIR:
		{
			GuardaDoctor();
			if (lista == NULL) {
				remove("datos.bin");
				eliminarnodos();
				PostQuitMessage(0);
			}
			else {
				escribir(*lista);
				PostQuitMessage(0);
			}
			return true;
		}
		}
		return true;
	}
	case WM_CLOSE:
	{

		eliminarnodos();
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
	hCboSpc = GetDlgItem(Dlg, IDC_ESPECIE);
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
		char fechahoy[100];
		SendDlgItemMessage(Dlg, IDC_FECHA, WM_GETTEXT, (WPARAM)100, (LPARAM)fechahoy);
		ConvierteFecha(fechahoy);
		diatemp = dia;
		mestemp = mes;
		añotemp = año;
		SendDlgItemMessage(Dlg, IDC_MASCULINO, BM_SETCHECK, BST_CHECKED, 0);
		SendDlgItemMessage(Dlg, IDC_CONTADO, BM_SETCHECK, BST_CHECKED, 0);
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
				ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, Principal);
				ShowWindow(ghDlg, gShow);
				fdato = false;
			}

			return true;
		}
		case IDCANCEL:
		{
			int opc = MessageBox(0, "¿Esta seguro que desea salir?", "Aviso", MB_ICONINFORMATION + MB_OKCANCEL);
			if (opc == IDOK) {
				EndDialog(Dlg, 0);
				ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, Principal);
				ShowWindow(ghDlg, gShow);
			}
			return true;
		}
		}
		return true;
	}
	case WM_CLOSE:
	{
		int opc = MessageBox(0, "¿Esta seguro que desea salir?", "Aviso", MB_ICONINFORMATION + MB_OKCANCEL);
		if (opc == IDOK) {
			EndDialog(Dlg, 0);
			ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, Principal);
			ShowWindow(ghDlg, gShow);
		}
		return true;
	}
	case WM_DESTROY:
	{
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
	bmp2 = (HBITMAP)LoadImage(NULL, doctor->imagen,
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
		SendDlgItemMessage(Dlg, IDC_DOC, WM_SETTEXT, 100, (LPARAM)doctor->doc);
		SendDlgItemMessage(Dlg, IDC_CED, WM_SETTEXT, 100, (LPARAM)doctor->cedula);
		SendDlgItemMessage(Dlg, IDC_INF, WM_SETTEXT, 100, (LPARAM)doctor->info);
		return true;
	}
	case WM_COMMAND:
	{
		
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			EndDialog(Dlg, 0);
			ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, Principal);
			ShowWindow(ghDlg, gShow);
			return true;
		}
		case ID_ARCHIVO_EDITD:
		{
			DialogBox(ghInst, MAKEINTRESOURCE(IDD_DIALOG6), Dlg, EditarD);
			EndDialog(Dlg, 0);
			ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG3), 0, DatosD);
			ShowWindow(ghDlg, gShow);
			SendDlgItemMessage(Dlg, IDC_DOC, WM_SETTEXT, 100, (LPARAM)doctor->doc);
			SendDlgItemMessage(Dlg, IDC_CED, WM_SETTEXT, 100, (LPARAM)doctor->cedula);
			SendDlgItemMessage(Dlg, IDC_INF, WM_SETTEXT, 100, (LPARAM)doctor->info);
			bmp1 = (HBITMAP)SendDlgItemMessage(Dlg, IDC_IMAGE,
				STM_GETIMAGE, IMAGE_BITMAP, 0);
			bmp2 = (HBITMAP)LoadImage(NULL, doctor->imagen,
				IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE);
			return true;
		}
		case IDCANCEL:
		{
			EndDialog(Dlg, 0);
			ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, Principal);
			ShowWindow(ghDlg, gShow);
			return true;
		}

		}
		return true;
	}
	case WM_CLOSE:
	{
		EndDialog(Dlg, 0);
		ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, Principal);
		ShowWindow(ghDlg, gShow);
		return true;
	}
	case WM_DESTROY:
	{
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
		char fechatempb[100];
		bool repetido = false;
		if (filtro == true) {
			SendDlgItemMessage(Dlg, IDC_CHECK1, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
			nodo*aux = lista;
			nodo*aux2 = NULL;
			bool repetido;
			while (aux != NULL) {
				repetido = false;
				aux2 = lista;
				while (aux2 != NULL && aux2 != aux) {
					if (strcmp(aux2->fecha, aux->fecha) == 0) {
						repetido = true;
					}
					aux2 = aux2->sig;
				}
				if (repetido == false) {
					if (aux->urgente == true) {
						SendDlgItemMessage(Dlg, IDC_LISTA, CB_ADDSTRING, 100, (LPARAM)aux->fecha);
					}
				}
				aux = aux->sig;
			}
		}
		else {
			nodo*aux = lista;
			nodo*aux2 = NULL;
			bool repetido;
			while (aux != NULL) {
				repetido = false;
				aux2 = lista;
				while (aux2 != NULL && aux2 != aux) {
					if (strcmp(aux2->fecha, aux->fecha) == 0) {
						repetido = true;
					}
					aux2 = aux2->sig;
				}
				if (repetido == false) {
					SendDlgItemMessage(Dlg, IDC_LISTA, CB_ADDSTRING, 100, (LPARAM)aux->fecha);
				}
				aux = aux->sig;
			}
		}
		if (busquedap) {
			SendDlgItemMessage(Dlg, IDC_LISTA, CB_SETCURSEL, (WPARAM)indexP, 0);
			SendDlgItemMessage(Dlg, IDC_HORA, CB_RESETCONTENT, 100, 0);
			nodo*aux = lista;  
			nodo*ant = NULL;
			while (aux != NULL) {
				if (strcmp(aux->fecha, fechatempp) == 0) {
					SendDlgItemMessage(Dlg, IDC_HORA, CB_ADDSTRING, 100, (LPARAM)aux->hora);
				}
				ant = aux;
				aux = aux->sig;
			}
		}
		return true;
	}
	case WM_COMMAND:
	{

		switch (LOWORD(wParam))
		{
		case IDC_URGENTE:
		{
			SendDlgItemMessage(Dlg, IDC_LISTA, CB_RESETCONTENT, 100, (LPARAM)0);
			filtro = true;
			EndDialog(Dlg, 0);
			ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG4), 0, Agenda);
			ShowWindow(ghDlg, gShow);
			return true;
		}
		case IDC_LISTA:
		{
			char fechatemp[100];
			int index = SendDlgItemMessage(Dlg, IDC_LISTA, CB_GETCURSEL, 0, 0);
			SendDlgItemMessage(Dlg, IDC_LISTA, CB_GETLBTEXT, (WPARAM)index, (LPARAM)fechatemp);
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
			{
				if (filtro == true) {
					SendDlgItemMessage(Dlg, IDC_HORA, CB_RESETCONTENT, 100, 0);
					nodo*aux = lista;
					nodo*ant = NULL;
					while (aux != NULL) {
						if (strcmp(aux->fecha, fechatemp) == 0) {
							if (aux->urgente == true) {
								SendDlgItemMessage(Dlg, IDC_HORA, CB_ADDSTRING, 100, (LPARAM)aux->hora);
							}
						}
						ant = aux;
						aux = aux->sig;
					}
				}
				else {
					SendDlgItemMessage(Dlg, IDC_HORA, CB_RESETCONTENT, 100, 0);
					nodo*aux = lista;
					nodo*ant = NULL;
					while (aux != NULL) {
						if (strcmp(aux->fecha, fechatemp) == 0) {
							SendDlgItemMessage(Dlg, IDC_HORA, CB_ADDSTRING, 100, (LPARAM)aux->hora);
						}
						ant = aux;
						aux = aux->sig;
					}
				}
			}
			}
			return true;
		}
		case IDC_HORA:
		{
			char fechatemp[100];
			int index = SendDlgItemMessage(Dlg, IDC_LISTA, CB_GETCURSEL, 0, 0);
			SendDlgItemMessage(Dlg, IDC_LISTA, CB_GETLBTEXT, (WPARAM)index, (LPARAM)fechatemp);
			char horatemp[100];
			int index2 = SendDlgItemMessage(Dlg, IDC_HORA, CB_GETCURSEL, 0, 0);
			SendDlgItemMessage(Dlg, IDC_HORA, CB_GETLBTEXT, (WPARAM)index2, (LPARAM)horatemp);
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
			{
				nodo*aux = lista;
				nodo*ant=NULL;
				while (aux != NULL) {

					if ((strcmp(aux->hora, horatemp) == 0) && (strcmp(aux->fecha, fechatemp) == 0)) {
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
					SendDlgItemMessage(Dlg, IDC_MONTO, WM_SETTEXT, 100, (LPARAM)aux->monto);
					SendDlgItemMessage(Dlg, IDC_GENERO, WM_SETTEXT, 100, (LPARAM)aux->genero);
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
						ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG4), 0, Agenda);
						ShowWindow(ghDlg, gShow);
					}
					else {
						ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, Principal);
						ShowWindow(ghDlg, gShow);
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
				EndDialog(Dlg, 0);
				ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG4), 0, Agenda);
				ShowWindow(ghDlg, gShow);
			}
			return true;
		}
		case IDCRESET:
		{
			filtro = false;
			EndDialog(Dlg, 0);
			ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG4), 0, Agenda);
			ShowWindow(ghDlg, gShow);
			return true;
		}
		case IDCANCEL:
		{
			filtro = false;
			EndDialog(Dlg, 0);
			ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, Principal);
			ShowWindow(ghDlg, gShow);
			return true;
		}

		}
		return true;
	}
	case WM_CLOSE:
	{
		filtro = false;
		EndDialog(Dlg, 0);
		ghDlg = CreateDialog(ghInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, Principal);
		ShowWindow(ghDlg, gShow);

		return true;
	}
	case WM_DESTROY:
	{
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
		nodo*aux = tempaux;
		nodo*ant = tempant;
		SendDlgItemMessage(Dlg, IDC_NOMBRE, WM_SETTEXT, 100, (LPARAM)aux->nombre);
		SendDlgItemMessage(Dlg, IDC_APELLIDO, WM_SETTEXT, 100, (LPARAM)aux->apellido);
		SendDlgItemMessage(Dlg, IDC_TELEFONO, WM_SETTEXT, 100, (LPARAM)aux->telefono);
		SendDlgItemMessage(Dlg, IDC_ESPECIE, WM_SETTEXT, 100, (LPARAM)aux->especie);
		SendDlgItemMessage(Dlg, IDC_MASCOTA, WM_SETTEXT, 100, (LPARAM)aux->mascota);
		SendDlgItemMessage(Dlg, IDC_MOTIVO, WM_SETTEXT, 100, (LPARAM)aux->motivo);
		SendDlgItemMessage(Dlg, IDC_COSTO, WM_SETTEXT, 100, (LPARAM)aux->costo);
		SendDlgItemMessage(Dlg, IDC_GENERO, WM_SETTEXT, 100, (LPARAM)aux->genero);
		fdato = false;
		hCboSpc = GetDlgItem(Dlg, IDC_ESPECIE);
		LlenaEspecies(hCboSpc, CB_ADDSTRING, _arch_esp);
		int indexS = SendDlgItemMessage(Dlg, IDC_ESPECIE, CB_FINDSTRING, (WPARAM)0, (LPARAM)aux->especie);
		SendDlgItemMessage(Dlg, IDC_ESPECIE, CB_SETCURSEL, (WPARAM)indexS, 0);
		if (strcmp(aux->genero, "Masculino") == 0) {
			SendDlgItemMessage(Dlg, IDC_MASCULINO, BM_SETCHECK, BST_CHECKED, 0);
		}
		else if (strcmp(aux->genero, "Femenino") == 0) {
			SendDlgItemMessage(Dlg, IDC_FEMENINO, BM_SETCHECK, BST_CHECKED, 0);
		}
		char*p;
		p = aux->monto;
		p++;
		string str = aux->monto;
		int ptres = str.find('3');
		int pseis = str.find('6');
		int pnueve = str.find('9');
		if (ptres > 0) {
			SendDlgItemMessage(Dlg, IDC_TRES, BM_SETCHECK, BST_CHECKED, 0);
		}
		if (pseis > 0) {
			SendDlgItemMessage(Dlg, IDC_SEIS, BM_SETCHECK, BST_CHECKED, 0);
		}
		if (pnueve > 0) {
			SendDlgItemMessage(Dlg, IDC_NUEVE, BM_SETCHECK, BST_CHECKED, 0);
		}
		if (ptres < 0 && pseis < 0 && pnueve < 0) {
			SendDlgItemMessage(Dlg, IDC_CONTADO, BM_SETCHECK, BST_CHECKED, 0);
		}
		if (aux->urgente) {
			SendDlgItemMessage(Dlg, IDC_URGENTE, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
		}
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
			strcpy_s(aux->especie, "");
			SendMessage(hCboSpc, CB_GETLBTEXT, (WPARAM)index, (LPARAM)aux->especie);
			if (SendDlgItemMessage(Dlg, IDC_CHECK1, BM_GETCHECK, 0, 0) == BST_CHECKED) {
				SendDlgItemMessage(Dlg, IDC_FECHA, WM_GETTEXT, (WPARAM)100, (LPARAM)aux->fecha);
				ConvierteFecha(aux->fecha);
			}
			if (SendDlgItemMessage(Dlg, IDC_CHECK2, BM_GETCHECK, 0, 0) == BST_CHECKED) {
				SendDlgItemMessage(Dlg, IDC_HORA, WM_GETTEXT, (WPARAM)100, (LPARAM)aux->hora);
			}
			if (SendDlgItemMessage(Dlg, IDC_TRES, BM_GETCHECK, 0, 0) == BST_CHECKED) {
				float tres = stof(aux->costo);
				float porciento;
				porciento = tres * 0.12;
				tres = tres + porciento;
				tres = tres / 3;
				sprintf_s(aux->monto, "%f", tres);
				strcat_s(aux->monto, " a 3 meses");
			}
			if (SendDlgItemMessage(Dlg, IDC_SEIS, BM_GETCHECK, 0, 0) == BST_CHECKED) {
				float seis = stof(aux->costo);
				float porciento;
				porciento = seis * 0.12;
				seis = seis + porciento;
				seis = seis / 6;
				sprintf_s(aux->monto, "%f", seis);
				strcat_s(aux->monto, " a 6 meses");
			}
			if (SendDlgItemMessage(Dlg, IDC_NUEVE, BM_GETCHECK, 0, 0) == BST_CHECKED) {
				float nueve = stof(aux->costo);
				float porciento;
				porciento = nueve * 0.12;
				nueve = nueve + porciento;
				nueve = nueve / 6;
				sprintf_s(aux->monto, "%f", nueve);
				strcat_s(aux->monto, " a 9 meses");
			}
			if (SendDlgItemMessage(Dlg, IDC_MASCULINO, BM_GETCHECK, 0, 0) == BST_CHECKED) {
				strcpy_s(aux->genero, "Masculino");
			}
			else if (SendDlgItemMessage(Dlg, IDC_FEMENINO, BM_GETCHECK, 0, 0) == BST_CHECKED) {
				strcpy_s(aux->genero, "Femenino");
			}
			if (SendDlgItemMessage(Dlg, IDC_URGENTE, BM_GETCHECK, 0, 0) == BST_CHECKED) {
				aux->urgente = true;
			}
			else {
				aux->urgente = false;
			}
			fdato = false;
			if (año < añotemp) {
				fdato = true;
				MessageBox(0, "Fecha inválida", "Aviso", MB_ICONINFORMATION);
			}
			else if (año == añotemp) {
				if (mes < mestemp) {
					fdato = true;
					MessageBox(0, "Fecha inválida", "Aviso", MB_ICONINFORMATION);
				}
				else if (mes == mestemp) {
					if (dia < diatemp) {
						fdato = true;
						MessageBox(0, "Fecha inválida", "Aviso", MB_ICONINFORMATION);
					}
				}
			}
			char fechatemp[100];
			char horatemp[100];
			int conta = 0;
			strcpy_s(fechatemp, aux->fecha);
			strcpy_s(horatemp, aux->hora);
			nodo*aux2 = lista;
			while (aux2 != NULL) {
				if (strcmp(aux2->fecha, fechatemp) == 0) {
					if (strcmp(aux2->hora, horatemp) == 0) {
						conta++;
					}
					if (conta > 1) {
						fdato = true;
						MessageBox(0, "Fecha ocupada", "Aviso", MB_ICONINFORMATION);
					}
				}
				aux2 = aux2->sig;
			}
			nodo*nuevo = aux;
			for (int i = 0; i < strlen(nuevo->nombre); i++)
			{
				if ((nuevo->nombre[i] < 'a' || nuevo->nombre[i] > 'z') && (nuevo->nombre[i] < 'A' || nuevo->nombre[i] > 'Z') &&
					(nuevo->nombre[i] != 'ñ'&&nuevo->nombre[i] != 'Ñ'&&nuevo->nombre[i] != 'á'&&nuevo->nombre[i] != 'é'&&nuevo->nombre[i] != 'í'&&nuevo->nombre[i] != 'ó'&&nuevo->nombre[i] != 'ú'
						&&nuevo->nombre[i] != 'Á'&&nuevo->nombre[i] != 'É'&&nuevo->nombre[i] != 'Í'&&nuevo->nombre[i] != 'Ó'&&nuevo->nombre[i] != 'Ú')) {
					fdato = true;
					MessageBox(0, "El nombre solo debe contener caracteres", "Aviso", MB_ICONINFORMATION);
					break;
				}
			}
			for (int i = 0; i < strlen(nuevo->apellido); i++)
			{
				if ((nuevo->apellido[i] < 'a' || nuevo->apellido[i] > 'z') && (nuevo->apellido[i] < 'A' || nuevo->apellido[i] > 'Z') &&
					(nuevo->apellido[i] != 'ñ'&&nuevo->apellido[i] != 'Ñ'&&nuevo->apellido[i] != 'á'&&nuevo->apellido[i] != 'é'&&nuevo->apellido[i] != 'í'&&nuevo->apellido[i] != 'ó'&&nuevo->apellido[i] != 'ú'
						&&nuevo->apellido[i] != 'Á'&&nuevo->apellido[i] != 'É'&&nuevo->apellido[i] != 'Í'&&nuevo->apellido[i] != 'Ó'&&nuevo->apellido[i] != 'Ú')) {
					fdato = true;
					MessageBox(0, "El apellido solo debe contener caracteres", "Aviso", MB_ICONINFORMATION);
					break;
				}
			}
			for (int i = 0; i < strlen(nuevo->mascota); i++)
			{
				if ((nuevo->mascota[i] < 'a' || nuevo->mascota[i] > 'z') && (nuevo->mascota[i] < 'A' || nuevo->mascota[i] > 'Z') &&
					(nuevo->mascota[i] != 'ñ'&&nuevo->mascota[i] != 'Ñ'&&nuevo->mascota[i] != 'á'&&nuevo->mascota[i] != 'é'&&nuevo->mascota[i] != 'í'&&nuevo->mascota[i] != 'ó'&&nuevo->mascota[i] != 'ú'
						&&nuevo->mascota[i] != 'Á'&&nuevo->mascota[i] != 'É'&&nuevo->mascota[i] != 'Í'&&nuevo->mascota[i] != 'Ó'&&nuevo->mascota[i] != 'Ú')) {
					fdato = true;
					MessageBox(0, "El nombre de la mascota solo debe contener caracteres", "Aviso", MB_ICONINFORMATION);
					break;
				}
			}
			for (int i = 0; i < strlen(nuevo->telefono); i++)
			{
				if ((strlen(nuevo->telefono) != 8) && (strlen(nuevo->telefono) != 10) && (strlen(nuevo->telefono) != 12)) {
					fdato = true;
					MessageBox(0, "El teléfono solo debe contener 8, 10 o 12 números", "Aviso", MB_ICONINFORMATION);
					break;
				}
			}
			for (int i = 0; i < strlen(nuevo->costo); i++) {
				if (nuevo->costo[i] != '0' && nuevo->costo[i] != '1' && nuevo->costo[i] != '2' && nuevo->costo[i] != '3' && nuevo->costo[i] != '4'  && nuevo->costo[i] != '5'
					&& nuevo->costo[i] != '6' && nuevo->costo[i] != '7' && nuevo->costo[i] != '8' && nuevo->costo[i] != '9'&& nuevo->costo[i] != '.') {
					fdato = true;
					MessageBox(0, "El costo solo debe de tener digitos", "Aviso", MB_ICONINFORMATION);
					break;
				}
			}
			char*p;
			p = nuevo->costo;
			p++;
			string str = nuevo->costo;
			int punto = str.find('.');
			if (punto > 0) {
				p = p + punto;
				string str2 = p;
				int punto2 = str2.find('.');
				if (punto2 > 0) {
					fdato = true;
					MessageBox(0, "El costo solo acepta un punto decimal", "Aviso", MB_ICONINFORMATION);
				}
			}
			if ((strcmp(nuevo->nombre, "") == 0) || (strcmp(nuevo->apellido, "") == 0) || (strcmp(nuevo->telefono, "") == 0) || (strcmp(nuevo->mascota, "") == 0)
				|| (strcmp(nuevo->motivo, "") == 0) || (strcmp(nuevo->costo, "") == 0) || (strcmp(nuevo->especie, "") == 0) || (strcmp(nuevo->fecha, "") == 0))
			{
				fdato = true;
				MessageBox(0, "Agregue los datos faltantes", "Aviso", MB_ICONINFORMATION);
			}
			if (fdato == false) {
				EndDialog(Dlg, 0);
			}
			return true;
		}
		case IDC_CANCEL: {
			int opc = MessageBox(0, "¿Esta seguro que desea salir?", "Aviso", MB_ICONINFORMATION + MB_OKCANCEL);
			if (opc == IDOK) {
				EndDialog(Dlg, 0);
				nodo*aux = tempaux;
				nodo*ant = tempant;
				SendDlgItemMessage(Dlg, IDC_NOMBRE, WM_SETTEXT, 100, (LPARAM)aux->nombre);
				SendDlgItemMessage(Dlg, IDC_APELLIDO, WM_SETTEXT, 100, (LPARAM)aux->apellido);
				SendDlgItemMessage(Dlg, IDC_TELEFONO, WM_SETTEXT, 100, (LPARAM)aux->telefono);
				SendDlgItemMessage(Dlg, IDC_ESPECIE, WM_SETTEXT, 100, (LPARAM)aux->especie);
				SendDlgItemMessage(Dlg, IDC_MASCOTA, WM_SETTEXT, 100, (LPARAM)aux->mascota);
				SendDlgItemMessage(Dlg, IDC_MOTIVO, WM_SETTEXT, 100, (LPARAM)aux->motivo);
				SendDlgItemMessage(Dlg, IDC_COSTO, WM_SETTEXT, 100, (LPARAM)aux->costo);
				SendDlgItemMessage(Dlg, IDC_GENERO, WM_SETTEXT, 100, (LPARAM)aux->genero);
			}
			return true;
		}
		}
		return true;
	}
	case WM_CLOSE:
	{
		int opc = MessageBox(0, "¿Esta seguro que desea salir?", "Aviso", MB_ICONINFORMATION + MB_OKCANCEL);
		if (opc == IDOK) {
			EndDialog(Dlg, 0);
			nodo*aux = tempaux;
			nodo*ant = tempant;
			SendDlgItemMessage(Dlg, IDC_NOMBRE, WM_SETTEXT, 100, (LPARAM)aux->nombre);
			SendDlgItemMessage(Dlg, IDC_APELLIDO, WM_SETTEXT, 100, (LPARAM)aux->apellido);
			SendDlgItemMessage(Dlg, IDC_TELEFONO, WM_SETTEXT, 100, (LPARAM)aux->telefono);
			SendDlgItemMessage(Dlg, IDC_ESPECIE, WM_SETTEXT, 100, (LPARAM)aux->especie);
			SendDlgItemMessage(Dlg, IDC_MASCOTA, WM_SETTEXT, 100, (LPARAM)aux->mascota);
			SendDlgItemMessage(Dlg, IDC_MOTIVO, WM_SETTEXT, 100, (LPARAM)aux->motivo);
			SendDlgItemMessage(Dlg, IDC_COSTO, WM_SETTEXT, 100, (LPARAM)aux->costo);
			SendDlgItemMessage(Dlg, IDC_GENERO, WM_SETTEXT, 100, (LPARAM)aux->genero);
		}
		return true;
	}
	case WM_DESTROY:
	{
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
			bool temp = true;
			char ntemp[100];
			SendDlgItemMessage(Dlg, IDC_DOC, WM_GETTEXT, 100, (LPARAM)ntemp);
			for (int i = 0; i < strlen(ntemp); i++)
			{
				if ((ntemp[i] < 'a' || ntemp[i] > 'z') && (ntemp[i] < 'A' || ntemp[i] > 'Z') &&
					(ntemp[i] != 'ñ'&&ntemp[i] != 'Ñ'&&ntemp[i] != 'á'&&ntemp[i] != 'é'&&ntemp[i] != 'í'&&ntemp[i] != 'ó'&&ntemp[i] != 'ú'
						&&ntemp[i] != 'Á'&&ntemp[i] != 'É'&&ntemp[i] != 'Í'&&ntemp[i] != 'Ó'&&ntemp[i] != 'Ú'&&ntemp[i] != 32)) {
					MessageBox(0, "El nombre solo debe contener caracteres", "Aviso", MB_ICONINFORMATION);
					temp = false;
					break;
				}
				else {
					strcpy_s(doctor->doc, ntemp);
				}
			}
			SendDlgItemMessage(Dlg, IDC_CED, WM_GETTEXT, 100, (LPARAM)doctor->cedula);
			SendDlgItemMessage(Dlg, IDC_INF, WM_GETTEXT, 100, (LPARAM)doctor->info);
			if (temp) {
				EndDialog(Dlg, 0);
			}
			return true;
		}
		case IDCANCEL: {
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
		int opc = MessageBox(0, "¿Esta seguro que desea salir?", "Aviso", MB_ICONINFORMATION + MB_OKCANCEL);
		if (opc == IDOK) {
			EndDialog(Dlg, 0);
		}
		return true;
	}
	case WM_DESTROY:
	{
		return true;
	}
	}
	return false;
}
void openfilename() {
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn); 
	ofn.lpstrFilter = "Todos\0*.*\0Archivos Texto\0*.TXT\0Archivos Word (97-2003)\0*.doc\0Archivos Word\0*.docx\0Imagenes jpg\0*.jpg\0";
	ofn.lpstrFile = doctor->imagen;
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
void LlenaDoctor() {
	ifstream archi2;
	char row[MAX_PATH] = "";
	archi2.open(_arch_doc);
	if (archi2.is_open())
	{
		archi2.getline(row, MAX_PATH);
		for (int i = 0; i < 4; i++) {
			if (i == 0) {
				strcpy_s(doctor->doc, row);
			}
			if (i == 1) {
				strcpy_s(doctor->cedula, row);
			}
			if (i == 2) {
				strcpy_s(doctor->info, row);
			}
			if (i == 3) {
				strcpy_s(doctor->imagen, row);
			}
			archi2.getline(row, MAX_PATH);
		}
		archi2.close();
	}

}
void GuardaDoctor() {
	ofstream archi3;
	archi3.open(_arch_doc);
	if (archi3.is_open())
	{
		archi3 << doctor->doc << endl;
		archi3 << doctor->cedula << endl;
		archi3 << doctor->info << endl;
		archi3 << doctor->imagen << endl;
		archi3.close();
	}
	
}
char*ConvierteFecha(char*Fecha)
{
	char FechaOk[11] = "";

	string cutMonth, FechaOri;
	cutMonth = Fecha; 
	string Day = cutMonth.substr(0, 2);
	string Month = cutMonth.substr(3, 2);
	string Year = cutMonth.substr(6, 4);

	FechaOri = Year + Month + Day;
	strcat_s(FechaOk, FechaOri.c_str());
	dia = atoi(Day.c_str());
	mes = atoi(Month.c_str());
	año = atoi(Year.c_str());
	return FechaOk;
}
void GuardaFecha(nodo*aux) {
	char strdia[100];
	sprintf_s(strdia, "%d", dia);
	char strmes[100];
	sprintf_s(strmes, "%d", mes);
	char straño[100];
	sprintf_s(straño, "%d", año);
	strcpy_s(aux->dia, strdia);
	strcpy_s(aux->mes, strmes);
	strcpy_s(aux->año, straño);
}
void Ordenar() {
	struct str {
		char nombre[100];
		char apellido[100];
		char fecha[100];
		char hora[100];
		char telefono[100];
		char especie[100];
		char mascota[100];
		char motivo[100];
		char costo[100];
		char monto[100];
		bool urgente;
		char genero[100];
		char dia[100];
		char mes[100];
		char año[100];
	};
	str temp[100];
	nodo* actual;
	nodo*siguiente;
	actual = lista;
	while (actual->sig != NULL)
	{
		siguiente = actual->sig;

		while (siguiente != NULL)
		{
			int dia1 = atoi(actual->dia);
			int dia2 = atoi(siguiente->dia);
			if (dia1 > dia2)
			{
				strcpy_s(temp->nombre, siguiente->nombre);
				strcpy_s(temp->apellido, siguiente->apellido);
				strcpy_s(temp->fecha, siguiente->fecha);
				strcpy_s(temp->hora, siguiente->hora);
				strcpy_s(temp->telefono, siguiente->telefono);
				strcpy_s(temp->especie, siguiente->especie);
				strcpy_s(temp->mascota, siguiente->mascota);
				strcpy_s(temp->motivo, siguiente->motivo);
				strcpy_s(temp->costo, siguiente->costo);
				strcpy_s(temp->monto, siguiente->monto);
				temp->urgente = siguiente->urgente;
				strcpy_s(temp->genero, siguiente->genero);
				strcpy_s(temp->dia, siguiente->dia);
				strcpy_s(temp->mes, siguiente->mes);
				strcpy_s(temp->año, siguiente->año);
				///////
				strcpy_s(siguiente->nombre, actual->nombre);
				strcpy_s(siguiente->apellido, actual->apellido);
				strcpy_s(siguiente->fecha, actual->fecha);
				strcpy_s(siguiente->hora, actual->hora);
				strcpy_s(siguiente->telefono, actual->telefono);
				strcpy_s(siguiente->especie, actual->especie);
				strcpy_s(siguiente->mascota, actual->mascota);
				strcpy_s(siguiente->motivo, actual->motivo);
				strcpy_s(siguiente->costo, actual->costo);
				strcpy_s(siguiente->monto, actual->monto);
				siguiente->urgente = actual->urgente;
				strcpy_s(siguiente->genero, actual->genero);
				strcpy_s(siguiente->dia, actual->dia);
				strcpy_s(siguiente->mes, actual->mes);
				strcpy_s(siguiente->año, actual->año);
				////////
				strcpy_s(actual->nombre, temp->nombre);
				strcpy_s(actual->apellido, temp->apellido);
				strcpy_s(actual->fecha, temp->fecha);
				strcpy_s(actual->hora, temp->hora);
				strcpy_s(actual->telefono, temp->telefono);
				strcpy_s(actual->especie, temp->especie);
				strcpy_s(actual->mascota, temp->mascota);
				strcpy_s(actual->motivo, temp->motivo);
				strcpy_s(actual->costo, temp->costo);
				strcpy_s(actual->monto, temp->monto);
				actual->urgente = temp->urgente;
				strcpy_s(actual->genero, temp->genero);
				strcpy_s(actual->dia, temp->dia);
				strcpy_s(actual->mes, temp->mes);
				strcpy_s(actual->año, temp->año);
			}
			siguiente = siguiente->sig;
		}
		actual = actual->sig;
		siguiente = actual->sig;

	}
	
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
	GuardaFecha(nuevo);
	SendDlgItemMessage(Dlg, IDC_HORA, WM_GETTEXT, (WPARAM)100, (LPARAM)nuevo->hora);
	if (SendDlgItemMessage(Dlg, IDC_MASCULINO, BM_GETCHECK, 0, 0) == BST_CHECKED) {
		strcpy_s(nuevo->genero, "Masculino");
	}
	else if (SendDlgItemMessage(Dlg, IDC_FEMENINO, BM_GETCHECK, 0, 0) == BST_CHECKED) {
		strcpy_s(nuevo->genero, "Femenino");
	}
	if (SendDlgItemMessage(Dlg, IDC_URGENTE, BM_GETCHECK, 0, 0) == BST_CHECKED) {
		nuevo->urgente = true;
	}
	else {
		nuevo->urgente = false;
	}
	if (SendDlgItemMessage(Dlg, IDC_CONTADO, BM_GETCHECK, 0, 0) == BST_CHECKED) {
		strcpy_s(nuevo->monto, nuevo->costo);
	}
	if (SendDlgItemMessage(Dlg, IDC_TRES, BM_GETCHECK, 0, 0) == BST_CHECKED) {
		float tres = stof(nuevo->costo);
		float porciento;
		porciento = tres * 0.12;
		tres = tres + porciento;
		tres = tres / 3;
		sprintf_s(nuevo->monto, "%f", tres);
		strcat_s(nuevo->monto, " a 3 meses");
	}
	if (SendDlgItemMessage(Dlg, IDC_SEIS, BM_GETCHECK, 0, 0) == BST_CHECKED) {
		float seis = stof(nuevo->costo);
		float porciento;
		porciento = seis * 0.12;
		seis = seis + porciento;
		seis = seis / 6;
		sprintf_s(nuevo->monto, "%f", seis);
		strcat_s(nuevo->monto, " a 6 meses");
	}
	if (SendDlgItemMessage(Dlg, IDC_NUEVE, BM_GETCHECK, 0, 0) == BST_CHECKED) {
		float nueve = stof(nuevo->costo);
		float porciento;
		porciento = nueve * 0.12;
		nueve = nueve + porciento;
		nueve = nueve / 6;
		sprintf_s(nuevo->monto, "%f", nueve);
		strcat_s(nuevo->monto, " a 9 meses");
	}
	//validaciones
	fdato = false;
	if (año < añotemp) {
		fdato = true;
		MessageBox(0, "Fecha inválida", "Aviso", MB_ICONINFORMATION);
	}
	else if (año == añotemp) {
		if (mes < mestemp) {
			fdato = true;
			MessageBox(0, "Fecha inválida", "Aviso", MB_ICONINFORMATION);
		}
		else if (mes == mestemp) {
			if (dia < diatemp) {
				fdato = true;
				MessageBox(0, "Fecha inválida", "Aviso", MB_ICONINFORMATION);
			}
		}
	}
	nodo*aux = lista;
	while (aux != NULL) {
		if (strcmp(aux->fecha, nuevo->fecha) == 0) {
			if (strcmp(aux->hora, nuevo->hora) == 0) {
				fdato = true;
				MessageBox(0, "Fecha ocupada", "Aviso", MB_ICONINFORMATION);
			}
		}
		aux = aux->sig;
	}
	for (int i = 0; i < strlen(nuevo->nombre); i++)
	{
		if ((nuevo->nombre[i] < 'a' || nuevo->nombre[i] > 'z') && (nuevo->nombre[i] < 'A' || nuevo->nombre[i] > 'Z') &&
			(nuevo->nombre[i] != 'ñ'&&nuevo->nombre[i] != 'Ñ'&&nuevo->nombre[i] != 'á'&&nuevo->nombre[i] != 'é'&&nuevo->nombre[i] != 'í'&&nuevo->nombre[i] != 'ó'&&nuevo->nombre[i] != 'ú'
				&&nuevo->nombre[i] != 'Á'&&nuevo->nombre[i] != 'É'&&nuevo->nombre[i] != 'Í'&&nuevo->nombre[i] != 'Ó'&&nuevo->nombre[i] != 'Ú'&&nuevo->nombre[i] != 32)) {
			fdato = true;
			MessageBox(0, "El nombre solo debe contener caracteres", "Aviso", MB_ICONINFORMATION);
			break;
		}
	}
	for (int i = 0; i < strlen(nuevo->apellido); i++)
	{
		if ((nuevo->apellido[i] < 'a' || nuevo->apellido[i] > 'z') && (nuevo->apellido[i] < 'A' || nuevo->apellido[i] > 'Z') &&
			(nuevo->apellido[i] != 'ñ'&&nuevo->apellido[i] != 'Ñ'&&nuevo->apellido[i] != 'á'&&nuevo->apellido[i] != 'é'&&nuevo->apellido[i] != 'í'&&nuevo->apellido[i] != 'ó'&&nuevo->apellido[i] != 'ú'
				&&nuevo->apellido[i] != 'Á'&&nuevo->apellido[i] != 'É'&&nuevo->apellido[i] != 'Í'&&nuevo->apellido[i] != 'Ó'&&nuevo->apellido[i] != 'Ú'&&nuevo->apellido[i] != 32)) {
			fdato = true;
			MessageBox(0, "El apellido solo debe contener caracteres", "Aviso", MB_ICONINFORMATION);
			break;
		}
	}
	for (int i = 0; i < strlen(nuevo->mascota); i++)
	{
		if ((nuevo->mascota[i] < 'a' || nuevo->mascota[i] > 'z') && (nuevo->mascota[i] < 'A' || nuevo->mascota[i] > 'Z') &&
			(nuevo->mascota[i] != 'ñ'&&nuevo->mascota[i] != 'Ñ'&&nuevo->mascota[i] != 'á'&&nuevo->mascota[i] != 'é'&&nuevo->mascota[i] != 'í'&&nuevo->mascota[i] != 'ó'&&nuevo->mascota[i] != 'ú'
				&&nuevo->mascota[i] != 'Á'&&nuevo->mascota[i] != 'É'&&nuevo->mascota[i] != 'Í'&&nuevo->mascota[i] != 'Ó'&&nuevo->mascota[i] != 'Ú')) {
			fdato = true;
			MessageBox(0, "El nombre de la mascota solo debe contener caracteres", "Aviso", MB_ICONINFORMATION);
			break;
		}
	}
	for (int i = 0; i < strlen(nuevo->telefono); i++)
	{
		if ((strlen(nuevo->telefono) != 8) && (strlen(nuevo->telefono) != 10) && (strlen(nuevo->telefono) != 12)) {
			fdato = true;
			MessageBox(0, "El teléfono solo debe contener 8, 10 o 12 números", "Aviso", MB_ICONINFORMATION);
			break;
		}
	}
	for (int i = 0; i < strlen(nuevo->costo); i++) {
		if (nuevo->costo[i]!='0' && nuevo->costo[i] != '1' && nuevo->costo[i] != '2' && nuevo->costo[i] != '3' && nuevo->costo[i] != '4'  && nuevo->costo[i] != '5'
			&& nuevo->costo[i] != '6' && nuevo->costo[i] != '7' && nuevo->costo[i] != '8' && nuevo->costo[i] != '9'&& nuevo->costo[i] !='.') {
			fdato = true;
			MessageBox(0, "El costo solo debe de tener digitos", "Aviso", MB_ICONINFORMATION);
			break;
		}
	}
	char*p;
	p = nuevo->costo;
	p++;
	string str = nuevo->costo;
	int punto = str.find('.');
	if (punto > 0) {
		p = p + punto;
		string str2 = p;
		int punto2 = str2.find('.');
		if (punto2 > 0) {
			fdato = true;
			MessageBox(0, "El costo solo acepta un punto decimal", "Aviso", MB_ICONINFORMATION);
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
	archivo.open("datos.bin", ios::out | ios::binary);
	if (archivo.is_open()) {
		nodo*borrar = NULL;
		nodo *aux = lista;
		while (aux != NULL) {
			archivo.write((char*)aux, sizeof(nodo));
			borrar = aux;
			aux = (*aux).sig;
			delete borrar;
		}
		archivo.close();
		/*delete lista;*/
	}
	else {
		MessageBox(0, "No es posible abrir el archivor", "Aviso", MB_ICONINFORMATION);
		/*delete lista;*/
		eliminarnodos();
		PostQuitMessage(0);
	}
}
void leer(nodo *) {
	fstream archivo("datos.bin", ios::out | ios::app);
	archivo.close();

	archivo.open("datos.bin", ios::in | ios::binary);
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
		MessageBox(0, "No es posible abrir el archivo", "Aviso", MB_ICONINFORMATION);
		PostQuitMessage(0);
	}
}
void eliminarnodos() {
	if (lista == NULL) {
		delete lista;
	}
	else {
		nodo*aux = lista;
		while (aux->sig != NULL) {
			lista = aux->sig;
			delete aux;
			aux = lista;
		}
		delete lista;
		lista = aux = NULL;
	}
}
