#include <iostream>
#include <string>
#include <vector>
// using namespace std;

class Icon{};

class Cell {
private:
    int y, x; // Coordinates of the cell
    std::string color;
    GenericPiece* content;

public:
    // Cell constructor
    
    Cell(int y, int x, GenericPiece* content, std::string color) : y(y), x(x), content(content), color(color) {}
    
    std::string toString() const {
        return "Cell [y=" + std::to_string(y) + ", x=" + std::to_string(x) + ", color=" + color + ", content=" + (content ? "Present" : "Absent") + "]";
    }

    // Getters

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

    GenericPiece* getContent() const {
        return content;
    }

    std::string getColor() const {
        return color;
    }

    // Setter
    void setContent(GenericPiece* content) {
        this->content = content;
    }

};

class GenericPiece {
protected:
    bool color;
    Icon icon;
    bool alive;

public:
    GenericPiece(bool color, Icon icon) : color(color), icon(icon), alive(true) {}

    bool isColor() const {
        return color;
    }

    bool isAlive() const {
        return alive;
    }

    void setAlive(bool alive) {
        this->alive = alive;
    }
    /*
    QLabel* getIcon() const {
        return icon;
    } 

    void setIcon(QLabel* icon) {
        this->icon = icon;
    } */

    // To be overridden
    virtual std::vector<Move> nextMoves(){
        return std::vector<Move>();
    }
};

class Move {
private:
    int fromX, fromY, toX, toY;
    bool valid, eat;

public:
    Move(int fromY, int fromX, int toY, int toX) : fromX(fromX), fromY(fromY), toX(toX), toY(toY), eat(false) {
        valid = controlValid();
    }

    Move(int fromY, int fromX, int toY, int toX, bool eat) : fromX(fromX), fromY(fromY), toX(toX), toY(toY), eat(eat) {
        valid = controlValid();
    }

    std::string toString() const {
        return "Move [fromX=" + std::to_string(fromX) + ", fromY=" + std::to_string(fromY) + ", toX=" + std::to_string(toX) +
               ", toY=" + std::to_string(toY) + ", valid=" + (valid ? "true" : "false") + ", eat=" + (eat ? "true" : "false") + "]";
    }

    bool controlValid() const {
        return !(fromX > 7 || fromX < 0 || fromY > 7 || fromY < 0 || toX > 7 || toX < 0 || toY > 7 || toY < 0);
    }

    bool operator==(const Move& other) const {
        return eat == other.eat && fromX == other.fromX && fromY == other.fromY && toX == other.toX && toY == other.toY && valid == other.valid;
    }

    bool operator!=(const Move& other) const {
        return !(*this == other);
    }

    size_t hashCode() const {
        // A simple hash function using XOR, you might need a more robust one
        return fromX ^ fromY ^ toX ^ toY ^ valid ^ eat;
    }

    int getFromX() const {
        return fromX;
    }

    int getFromY() const {
        return fromY;
    }

    int getToX() const {
        return toX;
    }

    int getToY() const {
        return toY;
    }

    bool isValid() const {
        return valid;
    }

    void setValid(bool valid) {
        this->valid = valid;
    }

    bool isEat() const {
        return eat;
    }

    void setEat(bool eat) {
        this->eat = eat;
    }
};

class Bishop : public GenericPiece {
public:
    Bishop(bool color, Icon icon) : GenericPiece(color, icon) {}

    std::vector<Move> nextMoves(Cell** chessboard, int Y, int X){
        std::vector<Move> next_moves;
        bool block[] = {false, false}; // block[0] is for diagonal1, block[1] is for diagonal2

        // Going ahead diagonally on y
        for (int i = Y + 1, l = X - 1, r = X + 1; i <= 7; ++i, --l, ++r) {
            if (l >= 0 && !block[0])
                diagonalMoveValidate(chessboard, next_moves, Y, X, i, l, block, 0);
            if (r <= 7 && !block[1])
                diagonalMoveValidate(chessboard, next_moves, Y, X, i, r, block, 1);
        }

        // Going back diagonally on y
        block[0] = block[1] = false;
        for (int i = Y - 1, l = X - 1, r = X + 1; i >= 0; --i, --l, ++r) {
            if (l >= 0 && !block[0])
                diagonalMoveValidate(chessboard, next_moves, Y, X, i, l, block, 0);
            if (r <= 7 && !block[1])
                diagonalMoveValidate(chessboard, next_moves, Y, X, i, r, block, 1);
        }

        return next_moves;
    }

private:
    void diagonalMoveValidate(Cell** chessboard, std::vector<Move>& next_moves, int fromY, int fromX, int toY, int toX, bool block[2], int diagonal) {
        if (chessboard[toY][toX].getContent() == nullptr)
            next_moves.push_back(Move(fromY, fromX, toY, toX, false));
        else {
            if (chessboard[toY][toX].getContent()->isColor() != color)
                next_moves.push_back(Move(fromY, fromX, toY, toX, true));
            block[diagonal] = true;
        }
    }
};

