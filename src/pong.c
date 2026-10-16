#include <stdio.h>

/* Размеры игрового поля */
#define WIDTH 80
#define HEIGHT 25

/* Кнопки управления */
#define P1_UP 'A'
#define P1_DOWN 'Z'
#define P2_UP 'K'
#define P2_DOWN 'M'
#define SKIP_STEP ' '

/* Структура состояния всей игры */
struct GameState {
    int p1_y;
    int p2_y;
    int ball_x;
    int ball_y;
    int ball_dx;
    int ball_dy;
    int score1;
    int score2;
    int is_running;
};

/* Инициализация начального состояния раунда */
struct GameState init_round(struct GameState state) {
    state.ball_x = WIDTH / 2;
    state.ball_y = HEIGHT / 2;
    state.ball_dx = (state.ball_dx > 0) ? -1 : 1;
    state.ball_dy = 1;
    state.p1_y = HEIGHT / 2;
    state.p2_y = HEIGHT / 2;
    return state;
}

/* Стартовая инициализация игры */
struct GameState init_game() {
    struct GameState state;
    state.score1 = 0;
    state.score2 = 0;
    state.ball_dx = 1;
    state.is_running = 1;
    return init_round(state);
}

/* Обработка ввода игроков в пошаговом режиме */
struct GameState handle_input(struct GameState state) {
    char input_char;
    int valid_input = 0;

    /* Цикл ожидания корректного ввода */
    while (!valid_input) {
        if (scanf("%c", &input_char) == 1) {
            /* Пропускаем символы перевода строки */
            if (input_char == '\n') {
                continue;
            }

            /* Переводим строчные буквы в прописные */
            if (input_char >= 'a' && input_char <= 'z') {
                input_char = input_char - 'a' + 'A';
            }

            if (input_char == SKIP_STEP) {
                valid_input = 1;
            } else if (input_char == P1_UP) {
                if (state.p1_y > 2) state.p1_y--;
                valid_input = 1;
            } else if (input_char == P1_DOWN) {
                if (state.p1_y < HEIGHT - 3) state.p1_y++;
                valid_input = 1;
            } else if (input_char == P2_UP) {
                if (state.p2_y > 2) state.p2_y--;
                valid_input = 1;
            } else if (input_char == P2_DOWN) {
                if (state.p2_y < HEIGHT - 3) state.p2_y++;
                valid_input = 1;
            }
        }
    }
    return state;
}

/* Проверка столкновения мяча с ракеткой */
int check_paddle_collision(int ball_x, int ball_y, int paddle_x, int paddle_y) {
    int collision = 0;
    if (ball_x == paddle_x) {
        if (ball_y == paddle_y || ball_y == paddle_y - 1 || ball_y == paddle_y + 1) {
            collision = 1;
        }
    }
    return collision;
}

/* Физика движения мяча и проверка условий гола */
struct GameState update_physics(struct GameState state) {
    state.ball_x += state.ball_dx;
    state.ball_y += state.ball_dy;

    /* Отскок от верхней и нижней границ */
    if (state.ball_y <= 1 || state.ball_y >= HEIGHT - 2) {
        state.ball_dy = -state.ball_dy;
    }

    /* Отскок от ракеток (Левая на x=2, Правая на x=WIDTH-3) */
    if (check_paddle_collision(state.ball_x, state.ball_y, 2, state.p1_y)) {
        state.ball_dx = -state.ball_dx;
    } else if (check_paddle_collision(state.ball_x, state.ball_y, WIDTH - 3, state.p2_y)) {
        state.ball_dx = -state.ball_dx;
    }

    /* Проверка гола */
    if (state.ball_x <= 0) {
        state.score2++;
        state = init_round(state);
    } else if (state.ball_x >= WIDTH - 1) {
        state.score1++;
        state = init_round(state);
    }

    /* Условие завершения игры */
    if (state.score1 == 21 || state.score2 == 21) {
        state.is_running = 0;
    }

    return state;
}

/* Отрисовка кадра с помощью стандартного вывода */
void render(struct GameState state) {
    /* Очистка экрана переводом строк */
    for (int i = 0; i < 25; i++) {
        printf("\n");
    }

    /* Вывод счета */
    for (int x = 0; x < WIDTH; x++) {
        if (x == WIDTH / 2 - 5) {
            printf("%02d : %02d", state.score1, state.score2);
            x += 6;
        } else {
            printf(" ");
        }
    }
    printf("\n");

    /* Отрисовка поля */
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == 0 || y == HEIGHT - 1) {
                printf("-");
            } else if (x == 0 || x == WIDTH - 1) {
                printf("|");
            } else if (x == WIDTH / 2) {
                printf(".");
            } else if (x == 2 && (y == state.p1_y || y == state.p1_y - 1 || y == state.p1_y + 1)) {
                printf("]");
            } else if (x == WIDTH - 3 && (y == state.p2_y || y == state.p2_y - 1 || y == state.p2_y + 1)) {
                printf("[");
            } else if (x == state.ball_x && y == state.ball_y) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

/* Экран завершения игры */
void show_winner(struct GameState state) {
    if (state.score1 == 21) {
        printf("PLAYER 1 WINS!\n");
    } else {
        printf("PLAYER 2 WINS!\n");
    }
}

int main() {
    struct GameState game;
    game = init_game();
    
    /* Первичная отрисовка поля */
    render(game);

    /* Основной цикл пошаговой игры */
    while (game.is_running) {
        game = handle_input(game);
        game = update_physics(game);
        render(game);
    }

    show_winner(game);
    return 0;
}

