#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define BOARD_SIZE 8

// Структуры
struct ChessPiece {
    string type; // "P", "N", "B", "R", "Q", "K"
    char color;  // 'W' или 'B'
    int row;
    int col;
};

struct ChessBoard {
    ChessPiece* board[BOARD_SIZE][BOARD_SIZE]; // Массив указателей
};

struct Player {
    string name;
    char color; // 'W' или 'B'
    int totalTime; // Общее время игры
};

struct Move {
    ChessPiece* piece;
    int startRow, startCol;
    int endRow, endCol;
};

struct GameState {
    ChessBoard board;
    Player whitePlayer;
    Player blackPlayer;
    Player* currentPlayer;
    vector<Move> moveHistory;
};

struct Position {
    int row, col;
};

struct Game {
    ChessBoard board;
    Player whitePlayer;
    Player blackPlayer;
    Player* currentPlayer;
    vector<Move> moveHistory;
};

struct GameInput {
    string startPos, endPos;
    int startRow, startCol, endRow, endCol;
};

struct ValidationResult {
    bool isValid;
    string errorMessage;
};

struct GameSummary {
    Player whitePlayer;
    Player blackPlayer;
    int totalMoves;
    string winner;
};

// ---------------- Функции ----------------

// Инициализация фигуры
void initChessPiece(ChessPiece& piece, const string& type, char color, int row, int col) {
    piece.type = type;
    piece.color = color;
    piece.row = row;
    piece.col = col;
}

// Печать фигуры
void printChessPiece(const ChessPiece& piece) {
    cout << "Тип: " << piece.type << ", Цвет: " << piece.color
        << ", Позиция: (" << piece.row << ", " << piece.col << ")" << endl;
}

// Инициализация доски
void initChessBoard(ChessBoard& board) {
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            board.board[row][col] = nullptr;
        }
    }

    // Инициализация белых фигур
    for (int col = 0; col < BOARD_SIZE; ++col) {
        board.board[6][col] = new ChessPiece{ "P", 'W', 6, col }; // Пешки
    }
    board.board[7][0] = new ChessPiece{ "R", 'W', 7, 0 }; // Ладья
    board.board[7][7] = new ChessPiece{ "R", 'W', 7, 7 }; // Ладья
    board.board[7][1] = new ChessPiece{ "N", 'W', 7, 1 }; // Конь
    board.board[7][6] = new ChessPiece{ "N", 'W', 7, 6 }; // Конь
    board.board[7][2] = new ChessPiece{ "B", 'W', 7, 2 }; // Слон
    board.board[7][5] = new ChessPiece{ "B", 'W', 7, 5 }; // Слон
    board.board[7][3] = new ChessPiece{ "Q", 'W', 7, 3 }; // Королева
    board.board[7][4] = new ChessPiece{ "K", 'W', 7, 4 }; // Король

    // Инициализация чёрных фигур
    for (int col = 0; col < BOARD_SIZE; ++col) {
        board.board[1][col] = new ChessPiece{ "P", 'B', 1, col }; // Пешки
    }
    board.board[0][0] = new ChessPiece{ "R", 'B', 0, 0 }; // Ладья
    board.board[0][7] = new ChessPiece{ "R", 'B', 0, 7 }; // Ладья
    board.board[0][1] = new ChessPiece{ "N", 'B', 0, 1 }; // Конь
    board.board[0][6] = new ChessPiece{ "N", 'B', 0, 6 }; // Конь
    board.board[0][2] = new ChessPiece{ "B", 'B', 0, 2 }; // Слон
    board.board[0][5] = new ChessPiece{ "B", 'B', 0, 5 }; // Слон
    board.board[0][3] = new ChessPiece{ "Q", 'B', 0, 3 }; // Королева
    board.board[0][4] = new ChessPiece{ "K", 'B', 0, 4 }; // Король
}

// Печать доски
void printChessBoard(const ChessBoard& board) {
    char columns[] = "abcdefgh";
    cout << "  a b c d e f g h" << endl;
    for (int row = 0; row < BOARD_SIZE; ++row) {
        cout << BOARD_SIZE - row << " ";
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (board.board[row][col]) {
                cout << board.board[row][col]->type << " ";
            }
            else {
                cout << ". ";
            }
        }
        cout << endl;
    }
}

