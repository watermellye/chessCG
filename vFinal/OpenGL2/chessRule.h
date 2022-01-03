#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <iostream>

#include "Model1.h"
#include <cmath>

class GamePiece {
   public:
    GamePiece(char PieceColor, int muid) : mPieceColor(PieceColor), uid(muid) {}
    ~GamePiece() {}
    virtual char GetPiece() = 0;
    char GetColor() { return mPieceColor; }
    int GetUid() { return uid; }
    bool IsLegalMove(int iSrcRow, int iSrcCol, int iDestRow, int iDestCol, GamePiece* GameBoard[8][8]) {
        GamePiece* qpDest = GameBoard[iDestRow][iDestCol];
        if ((qpDest == 0) || (mPieceColor != qpDest->GetColor())) {
            return AreSquaresLegal(iSrcRow, iSrcCol, iDestRow, iDestCol, GameBoard);
        }
        return false;
    }

   private:
    virtual bool AreSquaresLegal(int iSrcRow, int iSrcCol, int iDestRow, int iDestCol, GamePiece* GameBoard[8][8]) = 0;
    char mPieceColor;
    int uid;
};

class PawnPiece : public GamePiece {
   public:
    PawnPiece(char PieceColor, int Uid) : GamePiece(PieceColor, Uid) {}
    ~PawnPiece() {}

