
// DOMINÓ (VERSIÓN 3 + PARTE OPCIONAL 1)

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

// CONSTANTES

const bool Oculto = false;
const bool TodosMaquina = false;
const short int MinJugadores = 2, MaxJugadores = 4;
const short int MinDigito = 6, MaxDigito = 9;
const short int NumFichas = ((MaxDigito + 1) * (MaxDigito + 2)) / 2;  // Todas las combinaciones posibles de fichas desde 0 a MaxDigito
const short int FichasInicio = 7;

// DEFINICIÓN DE TIPOS DE ARRAY(S) Y STRUCT(S)

typedef struct {
	short int izquierda;
	short int derecha;
} tFicha;

typedef tFicha tArrayFichas[NumFichas];

typedef struct {
	short int cont = 0;
	tArrayFichas arrayFichas;
} tListaFichas;

typedef tListaFichas tJugadores[MaxJugadores];

typedef int tPuntosJugadores[MaxJugadores];

typedef struct {
	int numJugador;
	int maxValor;
	string tablero;
	tListaFichas pozo;
	tJugadores arrayJugadores;
	tPuntosJugadores arrayPuntosJug;
} tJuego;

// PROTOTIPOS FUNCIONES

void cargarPartida(bool& quiereCargar);
// Pregunta al usuario si quiere cargar una partida, finalizando cuando la opción introducida sea válida
bool leerJuego(tJuego& juego);
// Lee la partida del archivo de entrada con todos sus datos y devuelve true si ha podido leerlo
void leerListaFichas(ifstream& entrada, tListaFichas& listaFichas);
// Lee las fichas del siguiente reglón del archivo de entrada
void errorCargar(bool& quiereCargar);
// Notifica el error al cargar el archivo y vuelve a preguntar invocando a cargarPartida
void comenzarPartida(tJuego& juego, short int& jugador, int& indice);
// Obtiene todos los datos necesarios para empezar una nueva partida, informando de quién empieza
void cuantosJugadores(tJuego& juego);
// Pregunta al usuario cuántos jugadores quiere que jueguen, finalizando cuando la opción introducida sea válida
void hastaQueValor(tJuego& juego);
// Pregunta al usuario hasta qué valor quiere que lleguen las fichas, finalizando cuando la opción introducida sea válida
void iniciar(tJuego& juego, short int& jugador);
// Genera todas las fichas del juego y las reparte hasta que algún jugador obtenga una ficha doble
void generaPozo(tJuego& juego);
// Genera todas las fichas del pozo dependiendo del MaxValor decidido por el usuario
void desordenaPozo(tJuego& juego);
// Desordena aleatoriamente las fichas del pozo
void robaFicha(tJuego& juego, const short int jugador);
// Roba una ficha del pozo y la añade a las fichas del jugador indicado
short int quienEmpieza(const tJuego& juego, int& indice);
/* Comprueba quién tiene la ficha doble de mayor valor, devolviendo el jugador que la tiene, la posición de
dicha ficha, y devuelve -1 si nadie tiene una ficha doble*/
string fichaToStr(const tJuego& juego, const short int jugador, const int ficha);
// Genera y devuelve la ficha (de forma |izquierda-derecha|) de la ficha del jugador indicado
void mostrarTablero(const tJuego& juego);
// Muestra en pantalla el tablero y las fichas de cada jugador en cada momento
void mostrarFichasJug(const tJuego& juego, const short int jug);
// Muestra en pantalla las fichas del jugador indicado
void jugHumano(tJuego& juego, int& opcion, const short int jugador, bool& win, int& ganador);
/* Invoca a mostrarMenu, y realiza las instrucciones correspondientes a la opción elegida, desde
robar una ficha, ponerla por la izquierda o por la derecha, etc*/
void mostrarMenu(int& opcion);
// Muestra el menú de opciones al jugador, finalizando cuando la opción introducida sea válida
bool opcion1(tJuego& juego, const short int jugador, short int& ficha, int& opcion, bool& win);
/* Es invocada cuando el jugador quiere poner la ficha por la izquierda, y realiza todos los pasos
necesarios para ello*/
bool opcion2(tJuego& juego, const short int jugador, short int& ficha, int& opcion, bool& win);
/* Es invocada cuando el jugador quiere poner la ficha por la derecha, y realiza todos los pasos
necesarios para ello*/
void opcion3(tJuego& juego, short int jugador, int& opcion, bool& hayMas);
// Es invocada cuando el jugador quiere robar una ficha, y realiza todos los pasos necesarios para ello
void seleccionaFicha(const tJuego& juego, const short int jugador, short int& ficha);
// Pregunta al usuario qué ficha quiere colocar, finalizando cuando la opción introducida sea válida
bool puedePonerIzq(const tJuego& juego, const short int jugador, const short int ficha);
// Devuelve true si la ficha del jugador indicado puede ponerse en el tablero por la izquierda
void ponerFichaIzq(tJuego& juego, const short int jugador, const short int ficha);
// Coloca la ficha del jugador indicado en el tablero por la izquierda
bool puedePonerDcha(const tJuego& juego, const short int jugador, const short int ficha);
// Devuelve true si la ficha del jugador indicado puede ponerse en el tablero por la derecha
void ponerFichaDcha(tJuego& juego, const short int jugador, const short int ficha);
// Coloca la ficha del jugador indicado en el tablero por la derecha
void eliminaFicha(tJuego& juego, const short int jugador, const short int ficha);
// Elimina la ficha que ha colocado el jugador indicado de sus fichas
bool puedeColocarFicha(const tJuego& juego, const short int jugador);
// Devuelve true si el jugador indicado puede poner alguna de sus fichas
void jugMaquina(tJuego& juego, const short int jugador, bool& win, int& ganador);
// Realiza la estrategia 1 o 2 dependiendo del jugador indicado
void noEstrategia(tJuego& juego, const short int jugador, bool& sinFichas);
/* En el caso de que no se haya podido llevar a cabo la estrategia correspondienter roba ficha si se puede
y lo muestra en pantalla */
bool estrategia1(tJuego& juego, const short int jugador);
/* Recorre las fichas del jugador indicado y añade al tablero la primera que pueda. Si no puede poner
ninguna devolverá false */
bool estrategia2(tJuego& juego, const short int jugador);
/* Busca la ficha que pueda ponerse con mayor puntuación (suma de sus dos valores) y la añade al tablero.
Si no puede poner ninguna devolverá false */
bool sinSalida(const tJuego& juego);
// Devuelve true si no quedan fichas para robar y ningún jugador puede poner
void terminarPartida(tJuego& juego, const int ganador, bool& volverAJugar, short int& jugador, int& indice, bool& win, bool& sinOpciones);
/* Realiza todas las instrucciones cuando alguien ha ganado, desde mostrar el ganador, los puntos,
invocar a quiereVolverAJugar y comenzar una nueva partida si es así*/
void haGanado(const int ganador);
// Escribe en pantalla el ganador de la partida
void puntosAcumulados(tJuego& juego);
// Muestra en pantalla los puntos de cada jugador, invocando previamente a sumaPuntos
void sumaPuntos(tJuego& juego, const short int jugador);
// Suma los puntos de cada jugador
void quiereVolverAJugar(bool& volverAJugar);
// Pregunta al usuario si quiere volver a jugar una partida, finalizando cuando la opción introducida sea válida
void quiereGuardar(bool& guardarPartida);
// Pregunta al usuario si quiere guardar la partida, finalizando cuando la opción introducida sea válida
void escribirJuego(const tJuego& juego);
// Escribe todos los datos de la partida en el archivo de salida
void escribirListaFichas(ofstream& salida, const tListaFichas& listaFichas);
// Escribe las fichas en el siguiente renglón del archivo de salida