// Проверка хода
ValidationResult validateMove(const ChessBoard& board, const ChessPiece& piece, int newRow, int newCol) {
    ValidationResult result{ true, "" };

    if (newRow < 0 || newRow >= BOARD_SIZE || newCol < 0 || newCol >= BOARD_SIZE) {
        result.isValid = false;
        result.errorMessage = "Недопустимое перемещение!";
        return result;
    }

    ChessPiece* target = board.board[newRow][newCol];
    if (target && target->color == piece.color) {
        result.isValid = false;
        result.errorMessage = "Клетка занята своей фигурой!";
        return result;
    }

    if (piece.type == "N") {
        int rowDiff = abs(newRow - piece.row);
        int colDiff = abs(newCol - piece.col);
        if (!(rowDiff == 2 && colDiff == 1) && !(rowDiff == 1 && colDiff == 2)) {
            result.isValid = false;
            result.errorMessage = "Невозможное движение для коня!";
            return result;
        }
    }

    // Проверка движения фигуры в зависимости от типа
    if (piece.type == "P") {
        // Проверка для пешки
        int forwardStep = (piece.color == 'W') ? -1 : 1;
        if (newCol == piece.col && newRow == piece.row + forwardStep) {
            if (board.board[newRow][newCol] == nullptr) {
                result.isValid = true;
            }
            else if (abs(newCol - piece.col) == 1 && target && target->color != piece.color) {
                result.isValid = true;
            }
            else {
                result.isValid = false;
                result.errorMessage = "Невозможное движение пешкой.";
            }
        }
        else if (newCol == piece.col && newRow == piece.row + 2 * forwardStep &&
            piece.row == (piece.color == 'W' ? 6 : 1) && board.board[newRow][newCol] == nullptr) {
            result.isValid = true;
        }
        else {
            result.isValid = false;
            result.errorMessage = "Невозможное движение пешкой.";
        }
    }
    else if (piece.type == "B") {
        // Проверка для слона
        if (abs(newRow - piece.row) == abs(newCol - piece.col)) {
            int rowStep = (newRow > piece.row) ? 1 : -1;
            int colStep = (newCol > piece.col) ? 1 : -1;
            int r = piece.row + rowStep, c = piece.col + colStep;
            while (r != newRow && c != newCol) {
                if (board.board[r][c] != nullptr) {
                    result.isValid = false;
                    result.errorMessage = "Невозможно двигаться слоном.";
                    return result;
                }
                r += rowStep;
                c += colStep;
            }
            result.isValid = true;
        }
        else {
            result.isValid = false;
            result.errorMessage = "Невозможное движение слоном.";
        }
    }
    else if (piece.type == "R") {
        // Проверка для ладьи
        if (newRow == piece.row || newCol == piece.col) {
            if (newRow == piece.row) {
                int colStep = (newCol > piece.col) ? 1 : -1;
                int c = piece.col + colStep;
                while (c != newCol) {
                    if (board.board[piece.row][c] != nullptr) {
                        result.isValid = false;
                        result.errorMessage = "Невозможно двигаться ладьей.";
                        return result;
                    }
                    c += colStep;
                }
            }
            else {
                int rowStep = (newRow > piece.row) ? 1 : -1;
                int r = piece.row + rowStep;
                while (r != newRow) {
                    if (board.board[r][piece.col] != nullptr) {
                        result.isValid = false;
                        result.errorMessage = "Невозможно двигаться ладьей.";
                        return result;
                    }
                    r += rowStep;
                }
            }
            result.isValid = true;
        }
        else {
            result.isValid = false;
            result.errorMessage = "Невозможное движение ладьей.";
        }
    }
    else if (piece.type == "Q") {
        // Проверка для ферзя (слон + ладья)
        if (abs(newRow - piece.row) == abs(newCol - piece.col)) {
            int rowStep = (newRow > piece.row) ? 1 : -1;
            int colStep = (newCol > piece.col) ? 1 : -1;
            int r = piece.row + rowStep, c = piece.col + colStep;
            while (r != newRow && c != newCol) {
                if (board.board[r][c] != nullptr) {
                    result.isValid = false;
                    result.errorMessage = "Невозможно двигаться ферзем.";
                    return result;
                }
                r += rowStep;
                c += colStep;
            }
            result.isValid = true;
        }
        else if (newRow == piece.row || newCol == piece.col) {
            int rowStep = (newRow > piece.row) ? 1 : -1;
            int colStep = (newCol > piece.col) ? 1 : -1;
            if (newRow == piece.row) {
                int c = piece.col + colStep;
                while (c != newCol) {
                    if (board.board[piece.row][c] != nullptr) {
                        result.isValid = false;
                        result.errorMessage = "Невозможно двигаться ферзем.";
                        return result;
                    }
                    c += colStep;
                }
            }
            else {
                int r = piece.row + rowStep;
                while (r != newRow) {
                    if (board.board[r][piece.col] != nullptr) {
                        result.isValid = false;
                        result.errorMessage = "Невозможно двигаться ферзем.";
                        return result;
                    }
                    r += rowStep;
                }
            }
            result.isValid = true;
        }
        else {
            result.isValid = false;
            result.errorMessage = "Невозможное движение ферзем.";
        }
    }
    else if (piece.type == "K") {
        // Проверка для короля
        if (abs(newRow - piece.row) <= 1 && abs(newCol - piece.col) <= 1) {
            result.isValid = true;
        }
        else {
            result.isValid = false;
            result.errorMessage = "Невозможное движение королем.";
        }
    }
    else {
        result.isValid = false;
        result.errorMessage = "Неизвестная фигура.";
    }

    return result;
}


