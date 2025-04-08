#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define ANCHO 40
#define ALTO 20
#define VELOCIDAD 80000

typedef struct {
    int x, y;
    int activo;
} Enemigo;

typedef struct {
    int x, y;
    int vidas;
    int score;
} Jugador;

typedef struct {
    int x, y;
} Obstaculo;

void pantallaBienvenida() {
    clear();
    mvprintw(ALTO / 2 - 3, ANCHO / 2 - 10, "¡Bienvenido al juego!");
    mvprintw(ALTO / 2 - 1, ANCHO / 2 - 15, "Creado por: Clara Sola Ruiz y Mario Casas Perez");
    mvprintw(ALTO / 2 + 1, ANCHO / 2 - 15, "Controles:");
    mvprintw(ALTO / 2 + 2, ANCHO / 2 - 15, "- Flechas: Mover");
    mvprintw(ALTO / 2 + 3, ANCHO / 2 - 15, "- Espacio: Atacar");
    mvprintw(ALTO / 2 + 4, ANCHO / 2 - 15, "- q: Salir");
    mvprintw(ALTO / 2 + 5, ANCHO / 2 - 15, "Explicacion: El bosque está lleno de enemigos y obstáculos.");
    mvprintw(ALTO / 2 + 6, ANCHO / 2 - 15, "Elimina a todos los enemigos en los 3 niveles para ganar antes de que se termine el tiempo.");
    mvprintw(ALTO / 2 + 8, ANCHO / 2 - 15, "Presiona cualquier tecla para empezar...");
    refresh();
    getch();
}

void pantallaResumen(const char *nombre, int score, int ganador, int por_tiempo) {
    clear();
    if (por_tiempo) {
        mvprintw(ALTO / 2 - 1, ANCHO / 2 - 10, "¡Se acabó el tiempo, %s!", nombre);
    } else if (ganador) {
        mvprintw(ALTO / 2 - 1, ANCHO / 2 - 10, "¡Felicidades %s, ganaste!", nombre);
    } else {
        mvprintw(ALTO / 2 - 1, ANCHO / 2 - 10, "GAME OVER, %s", nombre);
    }
    mvprintw(ALTO / 2 + 1, ANCHO / 2 - 15, "Puntuación final: %d", score);
    mvprintw(ALTO / 2 + 2, ANCHO / 2 - 15, "¿Quieres jugar de nuevo? (s/n): ");
    refresh();
}

void dibujaBordes() {
    attron(COLOR_PAIR(4));
    for (int i = 0; i < ANCHO + 2; i++) {
        mvprintw(ALTO + 1, i, "#");
    }
    for (int i = 0; i <= ALTO; i++) {
        mvprintw(i, ANCHO + 1, "#");
    }
    attroff(COLOR_PAIR(4));
}

void iniciaEnemigos(Enemigo enemigos[], int n) {
    for (int i = 0; i < n; i++) {
        enemigos[i].x = rand() % ANCHO;
        enemigos[i].y = rand() % ALTO;
        enemigos[i].activo = 1;
    }
}

void iniciaObstaculos(Obstaculo obstaculos[], int n) {
    for (int i = 0; i < n; i++) {
        obstaculos[i].x = rand() % ANCHO;
        obstaculos[i].y = rand() % ALTO;
    }
}

int colisionaConObstaculo(int x, int y, Obstaculo obstaculos[], int n) {
    for (int i = 0; i < n; i++) {
        if (x == obstaculos[i].x && y == obstaculos[i].y) return 1;
    }
    return 0;
}

void moverJugador(Jugador *jugador, int tecla, Obstaculo obstaculos[], int n_obstaculos) {
    int nuevo_x = jugador->x, nuevo_y = jugador->y;
    if (tecla == KEY_LEFT && jugador->x > 0) nuevo_x--;
    if (tecla == KEY_RIGHT && jugador->x < ANCHO - 1) nuevo_x++;
    if (tecla == KEY_UP && jugador->y > 0) nuevo_y--;
    if (tecla == KEY_DOWN && jugador->y < ALTO - 1) nuevo_y++;

    if (!colisionaConObstaculo(nuevo_x, nuevo_y, obstaculos, n_obstaculos)) {
        jugador->x = nuevo_x;
        jugador->y = nuevo_y;
    }
}

void moverEnemigos(Enemigo enemigos[], int n, Jugador jugador, Obstaculo obstaculos[], int n_obstaculos) {
    for (int i = 0; i < n; i++) {
        if (enemigos[i].activo) {
            int dx = jugador.x - enemigos[i].x;
            int dy = jugador.y - enemigos[i].y;
            int nuevo_x = enemigos[i].x + (dx != 0 ? (dx > 0 ? 1 : -1) : 0);
            int nuevo_y = enemigos[i].y + (dy != 0 ? (dy > 0 ? 1 : -1) : 0);

            if (!colisionaConObstaculo(nuevo_x, nuevo_y, obstaculos, n_obstaculos)) {
                enemigos[i].x = nuevo_x;
                enemigos[i].y = nuevo_y;
            }
        }
    }
}

