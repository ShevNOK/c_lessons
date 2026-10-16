#include <ncurses.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25

#define P1_UP 'A'
#define P1_DOWN 'Z'
#define P2_UP 'K'
#define P2_DOWN 'M'
#define EXIT_KEY 'Q'

#define DELAY 50000

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

struct GameState init_round(struct GameState state) {
    state.ball_x = WIDTH / 2;
    state.ball_y = HEIGHT / 2;
    state.ball_dx = (state.ball_dx > 0) ? -1 : 1;
    state.ball_dy = 1;
    state.p1_y = HEIGHT / 2;
    state.p2_y = HEIGHT / 2;
    return state;
}

struct GameState init_game() {
    struct GameState state;
    state.score1 = 0;
    state.score2 = 0;
    state.ball_dx = 1;
    state.is_running = 1;
    return init_round(state);
}

struct GameState handle_input(struct GameState state) {
    int ch = getch();
    if (ch >= 'a' && ch <= 'z') {
        ch = ch - 'a' + 'A';
    }
    if (ch == EXIT_KEY) {
        state.is_running = 0;
    } else if (ch == P1_UP && state.p1_y > 2) {
        state.p1_y--;
    } else if (ch == P1_DOWN && state.p1_y < HEIGHT - 3) {
        state.p1_y++;
    } else if (ch == P2_UP && state.p2_y > 2) {
        state.p2_y--;
    } else if (ch == P2_DOWN && state.p2_y < HEIGHT - 3) {
        state.p2_y++;
    }
    return state;
}

int check_paddle_collision(int ball_x, int ball_y, int paddle_x, int paddle_y) {
    int collision = 0;
    if (ball_x == paddle_x) {
        if (ball_y == paddle_y || ball_y == paddle_y - 1 || ball_y == paddle_y + 1) {
            collision = 1;
        }
    }
    return collision;
}

struct GameState update_physics(struct GameState state) {
    state.ball_x += state.ball_dx;
    state.ball_y += state.ball_dy;

    if (state.ball_y <= 1 || state.ball_y >= HEIGHT - 2) {
        state.ball_dy = -state.ball_dy;
    }

    if (check_paddle_collision(state.ball_x, state.ball_y, 2, state.p1_y)) {
        state.ball_dx = -state.ball_dx;
    } else if (check_paddle_collision(state.ball_x, state.ball_y, WIDTH - 3, state.p2_y)) {
        state.ball_dx = -state.ball_dx;
    }

    if (state.ball_x <= 0) {
        state.score2++;
        state = init_round(state);
    } else if (state.ball_x >= WIDTH - 1) {
        state.score1++;
        state = init_round(state);
    }

    if (state.score1 == 21 || state.score2 == 21) {
        state.is_running = 0;
    }

    return state;
}

void render(struct GameState state) {
    clear();
    mvprintw(0, WIDTH / 2 - 5, "%02d : %02d", state.score1, state.score2);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == 0 || y == HEIGHT - 1) {
                mvaddch(y, x, '-');
            } else if (x == 0 || x == WIDTH - 1) {
                mvaddch(y, x, '|');
            } else if (x == WIDTH / 2) {
                mvaddch(y, x, '.');
            } else if (x == 2 && (y == state.p1_y || y == state.p1_y - 1 || y == state.p1_y + 1)) {
                mvaddch(y, x, ']');
            } else if (x == WIDTH - 3 && (y == state.p2_y || y == state.p2_y - 1 || y == state.p2_y + 1)) {
                mvaddch(y, x, '[');
            } else if (x == state.ball_x && y == state.ball_y) {
                mvaddch(y, x, '*');
            }
        }
    }
    refresh();
}

void show_winner(struct GameState state) {
    clear();
    if (state.score1 == 21) {
        mvprintw(HEIGHT / 2, WIDTH / 2 - 7, "PLAYER 1 WINS!");
    } else if (state.score2 == 21) {
        mvprintw(HEIGHT / 2, WIDTH / 2 - 7, "PLAYER 2 WINS!");
    } else {
        mvprintw(HEIGHT / 2, WIDTH / 2 - 5, "GAME OVER");
    }
    mvprintw(HEIGHT / 2 + 2, WIDTH / 2 - 12, "Press any key to exit...");
    refresh();
    nodelay(stdscr, FALSE);
    getch();
}

int main() {
    struct GameState game;
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    game = init_game();

    while (game.is_running) {
        game = handle_input(game);
        game = update_physics(game);
        render(game);
        usleep(DELAY);
    }

    show_winner(game);
    endwin();
    return 0;
}