// Перемещение фигуры
bool moveChessPiece(ChessBoard& board, ChessPiece& piece, int newRow, int newCol) {
    if (newRow < 0 || newRow >= BOARD_SIZE || newCol < 0 || newCol >= BOARD_SIZE) {
        cout << "Недопустимое перемещение!" << endl;
        return false;
    }

    ChessPiece* target = board.board[newRow][newCol];
    if (target && target->color == piece.color) {
        cout << "Клетка занята своей фигурой!" << endl;
        return false;
    }

    if (piece.type == "N") { // Проверка хода коня
        int rowDiff = abs(newRow - piece.row);
        int colDiff = abs(newCol - piece.col);
        if (!(rowDiff == 2 && colDiff == 1) && !(rowDiff == 1 && colDiff == 2)) {
            cout << "Невозможное движение для коня." << endl;
            return false;
        }
    }

    // Проверка движения пешки
    if (piece.type == "P") {
        int forwardStep = (piece.color == 'W') ? -1 : 1;  // Вверх для белых, вниз для черных
        if (newCol == piece.col && newRow == piece.row + forwardStep) {
            // Обычное движение на 1 клетку вперед
            if (board.board[newRow][newCol] == nullptr) {  // Клетка должна быть пустой
                board.board[piece.row][piece.col] = nullptr;
                piece.row = newRow;
                piece.col = newCol;
                board.board[newRow][newCol] = &piece;
                return true;
            }
        }
        else if (newCol == piece.col && newRow == piece.row + 2 * forwardStep &&
            piece.row == (piece.color == 'W' ? 6 : 1) && board.board[newRow][newCol] == nullptr) {
            // Движение на 2 клетки вперед с начальной позиции
            board.board[piece.row][piece.col] = nullptr;
            piece.row = newRow;
            piece.col = newCol;
            board.board[newRow][newCol] = &piece;
            return true;
        }
        else if (abs(newCol - piece.col) == 1 && newRow == piece.row + forwardStep &&
            target && target->color != piece.color) {
            // Захват диагонально
            board.board[piece.row][piece.col] = nullptr;
            piece.row = newRow;
            piece.col = newCol;
            board.board[newRow][newCol] = &piece;
            return true;
        }
    }

    cout << "Невозможно выполнить ход пешкой." << endl;
    return false;
}


// Разбиение ввода
GameInput parseInput(const string& input) {
    GameInput gameInput;
    gameInput.startPos = input.substr(0, 2);
    gameInput.endPos = input.substr(3, 2);

    char startColChar = gameInput.startPos[0];
    char startRowChar = gameInput.startPos[1];
    char endColChar = gameInput.endPos[0];
    char endRowChar = gameInput.endPos[1];

    gameInput.startRow = BOARD_SIZE - (startRowChar - '0');
    gameInput.startCol = startColChar - 'a';
    gameInput.endRow = BOARD_SIZE - (endRowChar - '0');
    gameInput.endCol = endColChar - 'a';

    return gameInput;
}

