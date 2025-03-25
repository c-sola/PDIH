#include <stdio.h>
#include <dos.h>

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define LIGHTBROWN 14
#define WHITE 15

unsigned char MODOTEXTO = 3;
unsigned char MODOGRAFICO = 4;

unsigned char cfondo = 0;
unsigned char ctexto = 4;

void mi_pausa()
{
	union REGS inregs, outregs;
	inregs.h.ah = 8;
	int86(0x21, &inregs, &outregs);
}

int mi_getchar()
{
	union REGS inregs, outregs;
	int caracter;

	inregs.h.ah = 1;
	int86(0x21, &inregs, &outregs);

	caracter = outregs.h.al;
	return caracter;
}

void mi_putchar(char c)
{
	union REGS inregs, outregs;

	inregs.h.ah = 2;
	inregs.h.dl = c;
	int86(0x21, &inregs, &outregs);
}

// Colocamos el cursor en una posición determinada
void gotoxy(int x, int y)
{
	union REGS inregs, outregs;

	inregs.h.ah = 0x02; // número de la función
	inregs.h.dh = x;	// número de fila
	inregs.h.dl = y;	// número de columna
	inregs.h.bh = 0x00; // Recibe el primer plano de la pantalla

	int86(0x10, &inregs, &outregs);
}

void setcursortype(int tipo_cursor)
{
	union REGS inregs, outregs;
	inregs.h.ah = 0x01;
	switch (tipo_cursor)
	{
	case 0: // invisible
		inregs.h.ch = 010;
		inregs.h.cl = 000;
		break;
	case 1: // normal
		inregs.h.ch = 010;
		inregs.h.cl = 010;
		break;
	case 2: // grueso
		inregs.h.ch = 000;
		inregs.h.cl = 010;
		break;
	}
	int86(0x10, &inregs, &outregs);
}

// Fijamos el modo de video que queramos, 3 para texto y 4 para gráfico
void setvideomode(unsigned char modo)
{

	union REGS inregs, outregs;
	inregs.h.al = modo;
	inregs.h.ah = 0x00;
	int86(0x10, &inregs, &outregs);
}

// Vamos a obtener el modo de video actual
unsigned int getvideomode(void)
{

	union REGS inregs, outregs;
	inregs.h.ah = 0x0F;				// cogemos el valor 0X0F que es la función de BIOS que obtiene el modo de video actual
	int86(0x10, &inregs, &outregs); // número de interrupción correspondiente

	return outregs.h.al;
}

// Obtenemos el caracter del teclado y lo mostramos por la pantalla
void getche()
{
	int tmp;
	printf("\nPulsa una tecla: ");
	tmp = mi_getchar();
	printf("\nHas pulsado: ");
	mi_putchar((char)tmp);
}

// ponemos un pixel en la coordenada X,Y de color C
void pixel(int x, int y, unsigned char C)
{
	union REGS inregs, outregs;
	inregs.x.cx = x;
	inregs.x.dx = y;
	inregs.h.al = C;
	inregs.h.ah = 0x0C;
	int86(0x10, &inregs, &outregs);
}

// borra toda la pantalla
void clrscr()
{
	union REGS inregs, outregs;
	inregs.h.ah = 0x06; // número de la función
	inregs.h.al = 0;	// número de líneas a desplazar, en donde 0 indica toda la pantalla
	inregs.h.dh = 25;	// fila inferior derecha de la parte a desplazar
	inregs.h.dl = 80;	// columna inferior derecha de la parte a desplazar
	inregs.h.bh = 0x00; // color
	inregs.x.cx = 0x00; // fila y columna superior izquierda de la parte a desplazar
	int86(0x10, &inregs, &outregs);
	return;
}

// se escribe un caracter por pantalla indicando el color
void cputchar(unsigned char caracter)
{

	union REGS inregs, outregs;
	inregs.h.ah = 0x09;
	inregs.h.al = caracter;
	inregs.h.bl = cfondo << 4 | ctexto;
	inregs.h.bh = 0x00;
	inregs.x.cx = 1;
	int86(0x10, &inregs, &outregs);
	return;
}