// EMPIEZA EL MAIN

int main()
{
	system("chcp 1252");
	system("cls");
	srand(time(NULL));

	// Declaración(/inicialización) de variables

	bool quiereCargar, win = false, sinOpciones = false, volverAJugar = true, guardarPartida;
	tJuego juego;
	int indice, opcion = -1, ganador;
	short int jugador;

	// Inicialización de la partida

	cargarPartida(quiereCargar);
	while (quiereCargar && !leerJuego(juego))
	{
		errorCargar(quiereCargar);
	}
	if (!quiereCargar)
	{
		cuantosJugadores(juego);
		hastaQueValor(juego);
		for (int i = 0; i < juego.numJugador; i++)
		{
			juego.arrayPuntosJug[i] = 0;
		}
		comenzarPartida(juego, jugador, indice);
	}
	else  // Si se lee un archivo le toca siempre al jugador 0
	{
		jugador = 0;
	}

	// Empieza el juego, por turnos

	while (opcion != 0 && volverAJugar)
	{
		system("cls");
		mostrarTablero(juego);
		while (opcion != 0 && !win && !sinOpciones)
		{
			while (jugador < juego.numJugador && opcion != 0 && !win && !sinOpciones)
			{
				if (jugador == 0 && !TodosMaquina)
				{
					jugHumano(juego, opcion, jugador, win, ganador);
				}
				else
				{
					jugMaquina(juego, jugador, win, ganador);
				}
				system("cls");
				mostrarTablero(juego);
				sinOpciones = sinSalida(juego);
				jugador++;
			}
			jugador = 0;
		}
		if (win || sinOpciones)
		{
			terminarPartida(juego, ganador, volverAJugar, jugador, indice, win, sinOpciones);
		}
	}
	if (opcion == 0)
	{
		quiereGuardar(guardarPartida);
		if (guardarPartida)
		{
			escribirJuego(juego);
		}
	}
	return 0;
}

