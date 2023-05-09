/// Model handles the logic of the minesweeper game
/// Created By: ezphelps

#include "model.h"

/// \brief Model::Model
/// Constructor.
/// Set the minefield2dArray to be empty.
/// \param parent
Model::Model(int width, int height, int numMines, QObject *parent)
    : QObject{parent},
      firstSquare(true),
      width(width),
      height(height),
      numMines(numMines)
{
    //Set minefield to be empty
    for(int i = 0;i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            minefield2dArray[i][j] = 0;
        }
    }
}

/// \brief Model::setMinefield
/// Set a minefield.
/// Logic laws for mine placement.
/// \param width
/// \param height
/// \param numMines
void Model::setMinefield(int xCord, int yCord, int width, int height, int numMines)
{
    //Add valid mines to the field.
    for(int i = 0; i < numMines; i++)
    {
        int x = std::rand() % width;
        int y = std::rand() % height;

        if(minefield2dArray[x][y] == 1)
        {
            i--;
        }
        else if(xCord == x && yCord == y)
        {
            i--;
        }
        else
        {
            minefield2dArray[x][y] = 1;
        }
    }

    //Print out minefield in console.
    for(int i = 0;i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(j != 29)
                std::cout<<minefield2dArray[j][i] << "  ";
            else
                std::cout<<minefield2dArray[j][i] << std::endl;
        }
    }
}

/// \brief Model::squareClicked
/// A square was clicked with the mouse
/// \param x
/// \param y
void Model::squareClicked(int x, int y)
{
    if(firstSquare)
    {
        setMinefield(x, y, width, height, numMines);
        firstSquare = false;
        emit validSquare(0, x, y);
    }
    else
    {
        if(minefield2dArray[x][y] == 1)
        {
            emit invalidSquare(x, y);
        }
    }
}



