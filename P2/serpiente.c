#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define VELOCIDAD 100000 // Tiempo en microsegundos entre movimientos

typedef struct
{
    int x, y;
} Punto; // Define un tipo Punto que representa la serpiente y la comida

Punto serpiente[100];
int largo_serpiente = 5;
Punto comida;
int dir_x = 1, dir_y = 0; // Movimiento inicial hacia la derecha
int ANCHO, ALTO;

void init_game();
void dibujar_juego();
void mover_serpiente();
int comprobar_colision();
void generar_comida();
void pantalla_bienvenida();
void game_over();

int main()
{
    int ch;
    srand(time(NULL));
    initscr();
    getmaxyx(stdscr, ALTO, ANCHO); // Para obtener el ancho y alto de la terminal
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    timeout(0);

    pantalla_bienvenida();
    init_game();

    while (1)
    {
        ch = getch();
        switch (ch)
        {
        case KEY_UP:
            if (dir_y != 1)
            {
                dir_x = 0;
                dir_y = -1;
            }
            break;
        case KEY_DOWN:
            if (dir_y != -1)
            {
                dir_x = 0;
                dir_y = 1;
            }
            break;
        case KEY_LEFT:
            if (dir_x != 1)
            {
                dir_x = -1;
                dir_y = 0;
            }
            break;
        case KEY_RIGHT:
            if (dir_x != -1)
            {
                dir_x = 1;
                dir_y = 0;
            }
            break;
        }

        mover_serpiente();

        if (comprobar_colision())
        {
            game_over();
        }

        dibujar_juego();
        usleep(VELOCIDAD);
    }

    endwin();
    return 0;
}

void init_game()
{
    largo_serpiente = 5; // Largo de la serpiente
    dir_x = 1;           // Mirando a la derecha
    dir_y = 0;

    // Colocar la serpiente en mitad de la pantalla
    for (int i = 0; i < largo_serpiente; i++)
    {
        serpiente[i].x = ANCHO / 2 - i;
        serpiente[i].y = ALTO / 2;
    }

    generar_comida(); // Tenemos que generar la primera comida
}

void dibujar_juego()
{
    clear();
    box(stdscr, 0, 0);

    // Mostrar el puntaje en la parte superior de la pantalla
    // La puntuación será el largo de la serpiente - 5 porque es como se empieza
    mvprintw(0, 2, " SNAKE | Puntuación: %d ", largo_serpiente - 5);

    // Pinta la comida
    mvaddch(comida.y, comida.x, '@');

    for (int i = 0; i < largo_serpiente; i++)
    {
        if (i == 0)
        {
            mvaddch(serpiente[i].y, serpiente[i].x, 'O'); // Dibuja la cabeza con 'O'
        }
        else
        {
            mvaddch(serpiente[i].y, serpiente[i].x, 'o'); // Dibuja el cuerpo con 'o'
        }
    }
    refresh();
}

void mover_serpiente()
{
    // Calcular la nueva posición de la cabeza sumando las direcciones dir_x y dir_y a las coordenadas de la cabeza
    Punto new_head = {serpiente[0].x + dir_x, serpiente[0].y + dir_y};

    // Cada posición toma la posición del segmento anterior, deslizandose el cuerpo, hasta que ocupe la cabeza.
    for (int i = largo_serpiente; i > 0; i--)
    {
        serpiente[i] = serpiente[i - 1];
    }

    // La nueva cabeza se coloca en la posición calculada
    serpiente[0] = new_head;

    // Comprobar si la serpiente ha comido
    if (serpiente[0].x == comida.x && serpiente[0].y == comida.y)
    {
        largo_serpiente++;
        generar_comida();
    }
}

int comprobar_colision()
{
    // Comprobar el choque con bordes
    if (serpiente[0].x <= 0 || serpiente[0].x >= ANCHO - 1 ||
        serpiente[0].y <= 0 || serpiente[0].y >= ALTO - 1)
        return 1;

    // Comprobar el choque consigo misma
    for (int i = 1; i < largo_serpiente; i++)
    {
        if (serpiente[i].x == serpiente[0].x && serpiente[i].y == serpiente[0].y)
            return 1;
    }
    return 0;
}

void generar_comida()
{
    bool valid = false;
    while (!valid)
    {
        comida.x = rand() % (ANCHO - 2) + 1;
        comida.y = rand() % (ALTO - 2) + 1;

        valid = true; // asumimos que es válida

        // Recorremos el largo de la serpiente para no colocar comdia en el cuerpo

        for (int i = 0; i < largo_serpiente; i++)
        {
            if (comida.x == serpiente[i].x && comida.y == serpiente[i].y)
            {
                valid = false; // Si choca salimos y volvemos a calcular una nueva posición de comida
                break;
            }
        }
    }
}

void pantalla_bienvenida()
{
    clear();

    int y_max, x_max;
    getmaxyx(stdscr, y_max, x_max);
    int mid_y = y_max / 2;
    int mid_x = x_max / 2;

    box(stdscr, 0, 0); // dibuja un recuadro

    mvprintw(mid_y - 4, mid_x - 20, "=== Bienvenido a SERPIENTE en NCURSES ===");
    mvprintw(mid_y - 2, mid_x - 22, "Autores: Clara Sola Ruiz y Mario Casas Perez");

    mvprintw(mid_y, mid_x - 10, "Controles del juego:");
    mvprintw(mid_y + 1, mid_x - 12, "- Flechas: Mover la serpiente");
    mvprintw(mid_y + 3, mid_x - 16, "Come comida para crecer.");
    mvprintw(mid_y + 4, mid_x - 18, "Evita chocar contigo misma o los bordes.");

    mvprintw(mid_y + 6, mid_x - 22, "Presiona cualquier tecla para comenzar...!");

    refresh();

    timeout(-1); // Espera una tecla sin límite de tiempo
    getch();     // Espera la tecla del usuario
    timeout(0);  // Vuelve al modo sin bloqueo para el juego
}

void game_over()
{
    clear();
    box(stdscr, 0, 0);
    mvprintw(ALTO / 2 - 2, ANCHO / 2 - 6, "¡Juego terminado!");
    mvprintw(ALTO / 2, ANCHO / 2 - 8, "Puntuación: %d", largo_serpiente - 5);
    mvprintw(ALTO / 2 + 2, ANCHO / 2 - 11, "¿Jugar otra vez? (s/n)");
    refresh();

    int c;
    while ((c = getch())) // Ver si se continua o no el juego.
    {
        if (c == 's' || c == 'S')
        {
            init_game();
            return;
        }
        else if (c == 'n' || c == 'N')
        {
            endwin();
            exit(0);
        }
    }
}