class Knight : public GenericPiece {
public:
    Knight(bool color, Icon icon) : GenericPiece(color, icon) {}

    Move convalidMove(const Move& move, Cell** board) const {
        if (board[move.getToY()][move.getToX()].getContent() != nullptr) {
            GenericPiece* temp = board[move.getToY()][move.getToX()].getContent();
            if (temp->isColor() == color)
                return move;
            else
                return Move(move.getFromY(), move.getFromX(), move.getToY(), move.getToX(), true);
        } else {
            return move;
        }
    }

    std::vector<Move> nextMoves(Cell** board, int Y, int X) const {
        Move possible_moves[] = {
            Move(Y, X, Y - 2, X - 1), Move(Y, X, Y - 2, X + 1),
            Move(Y, X, Y + 1, X + 2), Move(Y, X, Y - 1, X + 2),
            Move(Y, X, Y + 2, X + 1), Move(Y, X, Y + 2, X - 1),
            Move(Y, X, Y - 1, X - 2), Move(Y, X, Y + 1, X - 2)
        };

        std::vector<Move> next_moves;
        for (const Move& move : possible_moves) {
            if (move.isValid()) {
                Move validMove = convalidMove(move, board);
                if (validMove.isValid())
                    next_moves.push_back(validMove);
            }
        }

        return next_moves;
    }
};

class King : public GenericPiece {
public:
    King(bool color, Icon icon) : GenericPiece(color, icon) {}

    Move convalidMove(const Move& move, Cell** board) const {
        if (board[move.getToY()][move.getToX()].getContent() != nullptr) {
            GenericPiece* temp = board[move.getToY()][move.getToX()].getContent();
            if (temp->isColor() == color)
                return move;
            else
                return Move(move.getFromY(), move.getFromX(), move.getToY(), move.getToX(), true);
        } else {
            return move;
        }
    }

    std::vector<Move> nextMoves(Cell** board, int Y, int X) const {
        Move possible_moves[] = {
            Move(Y, X, Y - 1, X - 1), Move(Y, X, Y - 1, X), Move(Y, X, Y - 1, X + 1),
            Move(Y, X, Y, X + 1),
            Move(Y, X, Y + 1, X - 1), Move(Y, X, Y + 1, X), Move(Y, X, Y + 1, X + 1),
            Move(Y, X, Y, X - 1)
        };

        std::vector<Move> next_moves;
        for (const Move& move : possible_moves) {
            if (move.isValid()) {
                Move validMove = convalidMove(move, board);
                if (validMove.isValid())
                    next_moves.push_back(validMove);
            }
        }

        return next_moves;
    }
};

class Pawn : public GenericPiece {
private:
    bool everMoved;

public:
    Pawn(bool color, Icon icon) : GenericPiece(color, icon), everMoved(true) {}

    Pawn(bool color, Icon icon, bool everMoved) : GenericPiece(color, icon), everMoved(everMoved) {}

