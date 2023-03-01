#include <windows.h>
#include "resource.h"
#include <fstream>
#include <CommDlg.h>
using namespace std;

struct Usuario
{
	char clave[15];
	char contra[50];
	char apellidopa[10];
	char apellidoma[20];
	char nombres[20];
	char usuarios_direccion[100];
	Usuario* Usuario_sig;
	Usuario* Usuario_ant;
};
Usuario* Usuario_aux, * Usuario_inicio, * NUsuario, * miInfo = nullptr;

struct Cliente
{
	char apepa[50];
	char apema[50];
	char CURP[50];
	char RFC[50];
	char fechadenac[50];
	char calle[25];
	char ciudad[15];
	char estado[50];
	char estadocivil[50];
	char telefono[50];
	char docindet[300];
	char sexo[50];
	char perfilries[50];
	char cliente_direccion[100];
	Cliente* Cliente_sig;
	Cliente* Cliente_ant;

};
Cliente* Cliente_inicio, * NUcliente,* Cliente_aux = nullptr;


struct vacunas
{
	char tipovacuna[50];
	char marca[30];
	char clavevacuna[30];
	char Descripcion[30];
	char Precio[99];
	char Numdosis[99];
	char vacunas_direccion[100];
	vacunas* Promocion_sig;
	vacunas* Promocion_ant;
};
vacunas* Promocion_inicio, * Promocion_aux = nullptr;

char zFile[MAX_PATH];
char direccion[MAX_PATH] = { 0 };

HMENU hmenu;
HINSTANCE hintance;
HWND ghDlg = 0;

void Agregar_usuario(Usuario* nuevo_u);
void Modificar_Usuario(Usuario* newInfo, char nomUsuario[50]);
void Eliminar_Usuario(char nomUsuario[50]);
void Escribir_Binario_Usuario();
void Leer_Binario_Usuario();

void Agregar_Cliente(Cliente* nuevo);
void Escribir_Binario_Cliente();
void Leer_Binario_Cliente();
void Eliminar_Cliente(char nomCliente[50]);



void Agregar_Promocion(vacunas* nuevo);
void Escribir_Binario_Promocion();
void Leer_Binario_Promocion();
void Eliminar_Promocion(char nomPromocion[50]);



HINSTANCE hInstGlobal;
HWND hPrinGlob;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Registrar(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VentanaInicio(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK repesonas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK revacunas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK recarnet(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	Leer_Binario_Usuario();
	Leer_Binario_Cliente();
	//Leer_Binario_Consumo();
	//Leer_Binario_Promocion();
	hInstGlobal = hInst;
	HWND hVInicio = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, WindowProc);

	ShowWindow(hVInicio, cmdshow);

	MSG Mensaje;
	ZeroMemory(&Mensaje, sizeof(Mensaje));

	while (GetMessage(&Mensaje, 0, 0, 0)) {
		TranslateMessage(&Mensaje);
		DispatchMessage(&Mensaje);
	}
	Escribir_Binario_Usuario(); //escribe el binario al terminar el programa
	Escribir_Binario_Cliente();
	/*Escribir_Binario_Promocion();
	Escribir_Binario_Consumo();*/
	return Mensaje.wParam;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

		/*case WM_INITDIALOG:
			break;*/

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1000: { //VALIDACION PARA EL INICIO DE SESION
			char usuarios_nombre[15];
			char usuarios_contrasenia[10];
			char usuarios_comercio[15];
			GetDlgItemText(hwnd, 1, usuarios_nombre, sizeof(usuarios_nombre));
			GetDlgItemText(hwnd, 2, usuarios_contrasenia, sizeof(usuarios_contrasenia));
			
			Usuario_aux = Usuario_inicio;
			if (Usuario_inicio == nullptr) {
				MessageBox(hwnd, "No hay usuario registrados.", "AVISO", MB_OK | MB_ICONERROR);
			}
			else {
				while (Usuario_aux != nullptr && strcmp(Usuario_aux->clave, usuarios_nombre) != 0) {
					Usuario_aux = Usuario_aux->Usuario_sig;
				}
				if (Usuario_aux == nullptr) {
					MessageBox(hwnd, "Usuario no encontrado", "AVISO", MB_OK | MB_ICONERROR);
				}
				else {
					if (strcmp(Usuario_aux->contra, usuarios_contrasenia) == 0)
					{
								EndDialog(hwnd, 0);
								DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, VentanaInicio);

						
					}
					else {
						MessageBox(hwnd, "Contraseña incorrecta", "AVISO", MB_OK | MB_ICONERROR);
					}
				}
			}
		}break;

		case 1002: { //DE INICIAR SESION A REGISTRARSE
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, Registrar);

		}
				 break;


		case 1001: { //DE INICIAR SESION A SALIR
			/*int Opcion = MessageBox(0, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (Opcion) {
			case IDYES: {
				DestroyWindow(hwnd);
			}
					  break;
			}*/
			Escribir_Binario_Usuario();
			/*Escribir_Binario_Cliente();
			Escribir_Binario_Promocion();
			Escribir_Binario_Consumo();*/
			PostQuitMessage(0);
		}
				 break;
		}

		break;


	}

	return FALSE;
}