// TERMINA EL MAIN

void cargarPartida(bool& quiereCargar)
{
	char opcion;
	cout << "¿Quieres cargar una sesión de juego guardada[S/N]? ";
	cin >> opcion;
	while (opcion != 'S' && opcion != 's' && opcion != 'N' && opcion != 'n')
	{
		cout << "Opción no válida." << endl;
		cout << "¿Quieres cargar una sesión de juego guardada[S/N]? ";
		cin >> opcion;
	}
	if (opcion == 'S' || opcion == 's')
	{
		quiereCargar = true;
	}
	else
	{
		quiereCargar = false;
	}
}

bool leerJuego(tJuego& juego)
{
	ifstream entrada;
	string nombreficheroIn;
	tListaFichas listaFichas;
	bool lectura = false;

	cout << "Introduzca el nombre del fichero para cargar la partida guardada: ";
	cin >> nombreficheroIn;

	entrada.open(nombreficheroIn);
	if (entrada.is_open())
	{
		entrada >> juego.numJugador;
		entrada >> juego.maxValor;
		entrada >> juego.tablero;
		entrada >> juego.pozo.cont;
		leerListaFichas(entrada, juego.pozo);
		for (int j = 0; j < juego.numJugador; j++)
		{
			entrada >> juego.arrayJugadores[j].cont;
			leerListaFichas(entrada, juego.arrayJugadores[j]);
			entrada >> juego.arrayPuntosJug[j];
		}
		lectura = true;
		if (juego.numJugador < MinJugadores || juego.numJugador > MaxJugadores || juego.maxValor < MinDigito || juego.maxValor > MaxDigito)
		{
			lectura = false;
		}
	}
	entrada.close();
	return lectura;
}

void leerListaFichas(ifstream& entrada, tListaFichas& listaFichas)
{
	for (short int i = 0; i < listaFichas.cont; i++)
	{
		entrada >> listaFichas.arrayFichas[i].izquierda;
		entrada >> listaFichas.arrayFichas[i].derecha;
	}
}

void errorCargar(bool& quiereCargar)
{
	cout << "Se ha producido un error al leer el archivo." << endl;
	system("pause");
	system("cls");
	cargarPartida(quiereCargar);
}

void comenzarPartida(tJuego& juego, short int& jugador, int& indice)
{
	iniciar(juego, jugador);
	if (quienEmpieza(juego, indice) == 0 && !TodosMaquina)
	{
		cout << endl << "Empiezas tú con " << fichaToStr(juego, jugador, indice) << endl;
	}
	else
	{
		cout << endl << "La máquina #" << jugador << " empieza con " << fichaToStr(juego, jugador, indice) << endl;
	}
	cout << "¡Comienza la ronda!" << endl;
	system("pause");
	system("cls");
	eliminaFicha(juego, jugador, indice);
	if (jugador == juego.numJugador - 1)
	{
		jugador = 0;
	}
	else
	{
		jugador++;
	}
}

void cuantosJugadores(tJuego& juego)
{
	int numeroJugadores;
	cout << "Número de jugadores[" << MinJugadores << "-" << MaxJugadores << "]: ";
	cin >> numeroJugadores;
	while (numeroJugadores < MinJugadores || numeroJugadores > MaxJugadores)
	{
		cout << "Número no válido." << endl;
		cout << "Número de jugadores[" << MinJugadores << "-" << MaxJugadores << "]: ";
		cin >> numeroJugadores;
	}
	juego.numJugador = numeroJugadores;
}

