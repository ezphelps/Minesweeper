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
      gameOver(false),
      width(width),
      height(height),
      numMines(numMines)
{
    resetArrays();
}

/// \brief Model::restartButton
///
void Model::restartButton()
{
    resetArrays();
    firstSquare = true;
    gameOver = false;

    emit resetMinefield(numMines);
}

/// \brief Model::squareClicked
/// A square was clicked with the mouse
/// \param x
/// \param y
void Model::squareClicked(int x, int y)
{
    if(gameOver)
    {
        return;
    }

    if(squaresClicked[x][y] == 0)
    {
        if(firstSquare)
        {
            firstSquare = false;
            setMinefield(x, y, width, height, numMines);
            revealNonMine(x,y);
        }
        else
        {
            if(minefield2dArray[x][y] == 1)  //hit mine
            {
                gameOver = true;

                //reveal all mines
                for(int i = 0; i < width; i++)
                {
                    for(int j = 0; j < height; j++)
                    {
                        if(minefield2dArray[i][j] == 1 && flagsArray[i][j] != 1)
                        {
                            emit invalidSquare(i, j);
                        }
                    }
                }
            }
            else
            {
                revealNonMine(x, y);
            }
        }
    }
}

/// \brief Model::rightClicked
/// Checks if a flag is already at the location of a right click.
/// Emits a signal to tell the view what to do.
/// \param x
/// \param y
void Model::rightClicked(int x, int y)
{
    if(flagsArray[x][y] == 0)
    {
        flagsArray[x][y] = 1;
        emit displayFlag(x, y);
    }
    else
    {
        flagsArray[x][y] = 0;
        emit removeFlag(x, y);
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
        else if((x >= (xCord - 1) && (x <= (xCord + 1))) && (y >= (yCord - 1) && y <= (yCord + 1)))
        {
            i--;
        }
        else
        {
            minefield2dArray[x][y] = 1;
        }
    }
}

/// \brief Model::getNumSurroundingMines
/// \param x
/// \param y
/// \return
/// num surrounding mines
int Model::getNumSurroundingMines(int x, int y)
{
    int num = 0;
    for (int i = (x - 1); i <= (x + 1); i++)
    {
        for (int j = (y - 1); j <= (y + 1); j++)
        {
            if ((i >= 0) && (i < width) &&
                (j >= 0) && (j < height) &&
                (minefield2dArray[i][j] == 1))
            {
                num++;
            }
        }
    }
    return num;
}

/// \brief ::Model::revealSquare
/// Emit a signal to reveal squares
/// \param x
/// \param y
void::Model::revealNonMine(int x, int y)
{
    paintSquare(getNumSurroundingMines(x,y), x, y);

    if(getNumSurroundingMines(x,y) == 0)
    {
        revealZeroSquare(x,y);
    }
}

/// \brief Model::revealZeroSquare
/// Recursively reveal the area around a zero square.
/// The caller is responsible for ensuring (x,y) is a zero square as it is assumed.
/// \param x
/// \param y
void Model::revealZeroSquare(int x, int y)
{
    //Check all nine squares
    for(int i = (x-1); i <= (x+1); i++)
    {
        for(int j = (y-1); j <= (y+1);j++)
        {
            //Make sure squares are in range
            if((i >= 0) && (i <= 29) &&
                (j >= 0) && (j <= 15))
            {
                //Each square must be hidden still.
                if (squaresClicked[i][j] == 0)
                {
                    revealNonMine(i,j);
                }
            }
        }
    }
}

/// \brief Model::paintSquare
/// Helper makes sure that painted squares are tracked in squaresClicked and emits a
/// signal to the window.
/// \param numSurroundingMines
/// \param x
/// \param y
void Model::paintSquare(int numSurroundingMines, int x, int y)
{
    emit validSquare(numSurroundingMines, x, y);
    squaresClicked[x][y] = 1;
}

/// \brief Model::resetArrays
void Model::resetArrays()
{
    for(int i = 0;i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            minefield2dArray[i][j] = 0;
            squaresClicked[i][j] = 0;
            flagsArray[i][j] = 0;
        }
    }
}