void textcolor(int color)
{
	union REGS inregs, outregs;

	ctexto = color; // Se guarda el nuevo color del texto

	inregs.h.ah = 0x09;					  // Función 09h de la interrupción 10h
	inregs.h.al = 'M';					  // Aqui solo configuramos el color, por lo tanto ponemos ' '
	inregs.h.bl = (cfondo << 4) | ctexto; // Combinamos color de fondo y texto
	inregs.h.bh = 0x00;
	inregs.x.cx = 1;

	int86(0x10, &inregs, &outregs);
}

void textbackground(int color)
{
	union REGS inregs, outregs;

	cfondo = color; // Se guarda el nuevo color de fondo

	inregs.h.ah = 0x09;
	inregs.h.al = 'M';
	inregs.h.bl = (cfondo << 4) | ctexto; // Combinamos color de fondo y texto
	inregs.h.bh = 0x00;
	inregs.x.cx = 1;

	int86(0x10, &inregs, &outregs);
}

// cambiamos el color del texto
void mi_textcolor(int color)
{
	ctexto = color;
}

// cambiamos el color de fondo
void mi_textbackground(int color)
{
	cfondo = color;
}

// dibujo de un recuadro en modo texto
void dibujaRecuadro(int supIzq_x, int supIzq_y, int infDer_x, int infDer_y, unsigned char ctexto, unsigned char cfondo)
{

	int i, j, k;
	char espacio = ' ';

	// cambiamos el color por el que se ha especificado por parámetro
	mi_textcolor(ctexto);
	mi_textbackground(cfondo);

	// línea superior
	for (i = supIzq_x; i < infDer_x - 1; i++)
	{
		gotoxy(i, supIzq_y);
		cputchar(espacio);
	}

	// lados
	for (k = supIzq_y; k < infDer_y; k++)
	{
		gotoxy(infDer_x - 1, k);
		cputchar(espacio);
		gotoxy(supIzq_x, k);
		cputchar(espacio);
	}

	// línea inferior
	for (j = supIzq_x; j < infDer_x - 1; j++)
	{
		gotoxy(j, infDer_y - 1);
		cputchar(espacio);
	}
}

// dibuja un círculo en modo texto
void dibujaCirculo(int xc, int yc, int radio, unsigned char ctexto, unsigned char cfondo)
{
	int x = 0, y = radio;
	int d = 3 - 2 * radio;

	// Configuramos colores
	mi_textcolor(ctexto);
	mi_textbackground(cfondo);

	while (y >= x)
	{
		// primeramente dibujamos los 8 puntos simétricos
		gotoxy(xc + x, yc + y);
		cputchar('O');
		gotoxy(xc - x, yc + y);
		cputchar('O');
		gotoxy(xc + x, yc - y);
		cputchar('O');
		gotoxy(xc - x, yc - y);
		cputchar('O');
		gotoxy(xc + y, yc + x);
		cputchar('O');
		gotoxy(xc - y, yc + x);
		cputchar('O');
		gotoxy(xc + y, yc - x);
		cputchar('O');
		gotoxy(xc - y, yc - x);
		cputchar('O');

		x++;

		// nos basamos en el algoritmo de Bresenham para calcular d
		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
		{
			d = d + 4 * x + 6;
		}
	}
}

// dibujo de una casa
void dibujaCasa()
{
	int x, y, ancho;

	// base de la casa como rectángulo
	for (x = 50; x < 150; x++)
	{
		for (y = 100; y < 180; y++)
		{
			pixel(x, y, 2); // color 2 (Rosa)
		}
	}

	// dibujamos el techo
	ancho = 0;
	for (y = 60; y < 100; y++)
	{
		for (x = 75 - ancho; x < 125 + ancho; x++)
		{
			pixel(x, y, 1); // color 1 (Azul)
		}
		ancho++;
	}

	// dibujamos la puerta como un rectángulo más pequeño
	for (x = 90; x < 110; x++)
	{
		for (y = 140; y < 180; y++)
		{
			pixel(x, y, 1); // color 1 (Azul)
		}
	}

	// dibujamos la ventana siendo un cuadrado
	for (x = 60; x < 80; x++)
	{
		for (y = 120; y < 140; y++)
		{
			pixel(x, y, 3); // color 3 (Blanco)
		}
	}
}

