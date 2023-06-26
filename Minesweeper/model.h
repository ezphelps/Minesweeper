/// Model handles the logic of the minesweeper game
/// Created By: ezphelps

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <algorithm>
#include <iostream>

using std::array;
using std::vector;
using std::pair;

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(int width, int height, int numMines, QObject *parent = nullptr);
public slots:
    void restartButton();
    void mouseDrag(int x, int y);
    void squareClicked(int x, int y);
    void rightClicked(int x, int y);
    void spaceHit(int x, int y);

signals:
    void resetMinefield(int numMines);
    void selectSquare(int x, int y);
    void unselectSquare(int x, int y);
    void validSquare(int numMines, int x, int y);
    void invalidSquare(int x, int y);
    void hitMine(int x, int y);
    void displayFlag(int minesLeft, int x, int y);
    void removeFlag(int minesLeft, int x, int y);
    void falseFlag(int x, int y);
    void playerWins();

private:
    array<array<int, 16>, 30> minefield2dArray;
    array<array<int, 16>, 30> squaresClicked;
    array<array<int, 16>, 30> flagsArray;

    bool firstSquare;
    bool gameOver;
    bool mouseDragging;
    int mouseX, mouseY;
    int width, height, numMines;
    int unmarkedMines;
    int squaresLeft;

    void setMinefield(int xCord, int yCord, int width, int height, int numMines);
    int getNumSurroundingMines(int x, int y);
    int getNumSurroundingFlags(int x, int y);
    void revealNonMine(int x, int y);
    void revealZeroSquare(int x, int y);
    void resetArrays();
    void endGame(int x, int y);
};

#endif // MODEL_H
