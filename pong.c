#include <stdio.h>

// Размеры поля и ракеток
#define FIELD_WIDTH 80
#define FIELD_HEIGHT 25
#define PADDLE_HEIGHT 3

void drawLine();
void drawScoreLine(int firstPlayerScore, int secondPlayerScore);
void drawPlayField(int firstPaddleXPosition, int firstPaddleYPosition, int secondPaddleXPosition,
                   int secondPaddleYPosition, int ballXPosition, int ballYPosition);
void drawFrame(int firstPlayerScore, int secondPlayerScore, int firstPaddleXPosition,
               int firstPaddleYPosition, int secondPaddleXPosition, int secondPaddleYPosition,
               int ballXPosition, int ballYPosition);
void drawTwinBender(int firstPlayerScore, int secondPlayerScore);
void drawWinFrame(int firstPlayerScore, int secondPlayerScore, int winnerID);
void chooseWinner(int firstPlayerScore, int secondPlayerScore);
int changeBallXSpeed(int ballXPosition, int ballYPosition, int ballXSpeed, int firstPaddleXPosition,
                     int firstPaddleYPosition, int secondPaddleXPosition, int secondPaddleYPosition);
int changeBallYSpeed(int ballXPosition, int ballYPosition, int ballYSpeed, int firstPaddleXPosition,
                     int firstPaddleYPosition, int secondPaddleXPosition, int secondPaddleYPosition);
int moveFirstPaddle(int input, int firstPaddleYPosition, int paddleSpeed);
int moveSecondPaddle(int input, int secondPaddleYPosition, int paddleSpeed);

int main() {
    //  Счет игроков
    int firstPlayerScore = 0;
    int secondPlayerScore = 0;

    // Флаг продолжения игры
    int isGameOn = 1;

    // Флаги выхода мяча за игровое поле
    int isBallOutLeft = 0;
    int isBallOutRight = 0;

    // Позиция и скорость мяча
    int ballXPosition = FIELD_WIDTH / 2 + 1;
    int ballYPosition = FIELD_HEIGHT / 2;
    int ballXSpeed = 1;
    int ballYSpeed = 1;

    // Позиции и скорости ракеток
    int paddleSpeed = 1;
    int firstPaddleXPosition = 3;
    int firstPaddleYPosition = FIELD_HEIGHT / 2;
    int secondPaddleXPosition = FIELD_WIDTH - 1;
    int secondPaddleYPosition = FIELD_HEIGHT / 2;

    // Главный цикл игры, переключает ходы
    while (isGameOn) {
        // Флаг перехода на следующий ход
        int isStayThisTurn = 1;
        // Цикл ввода от пользователя
        while (isStayThisTurn) {
            isStayThisTurn = 0;

            drawFrame(firstPlayerScore, secondPlayerScore, firstPaddleXPosition, firstPaddleYPosition,
                      secondPaddleXPosition, secondPaddleYPosition, ballXPosition, ballYPosition);

            // Чтение ввода пользователя
            char input, next;
            scanf("%c%c", &input, &next);

            // Обработка ввода для управления ракетками
            if ((input == 'A' || input == 'Z') && next == '\n') {
                firstPaddleYPosition = moveFirstPaddle(input, firstPaddleYPosition, paddleSpeed);
            } else if ((input == 'K' || input == 'M') && next == '\n') {
                secondPaddleYPosition = moveSecondPaddle(input, secondPaddleYPosition, paddleSpeed);
            } else if (input == ' ' && next == '\n') {
                isStayThisTurn = 0;
            } else if (input == 'Q' && next == '\n') {
                isGameOn = 0;
                chooseWinner(firstPlayerScore, secondPlayerScore);
            } else {
                char ch = ' ';
                while (ch != '\n') {
                    scanf("%c", &ch);
                }
                isStayThisTurn = 1;
            }
        }

        // Условие выхода мяча за игровое поле
        if (ballXPosition < 2) {
            isBallOutLeft = 1;
        } else if (ballXPosition > FIELD_WIDTH) {
            isBallOutRight = 1;
        }

        // Обработка движения мяча
        if (isBallOutLeft || isBallOutRight) {
            ballXPosition = FIELD_WIDTH / 2;
            ballYPosition = FIELD_HEIGHT / 2;
            firstPaddleYPosition = FIELD_HEIGHT / 2;
            secondPaddleYPosition = FIELD_HEIGHT / 2;

            if (isBallOutLeft) {
                secondPlayerScore += 1;
                isBallOutLeft = 0;
            } else if (isBallOutRight) {
                firstPlayerScore += 1;
                isBallOutRight = 0;
            }

            if (firstPlayerScore >= 21 || secondPlayerScore >= 21) {
                isGameOn = 0;
                chooseWinner(firstPlayerScore, secondPlayerScore);
            }
        } else {
            ballXSpeed = changeBallXSpeed(ballXPosition, ballYPosition, ballXSpeed, firstPaddleXPosition,
                                          firstPaddleYPosition, secondPaddleXPosition, secondPaddleYPosition);
            ballYSpeed = changeBallYSpeed(ballXPosition, ballYPosition, ballYSpeed, firstPaddleXPosition,
                                          firstPaddleYPosition, secondPaddleXPosition, secondPaddleYPosition);
            ballXPosition += ballXSpeed;
            ballYPosition += ballYSpeed;
        }
    }
    return 0;
}