void hastaQueValor(tJuego& juego)
{
	int maximoValor;
	cout << "Máximo valor de las fichas [" << MinDigito << "-" << MaxDigito << "]: ";
	cin >> maximoValor;;
	while (maximoValor < MinDigito || maximoValor > MaxDigito)
	{
		cout << "Número no válido." << endl;
		cout << "Máxima puntuación por ficha[" << MinDigito << "-" << MaxDigito << "]: ";
		cin >> maximoValor;
	}
	juego.maxValor = maximoValor;
}

void iniciar(tJuego& juego, short int& jugador)
{
	int indice;

	jugador = -1;
	while (jugador == -1)
	{
		generaPozo(juego);
		desordenaPozo(juego);
		for (int j = 0; j < juego.numJugador; j++)
		{
			for (int i = 0; i < FichasInicio; i++)
			{
				robaFicha(juego, j);
			}
		}
		jugador = quienEmpieza(juego, indice);
		if (jugador == -1)
		{
			cout << "Nadie tiene una ficha repetida. Hay que volver a repartir..." << endl;
			juego.pozo.cont = 0;
			for (int k = 0; k < juego.numJugador; k++)
			{
				juego.arrayJugadores[k].cont = 0;
			}
		}
	}
	juego.tablero = fichaToStr(juego, jugador, indice);
}

void generaPozo(tJuego& juego)
{
	short int posicion = 0, valorIzq = 0, valorInicial = 0;
	while (posicion < ((juego.maxValor + 1) * (juego.maxValor + 2)) / 2)
	{
		for (short int contador = valorInicial; contador <= juego.maxValor; contador++)
		{
			juego.pozo.arrayFichas[posicion].izquierda = valorIzq;
			juego.pozo.arrayFichas[posicion].derecha = contador;
			posicion++;
			juego.pozo.cont++;
		}
		valorIzq++;
		valorInicial++;
	}
}

void desordenaPozo(tJuego& juego)
{
	int idx, i;
	short int tmp1, tmp2;

	for (int i = (((juego.maxValor + 1)* (juego.maxValor + 2)) / 2) - 1; i >= 0; i--)
	{
		idx = rand() % (i + 1);
		if (i != idx)
		{
			tmp1 = juego.pozo.arrayFichas[i].izquierda;
			tmp2 = juego.pozo.arrayFichas[i].derecha;
			juego.pozo.arrayFichas[i].izquierda = juego.pozo.arrayFichas[idx].izquierda;
			juego.pozo.arrayFichas[i].derecha = juego.pozo.arrayFichas[idx].derecha;
			juego.pozo.arrayFichas[idx].izquierda = tmp1;
			juego.pozo.arrayFichas[idx].derecha = tmp2;
		}
	}
}

void robaFicha(tJuego& juego, const short int jugador)
{
	short int pozoCont = juego.pozo.cont, fichasCont = juego.arrayJugadores[jugador].cont;
	juego.arrayJugadores[jugador].arrayFichas[fichasCont].izquierda = juego.pozo.arrayFichas[pozoCont - 1].izquierda;
	juego.arrayJugadores[jugador].arrayFichas[fichasCont].derecha = juego.pozo.arrayFichas[pozoCont - 1].derecha;
	juego.pozo.cont--;
	juego.arrayJugadores[jugador].cont++;
}

short int quienEmpieza(const tJuego& juego, int& indice)
{
	short int jugador = 0, mayor = -1;
	for (short int i = 0; i < juego.numJugador; i++)
	{
		for (int j = 0; j < juego.arrayJugadores[i].cont; j++)
		{
			if ((juego.arrayJugadores[i].arrayFichas[j].izquierda == juego.arrayJugadores[i].arrayFichas[j].derecha) && (juego.arrayJugadores[i].arrayFichas[j].izquierda > mayor))
			{
				mayor = juego.arrayJugadores[i].arrayFichas[j].izquierda;
				jugador = i;
				indice = j;
			}
		}
	}
	if (mayor == -1)
	{
		jugador = -1;
	}
	return jugador;
}

string fichaToStr(const tJuego& juego, const short int jugador, const int ficha)
{
	string tablero, fichaIzq, fichaDer;
	short int izquierda = juego.arrayJugadores[jugador].arrayFichas[ficha].izquierda;
	short int derecha = juego.arrayJugadores[jugador].arrayFichas[ficha].derecha;

	fichaIzq = to_string(izquierda);
	fichaDer = to_string(derecha);
	tablero = "|" + fichaIzq + "-" + fichaDer + "|";

	return tablero;
}