    std::vector<Move> nextMoves(Cell** chessboard, int Y, int X, Move move) const {
        std::vector<Move> next_moves;

        if (color) {
            if (Y - 1 >= 0 && Y - 1 <= 7) {
                if (chessboard[Y - 1][X].getContent() == nullptr) {
                    next_moves.emplace_back(Y, X, Y - 1, X);
                    if (Y - 2 >= 0 && Y - 2 <= 7 && everMoved && chessboard[Y - 2][X].getContent() == nullptr)
                        next_moves.emplace_back(Y, X, Y - 2, X);
                }

                if (X + 1 >= 0 && X + 1 <= 7 && chessboard[Y - 1][X + 1].getContent() != nullptr &&
                    chessboard[Y - 1][X + 1].getContent()->isColor() != color)
                    next_moves.emplace_back(Y, X, Y - 1, X + 1, true);

                if (X - 1 >= 0 && X - 1 <= 7 && chessboard[Y - 1][X - 1].getContent() != nullptr &&
                    chessboard[Y - 1][X - 1].getContent()->isColor() != color)
                    next_moves.emplace_back(Y, X, Y - 1, X - 1, true);
            }
        } else {
            if (Y + 1 >= 0 && Y + 1 <= 7) {
                if (chessboard[Y + 1][X].getContent() == nullptr) {
                    next_moves.emplace_back(Y, X, Y + 1, X);
                    if (Y + 2 >= 0 && Y + 2 <= 7 && chessboard[Y + 2][X].getContent() == nullptr && everMoved)
                        next_moves.emplace_back(Y, X, Y + 2, X);
                }

                if (X + 1 >= 0 && X + 1 <= 7 && chessboard[Y + 1][X + 1].getContent() != nullptr &&
                    chessboard[Y + 1][X + 1].getContent()->isColor() != color)
                    next_moves.emplace_back(Y, X, Y + 1, X + 1, true);

                if (X - 1 >= 0 && X - 1 <= 7 && chessboard[Y + 1][X - 1].getContent() != nullptr &&
                    chessboard[Y + 1][X - 1].getContent()->isColor() != color)
                    next_moves.emplace_back(Y, X, Y + 1, X - 1, true);
            }
        }

        // if (move != nullptr) {
        if (true) {
            GenericPiece* p = chessboard[move.getToY()][move.getToX()].getContent();
            if (dynamic_cast<Pawn*>(p) &&
                ((p->isColor() && move.getFromY() == move.getToY() + 2) ||
                 (!p->isColor() && move.getFromY() == move.getToY() - 2)) &&
                move.getToY() == Y) {
                int yToGo = (color) ? Y - 1 : Y + 1;

                if (move.getToX() > X)
                    next_moves.emplace_back(Y, X, yToGo, X + 1, true);
                else
                    next_moves.emplace_back(Y, X, yToGo, X - 1, true);
            }
        }

        return next_moves;
    }

    bool hasEverMoved() const {
        return everMoved;
    }

    void setEverMoved(bool everMoved) {
        this->everMoved = everMoved;
    }
};

class Tower : public GenericPiece {
public:
    Tower(bool color, Icon icon) : GenericPiece(color, icon) {}

    void horizontal_move_validate(Cell** chessboard, std::vector<Move>& next_moves, int Y, int X, int i, bool block[]) {
        if (chessboard[Y][i].getContent() == nullptr)
            next_moves.emplace_back(Y, X, Y, i, false);
        else {
            if (chessboard[Y][i].getContent()->isColor() != color)
                next_moves.emplace_back(Y, X, Y, i, true);
            block[0] = true;
        }
    }

    void vertical_move_validate(Cell** chessboard, std::vector<Move>& next_moves, int Y, int X, int i, bool block[]) {
        if (chessboard[i][X].getContent() == nullptr)
            next_moves.emplace_back(Y, X, i, X, false);
        else {
            if (chessboard[i][X].getContent()->isColor() != color)
                next_moves.emplace_back(Y, X, i, X, true);
            block[1] = true;
        }
    }

    std::vector<Move> nextMoves(Cell** chessboard, int Y, int X) const {
        std::vector<Move> next_moves;

        bool block[] = {false, false}; // block[0] is for horizontal, block[1] is for vertical

        // going _ahead:
        for (int i = std::min(X, Y) + 1; i <= 7; i++) {
            // horizontal_moves:
            if (i > X && !block[0]) {
                // Tower::horizontal_move_validate(chessboard, next_moves, Y, X, i, block);
            }
            // vertical_moves:
            if (i > Y && !block[1]) {
                // Tower::vertical_move_validate(chessboard, next_moves, Y, X, i, block);
            }
        }

        // going _back:
        block[0] = block[1] = false; // block[0] is for horizontal, block[1] is for vertical
        for (int i = std::max(Y, X) - 1; i >= 0; i--) {
            // horizontal_moves:
            if (i < X && !block[0]) {
                // Tower::horizontal_move_validate(chessboard, next_moves, Y, X, i, block);
            }
            // vertical_moves:
            if (i < Y && !block[1]) {
                // Tower::vertical_move_validate(chessboard, next_moves, Y, X, i, block);
            }
        }

        return next_moves;
    }
};

