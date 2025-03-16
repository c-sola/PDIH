#include <stdio.h>
#include <dos.h>

int mi_getchar(){
    union REGS inregs, outregs;
    int caracter;

    inregs.h.ah = 1;  // Función para leer un carácter desde el teclado
    int86(0x21, &inregs, &outregs);  // Llamada a la interrupción de DOS

    caracter = outregs.h.al;  // Obtener el carácter leído
    return caracter;
}

void mi_putchar(char c){
    union REGS inregs, outregs;
    
    inregs.h.ah = 2;  // Función para mostrar un carácter en pantalla
    inregs.h.dl = c;  // Carácter a mostrar
    
    int86(0x21, &inregs, &outregs);  // Llamada a la interrupción de DOS
}

int main() {
    char c;
    
    printf("Introduce caracteres, presiona ESC para salir:\n");

    while(1) {
        c = mi_getchar();  // Leer un carácter desde el teclado
        if (c == 27) {  // El código ASCII de ESC es 27
            break; 
        }
        mi_putchar(c); 
    }

    return 0;
}