void mostrarTablero(const tJuego& juego)
{
	int jug = 0;
	cout << "-------------------" << endl;
	cout << "|  T A B L E R O  |" << endl;
	cout << "-------------------" << endl;
	cout << endl << juego.tablero << endl;
	cout << endl << endl;

	if (!TodosMaquina)
	{
		cout << left << setw(13) << "Jugador";
		mostrarFichasJug(juego, jug);
		cout << endl << endl;
		jug = 1;
	}

	for (int i = jug; i < juego.numJugador; i++)
	{
		cout << "Máquina #" << left << setw(4) << i;
		if (!Oculto)
		{
			mostrarFichasJug(juego, i);
			cout << endl;
		}
		else
		{
			cout << "-->  " << juego.arrayJugadores[i].cont << " fichas" << endl;
		}
	}
	cout << endl;
}

void mostrarFichasJug(const tJuego& juego, const short int jug)
{
	short int fichasCont = juego.arrayJugadores[jug].cont;
	for (int i = 0; i < fichasCont; i++)
	{
		cout << fichaToStr(juego, jug, i) << " ";
	}
}

void jugHumano(tJuego& juego, int& opcion, const short int jugador, bool& win, int& ganador)
{
	bool hayMas = true, movimiento = false;
	short int ficha;
	mostrarMenu(opcion);

	while (opcion != 0 && !movimiento && hayMas)
	{
		switch (opcion)
		{
		case 1:
		{
			movimiento = opcion1(juego, jugador, ficha, opcion, win);
		}
		break;
		case 2:
		{
			movimiento = opcion2(juego, jugador, ficha, opcion, win);
		}
		break;
		case 3:
		{
			opcion3(juego, jugador, opcion, hayMas);
		}
		break;
		}
	}
	if (win)
	{
		ganador = 0;
	}
}

void mostrarMenu(int& opcion)
{
	cout << endl;
	cout << "-------------------" << endl;
	cout << "| O P C I O N E S |" << endl;
	cout << "-------------------" << endl;
	cout << "1. Poner una ficha a la izquierda" << endl;
	cout << "2. Poner una ficha a la derecha" << endl;
	cout << "3. Nueva ficha" << endl;
	cout << "0. Salir" << endl;
	cout << endl << "Tu opción: ";
	cin >> opcion;
	while ((opcion < 0) || (opcion > 3))
	{
		cout << "Opción no válida." << endl;
		cout << "Nueva opción: ";
		cin >> opcion;
	}
}

bool opcion1(tJuego& juego, const short int jugador, short int& ficha, int& opcion, bool& win)
{
	bool movimiento = false;
	seleccionaFicha(juego, jugador, ficha);
	if (puedePonerIzq(juego, jugador, ficha))
	{
		ponerFichaIzq(juego, jugador, ficha);
		eliminaFicha(juego, jugador, ficha);
		movimiento = true;
		if (juego.arrayJugadores[jugador].cont == 0)
		{
			win = true;
		}
	}
	else
	{
		cout << "¡Imposible!" << endl;
		system("pause");
		system("cls");
		mostrarTablero(juego);
		mostrarMenu(opcion);
	}
	return movimiento;
}

bool opcion2(tJuego& juego, const short int jugador, short int& ficha, int& opcion, bool& win)
{
	int fichasCont = juego.arrayJugadores[jugador].cont;
	bool movimiento = false;
	seleccionaFicha(juego, jugador, ficha);
	if (puedePonerDcha(juego, jugador, ficha))
	{
		ponerFichaDcha(juego, jugador, ficha);
		eliminaFicha(juego, jugador, ficha);
		movimiento = true;
		if (juego.arrayJugadores[jugador].cont == 0)
		{
			win = true;
		}
	}
	else
	{
		cout << "¡Imposible!" << endl;
		system("pause");
		system("cls");
		mostrarTablero(juego);
		mostrarMenu(opcion);
	}
	return movimiento;
}