// Игровой процесс
void playGame(Game& game) {
    string input;
    while (true) {
        printChessBoard(game.board);
        cout << "Ход игрока " << (game.currentPlayer->name) << " (" << game.currentPlayer->color << "): ";
        cin >> input;

        if (input == "quit" || input == "exit") {
            cout << "Игра завершена." << endl;
            break;
        }

        GameInput gameInput = parseInput(input);

        ChessPiece* piece = game.board.board[gameInput.startRow][gameInput.startCol];
        if (!piece || piece->color != game.currentPlayer->color) {
            cout << "Некорректный выбор фигуры или не ваша фигура." << endl;
            continue;
        }

        ValidationResult result = validateMove(game.board, *piece, gameInput.endRow, gameInput.endCol);
        if (!result.isValid) {
            cout << result.errorMessage << endl;
            continue;
        }

        if (moveChessPiece(game.board, *piece, gameInput.endRow, gameInput.endCol)) {
            game.moveHistory.push_back({ piece, gameInput.startRow, gameInput.startCol, gameInput.endRow, gameInput.endCol });
            game.currentPlayer = (game.currentPlayer == &game.whitePlayer) ? &game.blackPlayer : &game.whitePlayer;
        }
    }
}

// Сводка об игре
GameSummary getGameSummary(const Game& game) {
    GameSummary summary;
    summary.whitePlayer = game.whitePlayer;
    summary.blackPlayer = game.blackPlayer;
    summary.totalMoves = game.moveHistory.size();
    summary.winner = (game.currentPlayer->color == 'W') ? "Черный" : "Белый";
    return summary;
}

// Функция для перезапуска игры
void restartGame(Game& game) {
    game.moveHistory.clear(); // Очистить историю ходов
    initChessBoard(game.board); // Инициализация новой доски
    game.currentPlayer = &game.whitePlayer; // Установить текущего игрока
    cout << "Игра началась заново!" << endl;
    printChessBoard(game.board); // Показать начальное состояние доски
}


int main() {
    setlocale(2, "rus");
    Game game;
    game.whitePlayer = { "Белый игрок", 'W', 0 };
    game.blackPlayer = { "Черный игрок", 'B', 0 };
    game.currentPlayer = &game.whitePlayer;

    initChessBoard(game.board);  // Инициализация доски


    string input;

    while (true) {
        //system("CLS");
        cout << "\nТекущий игрок: " << game.currentPlayer->name << " (" << game.currentPlayer->color << ")" << endl;
        printChessBoard(game.board);
        cout << "Введите ход (например, e2 e4) или 'exit' для выхода, 'restart' для перезапуска игры: ";
        getline(cin, input);

        if (input == "exit") {
            cout << "Игра завершена." << endl;
            break;
        }
        else if (input == "restart") {
            restartGame(game);  // Перезапускаем игру
        }
        else {
            GameInput gameInput = parseInput(input);
            ChessPiece* piece = game.board.board[gameInput.startRow][gameInput.startCol];

            if (!piece || piece->color != game.currentPlayer->color) {
                cout << "Выбранная клетка пуста или фигура принадлежит сопернику!" << endl;
                continue;
            }

            if (moveChessPiece(game.board, *piece, gameInput.endRow, gameInput.endCol)) {
                game.moveHistory.push_back({ piece, gameInput.startRow, gameInput.startCol, gameInput.endRow, gameInput.endCol });
                game.currentPlayer = (game.currentPlayer == &game.whitePlayer) ? &game.blackPlayer : &game.whitePlayer;
            }
            else {
                cout << "Невозможно выполнить ход. Попробуйте снова." << endl;
            }
        }
    }

    GameSummary summary = getGameSummary(game);
    cout << "\nИгра завершена!" << endl;
    cout << "Общее количество ходов: " << summary.totalMoves << endl;
    cout << "Победитель: " << summary.winner << " игрок" << endl;
    return 0;
}