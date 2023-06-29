/// Model handles the logic of the minesweeper game
/// Created By: ezphelps

#include "model.h"

/// \brief Model::Model
/// Constructor.
/// Set the minefield2dArray to be empty.
/// \param parent
Model::Model(int width, int height, int numMines, QObject *parent)
    : QObject{parent},
      timer(this),
      seconds(0),
      firstSquare(true),
      gameOver(false),
      mouseDragging(false),
      mouseX(0),
      mouseY(0),
      width(width),
      height(height),
      numMines(numMines),
      unmarkedMines(numMines)
{
    squaresLeft = (width * height) - numMines;
    resetArrays();

    connect(&timer, &QTimer::timeout, this, &Model::timerExpired);
}

/// \brief Model::restartButton
void Model::restartButton()
{
    resetArrays();
    timer.stop();
    seconds = 0;
    firstSquare = true;
    gameOver = false;
    unmarkedMines = numMines;
    squaresLeft = (width * height) - numMines;

    emit resetMinefield(numMines);
    emit updateSeconds(0);
}

/// \brief Model::mouseDrag
/// Logic to show the square currently being selected.
/// \param x
/// \param y
void Model::mouseDrag(int x, int y)
{
    if(gameOver)
    {
        return;
    }

    //The mouse drag just started.
    if(!mouseDragging)
    {
        mouseDragging = true;
        mouseX = x;
        mouseY = y;

        if(squaresClicked[x][y] == 0)
        {
            emit selectSquare(mouseX, mouseY);
        }
    }
    //The mouse drag is already going.  Do something if its in a new square.
    else if(mouseX != x || mouseY != y)
    {
        //Unselect a square if it hasn't been revealed already.
        if(squaresClicked[mouseX][mouseY] == 0)
        {
            emit unselectSquare(mouseX, mouseY);
        }

        mouseX = x;
        mouseY = y;

        //Select the new square if that one hasn't been revealed.
        if(squaresClicked[x][y] == 0)
        {
            emit selectSquare(mouseX, mouseY);
        }
    }
}