void opcion3(tJuego& juego, const short int jugador, int& opcion, bool& hayMas)
{
	int pozoCont = juego.pozo.cont;
	if (pozoCont > 0 && !puedeColocarFicha(juego, jugador))
	{
		system("cls");
		robaFicha(juego, jugador);
		mostrarTablero(juego);
		mostrarMenu(opcion);
	}
	else if (puedeColocarFicha(juego, jugador))
	{
		cout << "¡Puedes colocar alguna ficha!" << endl;
		system("pause");
		system("cls");
		mostrarTablero(juego);
		mostrarMenu(opcion);
	}
	else
	{
		cout << "No puedes robar más fichas. Siguiente turno..." << endl;
		system("pause");
		hayMas = false;
	}
}

void seleccionaFicha(const tJuego& juego, const short int jugador, short int& ficha)
{
	int fichasCont = juego.arrayJugadores[jugador].cont;
	cout << "Número de ficha [1.." << fichasCont << "]: ";
	cin >> ficha;
	while (ficha < 1 || ficha > fichasCont)
	{
		cout << "Opción no válida. Introduzca otro valor: ";
		cin >> ficha;
	}
	ficha--;
}

bool puedePonerIzq(const tJuego& juego, const short int jugador, const short int ficha)
{
	bool puedeIzq = false;
	short int fichaIzq = (short int(juego.tablero[1]) - short int('0'));
	short int fichaN1 = juego.arrayJugadores[jugador].arrayFichas[ficha].izquierda, fichaN2 = juego.arrayJugadores[jugador].arrayFichas[ficha].derecha;

	if (fichaIzq == fichaN1 || fichaIzq == fichaN2)
	{
		puedeIzq = true;
	}

	return puedeIzq;
}

void ponerFichaIzq(tJuego& juego, const short int jugador, const short int ficha)
{
	short int aux, fichaIzq = (short int(juego.tablero[1]) - short int('0'));
	short int fichaN1 = juego.arrayJugadores[jugador].arrayFichas[ficha].izquierda, fichaN2 = juego.arrayJugadores[jugador].arrayFichas[ficha].derecha;
	string tablero = juego.tablero;

	if (fichaN2 == fichaIzq)
	{
		juego.tablero = fichaToStr(juego, jugador, ficha) + tablero;
	}
	else
	{
		aux = fichaN1;
		fichaN1 = fichaN2;
		fichaN2 = aux;
		juego.arrayJugadores[jugador].arrayFichas[ficha].izquierda = fichaN1;
		juego.arrayJugadores[jugador].arrayFichas[ficha].derecha = fichaN2;
		juego.tablero = fichaToStr(juego, jugador, ficha) + tablero;
	}
}

bool puedePonerDcha(const tJuego& juego, const short int jugador, const short int ficha)
{
	bool puedeDer = false;
	short int fichaDer = (short int(juego.tablero[juego.tablero.size() - 2]) - short int('0'));
	short int fichaN1 = juego.arrayJugadores[jugador].arrayFichas[ficha].izquierda, fichaN2 = juego.arrayJugadores[jugador].arrayFichas[ficha].derecha;

	if (fichaDer == fichaN1 || fichaDer == fichaN2)
	{
		puedeDer = true;
	}

	return puedeDer;
}

void ponerFichaDcha(tJuego& juego, const short int jugador, const short int ficha)
{
	string tablero = juego.tablero;
	short int aux, fichaDer = (short int(tablero[tablero.size() - 2]) - short int('0'));
	short int fichaN1 = juego.arrayJugadores[jugador].arrayFichas[ficha].izquierda, fichaN2 = juego.arrayJugadores[jugador].arrayFichas[ficha].derecha;

	if (fichaN1 == fichaDer)
	{
		juego.tablero = tablero + fichaToStr(juego, jugador, ficha);
	}
	else
	{
		aux = fichaN1;
		fichaN1 = fichaN2;
		fichaN2 = aux;
		juego.arrayJugadores[jugador].arrayFichas[ficha].izquierda = fichaN1;
		juego.arrayJugadores[jugador].arrayFichas[ficha].derecha = fichaN2;
		juego.tablero = tablero + fichaToStr(juego, jugador, ficha);
	}
}

void eliminaFicha(tJuego& juego, const short int jugador, const short int ficha)
{
	short int fichasCont = juego.arrayJugadores[jugador].cont;
	for (int i = ficha; i < fichasCont; i++)
	{
		juego.arrayJugadores[jugador].arrayFichas[i].izquierda = juego.arrayJugadores[jugador].arrayFichas[i + 1].izquierda;
		juego.arrayJugadores[jugador].arrayFichas[i].derecha = juego.arrayJugadores[jugador].arrayFichas[i + 1].derecha;
	}
	juego.arrayJugadores[jugador].cont--;
}