void dibujaMonigote()
{
	int x, y;

	// dibujamos la cabeza con un cuadrado
	for (x = 170; x < 185; x++)
	{
		for (y = 90; y < 105; y++)
		{
			pixel(x, y, 3); // color 3 (Blanco)
		}
	}

	// dibujamos el cuerpo como un rectángulo más corto
	for (x = 175; x < 180; x++)
	{
		for (y = 105; y < 125; y++)
		{
			pixel(x, y, 2); // color 2 (Rosa)
		}
	}

	// dibujamos los brazos como un rectángulo pequeño
	for (x = 165; x < 190; x++)
	{
		for (y = 110; y < 113; y++)
		{
			pixel(x, y, 1); // color 1 (Azul)
		}
	}

	// dibujamos las piernas los cuales son dos rectángulos finos
	for (x = 175; x < 177; x++)
	{
		for (y = 125; y < 140; y++)
		{
			pixel(x, y, 1); // color 1 (Azul)
		}
	}
	for (x = 178; x < 180; x++)
	{
		for (y = 125; y < 140; y++)
		{
			pixel(x, y, 1); // color 1 (Azul)
		}
	}
}

void dibujosGrafico()
{
	setvideomode(MODOGRAFICO);

	dibujaCasa();
	dibujaMonigote();

	mi_pausa();

	setvideomode(MODOTEXTO);
}

void ascii_art()
{
	clrscr(); // Borra la pantalla antes de dibujar

	// Orejas
	gotoxy(10, 5);
	cputchar('(');

	gotoxy(10, 6);
	cputchar('\\');

	// Orejas
	gotoxy(10, 7);
	cputchar('(');

	gotoxy(10, 8);
	cputchar('\\');

	// Cara
	gotoxy(11, 5);
	cputchar('(');

	gotoxy(11, 6);
	cputchar('-');

	gotoxy(11, 7);
	cputchar('.');

	gotoxy(11, 8);
	cputchar('-');

	gotoxy(11, 9);
	cputchar(')');

	// Piernas

	gotoxy(12, 5);
	cputchar('o');

	gotoxy(12, 6);
	cputchar('_');

	gotoxy(12, 7);
	cputchar('(');

	gotoxy(12, 8);
	cputchar('"');

	gotoxy(12, 9);
	cputchar(')');

	gotoxy(12, 10);
	cputchar('(');

	gotoxy(12, 11);
	cputchar('"');

	gotoxy(12, 12);
	cputchar(')');

	mi_pausa();
}

void dibujaPerro()
{
	clrscr(); // Borra la pantalla antes de dibujar

	// Orejas
	gotoxy(10, 5);
	cputchar('/');
	gotoxy(10, 7);
	cputchar('\\');

	// Cabeza
	gotoxy(10, 8);
	cputchar('_');
	gotoxy(10, 9);
	cputchar('_');
	gotoxy(11, 10);
	cputchar('\\');

	// Ocico
	gotoxy(11, 11);
	cputchar('_');
	gotoxy(11, 12);
	cputchar('_');
	gotoxy(11, 13);
	cputchar('_');
	gotoxy(11, 14);
	cputchar('_');
	gotoxy(12, 14);
	cputchar('O');

	// Boca
	gotoxy(13, 13);
	cputchar('/');
	gotoxy(13, 12);
	cputchar('_');
	gotoxy(13, 11);
	cputchar('_');
	gotoxy(13, 10);
	cputchar('_');
	gotoxy(13, 9);
	cputchar('_');
	gotoxy(13, 8);
	cputchar('_');
	gotoxy(13, 7);
	cputchar('C');
	gotoxy(14, 12);
	cputchar('U');

	// Cuello
	gotoxy(14, 8);
	cputchar('/');
	gotoxy(14, 7);
	cputchar('_');
	gotoxy(14, 6);
	cputchar('_');
	gotoxy(14, 5);
	cputchar('_');
	gotoxy(14, 4);
	cputchar('_');
	gotoxy(14, 3);
	cputchar('_');
	gotoxy(14, 2);
	cputchar('_');

	// Espalda
	gotoxy(13, 1);
	cputchar('/');
	gotoxy(12, 3);
	cputchar('/');

	// Oreja
	gotoxy(11, 4);
	cputchar('C');

	// Ojo
	gotoxy(11, 9);
	cputchar('@');

	// Posicion inicial del cursor
	gotoxy(0, 0);

	mi_pausa(); // Detener la ejecucion para observar el dibujo
}

