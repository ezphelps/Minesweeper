/// Model handles the logic of the minesweeper game
/// Created By: ezphelps

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <iostream>

using std::array;

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(int width, int height, int numMines, QObject *parent = nullptr);
public slots:
    void squareClicked(int x, int y);

signals:
    void validSquare(int numMines, int x, int y);
    void invalidSquare(int x, int y);

private:
    array<array<int, 16>, 30> minefield2dArray;
    bool firstSquare;
    int width, height, numMines;

    void setMinefield(int xCord, int yCord, int width, int height, int numMines);
    int getNumSurroundingMines(int x, int y);


};

#endif // MODEL_H
