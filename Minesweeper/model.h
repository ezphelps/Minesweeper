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

signals:

private:
    array<array<int, 16>, 30> minefield2dArray;

    void setMinefield(int width, int height, int numMines);


};

#endif // MODEL_H
