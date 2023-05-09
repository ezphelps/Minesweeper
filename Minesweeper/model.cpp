/// Model handles the logic of the minesweeper game
/// Created By: ezphelps

#include "model.h"

/// \brief Model::Model
/// Constructor.
/// Set the minefield2dArray to be empty.
/// \param parent
Model::Model(int width, int height, int numMines, QObject *parent)
    : QObject{parent}
{
    //Set minefield to be empty
    for(int i = 0;i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            minefield2dArray[i][j] = 0;
        }
    }

    setMinefield(width, height, numMines);
}

/// \brief Model::setMinefield
/// Set a minefield.
/// Logic laws for mine placement.
/// \param width
/// \param height
/// \param numMines
void Model::setMinefield(int width, int height, int numMines)
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
        else
        {
            minefield2dArray[x][y] = 1;
        }
    }

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



