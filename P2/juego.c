#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define ANCHO 40
#define ALTO 20
#define VELOCIDAD 50000 //Esta es la velocidad del juego, se puede cambiar
#define MAX_ASTEROIDES 10

typedef struct {
    int x, y;
    int activo;
} Asteroide;

typedef struct {
    int x, y;
} Nave;

typedef struct {
    int x, y;
    int activo;
} Disparo;

void pantallaBienvenida() {
    clear();
    mvprintw(ALTO / 2 - 3, ANCHO / 2 - 10, "¡Bienvenido al juego!");
    mvprintw(ALTO / 2 - 1, ANCHO / 2 - 15, "Creado por: Clara Sola Ruiz y Mario Casas Perez.");
    mvprintw(ALTO / 2 + 1, ANCHO / 2 - 15, "Controles:");
    mvprintw(ALTO / 2 + 2, ANCHO / 2 - 15, "- Flechas: Mover");
    mvprintw(ALTO / 2 + 3, ANCHO / 2 - 15, "- Espacio: Disparar");
    mvprintw(ALTO / 2 + 4, ANCHO / 2 - 15, "- q: Salir");
    mvprintw(ALTO / 2 + 5, ANCHO / 2 - 15, "Explicacion: este juego consiste en destruir la mayor cantidad posible de asteroides para ganar. Si un asteroide choca con tu nave, pierdes.");
    mvprintw(ALTO / 2 + 7, ANCHO / 2 - 15, "Presiona cualquier tecla para empezar...");
    refresh();
    getch();
}

void iniciaAsteroides(Asteroide asteroides[], int n) {
    for (int i = 0; i < n; i++) {
        asteroides[i].x = rand() % ANCHO;
        asteroides[i].y = rand() % (ALTO / 2);
        asteroides[i].activo = 1;
    }
}

void actualizaAsteroides(Asteroide asteroides[], int n) {
    for (int i = 0; i < n; i++) {
        if (asteroides[i].activo) {
            asteroides[i].y++;
            if (asteroides[i].y >= ALTO) {
                asteroides[i].y = 0;
                asteroides[i].x = rand() % ANCHO;
                asteroides[i].activo = 1;
            }
        }
    }
}

void moverNave(Nave *nave, int tecla) {
    if (tecla == KEY_LEFT && nave->x > 0) nave->x--;
    if (tecla == KEY_RIGHT && nave->x < ANCHO - 1) nave->x++;
    if (tecla == KEY_UP && nave->y > ALTO / 2) nave->y--; 
    if (tecla == KEY_DOWN && nave->y < ALTO - 1) nave->y++;
}

void iniciaDisparo(Disparo *disparo, Nave nave) {
    if (!disparo->activo) {
        disparo->x = nave.x;
        disparo->y = nave.y - 1;
        disparo->activo = 1;
    }
}

void actualizaDisparo(Disparo *disparo) {
    if (disparo->activo) {
        disparo->y--;
        if (disparo->y < 0) disparo->activo = 0;
    }
}

int detectaColision(Asteroide *asteroide, Disparo *disparo) {
    if (disparo->activo && asteroide->activo &&
        disparo->x == asteroide->x && disparo->y == asteroide->y) {
        asteroide->activo = 0;
        disparo->activo = 0;
        return 100;
    }
    return 0;
}

int detectaColisionNave(Nave *nave, Asteroide asteroides[], int n) {
    for (int i = 0; i < n; i++) {
        if (asteroides[i].activo && asteroides[i].x == nave->x && asteroides[i].y == nave->y) {
            return 1;
        }
    }
    return 0;
}

int main() {
    char nombre[20];
    initscr();
    noecho();
    curs_set(FALSE);

	pantallaBienvenida();
	
	clear();
	
    mvprintw(ALTO / 2, ANCHO / 2 - 10, "Dime tu nombre: ");
    echo();
    scanw("%19s", nombre);
    noecho();

    int jugar = 1;
    while (jugar) {
        Nave nave = {ANCHO / 2, ALTO - 3};
        Asteroide asteroides[MAX_ASTEROIDES];
        iniciaAsteroides(asteroides, MAX_ASTEROIDES);
        Disparo disparo = {0, 0, 0};
        int puntuacion = 0;
        int game_over = 0;

        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        srand(time(NULL));

        while (!game_over) {
            int tecla = getch();
            if (tecla == 'q') return 0;

            moverNave(&nave, tecla);
            if (tecla == ' ') iniciaDisparo(&disparo, nave);
            actualizaAsteroides(asteroides, MAX_ASTEROIDES);
            actualizaDisparo(&disparo);

            for (int i = 0; i < MAX_ASTEROIDES; i++) {
                puntuacion += detectaColision(&asteroides[i], &disparo);
            }

            if (detectaColisionNave(&nave, asteroides, MAX_ASTEROIDES)) game_over = 1;

            clear();
            mvprintw(nave.y, nave.x, " A ");
            
            if (disparo.activo) mvprintw(disparo.y, disparo.x, "|");
            for (int i = 0; i < MAX_ASTEROIDES; i++) {
                if (asteroides[i].activo) mvprintw(asteroides[i].y, asteroides[i].x, "*");
            }
            mvprintw(ALTO, 0, "Puntuación: %d", puntuacion);
            refresh();
            usleep(VELOCIDAD);
        }

        nodelay(stdscr, FALSE);
        mvprintw(ALTO / 2, ANCHO / 2 - 5, "GAME OVER");
        mvprintw(ALTO / 2 + 1, ANCHO / 2 - 8, "Puntuación: %d", puntuacion);
        mvprintw(ALTO / 2 + 2, ANCHO / 2 - 12, "¿Jugar de nuevo? (s/n): ");
        echo();
        char respuesta;
        scanw(" %c", &respuesta);
        noecho();
        if (respuesta == 'n') jugar = 0;
    }

    mvprintw(ALTO / 2 + 3, ANCHO / 2 - 12, "Gracias por jugar, %s!", nombre);
    refresh();
    getch();

    endwin();
    return 0;
}