/// \brief Model::squareClicked
/// The mouse was released over this square.
/// \param x
/// \param y
void Model::squareClicked(int x, int y)
{
    if(gameOver)
    {
        return;
    }

    mouseDragging = false;

    //Check if the square has been revealed yet.
    if(squaresClicked[x][y] == 0)
    {
        if(firstSquare)
        {
            firstSquare = false;
            setMinefield(x, y, width, height, numMines);
            revealNonMine(x,y);
            timer.start(1000);
        }
        else
        {
            //User hit a mine.
            if(minefield2dArray[x][y] == 1)
            {
                endGame(x, y);
            }
            //User did not hit a mine.
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
    if(gameOver)
    {
        return;
    }

    //Display flag.
    if(flagsArray[x][y] == 0)
    {
        flagsArray[x][y] = 1;
        unmarkedMines--;
        emit displayFlag(unmarkedMines, x, y);
    }
    //Remove the flag.
    else
    {
        flagsArray[x][y] = 0;
        unmarkedMines++;
        emit removeFlag(unmarkedMines, x, y);
    }
}

/// \brief Model::spaceHit
/// Handles logic of a space bar hit.
/// \param x
/// \param y
void Model::spaceHit(int x, int y)
{
    if(gameOver)
    {
        return;
    }

    //The square is not revealed.
    if(squaresClicked[x][y] == 0)
    {
        if(flagsArray[x][y] == 0)
        {
            flagsArray[x][y] = 1;
            unmarkedMines--;
            emit displayFlag(unmarkedMines, x, y);
        }
        else
        {
            flagsArray[x][y] = 0;
            unmarkedMines++;
            emit removeFlag(unmarkedMines, x, y);
        }
    }
    else
    {
        //All surrounding mines must be marked.
        if(getNumSurroundingFlags(x,y) == getNumSurroundingMines(x,y))
        {
            //Open up mines.
            for (int i = (x - 1); i <= (x + 1); i++)
            {
                for (int j = (y - 1); j <= (y + 1); j++)
                {
                    //Check if the square is clicked or flagged.
                    if (squaresClicked[i][j] == 0 && flagsArray[i][j] == 0)
                    {
                        if ((i >= 0) && (i < width) &&
                            (j >= 0) && (j < height))
                        {
                            //make sure the square is valid.
                            if(minefield2dArray[i][j] == 1)
                            {
                                endGame(i, j);
                                return;
                            }
                            revealNonMine(i, j);
                        }
                    }
                }
            }
        }
    }
}

/// \brief Model::timerExpired
/// Gets signal from the timer.
/// Updates seconds then emits a signal for the view.
void Model::timerExpired()
{
    if(seconds < 999)
    {
        seconds++;
        emit updateSeconds(seconds);
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

        //If there is already a mine don't increment.
        if(minefield2dArray[x][y] == 1)
        {
            i--;
        }
        //Mine also cannot be placed around the square the user clicked.
        else if((x >= (xCord - 1) && (x <= (xCord + 1))) && (y >= (yCord - 1) && y <= (yCord + 1)))
        {
            i--;
        }
        //Place the mine.
        else
        {
            minefield2dArray[x][y] = 1;
        }
    }
}

/// \brief Model::getNumSurroundingMines
/// \param x
/// \param y
/// \return - number of surrounding mines
int Model::getNumSurroundingMines(int x, int y)
{
    int num = 0;
    for (int i = (x - 1); i <= (x + 1); i++)
    {
        for (int j = (y - 1); j <= (y + 1); j++)
        {
            //Bounds check as well as looking for mines.
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

/// \brief Model::getNumSurroundingFlags
/// \param x
/// \param y
/// \return - number of surrounding flags
int Model::getNumSurroundingFlags(int x, int y)
{
    int num = 0;
    for (int i = (x - 1); i <= (x + 1); i++)
    {
        for (int j = (y - 1); j <= (y + 1); j++)
        {
            //Bounds check as well as looking for flags.
            if ((i >= 0) && (i < width) &&
                (j >= 0) && (j < height) &&
                (flagsArray[i][j] == 1))
            {
                num++;
            }
        }
    }
    return num;
}

/// \brief ::Model::revealSquare
/// Emit a signal to reveal squares.
/// Driver method if a zero square is clicked.
/// \param x
/// \param y
void::Model::revealNonMine(int x, int y)
{
    emit validSquare(getNumSurroundingMines(x,y), x, y);
    squaresClicked[x][y] = 1;

    //Recursive call.
    if(getNumSurroundingMines(x,y) == 0)
    {
        revealZeroSquare(x,y);
    }

    //Player wins
    if(--squaresLeft == 0)
    {
        gameOver = true;
        timer.stop();

        for(int i = 0; i < width; i++)
        {
            for(int j = 0; j < height; j++)
            {
                if(minefield2dArray[i][j] == 1 && flagsArray[i][j] == 0)
                {
                    flagsArray[i][j] = 1;
                    numMines--;
                    emit displayFlag(numMines, i, j);
                }
            }
        }

        emit playerWins();
    }
}

/// \brief Model::revealZeroSquare
/// Recursively reveal the area around a zero square.
/// The calling method is responsible for ensuring (x,y) is a zero square as it is assumed.
/// \param x
/// \param y
void Model::revealZeroSquare(int x, int y)
{
    //Check all surrounding nine squares
    for(int i = (x-1); i <= (x+1); i++)
    {
        for(int j = (y-1); j <= (y+1);j++)
        {
            //Make sure squares are in range
            if((i >= 0) && (i <= 29) &&
                (j >= 0) && (j <= 15))
            {
                //Each square being revealed must be hidden still.
                //Base case will be all surrounding squares are revealed.
                if (squaresClicked[i][j] == 0)
                {
                    revealNonMine(i,j);
                }
            }
        }
    }
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

/// \brief Model::endGame
/// Reveals all the mines and ends the game.
void Model::endGame(int x, int y)
{
    gameOver = true;
    timer.stop();

    //reveal all mines
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            if(i == x && j == y)
            {
                //hit mine
                emit hitMine(i, j);
            }
            else if(minefield2dArray[i][j] == 0 && flagsArray[i][j] == 1)
            {
                //false flag
                emit falseFlag(i, j);
            }
            else if(minefield2dArray[i][j] == 1 && flagsArray[i][j] != 1)
            {
                //regular mine
                emit invalidSquare(i, j);
            }
        }
    }
}