bool puedeColocarFicha(const tJuego& juego, const short int jugador)
{
	bool puede = false;
	short int contador = 0;
	int fichasCont = juego.arrayJugadores[jugador].cont;
	while (contador < fichasCont && !puede)
	{
		if (puedePonerIzq(juego, jugador, contador) || puedePonerDcha(juego, jugador, contador))
		{
			puede = true;
		}
		contador++;
	}
	return puede;
}

void jugMaquina(tJuego& juego, const short int jugador, bool& win, int& ganador)
{
	bool sinFichas = false;

	cout << "Turno de la máquina #" << jugador << endl;
	if (jugador == 1)
	{
		while (!estrategia2(juego, jugador) && !sinFichas)
		{
			noEstrategia(juego, jugador, sinFichas);
			cout << "Turno de la máquina #" << jugador << endl;
		}
	}
	else
	{
		while (!estrategia1(juego, jugador) && !sinFichas)
		{
			noEstrategia(juego, jugador, sinFichas);
			cout << "Turno de la máquina #" << jugador << endl;
		}
	}
	if (!sinFichas)
	{
		cout << "¡Siguiente!" << endl;
		system("pause");
	}
	if (juego.arrayJugadores[jugador].cont == 0)
	{
		win = true;
		ganador = jugador;
	}
}

void noEstrategia(tJuego& juego, const short int jugador, bool& sinFichas)
{
	if (juego.pozo.cont == 0)
	{
		sinFichas = true;
		cout << "No se pueden robar fichas! Siguiente turno..." << endl;
	}
	else
	{
		robaFicha(juego, jugador);
		cout << "No puede poner ninguna ficha. Robando..." << endl;
	}
	system("pause");
	system("cls");
	mostrarTablero(juego);
}

bool estrategia1(tJuego& juego, const short int jugador)
{
	bool movimiento = false;
	short int contador = 0, fichasCont = juego.arrayJugadores[jugador].cont;

	while (contador < fichasCont && !movimiento)
	{
		if (puedePonerIzq(juego, jugador, contador))
		{
			cout << "Ficha" << fichaToStr(juego, jugador, contador) << "a la izquierda..." << endl;
			ponerFichaIzq(juego, jugador, contador);
			eliminaFicha(juego, jugador, contador);
			movimiento = true;
		}
		else if (puedePonerDcha(juego, jugador, contador))
		{
			cout << "Ficha" << fichaToStr(juego, jugador, contador) << "a la derecha..." << endl;
			ponerFichaDcha(juego, jugador, contador);
			eliminaFicha(juego, jugador, contador);
			movimiento = true;
		}
		contador++;
	}
	return movimiento;
}

bool estrategia2(tJuego& juego, const short int jugador)
{
	bool movimiento = false;
	short int contador = -1, fichasCont = juego.arrayJugadores[jugador].cont;
	int mayor = -1, suma;

	for (short int i = 0; i < fichasCont; i++)
	{
		if (puedePonerIzq(juego, jugador, i) || puedePonerDcha(juego, jugador, i))
		{
			suma = juego.arrayJugadores[jugador].arrayFichas[i].izquierda + juego.arrayJugadores[jugador].arrayFichas[i].derecha;

			if (suma > mayor)
			{
				mayor = suma;
				contador = i;
			}
		}
	}
	if (contador != -1)
	{
		if (puedePonerIzq(juego, jugador, contador))
		{
			cout << "Ficha" << fichaToStr(juego, jugador, contador) << "a la izquierda..." << endl;
			ponerFichaIzq(juego, jugador, contador);
		}
		else
		{
			cout << "Ficha" << fichaToStr(juego, jugador, contador) << "a la derecha..." << endl;
			ponerFichaDcha(juego, jugador, contador);
		}
		movimiento = true;
		eliminaFicha(juego, jugador, contador);
	}

	return movimiento;
}

bool sinSalida(const tJuego& juego)
{
	bool noSalida = false, ningunoPuede = true;
	short int jug = 0, fich = 0;
	while (jug < juego.numJugador && ningunoPuede)
	{
		while (fich < juego.arrayJugadores[jug].cont && ningunoPuede)
		{
			if (puedePonerIzq(juego, jug, fich) || puedePonerDcha(juego, jug, fich))
			{
				ningunoPuede = false;
			}
			fich++;
		}
		jug++;
		fich = 0;
	}
	if (juego.pozo.cont == 0 && ningunoPuede)
	{
		noSalida = true;
	}
	return noSalida;
}