void atacar(Jugador *jugador, Enemigo enemigos[], int n) {
    for (int i = 0; i < n; i++) {
        if (enemigos[i].activo && abs(jugador->x - enemigos[i].x) <= 1 && abs(jugador->y - enemigos[i].y) <= 1) {
            enemigos[i].activo = 0;
            jugador->score += 100;
        }
    }
}

int verificaColisionEnemigos(Jugador *jugador, Enemigo enemigos[], int n) {
    for (int i = 0; i < n; i++) {
        if (enemigos[i].activo && jugador->x == enemigos[i].x && jugador->y == enemigos[i].y) {
            jugador->vidas--;
            enemigos[i].activo = 0;
            return 1;
        }
    }
    return 0;
}

int main() {
    initscr();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLUE);
    init_pair(2, COLOR_RED, COLOR_BLUE);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLUE);
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    srand(time(NULL));

    pantallaBienvenida();

    clear();
    mvprintw(ALTO / 2, ANCHO / 2 - 10, "Dime tu nombre: ");
    refresh();

    char nombre[20];
    echo();
    scanw("%19s", nombre);
    noecho();

    int jugar = 1;
    while (jugar) {
        int nivel = 1;
        int enemigos_por_nivel[] = {5, 7, 10};
        int ganador = 0;

        time_t inicio_tiempo = time(NULL);
        const int TIEMPO_LIMITE = 120;
		Jugador jugador = {ANCHO / 2, ALTO / 2, 3, 0};
        while (nivel <= 3) {
            Enemigo enemigos[enemigos_por_nivel[nivel - 1]];
            Obstaculo obstaculos[nivel + 3];
            iniciaEnemigos(enemigos, enemigos_por_nivel[nivel - 1]);
            iniciaObstaculos(obstaculos, nivel + 3);

            int game_over = 0;

            while (!game_over) {
                time_t tiempo_actual = time(NULL);
                int tiempo_restante = TIEMPO_LIMITE - (int)(difftime(tiempo_actual, inicio_tiempo));

                if (tiempo_restante <= 0) {
                    pantallaResumen(nombre, jugador.score, 0, 1);
                    echo();
                    char respuesta;
                    scanw(" %c", &respuesta);
                    noecho();
                    if (respuesta == 'n') {
                        bkgdset(COLOR_PAIR(0)); 
                        clear(); 
                        refresh();
                        endwin();
                        return 0;
                    } else {
                        nivel = 1;
                        ganador = 0; 
                        inicio_tiempo = time(NULL);
                        game_over = 1;
                        continue; 
                    }
                }

                int tecla = getch();
                if (tecla == 'q') return 0;

                moverJugador(&jugador, tecla, obstaculos, nivel + 3);
                moverEnemigos(enemigos, enemigos_por_nivel[nivel - 1], jugador, obstaculos, nivel + 3);
                if (tecla == ' ') atacar(&jugador, enemigos, enemigos_por_nivel[nivel - 1]);
                verificaColisionEnemigos(&jugador, enemigos, enemigos_por_nivel[nivel - 1]);

                clear();

                attron(COLOR_PAIR(4));
                for (int y = 0; y <= ALTO; y++) {
                    for (int x = 0; x <= ANCHO; x++) {
                        mvprintw(y, x, " ");
                    }
                }
                attroff(COLOR_PAIR(4)); 

                attron(COLOR_PAIR(1));
                mvprintw(jugador.y, jugador.x, "@");
                attroff(COLOR_PAIR(1));

                for (int i = 0; i < enemigos_por_nivel[nivel - 1]; i++) {
                    if (enemigos[i].activo) {
                        attron(COLOR_PAIR(2));
                        mvprintw(enemigos[i].y, enemigos[i].x, "E");
                        attroff(COLOR_PAIR(2));
                    }
                }

                attron(COLOR_PAIR(3));
                for (int i = 0; i < nivel + 3; i++) {
                    mvprintw(obstaculos[i].y, obstaculos[i].x, "#");
                }
                attroff(COLOR_PAIR(3));

                dibujaBordes();

                attron(COLOR_PAIR(3));
                mvprintw(ALTO + 2, 1, "Vidas: %d | Nivel: %d | Tiempo restante: %d segundos | Score: %d",
                         jugador.vidas, nivel, tiempo_restante, jugador.score);
                attroff(COLOR_PAIR(3));

                refresh();

                if (jugador.vidas <= 0) {
                    game_over = 1;
                    break;
                }

                int enemigos_restantes = 0;
                for (int i = 0; i < enemigos_por_nivel[nivel - 1]; i++)
                    enemigos_restantes += enemigos[i].activo;
                if (enemigos_restantes == 0) {
                    nivel++;
                    break;
                }

                usleep(VELOCIDAD);
            }

            if (jugador.vidas <= 0)
                break;
        }

        if (nivel > 3 && ganador == 0) {
            ganador = 1;
        }

        pantallaResumen(nombre, jugador.score, ganador, 0);
        echo();
        char respuesta;
        scanw(" %c", &respuesta);
        noecho();
        if (respuesta == 'n') {
            bkgdset(COLOR_PAIR(0));
            clear();
            refresh();
            jugar = 0;
            endwin();
            return 0;
        }
    }
    endwin();
    return 0;
}