LRESULT CALLBACK Registrar(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	switch (msg) {
		{

	case WM_INITDIALOG:
	{
	}

		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case  1004: {//De Registro REGRESAR a Inicio Sesion
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG1), NULL, WindowProc);
		}
				  break;

		case 1003: { //Boton de Aceptar registro
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthClave = 0;
			int lengthContra = 0;
			int lengthapepa = 0;
			int lengthapema = 0;
			int lengthnombres = 0;
			Usuario* Nuevo_usuario = new Usuario;
			GetDlgItemText(hwnd, 3, Nuevo_usuario->clave, sizeof(Nuevo_usuario->clave));
			GetDlgItemText(hwnd, 4, Nuevo_usuario->contra, sizeof(Nuevo_usuario->contra));
			GetDlgItemText(hwnd, 5, Nuevo_usuario->apellidopa, sizeof(Nuevo_usuario->apellidopa));
			GetDlgItemText(hwnd, 5, Nuevo_usuario->apellidoma, sizeof(Nuevo_usuario->apellidoma));
			GetDlgItemText(hwnd, 5, Nuevo_usuario->nombres, sizeof(Nuevo_usuario->nombres));

			lengthClave = strlen(Nuevo_usuario->clave);
			lengthContra = strlen(Nuevo_usuario->contra);
			lengthapepa = strlen(Nuevo_usuario->apellidopa);
			lengthapema = strlen(Nuevo_usuario->apellidoma);
			lengthnombres = strlen(Nuevo_usuario->nombres);

			char usuarios_nombre_registro[15];
			GetDlgItemText(hwnd, 3, usuarios_nombre_registro, sizeof(usuarios_nombre_registro));
			Usuario_aux = Usuario_inicio;




			if (lengthClave <= 0 || lengthClave > 15)//valida nombre
			{
				MessageBox(hwnd, "Favor de ingresar de un maximo de 15 caracteres alfabeticos (Sin espacios).", "Clave", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				for (int i = 0; i < lengthClave; i++)
				{
					if (ispunct(Nuevo_usuario->clave[i]))
					{
						CCaracteres++;
					}
				
					if (isspace(Nuevo_usuario->clave[i]))
					{
						CEspacios++;
					}
				}
				if (CCaracteres != 0 || CEspacios != 0)
				{
					MessageBox(hwnd, "Favor de solo ingresar caracteres alfabeticos (Sin espacios).", "Clave", MB_OK | MB_ICONEXCLAMATION);
				}
				else
				{
					CLetras = 0;
					CNumeros = 0;
					CCaracteres = 0;
					CEspacios = 0;
					while (Usuario_aux != nullptr && strcmp(Usuario_aux->clave, usuarios_nombre_registro) != 0) {//validacion mismmo nombre
						Usuario_aux = Usuario_aux->Usuario_sig;
					}
					if (Usuario_aux != nullptr) {
						MessageBox(hwnd, "Clave existente, intente con otro nombre", "AVISO", MB_OK | MB_ICONERROR);
					}
					else {
						if (lengthContra < 3 || lengthContra>10)//valida contraseña
						{
							MessageBox(hwnd, "Favor de ingresar de 3 a 10 caracteres. Minimo 1 letra, 1 numero y 1 caracter especial (Sin espacios).", "CONTRASEÑA", MB_OK | MB_ICONEXCLAMATION);
						}
						else {
							for (int i = 0; i < lengthContra; i++)
							{
								if (isalpha(Nuevo_usuario->contra[i]))
								{
									CLetras++;
								}
								if (ispunct(Nuevo_usuario->contra[i]))
								{
									CCaracteres++;
								}
								if (isdigit(Nuevo_usuario->contra[i]))
								{
									CNumeros++;
								}
								if (isspace(Nuevo_usuario->contra[i]))
								{
									CEspacios++;
								}
							}
							if (CCaracteres == 0 || CNumeros == 0 || CLetras == 0 || CEspacios != 0)
							{
								MessageBox(hwnd, "Favor de ingresar minimo 1 letra, 1 numero (Sin espacios).", "CONTRASEÑA", MB_OK | MB_ICONEXCLAMATION);
							}
							else
							{
								CLetras = 0;
								CNumeros = 0;
								CCaracteres = 0;
								CEspacios = 0;
							}
							while (Usuario_aux != nullptr && strcmp(Usuario_aux->clave, usuarios_nombre_registro) != 0) {//validacion mismmo nombre
								Usuario_aux = Usuario_aux->Usuario_sig;
							}
							if (Usuario_aux != nullptr) {
								
							}
							else {
								if (lengthapepa < 3 || lengthapepa>10)//valida contraseña
								{
									MessageBox(hwnd, "Favor de ingresar de 3 a 10 caracteres. Minimo 1 letra, 1 numero y 1 caracter especial (Sin espacios).", "Apellido Paterno", MB_OK | MB_ICONEXCLAMATION);
								}
								else {
									for (int i = 0; i < lengthapepa; i++)
									{
										if (isalpha(Nuevo_usuario->apellidopa[i]))
										{
											CLetras++;
										}
										if (ispunct(Nuevo_usuario->apellidopa[i]))
										{
											CCaracteres++;
										}
										if (isdigit(Nuevo_usuario->apellidopa[i]))
										{
											CNumeros++;
										}
										if (isspace(Nuevo_usuario->apellidopa[i]))
										{
											CEspacios++;
										}
									}
									if (CCaracteres != 0 || CNumeros != 0 || CLetras == 0 || CEspacios != 0)
									{
										MessageBox(hwnd, "Favor de ingresar minimo 1 letra, 1 numero y 1 caracter especial(Sin espacios).", "Apellido Paterno", MB_OK | MB_ICONEXCLAMATION);
									}
									else
									{
										CLetras = 0;
										CNumeros = 0;
										CCaracteres = 0;
										CEspacios = 0;
									}
									if (lengthapema < 3 || lengthapema>10)//valida contraseña
									{
										MessageBox(hwnd, "Favor de ingresar de 3 a 10 caracteres. Minimo 1 letra, 1 numero y 1 caracter especial (Sin espacios).", "Apellido Materno", MB_OK | MB_ICONEXCLAMATION);
									}
									else {
										for (int i = 0; i < lengthapepa; i++)
										{
											if (isalpha(Nuevo_usuario->apellidoma[i]))
											{
												CLetras++;
											}
											if (ispunct(Nuevo_usuario->apellidoma[i]))
											{
												CCaracteres++;
											}
											if (isdigit(Nuevo_usuario->apellidoma[i]))
											{
												CNumeros++;
											}
											if (isspace(Nuevo_usuario->apellidoma[i]))
											{
												CEspacios++;
											}
										}
										if (CCaracteres != 0 || CNumeros != 0 || CLetras == 0 || CEspacios != 0)
										{
											MessageBox(hwnd, "Favor de ingresar minimo 1 letra, 1 numero y 1 caracter especial(Sin espacios).", "Apellido Materno", MB_OK | MB_ICONEXCLAMATION);
										}
										else
										{
											CLetras = 0;
											CNumeros = 0;
											CCaracteres = 0;
											CEspacios = 0;
										}
										if (lengthnombres < 3 || lengthnombres>10)//valida contraseña
										{
											MessageBox(hwnd, "Favor de ingresar de 3 a 10 caracteres. Minimo 1 letra (Sin espacios).", "Nombre", MB_OK | MB_ICONEXCLAMATION);
										}
										else {
											for (int i = 0; i < lengthnombres; i++)
											{
												if (isalpha(Nuevo_usuario->apellidoma[i]))
												{
													CLetras++;
												}
												if (ispunct(Nuevo_usuario->apellidoma[i]))
												{
													CCaracteres++;
												}
												if (isdigit(Nuevo_usuario->apellidoma[i]))
												{
													CNumeros++;
												}
												
											}
											if (CCaracteres != 0 || CNumeros != 0 || CLetras == 0 )
											{
												MessageBox(hwnd, "Favor de ingresar minimo 1 letra (Sin espacios).", "Apellido Materno", MB_OK | MB_ICONEXCLAMATION);
											}
											else {
												strcpy_s(Nuevo_usuario->usuarios_direccion, "");
												Agregar_usuario(Nuevo_usuario);
												delete Nuevo_usuario;
												Escribir_Binario_Usuario();

												SetDlgItemText(hwnd, 3, "");
												SetDlgItemText(hwnd, 4, "");
												SetDlgItemText(hwnd, 5, "");
												SetDlgItemText(hwnd, 6, "");
												SetDlgItemText(hwnd, 7, "");
											}
											
										}
									}
								}
							}
						}
					}

				}

			}

		}

		}
	}
