#include <stdlib.h>
#include <iostream>

#define BOARD_SIZE 8

typedef struct {
    char type[3];   // "P", "N", "B", "R", "Q", "K"
    char color;     // 'W' или 'B'
    int row;
    int col;
} ChessPiece;

typedef struct {
    ChessPiece board[BOARD_SIZE][BOARD_SIZE];  // 8x8 доска
} ChessBoard;

typedef struct {
    ChessBoard board;
    char currentPlayer;  // Текущий игрок 'W' или 'B'
    int totalMoves;      // Количество сделанных ходов
    int gameStatus;      // Статус игры (0 - продолжается, 1 - белые победили, -1 - черные победили, 2 - ничья)
} ChessGame;

typedef struct {
    char winner[2];     // Победившая сторона 'W' или 'B'
    int totalMoves;
    char date[11];      // "YYYY-MM-DD"
} MatchHistory;

void initializeBoard(ChessBoard* board) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            ChessPiece piece = { "", ' ', row, col };
            board->board[row][col] = piece;
        }
    }
}

void printBoard(ChessBoard* board) {
    char columns[] = "abcdefgh";
    printf("  a b c d e f g h\n");
    for (int row = 0; row < BOARD_SIZE; row++) {
        printf("%d ", BOARD_SIZE - row);  // Нумерация строк сверху вниз
        for (int col = 0; col < BOARD_SIZE; col++) {
            ChessPiece piece = board->board[row][col];
            if (piece.type[0] != '\0') {
                printf("%c ", piece.type[0]);
            }
            else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

int movePiece(ChessBoard* board, ChessPiece* piece, int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return 0;  // Ход вне границ
    }

    ChessPiece target = board->board[row][col];
    if (target.type[0] != '\0' && target.color == piece->color) {
        return 0;  // Нельзя двигаться на фигуру того же цвета
    }

    board->board[row][col] = *piece;
    piece->row = row;
    piece->col = col;

    return 1;  // Успешное движение
}

int main() {
    setlocale(2, "rus");
    ChessGame game;
    initializeBoard(&game.board);
    game.currentPlayer = 'W';  // Белые начинают
    game.totalMoves = 0;
    game.gameStatus = 0;  // Игра продолжается

    printf("Начало игры\n");
    printBoard(&game.board);

    // Первый ход белой пешки
    ChessPiece pawnWhite = { "P", 'W', 6, 4 };  // Пешка белых из начальной позиции
    if (movePiece(&game.board, &pawnWhite, 4, 4)) {
        printf("Ход выполнен: белая пешка на e4\n");
    }
    else {
        printf("Ошибка при движении пешки белых\n");
    }

    printBoard(&game.board);

    // Ход черной пешки
    ChessPiece pawnBlack = { "P", 'B', 1, 4 };  // Пешка черных из начальной позиции
    if (movePiece(&game.board, &pawnBlack, 3, 4)) {
        printf("Ход выполнен: черная пешка на e5\n");
    }
    else {
        printf("Ошибка при движении пешки черных\n");
    }

    printBoard(&game.board);

    // Ход слона белых
    ChessPiece bishopWhite = { "B", 'W', 7, 2 };  // Белый слон из начальной позиции
    if (movePiece(&game.board, &bishopWhite, 5, 4)) {
        printf("Ход выполнен: белый слон на d4\n");
    }
    else {
        printf("Ошибка при движении слона белых\n");
    }

    printBoard(&game.board);

    // Ход коня черных
    ChessPiece knightBlack = { "N", 'B', 0, 1 };  // Черный конь из начальной позиции
    if (movePiece(&game.board, &knightBlack, 2, 2)) {
        printf("Ход выполнен: черный конь на c3\n");
    }
    else {
        printf("Ошибка при движении коня черных\n");
    }

    printBoard(&game.board);

    // Конец игры
    return 0;
}