   private:
    virtual char GetPiece() { return 'P'; }
    bool AreSquaresLegal(int iSrcRow, int iSrcCol, int iDestRow, int iDestCol, GamePiece* GameBoard[8][8]) {
        GamePiece* qpDest = GameBoard[iDestRow][iDestCol];
        if (qpDest == 0) {
            // Destination square is unoccupied
            if (iSrcCol == iDestCol) {
                if (GetColor() == 'W') {
                    if (iDestRow == iSrcRow + 1) {
                        return true;
                    }
                } else {
                    if (iDestRow == iSrcRow - 1) {
                        return true;
                    }
                }
            }
        } else {
            // Dest holds piece of opposite color
            if ((iSrcCol == iDestCol + 1) || (iSrcCol == iDestCol - 1)) {
                if (GetColor() == 'W') {
                    if (iDestRow == iSrcRow + 1) {
                        return true;
                    }
                } else {
                    if (iDestRow == iSrcRow - 1) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
};

class KnightPiece : public GamePiece {
   public:
    KnightPiece(char PieceColor, int Uid) : GamePiece(PieceColor, Uid) {}
    ~KnightPiece() {}

   private:
    virtual char GetPiece() { return 'N'; }
    bool AreSquaresLegal(int iSrcRow, int iSrcCol, int iDestRow, int iDestCol, GamePiece* GameBoard[8][8]) {
        // Destination square is unoccupied or occupied by opposite color
        if ((iSrcCol == iDestCol + 1) || (iSrcCol == iDestCol - 1)) {
            if ((iSrcRow == iDestRow + 2) || (iSrcRow == iDestRow - 2)) {
                return true;
            }
        }
        if ((iSrcCol == iDestCol + 2) || (iSrcCol == iDestCol - 2)) {
            if ((iSrcRow == iDestRow + 1) || (iSrcRow == iDestRow - 1)) {
                return true;
            }
        }
        return false;
    }
};

class BishopPiece : public GamePiece {
   public:
    BishopPiece(char PieceColor, int Uid) : GamePiece(PieceColor, Uid) {}
    ~BishopPiece() {}

   private:
    virtual char GetPiece() { return 'B'; }
    bool AreSquaresLegal(int iSrcRow, int iSrcCol, int iDestRow, int iDestCol, GamePiece* GameBoard[8][8]) {
        if ((iDestCol - iSrcCol == iDestRow - iSrcRow) || (iDestCol - iSrcCol == iSrcRow - iDestRow)) {
            // Make sure that all invervening squares are empty
            int iRowOffset = (iDestRow - iSrcRow > 0) ? 1 : -1;
            int iColOffset = (iDestCol - iSrcCol > 0) ? 1 : -1;
            int iCheckRow;
            int iCheckCol;
            for (iCheckRow = iSrcRow + iRowOffset, iCheckCol = iSrcCol + iColOffset; iCheckRow != iDestRow; iCheckRow = iCheckRow + iRowOffset, iCheckCol = iCheckCol + iColOffset) {
                if (GameBoard[iCheckRow][iCheckCol] != 0) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

class RookPiece : public GamePiece {
   public:
    RookPiece(char PieceColor, int Uid) : GamePiece(PieceColor, Uid) {}
    ~RookPiece() {}

   private:
    virtual char GetPiece() { return 'R'; }
    bool AreSquaresLegal(int iSrcRow, int iSrcCol, int iDestRow, int iDestCol, GamePiece* GameBoard[8][8]) {
        if (iSrcRow == iDestRow) {
            // Make sure that all invervening squares are empty
            int iColOffset = (iDestCol - iSrcCol > 0) ? 1 : -1;
            for (int iCheckCol = iSrcCol + iColOffset; iCheckCol != iDestCol; iCheckCol = iCheckCol + iColOffset) {
                if (GameBoard[iSrcRow][iCheckCol] != 0) {
                    return false;
                }
            }
            return true;
        } else if (iDestCol == iSrcCol) {
            // Make sure that all invervening squares are empty
            int iRowOffset = (iDestRow - iSrcRow > 0) ? 1 : -1;
            for (int iCheckRow = iSrcRow + iRowOffset; iCheckRow != iDestRow; iCheckRow = iCheckRow + iRowOffset) {
                if (GameBoard[iCheckRow][iSrcCol] != 0) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

class QueenPiece : public GamePiece {
   public:
    QueenPiece(char PieceColor, int Uid) : GamePiece(PieceColor, Uid) {}
    ~QueenPiece() {}

   private:
    virtual char GetPiece() { return 'Q'; }
    bool AreSquaresLegal(int iSrcRow, int iSrcCol, int iDestRow, int iDestCol, GamePiece* GameBoard[8][8]) {
        if (iSrcRow == iDestRow) {
            // Make sure that all invervening squares are empty
            int iColOffset = (iDestCol - iSrcCol > 0) ? 1 : -1;
            for (int iCheckCol = iSrcCol + iColOffset; iCheckCol != iDestCol; iCheckCol = iCheckCol + iColOffset) {
                if (GameBoard[iSrcRow][iCheckCol] != 0) {
                    return false;
                }
            }
            return true;
        } else if (iDestCol == iSrcCol) {
            // Make sure that all invervening squares are empty
            int iRowOffset = (iDestRow - iSrcRow > 0) ? 1 : -1;
            for (int iCheckRow = iSrcRow + iRowOffset; iCheckRow != iDestRow; iCheckRow = iCheckRow + iRowOffset) {
                if (GameBoard[iCheckRow][iSrcCol] != 0) {
                    return false;
                }
            }
            return true;
        } else if ((iDestCol - iSrcCol == iDestRow - iSrcRow) || (iDestCol - iSrcCol == iSrcRow - iDestRow)) {
            // Make sure that all invervening squares are empty
            int iRowOffset = (iDestRow - iSrcRow > 0) ? 1 : -1;
            int iColOffset = (iDestCol - iSrcCol > 0) ? 1 : -1;
            int iCheckRow;
            int iCheckCol;
            for (iCheckRow = iSrcRow + iRowOffset, iCheckCol = iSrcCol + iColOffset; iCheckRow != iDestRow; iCheckRow = iCheckRow + iRowOffset, iCheckCol = iCheckCol + iColOffset) {
                if (GameBoard[iCheckRow][iCheckCol] != 0) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

class KingPiece : public GamePiece {
   public:
    KingPiece(char PieceColor, int Uid) : GamePiece(PieceColor, Uid) {}
    ~KingPiece() {}

   private:
    virtual char GetPiece() { return 'K'; }
    bool AreSquaresLegal(int iSrcRow, int iSrcCol, int iDestRow, int iDestCol, GamePiece* GameBoard[8][8]) {
        int iRowDelta = iDestRow - iSrcRow;
        int iColDelta = iDestCol - iSrcCol;
        if (((iRowDelta >= -1) && (iRowDelta <= 1)) && ((iColDelta >= -1) && (iColDelta <= 1))) {
            return true;
        }
        return false;
    }
};

class CBoard {
   public:
    CBoard() {
        for (int iRow = 0; iRow < 8; ++iRow) {
            for (int iCol = 0; iCol < 8; ++iCol) {
                MainGameBoard[iRow][iCol] = 0;
            }
        }
        // Allocate and place black pieces
        for (int iCol = 0; iCol < 8; ++iCol) {
            MainGameBoard[6][iCol] = new PawnPiece('B', iCol);
        }
        MainGameBoard[7][0] = new RookPiece('B', 8);
        MainGameBoard[7][1] = new KnightPiece('B', 10);
        MainGameBoard[7][2] = new BishopPiece('B', 14);
        MainGameBoard[7][3] = new KingPiece('B', 12);
        MainGameBoard[7][4] = new QueenPiece('B', 13);
        MainGameBoard[7][5] = new BishopPiece('B', 15);
        MainGameBoard[7][6] = new KnightPiece('B', 11);
        MainGameBoard[7][7] = new RookPiece('B', 9);
        // Allocate and place white pieces
        for (int iCol = 0; iCol < 8; ++iCol) {
            MainGameBoard[1][iCol] = new PawnPiece('W', iCol + 16);
        }
        MainGameBoard[0][0] = new RookPiece('W', 24);
        MainGameBoard[0][1] = new KnightPiece('W', 26);
        MainGameBoard[0][2] = new BishopPiece('W', 30);
        MainGameBoard[0][3] = new KingPiece('W', 28);
        MainGameBoard[0][4] = new QueenPiece('W', 29);
        MainGameBoard[0][5] = new BishopPiece('W', 31);
        MainGameBoard[0][6] = new KnightPiece('W', 27);
        MainGameBoard[0][7] = new RookPiece('W', 25);
    }
    ~CBoard() {
        for (int iRow = 0; iRow < 8; ++iRow) {
            for (int iCol = 0; iCol < 8; ++iCol) {
                delete MainGameBoard[iRow][iCol];
                MainGameBoard[iRow][iCol] = 0;
            }
        }
    }

    void Print() {
        using namespace std;
        const int kiSquareWidth = 4;
        const int kiSquareHeight = 3;
        for (int iRow = 0; iRow < 8 * kiSquareHeight; ++iRow) {
            int iSquareRow = iRow / kiSquareHeight;
            // Print side border with numbering
            if (iRow % 3 == 1) {
                cout << '-' << (char)('1' + 7 - iSquareRow) << '-';
            } else {
                cout << "---";
            }
            // Print the chess board
            for (int iCol = 0; iCol < 8 * kiSquareWidth; ++iCol) {
                int iSquareCol = iCol / kiSquareWidth;
                if (((iRow % 3) == 1) && ((iCol % 4) == 1 || (iCol % 4) == 2) && MainGameBoard[7 - iSquareRow][iSquareCol] != 0) {
                    if ((iCol % 4) == 1) {
                        cout << MainGameBoard[7 - iSquareRow][iSquareCol]->GetColor();
                    } else {
                        cout << MainGameBoard[7 - iSquareRow][iSquareCol]->GetPiece();
                    }
                } else {
                    if ((iSquareRow + iSquareCol) % 2 == 1) {
                        HANDLE handle;
                        WORD wOldColorAttrs;
                        CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
                        handle = GetStdHandle(STD_OUTPUT_HANDLE);
                        GetConsoleScreenBufferInfo(handle, &csbiInfo);
                        wOldColorAttrs = csbiInfo.wAttributes;
                        SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
                        cout << ' ';
                        SetConsoleTextAttribute(handle, wOldColorAttrs);
                    } else {
                        cout << ' ';
                    }
                }
            }
            cout << endl;
        }
        // Print the bottom border with numbers
        for (int iRow = 0; iRow < kiSquareHeight; ++iRow) {
            if (iRow % 3 == 1) {
                cout << "---";
                for (int iCol = 0; iCol < 8 * kiSquareWidth; ++iCol) {
                    int iSquareCol = iCol / kiSquareWidth;
                    if ((iCol % 4) == 1) {
                        cout << (iSquareCol + 1);
                    } else {
                        cout << '-';
                    }
                }
                cout << endl;
            } else {
                for (int iCol = 1; iCol < 9 * kiSquareWidth; ++iCol) {
                    cout << '-';
                }
                cout << endl;
            }
        }
    }

    bool IsInCheck(char PieceColor) {
        // Find the king
        int iKingRow;
        int iKingCol;
        for (int iRow = 0; iRow < 8; ++iRow) {
            for (int iCol = 0; iCol < 8; ++iCol) {
                if (MainGameBoard[iRow][iCol] != 0) {
                    if (MainGameBoard[iRow][iCol]->GetColor() == PieceColor) {
                        if (MainGameBoard[iRow][iCol]->GetPiece() == 'K') {
                            iKingRow = iRow;
                            iKingCol = iCol;
                        }
                    }
                }
            }
        }
        // Run through the opponent's pieces and see if any can take the king
        for (int iRow = 0; iRow < 8; ++iRow) {
            for (int iCol = 0; iCol < 8; ++iCol) {
                if (MainGameBoard[iRow][iCol] != 0) {
                    if (MainGameBoard[iRow][iCol]->GetColor() != PieceColor) {
                        if (MainGameBoard[iRow][iCol]->IsLegalMove(iRow, iCol, iKingRow, iKingCol, MainGameBoard)) {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    bool CanMove(char PieceColor) {
        // Run through all pieces
        for (int iRow = 0; iRow < 8; ++iRow) {
            for (int iCol = 0; iCol < 8; ++iCol) {
                if (MainGameBoard[iRow][iCol] != 0) {
                    // If it is a piece of the current player, see if it has a legal move
                    if (MainGameBoard[iRow][iCol]->GetColor() == PieceColor) {
                        for (int iMoveRow = 0; iMoveRow < 8; ++iMoveRow) {
                            for (int iMoveCol = 0; iMoveCol < 8; ++iMoveCol) {
                                if (MainGameBoard[iRow][iCol]->IsLegalMove(iRow, iCol, iMoveRow, iMoveCol, MainGameBoard)) {
                                    // Make move and check whether king is in check
                                    GamePiece* qpTemp = MainGameBoard[iMoveRow][iMoveCol];
                                    MainGameBoard[iMoveRow][iMoveCol] = MainGameBoard[iRow][iCol];
                                    MainGameBoard[iRow][iCol] = 0;
                                    bool bCanMove = !IsInCheck(PieceColor);
                                    // Undo the move
                                    MainGameBoard[iRow][iCol] = MainGameBoard[iMoveRow][iMoveCol];
                                    MainGameBoard[iMoveRow][iMoveCol] = qpTemp;
                                    if (bCanMove) {
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    GamePiece* MainGameBoard[8][8];
};

class ChessBoard {
   public:
    ChessBoard() : mcPlayerTurn('W') {
        // 初始化渲染 todo
    }
    ~ChessBoard() {}

    void Start() {
        do {
            GetNextMove(mqGameBoard.MainGameBoard);
            AlternateTurn();
        } while (!IsGameOver());
        mqGameBoard.Print();
    }
    void getInfo(inform info[], int tim) {
        // 自动走一步
        int xa, ya, xb, yb, tryCount = 0;
        do {
            xa = rand() % 8;
            ya = rand() % 8;
            xb = rand() % 8;
            yb = rand() % 8;
            // printf("尝试(%d,%d)->(%d,%d)\n", xa, ya, xb, yb);
            if (TryNextMove(mqGameBoard.MainGameBoard, xa * 10 + ya, xb * 10 + yb, (++tryCount < 5000) ? 1 : 0)) {  // 演示5000，平时500
                printf("(%d,%d)->(%d,%d)  tryCount=%d\n", xa, ya, xb, yb, tryCount);
                AlternateTurn();
                break;
            }
        } while (1);
        for (int k = 0; k < 32; ++k)
            if (info[k].xpos == xb && info[k].ypos == yb) {
                info[k] = {xb, yb, tim, Fade, 1};  // 被吃掉了，播放吃掉动画
                // printf("%d号棋子被吃  ", k);
            }
        int id = mqGameBoard.MainGameBoard[xb][yb]->GetUid();
        info[id] = {xb, yb, tim, Move, 1, xa, ya};  // 移动
        // printf("%d号棋子移动\n", id);
    }

    int TryNextMove(GamePiece* GameBoard[8][8], int iStartMove, int iEndMove, bool mustEat) {
        int iStartRow = iStartMove / 10;
        int iStartCol = iStartMove % 10;
        int iEndRow = iEndMove / 10;
        int iEndCol = iEndMove % 10;
        // 判断移动是否在合法范围内
        GamePiece* qpCurrPiece = GameBoard[iStartRow][iStartCol];
        // 判断终点是否为空或敌方棋子
        if ((qpCurrPiece != 0) && (qpCurrPiece->GetColor() == mcPlayerTurn)) {
            if (GameBoard[iEndRow][iEndCol] == 0 && mustEat) return 0;
            // if (mustEat && (abs(iStartCol - iEndCol) + abs(iStartRow - iEndRow) < 5)) return 0;
            // 判断该移动是否合法；若有吃子，判断吃子是否合法。
            if (qpCurrPiece->IsLegalMove(iStartRow, iStartCol, iEndRow, iEndCol, GameBoard)) {
                // 移动
                GamePiece* qpTemp = GameBoard[iEndRow][iEndCol];
                GameBoard[iEndRow][iEndCol] = GameBoard[iStartRow][iStartCol];
                GameBoard[iStartRow][iStartCol] = 0;

                // 移动后判断是否构成将军。如果不构成，该步终于合法.
                if (!mqGameBoard.IsInCheck(mcPlayerTurn))
                    return 1;  // 只有到达这个地方，移动才是合法的；此外均不合法，重新输入。
                else {
                    // 构成将军，不合法，撤回。
                    GameBoard[iStartRow][iStartCol] = GameBoard[iEndRow][iEndCol];
                    GameBoard[iEndRow][iEndCol] = qpTemp;
                }
            }
        }
        return 0;
    }
    void GetNextMove(GamePiece* GameBoard[8][8]) {
        using namespace std;
        bool bValidMove = false;
        do {
            // 规则部分，最终会删除
            {
                system("cls");
                cout << " WP = White pawn &  BP = Black pawn" << endl;
                cout << " WN = White Knight & BN = Black Knight" << endl;
                cout << " WB = White Bishop & BB = Black Bishop" << endl;
                cout << " WR = White Rook & BR = Black Rook" << endl;
                cout << " WQ = White Queen & BQ = Black Queen" << endl;
                cout << " WK = White King & BK =Black King" << endl;
                cout << "Rule for move is :" << endl;
                cout << "Move by selecting row & column to another valid location using row & column" << endl << endl << endl;
            }

            // 渲染棋子，会被opengl替代
            mqGameBoard.Print();

            // 获取将要移动的坐标，输入方式将会替代为图形化界面中的点击
            cout << mcPlayerTurn << "'s Move: ";
            int iStartMove;
            cin >> iStartMove;
            int iStartRow = (iStartMove / 10) - 1;
            int iStartCol = (iStartMove % 10) - 1;
            cout << "To: ";
            int iEndMove;
            cin >> iEndMove;
            int iEndRow = (iEndMove / 10) - 1;
            int iEndCol = (iEndMove % 10) - 1;

            // 判断移动是否在合法范围内
            if ((iStartRow >= 0 && iStartRow <= 7) && (iStartCol >= 0 && iStartCol <= 7) && (iEndRow >= 0 && iEndRow <= 7) && (iEndCol >= 0 && iEndCol <= 7)) {
                GamePiece* qpCurrPiece = GameBoard[iStartRow][iStartCol];
                // 判断终点是否为空或敌方棋子
                if ((qpCurrPiece != 0) && (qpCurrPiece->GetColor() == mcPlayerTurn)) {
                    // 判断该移动是否合法；若有吃子，判断吃子是否合法。
                    if (qpCurrPiece->IsLegalMove(iStartRow, iStartCol, iEndRow, iEndCol, GameBoard)) {
                        // 移动
                        GamePiece* qpTemp = GameBoard[iEndRow][iEndCol];
                        GameBoard[iEndRow][iEndCol] = GameBoard[iStartRow][iStartCol];
                        GameBoard[iStartRow][iStartCol] = 0;

                        // 移动后判断是否构成将军。如果不构成，该步终于合法，bValidMove = ture
                        if (!mqGameBoard.IsInCheck(mcPlayerTurn)) {
                            delete qpTemp;
                            bValidMove = true;  // 只有到达这个地方，移动才是合法的；此外均不合法，重新输入。
                                                // opengl中渲染棋子移动动画。

                        } else {  // 如果构成将军，不合法，撤回。
                            GameBoard[iStartRow][iStartCol] = GameBoard[iEndRow][iEndCol];
                            GameBoard[iEndRow][iEndCol] = qpTemp;
                        }
                    }
                }
            }
            if (!bValidMove) {
                cout << "Invalid Move!" << endl;
            }
        } while (!bValidMove);
    }

    void AlternateTurn() { mcPlayerTurn = (mcPlayerTurn == 'W') ? 'B' : 'W'; }

    bool IsGameOver() {
        // Check that the current player can move
        // If not, we have a stalemate or checkmate
        bool bCanMove(false);
        bCanMove = mqGameBoard.CanMove(mcPlayerTurn);
        if (!bCanMove) {
            if (mqGameBoard.IsInCheck(mcPlayerTurn)) {
                AlternateTurn();
                std::cout << "Checkmate, " << mcPlayerTurn << " Wins!" << std::endl;
            } else {
                std::cout << "Stalemate!" << std::endl;
            }
        }
        return !bCanMove;
    }

   private:
    CBoard mqGameBoard;  // 初始化棋盘
    char mcPlayerTurn;
};