return false;
}

LRESULT CALLBACK VentanaInicio(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

		case WM_INITDIALOG: 
		{
		}
	    break;
		case WM_COMMAND: 
			switch (LOWORD(wParam))
			{
				{
			case 9999:
			{
				EndDialog(hwnd, 0);
				DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG4), hwnd, repesonas);
			}
			break;
			case 9998:
			{
				EndDialog(hwnd, 0);
				DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG5), hwnd, revacunas);
			}
			break;
			case 9997:
			{
				EndDialog(hwnd, 0);
				DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG6), hwnd, recarnet);
			}
			break;
				}
		}break;
	}
	return FALSE;
}

LRESULT CALLBACK repesonas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		{

	case WM_INITDIALOG:
	{

		char TipodeUsuario[7][25] = { "Casado(a)", "Conviviente","Anulado(a)","Separado de unión legal","Viudo(a)","Soltero(a)" };
		for (int i = 0; i < 7; i++) {
			SendDlgItemMessage(hwnd, 15, CB_INSERTSTRING, (WPARAM)i, (LPARAM)TipodeUsuario[i]);

		}
		SendMessage(GetDlgItem(hwnd, 15), CB_SETCURSEL, (WPARAM)0, 0);

		char sexousuario[2][25] = { "Masculino", "Femenino" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, 17, CB_INSERTSTRING, (WPARAM)i, (LPARAM)sexousuario[i]);

		}
		SendMessage(GetDlgItem(hwnd, 17), CB_SETCURSEL, (WPARAM)0, 0);;
		SYSTEMTIME fechaHoy;
		ZeroMemory(&fechaHoy, sizeof(fechaHoy));
		GetLocalTime(&fechaHoy);

		char fechaCompleta[11] = { 0 };
		char charFecha[5] = { 0 };
		sprintf_s(charFecha, "%d", fechaHoy.wDay);
		strcat_s(fechaCompleta, charFecha);
		strcat_s(fechaCompleta, "/");
		sprintf_s(charFecha, "%d", fechaHoy.wMonth);
		strcat_s(fechaCompleta, charFecha);
		strcat_s(fechaCompleta, "/");
		sprintf_s(charFecha, "%d", fechaHoy.wYear);
		strcat_s(fechaCompleta, charFecha);
		SetDlgItemText(hwnd, 11, fechaCompleta);
	}

		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case  1005: {//De Registro REGRESAR a Inicio Sesion
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), NULL, VentanaInicio);
		}
				  break;

		case 20:
		{
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp; //1

				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE); //2
				SetDlgItemText(hwnd, 21, zFile);
				SendDlgItemMessage(hwnd, 22, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 
			}
			else {
				MessageBox(hwnd, "No seleccionó foto", "AVISO", MB_OK | MB_ICONERROR);
				strcpy_s(zFile, NUcliente->docindet);

			}
		}
		break;
		case 1006: { //Boton de Aceptar registro
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthapepa = 0;
			int lengthapema = 0;
			int lengthCURP = 0;
			int lengthRFC = 0;
			int lengthcalle = 0;
			int lengthciudad = 0;
			int lengthestado = 0;
			int lengthtelefono = 0; 
			int lengthperfilries = 0;
	
			char perfil[50];
			Cliente* Nuevo_usuario = new Cliente;
			GetDlgItemText(hwnd, 8 , Nuevo_usuario->apepa, sizeof(Nuevo_usuario->apepa));
			GetDlgItemText(hwnd, 9 , Nuevo_usuario->apema, sizeof(Nuevo_usuario->apema));
			GetDlgItemText(hwnd, 10, Nuevo_usuario->CURP, sizeof(Nuevo_usuario->CURP));
			GetDlgItemText(hwnd, 19, Nuevo_usuario->RFC, sizeof(Nuevo_usuario->RFC));
			GetDlgItemText(hwnd, 11, Nuevo_usuario->fechadenac, sizeof(Nuevo_usuario->fechadenac));
			GetDlgItemText(hwnd, 12, Nuevo_usuario->calle, sizeof(Nuevo_usuario->calle));
			GetDlgItemText(hwnd, 13, Nuevo_usuario->ciudad, sizeof(Nuevo_usuario->ciudad));
			GetDlgItemText(hwnd, 14, Nuevo_usuario->estado, sizeof(Nuevo_usuario->estado));
			GetDlgItemText(hwnd, 15, Nuevo_usuario->estadocivil, sizeof(Nuevo_usuario->estadocivil));
			GetDlgItemText(hwnd, 16, Nuevo_usuario->telefono, sizeof(Nuevo_usuario->telefono));
			GetDlgItemText(hwnd, 17, Nuevo_usuario->sexo, sizeof(Nuevo_usuario->sexo));
			GetDlgItemText(hwnd, 18, Nuevo_usuario->perfilries, sizeof(Nuevo_usuario->perfilries));
			GetDlgItemText(hwnd, 21, Nuevo_usuario->docindet, sizeof(Nuevo_usuario->docindet));
			lengthapepa = strlen(Nuevo_usuario->apepa);
			lengthapema = strlen(Nuevo_usuario->apema);
			lengthCURP = strlen(Nuevo_usuario->CURP);
			lengthRFC = strlen(Nuevo_usuario->RFC);
			lengthcalle = strlen(Nuevo_usuario->calle);
			lengthciudad = strlen(Nuevo_usuario->ciudad);
			lengthestado = strlen(Nuevo_usuario->estado);
			lengthtelefono= strlen(Nuevo_usuario->telefono);
			lengthperfilries= strlen(Nuevo_usuario->perfilries);
			char usuarios_nombre_registro[15];
		/*	GetDlgItemText(hwnd, 3, usuarios_nombre_registro, sizeof(usuarios_nombre_registro));
			Usuario_aux = Usuario_inicio;*/
			char comparar[15];
			GetDlgItemText(hwnd, 8, comparar, sizeof(comparar));
			Cliente_aux = Cliente_inicio;
			/*Cliente_inicio, * NUcliente, * Cliente_aux*/


			if (lengthapepa <= 0 || lengthapepa > 15)//valida nombre
			{
				MessageBox(hwnd, "Favor de ingresar de un maximo de 15 caracteres alfabeticos (Sin espacios).", "Apellido Páterno", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				for (int i = 0; i < lengthapepa; i++)
				{
					if (isalpha(Nuevo_usuario->apepa[i]))
					{
						CLetras++;
					}
					if (ispunct(Nuevo_usuario->apepa[i]))
					{
						CCaracteres++;
					}
					if (isdigit(Nuevo_usuario->apepa[i]))
					{
						CNumeros++;
					}

				}
				if (CCaracteres != 0 || CNumeros != 0 || CLetras == 0)
				{
					MessageBox(hwnd, "Favor de ingresar minimo 1 letra (Sin espacios).", "Apellido Paterno", MB_OK | MB_ICONEXCLAMATION);
				}
				else
				{
					CLetras = 0;
					CNumeros = 0;
					CCaracteres = 0;
					CEspacios = 0;
				}
				while (Cliente_aux != nullptr && strcmp(Cliente_aux->apepa, comparar) != 0) {//validacion mismmo nombre
					Cliente_aux = Cliente_aux->Cliente_sig;
				}
				if (Cliente_aux != nullptr) {
					MessageBox(hwnd, "Persona ya existente, intente otro nombre", "AVISO", MB_OK | MB_ICONERROR);
				}
				if (lengthapema <= 0 || lengthapema > 15)//valida nombre
				{
					MessageBox(hwnd, "Favor de ingresar de un maximo de 15 caracteres alfabeticos (Sin espacios).", "Apellido Máterno", MB_OK | MB_ICONEXCLAMATION);
				}
				else {
					for (int i = 0; i < lengthapema; i++)
					{
						if (isalpha(Nuevo_usuario->apema[i]))
						{
							CLetras++;
						}
						if (ispunct(Nuevo_usuario->apema[i]))
						{
							CCaracteres++;
						}
						if (isdigit(Nuevo_usuario->apema[i]))
						{
							CNumeros++;
						}

					}
					if (CCaracteres != 0 || CNumeros != 0 || CLetras == 0)
					{
						MessageBox(hwnd, "Favor de ingresar minimo 1 letra (Sin espacios).", "Apellido Materno", MB_OK | MB_ICONEXCLAMATION);
					}
					else
					{
						CLetras = 0;
						CNumeros = 0;
						CCaracteres = 0;
						CEspacios = 0;
					}
					if (lengthCURP <= 0 || lengthCURP > 22)//valida nombre
					{
						MessageBox(hwnd, "Favor de ingresar de un maximo de 15 caracteres alfabeticos (Sin espacios).", "CURP", MB_OK | MB_ICONEXCLAMATION);
					}
					else {
						for (int i = 0; i < lengthCURP; i++)
						{
							if (isalpha(Nuevo_usuario->CURP[i]))
							{
								CLetras++;
							}
							if (ispunct(Nuevo_usuario->CURP[i]))
							{
								CCaracteres++;
							}
							if (isdigit(Nuevo_usuario->CURP[i]))
							{
								CNumeros++;
							}
					
						}
						if (CCaracteres != 0 || CNumeros == 0 || CLetras == 0 || CNumeros>8 || CLetras >12)
						{
							MessageBox(hwnd, "Favor de ingresar minimo 1 letra (Sin espacios).", "CURP", MB_OK | MB_ICONEXCLAMATION);
						}
						else
						{
							CLetras = 0;
							CNumeros = 0;
							CCaracteres = 0;
							CEspacios = 0;
						}
								if (lengthcalle <= 0 || lengthcalle > 15)//valida nombre
								{
									MessageBox(hwnd, "Favor de ingresar la calle.", "Calle", MB_OK | MB_ICONEXCLAMATION);
								}
								else {
									for (int i = 0; i < lengthcalle; i++)
									{
										if (isalpha(Nuevo_usuario->calle[i]))
										{
											CLetras++;
										}
										if (ispunct(Nuevo_usuario->calle[i]))
										{
											CCaracteres++;
										}
										if (isdigit(Nuevo_usuario->calle[i]))
										{
											CNumeros++;
										}

									}
									if (CCaracteres != 0 || CNumeros == 0 || CLetras == 0 )
									{
										MessageBox(hwnd, "Favor de ingresar la calle correcta.", "calle", MB_OK | MB_ICONEXCLAMATION);
									}
									else
									{
										CLetras = 0;
										CNumeros = 0;
										CCaracteres = 0;
										CEspacios = 0;
									}
									if (lengthciudad <= 0 || lengthciudad > 30)//valida nombre
									{
										MessageBox(hwnd, "Favor de ingresar la ciudad.", "ciudad", MB_OK | MB_ICONEXCLAMATION);
									}
									else {
										for (int i = 0; i < lengthciudad; i++)
										{
											
											if (ispunct(Nuevo_usuario->ciudad[i]))
											{
												CCaracteres++;
											}
			

										}
										if (CCaracteres != 0 )
										{
											MessageBox(hwnd, "Favor de ingresar la ciudad correcta.", "ciudad", MB_OK | MB_ICONEXCLAMATION);
										}
										else
										{
											CLetras = 0;
											CNumeros = 0;
											CCaracteres = 0;
											CEspacios = 0;
										}
										if (lengthestado <= 0 || lengthestado > 22)//valida nombre
										{
											MessageBox(hwnd, "Favor de ingresar el estado.", "estado", MB_OK | MB_ICONEXCLAMATION);
										}
										else {
											for (int i = 0; i < lengthestado; i++)
											{
												if (isalpha(Nuevo_usuario->estado[i]))
												{
													CLetras++;
												}
												if (ispunct(Nuevo_usuario->estado[i]))
												{
													CCaracteres++;
												}
										

											}
											if (CCaracteres != 0 ||  CLetras == 0)
											{
												MessageBox(hwnd, "Favor de ingresar el estad correcto.", "estado", MB_OK | MB_ICONEXCLAMATION);
											}
											else
											{
												CLetras = 0;
												CNumeros = 0;
												CCaracteres = 0;
												CEspacios = 0;
											}

											if (lengthperfilries <= 0 || lengthperfilries > 15)//valida nombre
											{
												MessageBox(hwnd, "Favor de ingresar perfil de riesgo.", "perfil de riesgo", MB_OK | MB_ICONEXCLAMATION);
											}
											else {
												for (int i = 0; i < lengthperfilries; i++)
												{
													if (isalpha(Nuevo_usuario->perfilries[i]))
													{
														CLetras++;
													}
													if (ispunct(Nuevo_usuario->perfilries[i]))
													{
														CCaracteres++;
													}
													if (isdigit(Nuevo_usuario->perfilries[i]))
													{
														CNumeros++;
													}

												}
												if (CCaracteres != 0 || CNumeros != 0 || CLetras == 0)
												{
													MessageBox(hwnd, "Favor de ingresar perfil de riesgo correcto.", "perfil de riesgo", MB_OK | MB_ICONEXCLAMATION);
												}
												else
												{
													CLetras = 0;
													CNumeros = 0;
													CCaracteres = 0;
													CEspacios = 0;
												}
												if (lengthtelefono <= 0 || lengthtelefono > 22)//valida nombre
												{
													MessageBox(hwnd, "Favor de ingresar un telefono.", "telefono", MB_OK | MB_ICONEXCLAMATION);
												}
												else {
													for (int i = 0; i < lengthtelefono; i++)
													{
														if (isalpha(Nuevo_usuario->telefono[i]))
														{
															CLetras++;
														}
														
														if (isdigit(Nuevo_usuario->telefono[i]))
														{
															CNumeros++;
														}

													}
													if (CNumeros == 0 || CLetras != 0)
													{
														MessageBox(hwnd, "Favor de ingresar un telefono correcto.", "telefono", MB_OK | MB_ICONEXCLAMATION);
													}
													else
													{
														strcpy_s(Nuevo_usuario->cliente_direccion, "");
														Agregar_Cliente(Nuevo_usuario);
														delete NUcliente;
														Escribir_Binario_Cliente();

														SetDlgItemText(hwnd, 8, "");
														SetDlgItemText(hwnd, 9, "");
														SetDlgItemText(hwnd, 10, "");
														SetDlgItemText(hwnd, 12, "");
														SetDlgItemText(hwnd, 13, "");
														SetDlgItemText(hwnd, 14, "");
														SetDlgItemText(hwnd, 16, "");
														SetDlgItemText(hwnd, 18, "");
														SetDlgItemText(hwnd, 21, "");
														SetDlgItemText(hwnd, 19, "");
													}
												}
												
										
									}
								}
							}
						}
					}
				}
			}

		}
		}
	}
	return false;
}