void dibuja_serpiente(){

	clrscr(); // Borra la pantalla antes de dibujar

	gotoxy(5, 5);
	cputchar('\\');
	gotoxy(5, 6);
	cputchar('/');

	gotoxy(6, 6);
	cputchar('\\');
	gotoxy(6, 7);
	cputchar('_');
	gotoxy(6, 8);
	cputchar('_');
	gotoxy(6, 8);
	cputchar('_');
	gotoxy(6, 10);
	cputchar('_');

	gotoxy(6, 11);
	cputchar('|');

	gotoxy(6, 12);
	cputchar('_');
	gotoxy(6, 13);
	cputchar('_');
	gotoxy(6, 14);
	cputchar('_');
	gotoxy(6, 15);
	cputchar('_');
	gotoxy(6, 16);
	cputchar('_');
	gotoxy(6, 17);
	cputchar('_');
	gotoxy(6, 18);
	cputchar('_');
	gotoxy(6, 19);
	cputchar('_');
	gotoxy(6, 20);
	cputchar('_');
	gotoxy(6, 21);
	cputchar('_');

	gotoxy(6, 22);
	cputchar('/');

	gotoxy(5, 12);
	cputchar('/');
	gotoxy(5, 13);
	cputchar('~');

	gotoxy(4, 14);
	cputchar('_');
	gotoxy(4, 15);
	cputchar('|');
	gotoxy(4, 16);
	cputchar('_');
	gotoxy(4, 17);
	cputchar('_');
	gotoxy(4, 18);
	cputchar('|');
	gotoxy(5, 19);
	cputchar('\\');
	gotoxy(5, 20);
	cputchar('_');
	gotoxy(5, 21);
	cputchar('/');
	gotoxy(4, 22);
	cputchar('|');

	gotoxy(3, 16);
	cputchar('/');
	gotoxy(3, 17);
	cputchar('^');
	gotoxy(3, 18);
	cputchar('\\');
	gotoxy(3, 19);
	cputchar('/');
	gotoxy(3, 20);
	cputchar('^');
	gotoxy(3, 21);
	cputchar('\\');

	gotoxy(4, 21);
	cputchar('O');

	gotoxy(7, 13);
	cputchar('\\');

	gotoxy(7, 14);
	cputchar('_');
	gotoxy(7, 15);
	cputchar('_');
	gotoxy(7, 16);
	cputchar('_');
	gotoxy(7, 17);
	cputchar('_');
	gotoxy(7, 18);
	cputchar('_');
	gotoxy(7, 19);
	cputchar('_');
	gotoxy(7, 20);
	cputchar('_');

	gotoxy(8, 21);
	cputchar('`');
	gotoxy(8, 22);
	cputchar('\\');
	gotoxy(9, 23);
	cputchar('|');
	gotoxy(10, 22);
	cputchar('/');
	gotoxy(11, 21);
	cputchar('/');
	gotoxy(12, 19);
	cputchar('/');
	gotoxy(13, 18);
	cputchar('/');
	gotoxy(14, 16);
	cputchar('/');
	gotoxy(15, 15);
	cputchar('/');
	gotoxy(16, 14);
	cputchar('(');
	gotoxy(17, 15);
	cputchar('\\');
	gotoxy(18, 17);
	cputchar('~');
	gotoxy(18, 18);
	cputchar('-');
	gotoxy(18, 19);
	cputchar('_');
	gotoxy(19, 20);
	cputchar('~');
	gotoxy(20, 21);
	cputchar('-');
	gotoxy(20, 22);
	cputchar('-');
	gotoxy(20, 23);
	cputchar('_');
	gotoxy(20, 24);
	cputchar('_');
	gotoxy(20, 25);
	cputchar('_');
	gotoxy(20, 26);
	cputchar('_');
	gotoxy(20, 27);
	cputchar('_');
	gotoxy(20, 28);
	cputchar('_');
	gotoxy(20, 29);
	cputchar('-');
	gotoxy(20, 30);
	cputchar('~');
	gotoxy(19, 31);
	cputchar('_');
	gotoxy(19, 32);
	cputchar('-');
	gotoxy(19, 33);
	cputchar('~');
	gotoxy(18, 34);
	cputchar('_');
	gotoxy(18, 35);
	cputchar('-');
	gotoxy(18, 36);
	cputchar('~');
	gotoxy(17, 37);
	cputchar('_');
	gotoxy(17, 38);
	cputchar('-');
	gotoxy(17, 39);
	cputchar('-');
	gotoxy(17, 40);
	cputchar('_');
	gotoxy(18, 41);
	cputchar('~');
	gotoxy(18, 42);
	cputchar('-');
	gotoxy(18, 43);
	cputchar('_');
	gotoxy(19, 44);
	cputchar('~');
	gotoxy(19, 45);
	cputchar('-');
	gotoxy(19, 46);
	cputchar('_');
	gotoxy(20, 47);
	cputchar('~');
	gotoxy(20, 48);
	cputchar('-');
	gotoxy(20, 49);
	cputchar('_');
	gotoxy(20, 50);
	cputchar('_');
	gotoxy(20, 51);
	cputchar('_');
	gotoxy(20, 52);
	cputchar('-');
	gotoxy(20, 53);
	cputchar('~');
	gotoxy(19, 54);
	cputchar('_');
	gotoxy(19, 55);
	cputchar('-');
	gotoxy(19, 56);
	cputchar('~');
	gotoxy(18, 57);
	cputchar('/');
	gotoxy(17, 58);
	cputchar('|');
	gotoxy(16, 58);
	cputchar('|');
	gotoxy(15, 58);
	cputchar('\\');
	gotoxy(14, 56);
	cputchar('\\');
	gotoxy(13, 54);
	cputchar('\\');
	gotoxy(12, 52);
	cputchar('\\');
	gotoxy(12, 51);
	cputchar('\\');
	gotoxy(11, 50);
	cputchar('\\');
	gotoxy(10, 48);
	cputchar('\\');
	gotoxy(9, 46);
	cputchar('\\');
	gotoxy(13, 52);
	cputchar('\\');
	gotoxy(14, 51);
	cputchar('\\');
	gotoxy(15, 50);
	cputchar('\\');
	gotoxy(16, 50);
	cputchar('|');
	gotoxy(17, 50);
	cputchar('/');
	gotoxy(17, 49);
	cputchar('_');

	gotoxy(5, 23);
	cputchar('\\');
	gotoxy(6, 25);
	cputchar('\\');
	gotoxy(7, 27);
	cputchar('\\');
	gotoxy(8, 28);
	cputchar('\\');
	gotoxy(9, 29);
	cputchar('|');
	gotoxy(10, 29);
	cputchar('/');
	gotoxy(11, 27);
	cputchar('/');
	gotoxy(12, 26);
	cputchar('/');
	gotoxy(13, 24);
	cputchar('/');
	gotoxy(14, 22);
	cputchar('/');
	gotoxy(15, 21);
	cputchar('/');
	gotoxy(16, 21);
	cputchar('(');
	gotoxy(17, 22);
	cputchar('~');
	gotoxy(17, 23);
	cputchar('-');
	gotoxy(17, 24);
	cputchar('_');
	gotoxy(17, 25);
	cputchar('_');
	gotoxy(17, 26);
	cputchar('_');
	gotoxy(17, 27);
	cputchar('_');
	gotoxy(17, 28);
	cputchar('-');
	gotoxy(17, 29);
	cputchar('~');
	gotoxy(16, 30);
	cputchar('_');
	gotoxy(16, 31);
	cputchar('-');
	gotoxy(16, 32);
	cputchar('~');
	gotoxy(15, 33);
	cputchar('_');
	gotoxy(15, 34);
	cputchar('-');
	gotoxy(15, 35);
	cputchar('-');
	gotoxy(15, 36);
	cputchar('~');
	gotoxy(14, 37);
	cputchar('_');
	gotoxy(14, 38);
	cputchar('-');
	gotoxy(14, 39);
	cputchar('-');
	gotoxy(14, 40);
	cputchar('-');
	gotoxy(14, 41);
	cputchar('-');
	gotoxy(14, 42);
	cputchar('_');
	gotoxy(15, 43);
	cputchar('~');
	gotoxy(15, 44);
	cputchar('-');
	gotoxy(15, 45);
	cputchar('_');
	gotoxy(16, 46);
	cputchar('~');
	gotoxy(16, 47);
	cputchar('-');
	gotoxy(16, 48);
	cputchar('_');
	gotoxy(17, 49);
	cputchar('~');
	gotoxy(17, 50);
	cputchar('-');
	gotoxy(17, 51);
	cputchar('_');
	gotoxy(17, 52);
	cputchar('-');
	gotoxy(17, 53);
	cputchar('~');
}