// Функция отрисовки горизонтальных границ
void drawLine() {
    for (int i = 0; i <= FIELD_WIDTH + 2; i++) printf("=");
    printf("\n");
}

// Функция отрисовки счета игроков
void drawScoreLine(int firstPlayerScore, int secondPlayerScore) {
    if (firstPlayerScore < 10 && secondPlayerScore < 10) {
        printf("| \tPlayer 1: %d\t|\tPlayer 2: %d\t|   Press \"Q\" to exit the game    |", firstPlayerScore,
               secondPlayerScore);
    } else if (firstPlayerScore >= 10 && secondPlayerScore >= 10) {
        printf("| \tPlayer 1: %d\t|\tPlayer 2: %d\t|   Press \"Q\" to exit the game    |", firstPlayerScore,
               secondPlayerScore);
    } else {
        printf("| \tPlayer 1: %d\t |\tPlayer 2: %d\t |  Press \"Q\" to exit the game    |", firstPlayerScore,
               secondPlayerScore);
    }
    printf("\n");
}

// Функция отрисовки основного поля
void drawPlayField(int firstPaddleXPosition, int firstPaddleYPosition, int secondPaddleXPosition,
                   int secondPaddleYPosition, int ballXPosition, int ballYPosition) {
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x <= FIELD_WIDTH + 2; x++) {
            // Отрисовка боковых границ
            if (x == 0 || x == FIELD_WIDTH + 2) {
                printf("|");
            }
            // Отрисовка ракетки 1
            else if (x == firstPaddleXPosition && y >= firstPaddleYPosition - PADDLE_HEIGHT / 2 &&
                     y <= firstPaddleYPosition + PADDLE_HEIGHT / 2) {
                printf("|");
            }
            // Отрисовка ракетки 2
            else if (x == secondPaddleXPosition && y >= secondPaddleYPosition - PADDLE_HEIGHT / 2 &&
                     y <= secondPaddleYPosition + PADDLE_HEIGHT / 2) {
                printf("|");
            }
            // Отрисовка мяча
            else if (x == ballXPosition && y == ballYPosition) {
                printf("o");
            }
            // Отрисовка пустой ячейки
            else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

// Функция для отображения кадра игры
void drawFrame(int firstPlayerScore, int secondPlayerScore, int firstPaddleXPosition,
               int firstPaddleYPosition, int secondPaddleXPosition, int secondPaddleYPosition,
               int ballXPosition, int ballYPosition) {
    // Очистка экрана
    printf("\033[2J");

    drawLine();
    drawScoreLine(firstPlayerScore, secondPlayerScore);
    drawLine();
    drawPlayField(firstPaddleXPosition, firstPaddleYPosition, secondPaddleXPosition, secondPaddleYPosition,
                  ballXPosition, ballYPosition);
    drawLine();
}

void drawTwinBender(int firstPlayerScore, int secondPlayerScore) {
    // Очистка экрана
    printf("\033[2J");
    printf("    _________________________________\n");
    printf("   |.--------_--_------------_--__--.|\n");
    printf("   ||    /\\ |_)|_)|   /\\ | |(_ |_   ||\n");
    printf("   ;;`,_/``\\|__|__|__/``\\|_| _)|__ ,:|\n");
    printf("  ((_(-,--------------.-.----------.-.)`)\n");
    printf("   \\__ )        ,'       `.       \\ _/\n");
    printf("    :  :        |_________|       :  :\n");
    printf("    |-'|       ,'-.-.--.-.`.      |`-|\n");
    printf("    |_.|      (( (*  )(*  )))     |._|\n");
    printf("    |  |       `.-`-'--`-'.'      |  |\n");
    printf("    |-'|        | ,-.-.-. |       |._|\n");
    printf("    |  |        |(|-|-|-|)|       |  |\n");
    printf("    :,':        |_`-'-'-'_|       ;`.;\n");
    printf("     \\  \\     ,'           `.    /._/\n");
    printf("      \\/ `._ /_______________\\_,'  /\n");
    printf("       \\  / :   ___________   : \\,'\n");
    printf("        `.| |  |           |  |,'\n");
    printf("          `.|  |  To both  |  |\n");
    printf("            |  |  players  |  |\n");
    if (firstPlayerScore < 10 && secondPlayerScore < 10) {
        printf("            |  |   %d : %d   |  |\n", firstPlayerScore, secondPlayerScore);
    } else if (firstPlayerScore >= 10 && secondPlayerScore >= 10) {
        printf("            |  |  %d : %d  |  |\n", firstPlayerScore, secondPlayerScore);
    } else {
        printf("            |  |  %d : %d   |  |\n", firstPlayerScore, secondPlayerScore);
    }
    printf("            |  |           |  |\n");
    printf("   -------------------------------------\n");
}

void drawWinFrame(int firstPlayerScore, int secondPlayerScore, int winnerID) {
    // Очистка экрана
    printf("\033[2J");
    printf("    _________________________________\n");
    printf("   |.--------_--_------------_--__--.|\n");
    printf("   ||    /\\ |_)|_)|   /\\ | |(_ |_   ||\n");
    printf("   ;;`,_/``\\|__|__|__/``\\|_| _)|__ ,:|\n");
    printf("  ((_(-,--------------.-.----------.-.)`)\n");
    printf("   \\__ )        ,'       `.       \\ _/\n");
    printf("    :  :        |_________|       :  :\n");
    printf("    |-'|       ,'-.-.--.-.`.      |`-|\n");
    printf("    |_.|      (( (*  )(*  )))     |._|\n");
    printf("    |  |       `.-`-'--`-'.'      |  |\n");
    printf("    |-'|        | ,-.-.-. |       |._|\n");
    printf("    |  |        |(|-|-|-|)|       |  |\n");
    printf("    :,':        |_`-'-'-'_|       ;`.;\n");
    printf("     \\  \\     ,'           `.    /._/\n");
    printf("      \\/ `._ /_______________\\_,'  /\n");
    printf("       \\  / :   ___________   : \\,'\n");
    printf("        `.| |  |           |  |,'\n");
    printf("          `.|  |    To     |  |\n");
    printf("            |  | Player  %d |  |\n", winnerID);
    if (firstPlayerScore < 10 && secondPlayerScore < 10) {
        printf("            |  |   %d : %d   |  |\n", firstPlayerScore, secondPlayerScore);
    } else if (firstPlayerScore >= 10 && secondPlayerScore >= 10) {
        printf("            |  |  %d : %d  |  |\n", firstPlayerScore, secondPlayerScore);
    } else {
        printf("            |  |  %d : %d   |  |\n", firstPlayerScore, secondPlayerScore);
    }
    printf("            |  |           |  |\n");
    printf("   -------------------------------------\n");
}

// Функция выбора победителя
void chooseWinner(int firstPlayerScore, int secondPlayerScore) {
    int winnerID;
    if (firstPlayerScore == secondPlayerScore) {
        drawTwinBender(firstPlayerScore, secondPlayerScore);
    } else if (firstPlayerScore > secondPlayerScore) {
        winnerID = 1;
        drawWinFrame(firstPlayerScore, secondPlayerScore, winnerID);
    } else {
        winnerID = 2;
        drawWinFrame(firstPlayerScore, secondPlayerScore, winnerID);
    }
}

// Функция для изменения скорости мяча по оси X
int changeBallXSpeed(int ballXPosition, int ballYPosition, int ballXSpeed, int firstPaddleXPosition,
                     int firstPaddleYPosition, int secondPaddleXPosition, int secondPaddleYPosition) {
    if ((ballXPosition == firstPaddleXPosition + 1 &&
         ballYPosition <= firstPaddleYPosition + PADDLE_HEIGHT / 2 &&
         ballYPosition >= firstPaddleYPosition - PADDLE_HEIGHT / 2) ||
        (ballXPosition == secondPaddleXPosition - 1 &&
         ballYPosition <= secondPaddleYPosition + PADDLE_HEIGHT / 2 &&
         ballYPosition >= secondPaddleYPosition - PADDLE_HEIGHT / 2)) {
        ballXSpeed *= -1;
    }
    if ((ballXPosition == firstPaddleXPosition + 1 &&
         (ballYPosition == firstPaddleYPosition + PADDLE_HEIGHT / 2 + 1 ||
          ballYPosition == firstPaddleYPosition - PADDLE_HEIGHT / 2 - 1)) ||
        (ballXPosition == secondPaddleXPosition - 1 &&
         (ballYPosition == secondPaddleYPosition + PADDLE_HEIGHT / 2 + 1 ||
          ballYPosition == secondPaddleYPosition - PADDLE_HEIGHT / 2 - 1))) {
        ballXSpeed *= -1;
    }
    return ballXSpeed;
}

// Функция для изменения скорости мяча по оси Y
int changeBallYSpeed(int ballXPosition, int ballYPosition, int ballYSpeed, int firstPaddleXPosition,
                     int firstPaddleYPosition, int secondPaddleXPosition, int secondPaddleYPosition) {
    if (ballYPosition == 0 || ballYPosition == FIELD_HEIGHT - 1) {
        ballYSpeed *= -1;
    }
    if ((ballXPosition == firstPaddleXPosition + 1 &&
         (ballYPosition == firstPaddleYPosition + PADDLE_HEIGHT / 2 + 1 ||
          ballYPosition == firstPaddleYPosition - PADDLE_HEIGHT / 2 - 1)) ||
        (ballXPosition == secondPaddleXPosition - 1 &&
         (ballYPosition == secondPaddleYPosition + PADDLE_HEIGHT / 2 + 1 ||
          ballYPosition == secondPaddleYPosition - PADDLE_HEIGHT / 2 - 1))) {
        ballYSpeed *= -1;
    }
    return ballYSpeed;
}

// Функция для изменения положения первой ракетки
int moveFirstPaddle(int input, int firstPaddleYPosition, int paddleSpeed) {
    if (input == 'A') {
        if (firstPaddleYPosition > PADDLE_HEIGHT / 2) {
            firstPaddleYPosition -= paddleSpeed;
        }
    } else if (input == 'Z') {
        if (firstPaddleYPosition < FIELD_HEIGHT - PADDLE_HEIGHT / 2 - 1) {
            firstPaddleYPosition += paddleSpeed;
        }
    }
    return firstPaddleYPosition;
}

// Функция для изменения положения второй ракетки
int moveSecondPaddle(int input, int secondPaddleYPosition, int paddleSpeed) {
    if (input == 'K') {
        if (secondPaddleYPosition > PADDLE_HEIGHT / 2) {
            secondPaddleYPosition -= paddleSpeed;
        }
    } else if (input == 'M') {
        if (secondPaddleYPosition < FIELD_HEIGHT - PADDLE_HEIGHT / 2 - 1) {
            secondPaddleYPosition += paddleSpeed;
        }
    }
    return secondPaddleYPosition;
}