LRESULT CALLBACK revacunas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
		switch (msg)
		{

			

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{

			case 1006:
			{//De Registro REGRESAR a Inicio Sesion
				EndDialog(hwnd, 0);
				DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), NULL, VentanaInicio);
			}
			break;
			case 1007:
			{
				int CLetras = 0;
				int CNumeros = 0;
				int CCaracteres = 0;
				int CEspacios = 0;
				int lengthtipo = 0;
				int lengthmarca = 0;
				int lengthclave = 0;
				int lengthdesc = 0;
				int lengthnum = 0;

				/*char tipovacuna[50];
				char marca[30];
				char clavevacuna[30];
				char Descripcion[30];
				char Precio[99];
				char Numdosis[99];*/

				char perfil[50];
				vacunas* Nuevo_usuario = new vacunas;
				GetDlgItemText(hwnd, 25, Nuevo_usuario->tipovacuna, sizeof(Nuevo_usuario->tipovacuna));
				GetDlgItemText(hwnd, 26, Nuevo_usuario->marca, sizeof(Nuevo_usuario->marca));
				GetDlgItemText(hwnd, 27, Nuevo_usuario->clavevacuna, sizeof(Nuevo_usuario->clavevacuna));
				GetDlgItemText(hwnd, 28, Nuevo_usuario->Descripcion, sizeof(Nuevo_usuario->Descripcion));
				GetDlgItemText(hwnd, 29, Nuevo_usuario->Precio, sizeof(Nuevo_usuario->Precio));
				GetDlgItemText(hwnd, 30, Nuevo_usuario->Numdosis, sizeof(Nuevo_usuario->Numdosis));
				lengthtipo = strlen(Nuevo_usuario->tipovacuna);
				lengthmarca = strlen(Nuevo_usuario->marca);
				lengthclave = strlen(Nuevo_usuario->clavevacuna);
				lengthdesc = strlen(Nuevo_usuario->Descripcion);
				lengthnum = strlen(Nuevo_usuario->Numdosis);


				char comparar[15];
				GetDlgItemText(hwnd, 25, comparar, sizeof(comparar));
				Promocion_aux = Promocion_inicio;
				/*Promocion_inicio, * Promocion_aux*/

				if (lengthtipo <= 0 || lengthtipo > 15)//valida nombre
				{
					MessageBox(hwnd, "Favor de ingresar de un maximo de 15 caracteres alfabeticos (Sin espacios).", "Tipo de vacuna", MB_OK | MB_ICONEXCLAMATION);
				}
				else {
					for (int i = 0; i < lengthtipo; i++)
					{
						if (isalpha(Nuevo_usuario->tipovacuna[i]))
						{
							CLetras++;
						}
						if (ispunct(Nuevo_usuario->tipovacuna[i]))
						{
							CCaracteres++;
						}
						if (isdigit(Nuevo_usuario->tipovacuna[i]))
						{
							CNumeros++;
						}

					}
					if (CCaracteres != 0 || CNumeros != 0 || CLetras == 0)
					{
						MessageBox(hwnd, "Favor de ingresar minimo 1 letra (Sin espacios).", "Tipo de vacuna", MB_OK | MB_ICONEXCLAMATION);
					}
					else
					{
						CLetras = 0;
						CNumeros = 0;
						CCaracteres = 0;
						CEspacios = 0;
					}
					if (lengthmarca <= 0 || lengthmarca > 15)//valida nombre
					{
						MessageBox(hwnd, "Favor de ingresar de un maximo de 15 caracteres alfabeticos (Sin espacios).", "Marca", MB_OK | MB_ICONEXCLAMATION);
					}
					else {
						for (int i = 0; i < lengthmarca; i++)
						{
							if (isalpha(Nuevo_usuario->marca[i]))
							{
								CLetras++;
							}
							if (ispunct(Nuevo_usuario->marca[i]))
							{
								CCaracteres++;
							}
							if (isdigit(Nuevo_usuario->marca[i]))
							{
								CNumeros++;
							}

						}
						if (CCaracteres != 0 || CNumeros != 0 || CLetras == 0)
						{
							MessageBox(hwnd, "Favor de ingresar minimo 1 letra (Sin espacios).", "Marca", MB_OK | MB_ICONEXCLAMATION);
						}
						else
						{
							CLetras = 0;
							CNumeros = 0;
							CCaracteres = 0;
							CEspacios = 0;
						}
						if (lengthclave <= 0 || lengthclave > 15)//valida nombre
						{
							MessageBox(hwnd, "Favor de ingresar de un maximo de 15 caracteres alfanumericos (Sin espacios).", "Clave", MB_OK | MB_ICONEXCLAMATION);
						}
						else {
							for (int i = 0; i < lengthclave; i++)
							{
							
								if (ispunct(Nuevo_usuario->clavevacuna[i]))
								{
									CCaracteres++;
								}
							

							}
							if (CCaracteres != 0)
							{
								MessageBox(hwnd, "Favor de ingresar minimo 1 letra (Sin espacios).", "Clave", MB_OK | MB_ICONEXCLAMATION);
							}
							else
							{
								CLetras = 0;
								CNumeros = 0;
								CCaracteres = 0;
								CEspacios = 0;
							}
							if (lengthdesc <= 0 || lengthdesc > 15)//valida nombre
							{
								MessageBox(hwnd, "Favor de ingresar de un maximo de 15 caracteres alfanumericos (Sin espacios).", "Descripcion", MB_OK | MB_ICONEXCLAMATION);
							}
							else {
								for (int i = 0; i < lengthdesc; i++)
								{
								
									if (ispunct(Nuevo_usuario->Descripcion[i]))
									{
										CCaracteres++;
									}
								

								}
								if (CCaracteres != 0 )
								{
									MessageBox(hwnd, "Favor de ingresar minimo 1 letra (Sin espacios).", "Descripcion", MB_OK | MB_ICONEXCLAMATION);
								}
								else
								{
									CLetras = 0;
									CNumeros = 0;
									CCaracteres = 0;
									CEspacios = 0;
								}
							}
							if (lengthnum <= 0 || lengthnum > 15)//valida nombre
							{
								MessageBox(hwnd, "Favor de ingresar de un maximo de 15 caracteres numericos (Sin espacios).", "Numero de dosis", MB_OK | MB_ICONEXCLAMATION);
							}
							else {
								for (int i = 0; i < lengthnum; i++)
								{
									if (isalpha(Nuevo_usuario->Numdosis[i]))
									{
										CLetras++;
									}
									if (ispunct(Nuevo_usuario->Numdosis[i]))
									{
										CCaracteres++;
									}
									if (isdigit(Nuevo_usuario->Numdosis[i]))
									{
										CNumeros++;
									}

								}
								if (CCaracteres != 0 || CNumeros == 0 || CLetras != 0)
								{
									MessageBox(hwnd, "Favor de ingresar minimo 1 letra (Sin espacios).", "Numero de dosis", MB_OK | MB_ICONEXCLAMATION);
								}
								else
								{
									strcpy_s(Nuevo_usuario->vacunas_direccion, "");
									Agregar_Promocion(Nuevo_usuario);
									delete NUcliente;
									Escribir_Binario_Cliente();

									SetDlgItemText(hwnd,25, "");
									SetDlgItemText(hwnd,26, "");
									SetDlgItemText(hwnd,27, "");
									SetDlgItemText(hwnd,28, "");
									SetDlgItemText(hwnd,29, "");
									SetDlgItemText(hwnd,30, "");
						
								}
							}
						}
					}
				}
			
			}
			break;
			}

			break;


		}

		return FALSE;
}

