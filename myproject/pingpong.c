#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>

#define YELLOW  "\x1b[33m"
#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"
#define RESET   "\x1b[0m"

typedef struct A {
    int x;
    int y;
} nuqta;

typedef struct Oyin {
    nuqta raket_ch[3];
    nuqta raket_ong[3];
    nuqta top;
    int maydon[80][25];
    int gamer_achko1;
    int gamer_achko2;
} Oyin;

void maydon_chiz(Oyin Game) {
    printf(YELLOW " +");
    for(int j=1; j<80; ++j)printf("--");
    printf("--+" RESET);
    puts("");
    for (int i = 0; i < 25; ++i) {
        printf(YELLOW " |" RESET);
        for (int j = 0; j < 80; ++j) {
            if (Game.maydon[j][i] == 1) {
                printf(GREEN "| " RESET);
            } else if (Game.maydon[j][i] == 2) {
                printf(RED "O " RESET);
            } else if(j==39){
                printf(YELLOW "| " RESET);
            }else{
                printf("  ");
            }
        }
        printf(YELLOW "|" RESET);
        puts("");
    }
    printf(YELLOW " +");
    for(int j=0; j<80; ++j)printf("--");
    printf("+\n" RESET);
}

// Kiritishlarni terminaldan o'qish uchun funksiyalar
void klaviatura_kirish(struct termios *orig_termios) {
    struct termios raw;

    tcgetattr(STDIN_FILENO, orig_termios);
    raw = *orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void klaviatura_kirish_of(struct termios *orig_termios) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios);
}

int main() {
    char ch;
    int k_y_ch = 0;
    int k_y_ong = 0;
    int top_x = -1, top_y = -1;
    Oyin Game;
    Game.gamer_achko1=0;
    Game.gamer_achko2=0;
    struct termios orig_termios;

    klaviatura_kirish(&orig_termios);

    // Raketka va to'pning boshlang'ich pozitsiyalari
    for (int i = 0; i < 3; ++i) {
        Game.raket_ch[i].x = 2;
        Game.raket_ch[i].y = 11 + i;
        Game.raket_ong[i].x = 77;
        Game.raket_ong[i].y = 11 + i;
    }
    Game.top.x = 39;
    Game.top.y = 12;

    while (1) {
        // Maydonni yangilash
        for (int i = 0; i < 80; ++i) {
            for (int j = 0; j < 25; ++j) {
                Game.maydon[i][j] = 0;
            }
        }

        // Raketkalar va to'pni joylash
        for (int i = 0; i < 3; ++i) {
            Game.maydon[Game.raket_ch[i].x][Game.raket_ch[i].y + k_y_ch] = 1;
            Game.maydon[Game.raket_ong[i].x][Game.raket_ong[i].y + k_y_ong] = 1;
        }
        Game.maydon[Game.top.x][Game.top.y] = 2;

        // Maydonni chizish
        printf(YELLOW "                                                                         +============+\n" RESET);
        printf(YELLOW "                                                                         ||" RESET GREEN " %d  " RESET YELLOW "||" RESET GREEN " %d  " RESET YELLOW "||\n" RESET, Game.gamer_achko1, Game.gamer_achko2);
        printf(YELLOW "\\------------------------------------------------------------------------+============+----------------------------------------------------------------------------/\n" RESET);
        maydon_chiz(Game);
        printf(YELLOW "/------------------------------------------------------------------------+============+----------------------------------------------------------------------------\\\n" RESET);

        // To'p va raketka urishlarini tekshirish
        int urildi = 0;
        for (int i = 0; i < 3; ++i) {
            if ((Game.top.x == 3 && Game.raket_ch[i].y + k_y_ch == Game.top.y) ||
                (Game.top.x == 76 && Game.raket_ong[i].y + k_y_ong == Game.top.y)) {
                top_x = -top_x; // Faqat gorizontal yo'nalishni o'zgartirish
                urildi = 1;
                break;
            }
        }

        // Devorga urilish
        if (Game.top.y + top_y < 0 || Game.top.y + top_y > 24) {
            top_y = -top_y; // Vertikal yo'nalishni o'zgartirish
        }

        // O'yinni qayta boshlash
        if (Game.top.x + top_x < 0) {
            Game.top.x = 39;
            Game.top.y = 12;
            top_x = -top_x; // Harakatni teskari o'zgartirish
            k_y_ch = k_y_ong = 0;
            Game.gamer_achko2+=1;
        }
        if(Game.top.x + top_x > 79){
            Game.top.x = 39;
            Game.top.y = 12;
            top_x = -top_x; // Harakatni teskari o'zgartirish
            k_y_ch = k_y_ong = 0;
            Game.gamer_achko1+=1;
        }

        // To'pning pozitsiyasini yangilash
        Game.top.x += top_x;
        Game.top.y += top_y;

        // Raketkalarni harakatlantirish
        if (read(STDIN_FILENO, &ch, 1) == 1) {
            if (ch == 'a' && k_y_ch > -11) k_y_ch--;
            if (ch == 'z' && k_y_ch < 11) k_y_ch++;
            if (ch == 'k' && k_y_ong > -11) k_y_ong--;
            if (ch == 'm' && k_y_ong < 11) k_y_ong++;
        }

        // Ekranni tozalash
        system("clear");
    }

    klaviatura_kirish_of(&orig_termios);

    return 0;
}