void escribirConColor()
{
	char caracter;
	int color;

	printf("Introduce un caracter: ");
	caracter = getchar();
	getchar(); // Debemos de coger el ENTER para evitar interferencias con la entrada del color

	printf("Ahora dime un numero de color (0-9): ");
	color = getchar() - '0'; // Esto lo que hace es convertir el número a carácter

	if (color < 0 || color > 9)
	{
		printf("Ese color no es valido. Se usara el blanco.\n");
		color = WHITE;
	}

	textcolor(color);
	cputchar(caracter);
}

void escribirConFondo()
{
	char caracter;
	int fondo;

	printf("Introduce un caracter: ");
	caracter = getchar();
	getchar(); // Debemos de coger el ENTER para evitar interferencias con la entrada del fondo de color

	printf("Ahora dime un numero de color (0-9): ");
	fondo = getchar() - '0'; // Esto lo que hace es convertir el número a carácter

	if (fondo < 0 || fondo > 9)
	{
		printf("Ese color no es valido. Se usara el blanco.\n");
		fondo = WHITE;
	}

	textbackground(fondo);
	cputchar(caracter);
}

void imprimir_cadena(int x, int y, int length, char *text) {
    
    int i;
    gotoxy(x, y);  // Posiciona el cursor
   
    for (i = 0; i < length && text[i] != '\0'; i++) {
        cputchar(text[i]);  // Imprime cada carácter de la cadena
		y++;
		gotoxy(x, y);
    }
}

void obtener_posicion_cursor(int *fila, int *columna) {
    union REGS inregs, outregs;
    
    inregs.h.ah = 0x03;  // Obtener posición del cursor
    inregs.h.bh = 0x00;  // Página de pantalla
    
    int86(0x10, &inregs, &outregs);  

    // DL (columna) y DH (fila)
    *columna = outregs.h.dl;
    *fila = outregs.h.dh;
}

int main() {
	
    int fila, columna;
	
    clrscr();

    gotoxy(5,10);
    obtener_posicion_cursor(&fila, &columna);
    printf("%d,%d\n", fila, columna);

    gotoxy(10,20);
    obtener_posicion_cursor(&fila, &columna);
    printf("%d,%d\n", fila, columna);

    gotoxy(12,40);
    obtener_posicion_cursor(&fila, &columna);
    printf("%d,%d\n", fila, columna);

    gotoxy(20,8);
    obtener_posicion_cursor(&fila, &columna);
    printf("%d,%d\n", fila, columna);
    
    mi_pausa();
}