LRESULT CALLBACK recarnet(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return false;
}















void Agregar_usuario(Usuario* nuevo_u) {
	if (Usuario_inicio == nullptr) {
		Usuario_inicio = new Usuario;
		Usuario_aux = Usuario_inicio;

		Usuario_aux->Usuario_sig = nullptr;
		Usuario_aux->Usuario_ant = nullptr;

		strcpy_s(Usuario_aux->clave, nuevo_u->clave);
		strcpy_s(Usuario_aux->contra, nuevo_u->contra);
		strcpy_s(Usuario_aux->apellidopa, nuevo_u->apellidopa);
		strcpy_s(Usuario_aux->apellidoma, nuevo_u->apellidoma);
		strcpy_s(Usuario_aux->usuarios_direccion, nuevo_u->usuarios_direccion);
	
	}
	else {
		Usuario_aux = Usuario_inicio;

		while (Usuario_aux->Usuario_sig != nullptr)
		{
			Usuario_aux = Usuario_aux->Usuario_sig;
		}

		Usuario_aux->Usuario_sig = new Usuario;

		Usuario_aux->Usuario_sig->Usuario_sig = nullptr;
		Usuario_aux->Usuario_sig->Usuario_ant = Usuario_aux;

		Usuario_aux = Usuario_aux->Usuario_sig;

		strcpy_s(Usuario_aux->clave, nuevo_u->clave);
		strcpy_s(Usuario_aux->contra, nuevo_u->contra);
		strcpy_s(Usuario_aux->apellidopa, nuevo_u->apellidopa);
		strcpy_s(Usuario_aux->apellidoma, nuevo_u->apellidoma);
		strcpy_s(Usuario_aux->usuarios_direccion, nuevo_u->usuarios_direccion);

	}
	MessageBox(0, "Usuario registrado", "AVISO", MB_OK);
}