void terminarPartida(tJuego& juego, const int ganador, bool& volverAJugar, short int& jugador, int& indice, bool& win, bool& sinOpciones)
{
	system("cls");
	mostrarTablero(juego);
	if (win)
	{
		haGanado(ganador);
		win = false;
	}
	else
	{
		sinOpciones = false;
		cout << endl << "No quedan salidas posibles. Saliendo del juego…" << endl;
		system("pause");
	}
	puntosAcumulados(juego);
	quiereVolverAJugar(volverAJugar);
	if (volverAJugar)
	{
		for (int i = 0; i < juego.numJugador; i++)
		{
			juego.arrayJugadores[i].cont = 0;
		}
		juego.pozo.cont = 0;
		comenzarPartida(juego, jugador, indice);
	}
}

void haGanado(const int ganador)
{
	cout << endl;
	if (ganador == 0 && !TodosMaquina)
	{
		cout << "¡Has ganado!" << endl;
	}
	else
	{
		cout << "¡Ha ganado la Máquina #" << ganador << "!" << endl;
	}
}

void puntosAcumulados(tJuego& juego)
{
	cout << endl << "Puntos acumulados…" << endl;
	for (int i = 0; i < juego.numJugador; i++)
	{
		sumaPuntos(juego, i);
		if (i == 0 && !TodosMaquina)
		{
			cout << "Jugador: " << juego.arrayPuntosJug[i] << endl;
		}
		else
		{
			cout << "Máquina #" << i << ": " << juego.arrayPuntosJug[i] << endl;
		}
	}
}

void sumaPuntos(tJuego& juego, const short int jugador)
{
	int fichasCont = juego.arrayJugadores[jugador].cont, puntos = juego.arrayPuntosJug[jugador];
	for (int i = 0; i < fichasCont; i++)
	{
		puntos = juego.arrayJugadores[jugador].arrayFichas[i].izquierda + juego.arrayJugadores[jugador].arrayFichas[i].derecha + puntos;
	}
	juego.arrayPuntosJug[jugador] = puntos;
}

void quiereVolverAJugar(bool& volverAJugar)
{
	char respuesta;
	cout << "¿Quiere volver a jugar [S/N]? ";
	cin >> respuesta;
	if (respuesta == 'N' || respuesta == 'n')
	{
		volverAJugar = false;
	}
	else if (respuesta == 'S' || respuesta == 's')
	{
		volverAJugar = true;
	}
}

void quiereGuardar(bool& guardarPartida)
{
	char respuesta;
	cout << "¿Quieres guardar tu sesión de juego [S/N]? ";
	cin >> respuesta;
	while (respuesta != 'S' && respuesta != 's' && respuesta != 'N' && respuesta != 'n')
	{
		cout << "Opción no válida." << endl;
		cout << "¿Quieres guardar tu sesión de juego [S/N]? ";
		cin >> respuesta;
	}
	if (respuesta == 'N' || respuesta == 'n')
	{
		guardarPartida = false;
	}
	else
	{
		guardarPartida = true;
	}
}

void escribirJuego(const tJuego& juego)
{
	ofstream salida;
	string nombreFicheroOut;
	tListaFichas listaFichas;

	cout << "Introduzca el nombre del fichero para guardar la partida: " << endl;
	cin >> nombreFicheroOut;

	salida.open(nombreFicheroOut);
	if (salida.is_open())
	{
		salida << juego.numJugador << " ";
		salida << juego.maxValor << endl;
		salida << juego.tablero << endl;
		salida << juego.pozo.cont << endl;
		escribirListaFichas(salida, juego.pozo);
		for (int j = 0; j < juego.numJugador; j++)
		{
			salida << endl << juego.arrayJugadores[j].cont << endl;
			escribirListaFichas(salida, juego.arrayJugadores[j]);
			salida << endl << juego.arrayPuntosJug[j];
		}
		salida << endl;
	}
	salida.close();
}

void escribirListaFichas(ofstream& salida, const tListaFichas& listaFichas)
{
	for (short int i = 0; i < listaFichas.cont; i++)
	{
		salida << listaFichas.arrayFichas[i].izquierda << " ";
		salida << listaFichas.arrayFichas[i].derecha << " ";
	}
}