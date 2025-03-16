#include <dos.h>
#include <stdio.h>
#define BYTE unsigned char 

void selecciona_modo_video(unsigned char modo) {
    union REGS inregs, outregs;
    inregs.h.ah = 0x00;
    inregs.h.al = modo;
    int86(0x10, &inregs, &outregs);
    return;
}

int mi_getchar(){ 
    union REGS inregs, outregs; 
    int caracter; 
    inregs.h.ah = 1; 
    int86(0x21, &inregs, &outregs); 
    caracter = outregs.h.al; 
    return caracter; 
} 

int main() {
    int tecla;

    selecciona_modo_video(0x13);

    printf("Presiona una tecla...\n");

    tecla = mi_getchar(); 

    selecciona_modo_video(0x03);

    return 0;
}