void Modificar_Usuario(Usuario* newInfo, char nomUsuario[50]) {

	Usuario_aux = Usuario_inicio;

	if (Usuario_aux == nullptr)
	{
		MessageBox(0, "No hay Usuario registrados", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		while (Usuario_aux != nullptr && strcmp(Usuario_aux->clave, nomUsuario) != 0) {

			Usuario_aux = Usuario_aux->Usuario_sig;
		}
		if (Usuario_aux == nullptr) {
			MessageBox(0, "Usuario no encontrado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {
			strcpy_s(Usuario_aux->clave, newInfo->clave);


			MessageBox(0, "Usuario modificado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
	}

}

void Eliminar_Usuario(char nomUsuario[50]) {
	Usuario_aux = Usuario_inicio;

	if (Usuario_aux == nullptr)
	{
		MessageBox(0, "No hay Usuario registrados", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		while (Usuario_aux != nullptr && strcmp(Usuario_aux->clave, nomUsuario) != 0) {

			Usuario_aux = Usuario_aux->Usuario_sig;
		}
		if (Usuario_aux == nullptr) {
			MessageBox(0, "Usuario no encontrado", "AVISO", MB_OK);
		}
		else if (Usuario_aux == Usuario_inicio) {	//El nodo es el primero 

			if (Usuario_aux->Usuario_sig == nullptr) {

				Usuario_inicio = nullptr;
				delete Usuario_aux;
				Usuario_aux = Usuario_inicio;
			}
			else {
				Usuario_inicio = Usuario_inicio->Usuario_sig;
				Usuario_aux->Usuario_sig->Usuario_ant = nullptr;
				delete Usuario_aux;
				Usuario_aux = Usuario_inicio;

			}

			MessageBox(0, "Usuario eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {

			if (Usuario_aux->Usuario_sig == nullptr) { //Si el nodo es el último
				Usuario_aux->Usuario_ant->Usuario_sig = nullptr;
				delete Usuario_aux;
				Usuario_aux = Usuario_inicio;
			}
			else {
				Usuario_aux->Usuario_sig->Usuario_ant = Usuario_aux->Usuario_ant;
				Usuario_aux->Usuario_ant->Usuario_sig = Usuario_aux->Usuario_sig;
				delete Usuario_aux;
				Usuario_aux = Usuario_inicio;
			}

			MessageBox(0, "Usuario eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);

		}
	}

}

void Escribir_Binario_Usuario()
{
	Usuario_aux = Usuario_inicio;

	ofstream escribirUsuario;
	escribirUsuario.open("C:\\Users\\Shado\\source\\repos\\avance\\Usuario.bin", ios::out | ios::binary | ios::trunc);

	if (escribirUsuario.is_open()) {
		while (Usuario_aux != nullptr)
		{
			escribirUsuario.write((char*)Usuario_aux, sizeof(Usuario));
			Usuario_aux = Usuario_aux->Usuario_sig;
		}

		escribirUsuario.close();
	}
	else
	{
		system("PAUSE");
	}
}

void Leer_Binario_Usuario()
{
	ifstream archivoUsuario;
	archivoUsuario.open("C:\\Users\\Shado\\source\\repos\\avance\\Usuario.bin", ios::in | ios::binary);


	if (archivoUsuario.is_open()) {

		Usuario* Usuario_leido = new Usuario;

		while (!archivoUsuario.read((char*)Usuario_leido, sizeof(Usuario)).eof()) {

			if (Usuario_inicio == nullptr) {
				Usuario_inicio = Usuario_leido;
				Usuario_inicio->Usuario_sig = nullptr;
				Usuario_inicio->Usuario_ant = nullptr;
				Usuario_aux = Usuario_inicio;
			}
			else {
				Usuario_aux->Usuario_sig = Usuario_leido;
				Usuario_aux->Usuario_sig->Usuario_ant = Usuario_aux;
				Usuario_aux = Usuario_aux->Usuario_sig;
				Usuario_aux->Usuario_sig = nullptr;
				//fin = aux;
			}

			Usuario_leido = new Usuario;
		}
		archivoUsuario.close();
		delete Usuario_leido;
	}
	else {
		system("PAUSE");
	}
}




void Agregar_Cliente(Cliente* nuevo) {


	if (Cliente_inicio == nullptr) {
		Cliente_inicio = new Cliente;
		Cliente_aux = Cliente_inicio;

		Cliente_aux->Cliente_sig = nullptr;
		Cliente_aux->Cliente_ant = nullptr;

		strcpy_s(Cliente_aux->apepa, nuevo->apepa);
		strcpy_s(Cliente_aux->apema, nuevo->apema);
		strcpy_s(Cliente_aux->CURP, nuevo->CURP);
		strcpy_s(Cliente_aux->RFC, nuevo->RFC);
		strcpy_s(Cliente_aux->fechadenac, nuevo->fechadenac);
		strcpy_s(Cliente_aux->calle, nuevo->calle);
		strcpy_s(Cliente_aux->estado, nuevo->estado);
		strcpy_s(Cliente_aux->estadocivil, nuevo->estadocivil);
		strcpy_s(Cliente_aux->telefono, nuevo->telefono);
		strcpy_s(Cliente_aux->sexo, nuevo->sexo);
		strcpy_s(Cliente_aux->perfilries, nuevo->perfilries);
		strcpy_s(Cliente_aux->cliente_direccion, nuevo->cliente_direccion);

	}
	else {
		Cliente_aux = Cliente_inicio;
		while (Cliente_aux->Cliente_sig != nullptr)
		{
			Cliente_aux = Cliente_aux->Cliente_sig;
		}

		Cliente_aux->Cliente_sig = new Cliente;

		Cliente_aux->Cliente_sig->Cliente_sig = nullptr;
		Cliente_aux->Cliente_sig->Cliente_ant = Cliente_aux;

		Cliente_aux = Cliente_aux->Cliente_sig;

	
		strcpy_s(Cliente_aux->apepa, nuevo->apepa);
		strcpy_s(Cliente_aux->apema, nuevo->apema);
		strcpy_s(Cliente_aux->CURP, nuevo->CURP);
		strcpy_s(Cliente_aux->RFC, nuevo->RFC);
		strcpy_s(Cliente_aux->fechadenac, nuevo->fechadenac);
		strcpy_s(Cliente_aux->calle, nuevo->calle);
		strcpy_s(Cliente_aux->estado, nuevo->estado);
		strcpy_s(Cliente_aux->estadocivil, nuevo->estadocivil); 
		strcpy_s(Cliente_aux->telefono, nuevo->telefono);
		strcpy_s(Cliente_aux->sexo, nuevo->sexo);
		strcpy_s(Cliente_aux->perfilries, nuevo->perfilries);
		strcpy_s(Cliente_aux->cliente_direccion, nuevo->cliente_direccion);

		Cliente_aux->Cliente_sig = nullptr;
	}
	MessageBox(0, "Cliente registrado", "AVISO", MB_OK);
}
void Escribir_Binario_Cliente()
{
	Cliente_aux = Cliente_inicio;

	ofstream escribirCliente;
	escribirCliente.open("C:\\Users\\Shado\\source\\repos\\avance\\Cliente.bin", ios::out | ios::binary | ios::trunc);
	if (escribirCliente.is_open()) {
		while (Cliente_aux != nullptr)
		{
			escribirCliente.write((char*)Cliente_aux, sizeof(Cliente));
			Cliente_aux = Cliente_aux->Cliente_sig;
		}

		escribirCliente.close();
	}
	else
	{
		system("PAUSE");
	}
}
void Leer_Binario_Cliente()
{
	ifstream archivoCliente;
	archivoCliente.open("C:\\Users\\Shado\\source\\repos\\avance\\Cliente.bin", ios::in | ios::binary);

	if (archivoCliente.is_open()) {

		Cliente* Cliente_leido = new Cliente;

		while (!archivoCliente.read((char*)Cliente_leido, sizeof(Cliente)).eof()) {

			if (Cliente_inicio == nullptr) {
				Cliente_inicio = Cliente_leido;
				Cliente_inicio->Cliente_sig = nullptr;
				Cliente_inicio->Cliente_ant = nullptr;
				Cliente_aux = Cliente_inicio;
			}
			else {
				Cliente_aux->Cliente_sig = Cliente_leido;
				Cliente_aux->Cliente_sig->Cliente_ant = Cliente_aux;
				Cliente_aux = Cliente_aux->Cliente_sig;
				Cliente_aux->Cliente_sig = nullptr;
				//fin = aux;
			}

			Cliente_leido = new Cliente;
		}
		archivoCliente.close();
		delete Cliente_leido;
	}
	else {
		system("PAUSE");
	}
}
void Eliminar_Cliente(char nomCliente[50]) {
	Cliente_aux = Cliente_inicio;

	if (Cliente_aux == nullptr)
	{
		MessageBox(0, "No hay clientes registrados", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		while (Cliente_aux != nullptr && strcmp(Cliente_aux->apepa, nomCliente) != 0) {

			Cliente_aux = Cliente_aux->Cliente_sig;
		}
		if (Cliente_aux == nullptr) {
			MessageBox(0, "Cliente no encontrado", "AVISO", MB_OK);
		}
		else if (Cliente_aux == Cliente_inicio) {	//El nodo es el primero 

			if (Cliente_aux->Cliente_sig == nullptr) {

				Cliente_inicio = nullptr;
				delete Cliente_aux;
				Cliente_aux = Cliente_inicio;
			}
			else {
				Cliente_inicio = Cliente_inicio->Cliente_sig;
				Cliente_aux->Cliente_sig->Cliente_ant = nullptr;
				delete Cliente_aux;
				Cliente_aux = Cliente_inicio;

			}

			MessageBox(0, "Cliente eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {

			if (Cliente_aux->Cliente_sig == nullptr) { //Si el nodo es el último
				Cliente_aux->Cliente_ant->Cliente_sig = nullptr;
				delete Cliente_aux;
				Cliente_aux = Cliente_inicio;
			}
			else {
				Cliente_aux->Cliente_sig->Cliente_ant = Cliente_aux->Cliente_ant;
				Cliente_aux->Cliente_ant->Cliente_sig = Cliente_aux->Cliente_sig;
				delete Cliente_aux;
				Cliente_aux = Cliente_inicio;
			}

			MessageBox(0, "Cliente eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);

		}
	}

}




void Agregar_Promocion(vacunas* nuevo) {


	if (Promocion_inicio == nullptr) {
		Promocion_inicio = new vacunas;
		Promocion_aux = Promocion_inicio;

		Promocion_aux->Promocion_sig = nullptr;
		Promocion_aux->Promocion_ant = nullptr;
		/*char tipovacuna[50];
		char marca[30];
		char clavevacuna[30];
		char Descripcion[30];
		int Precio[99];
		int Numdosis[99];*/
		strcpy_s(Promocion_aux->tipovacuna, nuevo->tipovacuna);
		strcpy_s(Promocion_aux->marca, nuevo->marca);
		strcpy_s(Promocion_aux->clavevacuna, nuevo->clavevacuna);
		strcpy_s(Promocion_aux->Descripcion, nuevo->Descripcion);
		strcpy_s(Promocion_aux->Precio, nuevo->Precio);
		strcpy_s(Promocion_aux->Numdosis, nuevo->Numdosis);
		/*strcpy_s(Promocion_aux->Promestatus, nuevo->Promestatus);*/

	/*	Promocion_aux->Prommonto = nuevo->Prommonto;
		Promocion_aux->Promdescuento = nuevo->Promdescuento;*/

	}
	else {
		Promocion_aux = Promocion_inicio;
		while (Promocion_aux->Promocion_sig != nullptr)
		{
			Promocion_aux = Promocion_aux->Promocion_sig;
		}

		Promocion_aux->Promocion_sig = new vacunas;

		Promocion_aux->Promocion_sig->Promocion_sig = nullptr;
		Promocion_aux->Promocion_sig->Promocion_ant = Promocion_aux;

		Promocion_aux = Promocion_aux->Promocion_sig;

		strcpy_s(Promocion_aux->tipovacuna, nuevo->tipovacuna);
		strcpy_s(Promocion_aux->marca, nuevo->marca);
		strcpy_s(Promocion_aux->clavevacuna, nuevo->clavevacuna);
		strcpy_s(Promocion_aux->Descripcion, nuevo->Descripcion);
		strcpy_s(Promocion_aux->Precio, nuevo->Precio);
		strcpy_s(Promocion_aux->Numdosis, nuevo->Numdosis);
		/*strcpy_s(Promocion_aux->Promestatus, nuevo->Promestatus);*/

		/*Promocion_aux->Prommonto = nuevo->Prommonto;
		Promocion_aux->Promdescuento = nuevo->Promdescuento;*/
		Promocion_aux->Promocion_sig = nullptr;
	}
	MessageBox(0, "Promocion registrada", "AVISO", MB_OK);
}
void Escribir_Binario_Promocion()
{
	Promocion_aux = Promocion_inicio;

	ofstream escribirPromocion;
	escribirPromocion.open("C:\\Users\\Shado\\source\\repos\\avance\\vacunas.bin", ios::out | ios::binary | ios::trunc);

	if (escribirPromocion.is_open()) {
		while (Promocion_aux != nullptr)
		{
			escribirPromocion.write((char*)Promocion_aux, sizeof(vacunas));
			Promocion_aux = Promocion_aux->Promocion_sig;
		}

		escribirPromocion.close();
	}
	else
	{
		system("PAUSE");
	}
}
void Leer_Binario_Promocion()
{
	ifstream archivoPromocion;
	archivoPromocion.open("C:\\Users\\Shado\\source\\repos\\avance\\vacunas.bin", ios::in | ios::binary);


	if (archivoPromocion.is_open()) {

		vacunas* Promocion_leido = new vacunas;

		while (!archivoPromocion.read((char*)Promocion_leido, sizeof(vacunas)).eof()) {

			if (Promocion_inicio == nullptr) {
				Promocion_inicio = Promocion_leido;
				Promocion_inicio->Promocion_sig = nullptr;
				Promocion_inicio->Promocion_ant = nullptr;
				Promocion_aux = Promocion_inicio;
			}
			else {
				Promocion_aux->Promocion_sig = Promocion_leido;
				Promocion_aux->Promocion_sig->Promocion_ant = Promocion_aux;
				Promocion_aux = Promocion_aux->Promocion_sig;
				Promocion_aux->Promocion_sig = nullptr;
				//fin = aux;
			}

			Promocion_leido = new vacunas;
		}
		archivoPromocion.close();
		delete Promocion_leido;
	}
	else {
		system("PAUSE");
	}
}
void Eliminar_Promocion(char nomPromocion[50]) {
	Promocion_aux = Promocion_inicio;

	if (Promocion_aux == nullptr)
	{
		MessageBox(0, "No hay promociones registradas", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		while (Promocion_aux != nullptr && strcmp(Promocion_aux->tipovacuna, nomPromocion) != 0) {

			Promocion_aux = Promocion_aux->Promocion_sig;
		}
		if (Promocion_aux == nullptr) {
			MessageBox(0, "Promocion no encontrada", "AVISO", MB_OK);
		}
		else if (Promocion_aux == Promocion_inicio) {	//El nodo es el primero 

			if (Promocion_aux->Promocion_sig == nullptr) {

				Promocion_inicio = nullptr;
				delete Promocion_aux;
				Promocion_aux = Promocion_inicio;
			}
			else {
				Promocion_inicio = Promocion_inicio->Promocion_sig;
				Promocion_aux->Promocion_sig->Promocion_ant = nullptr;
				delete Promocion_aux;
				Promocion_aux = Promocion_inicio;

			}

			MessageBox(0, "Promocion eliminada", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {

			if (Promocion_aux->Promocion_sig == nullptr) { //Si el nodo es el último
				Promocion_aux->Promocion_ant->Promocion_sig = nullptr;
				delete Promocion_aux;
				Promocion_aux = Promocion_inicio;
			}
			else {
				Promocion_aux->Promocion_sig->Promocion_ant = Promocion_aux->Promocion_ant;
				Promocion_aux->Promocion_ant->Promocion_sig = Promocion_aux->Promocion_sig;
				delete Promocion_aux;
				Promocion_aux = Promocion_inicio;
			}

			MessageBox(0, "Promoción eliminada", "AVISO", MB_OK | MB_ICONINFORMATION);

		}
	}

}