class Queen : public GenericPiece {
public:
    Queen(bool color, Icon icon) : GenericPiece(color, icon) {}

    void horizontal_move_validate(Cell** chessboard, std::vector<Move>& next_moves, int Y, int X, int i, bool block[]) {
        if (chessboard[Y][i].getContent() == nullptr)
            next_moves.emplace_back(Y, X, Y, i, false);
        else {
            if (chessboard[Y][i].getContent()->isColor() != color)
                next_moves.emplace_back(Y, X, Y, i, true);
            block[0] = true;
        }
    }

    void vertical_move_validate(Cell** chessboard, std::vector<Move>& next_moves, int Y, int X, int i, bool block[]) {
        if (chessboard[i][X].getContent() == nullptr)
            next_moves.emplace_back(Y, X, i, X, false);
        else {
            if (chessboard[i][X].getContent()->isColor() != color)
                next_moves.emplace_back(Y, X, i, X, true);
            block[1] = true;
        }
    }

    void diagonal_move_validate(Cell** chessboard, std::vector<Move>& next_moves, int fromY, int fromX, int toY, int toX, bool block[], int diagonal) {
        if (chessboard[toY][toX].getContent() == nullptr)
            next_moves.emplace_back(fromY, fromX, toY, toX, false);
        else {
            if (chessboard[toY][toX].getContent()->isColor() != color)
                next_moves.emplace_back(fromY, fromX, toY, toX, true);
            block[diagonal] = true;
        }
    }

    std::vector<Move> nextMoves(Cell** chessboard, int Y, int X) const {
        std::vector<Move> next_moves;

        // going ahead diagonally on y
        bool block[] = {false, false}; // block[0] is for diagonal1 (from top-left to bottom-right), block[1] is for diagonal2 (from top-right to bottom-left)
        for (int i = Y + 1, l = X - 1, r = X + 1; i <= 7; i++, l--, r++) {
            if (l >= 0 && !block[0]){
                // Queen::diagonal_move_validate(chessboard, next_moves, Y, X, i, l, block, 0);
            }
            if (r <= 7 && !block[1]) {
                // Queen::diagonal_move_validate(chessboard, next_moves, Y, X, i, r, block, 1);
            }
        }

        // going back diagonally on y
        block[0] = block[1] = false; // block[0] is for diagonal1 (from top-left to bottom-right), block[1] is for diagonal2 (from top-right to bottom-left)
        for (int i = Y - 1, l = X - 1, r = X + 1; i >= 0; i--, l--, r++) {
            if (l >= 0 && !block[0]){
                // Queen::diagonal_move_validate(chessboard, next_moves, Y, X, i, l, block, 0);
            }
            if (r <= 7 && !block[1]){
                // Queen::diagonal_move_validate(chessboard, next_moves, Y, X, i, r, block, 1);
            }
        }

        // going ahead horizontally and vertically
        block[0] = block[1] = false; // block[0] is for horizontal, block[1] is for vertical
        for (int i = std::min(X, Y) + 1; i <= 7; i++) {
            // horizontal_moves:
            if (i > X && !block[0]) {
                // Queen::horizontal_move_validate(chessboard, next_moves, Y, X, i, block);
            }
            // vertical_moves:
            if (i > Y && !block[1]) {
                // Queen::vertical_move_validate(chessboard, next_moves, Y, X, i, block);
            }
        }

        // going back horizontally and vertically
        block[0] = block[1] = false; // block[0] is for horizontal, block[1] is for vertical
        for (int i = std::max(Y, X) - 1; i >= 0; i--) {
            // horizontal_moves:
            if (i < X && !block[0]) {
                // Queen::horizontal_move_validate(chessboard, next_moves, Y, X, i, block);
            }
            // vertical_moves:
            if (i < Y && !block[1]) {
                // Queen::vertical_move_validate(chessboard, next_moves, Y, X, i, block);
            }
        }

        return next_moves;
    }
};





