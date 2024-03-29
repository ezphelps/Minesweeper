/// MinesweeperWindow represents the ui of the minesweeper gameplay window.
/// Written by: ezphelps

#include "minesweeperwindow.h"
#include "ui_minesweeperwindow.h"

/// \brief MinesweeperWindow::MinesweeperWindow
/// Constructor
/// Sets up connections.
/// Draws a starter minefield.
/// \param parent
MinesweeperWindow::MinesweeperWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MinesweeperWindow),
      model(30, 16, 99),
      sizeX(30),
      sizeY(16),
      minefieldImage(QImage(sizeX*32, sizeY*32, QImage::Format_ARGB32))
{
    ui->setupUi(this);

    //Set pointer in minefield to "this"
    connect(this, &MinesweeperWindow::setWindowPtr, ui->label, &MineField::setPtr);
    emit setWindowPtr(this);

    //button
    connect(ui->restartButton, &QPushButton::released, this, &MinesweeperWindow::restartButtonSlot);
    connect(this, &MinesweeperWindow::restartButton, &model, &Model::restartButton);
    connect(&model, &Model::resetMinefield, this, &MinesweeperWindow::resetMinefieldSlot);

    //timer
    connect(&model, &Model::updateSeconds, this, &MinesweeperWindow::updateSecondsSlot);

    //mouse press
    connect(ui->label, &MineField::mousePressed, this, &MinesweeperWindow::mouseDraggingSlot);
    connect(this, &MinesweeperWindow::mouseDrag, &model, &Model::mouseDrag);
    connect(&model, &Model::selectSquare, this, &MinesweeperWindow::showSelectedSquareSlot);
    connect(&model, &Model::unselectSquare, this, &MinesweeperWindow::unselectSquareSlot);


    //left click
    connect(ui->label, &MineField::mouseRelease, this, &MinesweeperWindow::squareClickedSlot);
    connect(this, &MinesweeperWindow::squareClicked, &model, &Model::squareClicked);
    connect(&model, &Model::validSquare, this, &MinesweeperWindow::validSquareSlot);
    connect(&model, &Model::invalidSquare, this, &MinesweeperWindow::invalidSquareSlot);
    connect(&model, &Model::hitMine, this, &MinesweeperWindow::hitMineSlot);

    //right click
    connect(ui->label, &MineField::rightClick, this, &MinesweeperWindow::rightCLickSlot);
    connect(this, &MinesweeperWindow::rightClicked, &model, &Model::rightClicked);
    connect(&model, &Model::displayFlag, this, &MinesweeperWindow::displayFlagSlot);
    connect(&model, &Model::removeFlag, this, &MinesweeperWindow::removeFlagSlot);
    connect(&model, &Model::falseFlag, this, &MinesweeperWindow::falseFlagSlot);

    //space bar
    connect(ui->label, &MineField::spaceHit, this, &MinesweeperWindow::spaceHitSlot);
    connect(this, &MinesweeperWindow::spaceHit, &model, &Model::spaceHit);

    //win
    connect(&model, &Model::playerWins, this, &MinesweeperWindow::playerWinsSlot);

    //Drawing the minefield.
    for(int i = 0; i < sizeX; i++) // Rows of squares
    {
        for(int j = 0; j < sizeY; j++) // Collumns of squares
        {
            //Each square
            for(int x = 0; x < 32; x++)
            {
                shadeSquare(i,j);
            }
        }
    }

    images[0] =  deserializer.deserializeSSP(":/squareImages/zero.ssp");
    images[1] =  deserializer.deserializeSSP(":/squareImages/one.ssp");
    images[2] =  deserializer.deserializeSSP(":/squareImages/two.ssp");
    images[3] =  deserializer.deserializeSSP(":/squareImages/three.ssp");
    images[4] =  deserializer.deserializeSSP(":/squareImages/four.ssp");
    images[5] =  deserializer.deserializeSSP(":/squareImages/five.ssp");
    images[6] =  deserializer.deserializeSSP(":/squareImages/six.ssp");
    images[7] =  deserializer.deserializeSSP(":/squareImages/seven.ssp");
    images[8] =  deserializer.deserializeSSP(":/squareImages/eight.ssp");
    images[9] =  deserializer.deserializeSSP(":/squareImages/mine.ssp");
    images[10] = deserializer.deserializeSSP(":/squareImages/hitMine.ssp");
    images[11] = deserializer.deserializeSSP(":/squareImages/flag.ssp");
    images[12] = deserializer.deserializeSSP(":/squareImages/falseFlag.ssp");

    timeNums[0] = deserializer.deserializeSSP(":/timeNums/timeZero.ssp");
    timeNums[1] = deserializer.deserializeSSP(":/timeNums/timeOne.ssp");
    timeNums[2] = deserializer.deserializeSSP(":/timeNums/timeTwo.ssp");
    timeNums[3] = deserializer.deserializeSSP(":/timeNums/timeThree.ssp");
    timeNums[4] = deserializer.deserializeSSP(":/timeNums/timeFour.ssp");
    timeNums[5] = deserializer.deserializeSSP(":/timeNums/timeFive.ssp");
    timeNums[6] = deserializer.deserializeSSP(":/timeNums/timeSix.ssp");
    timeNums[7] = deserializer.deserializeSSP(":/timeNums/timeSeven.ssp");
    timeNums[8] = deserializer.deserializeSSP(":/timeNums/timeEight.ssp");
    timeNums[9] = deserializer.deserializeSSP(":/timeNums/timeNine.ssp");

    buttonImages[0] = deserializer.deserializeSSP(":/squareImages/smileyFace.ssp");
    buttonImages[1] = deserializer.deserializeSSP(":/squareImages/ohFace.ssp");
    buttonImages[2] = deserializer.deserializeSSP(":/squareImages/deathFace.ssp");
    buttonImages[3] = deserializer.deserializeSSP(":/squareImages/shadesFace.ssp");

    ui->restartButton->setIcon(QIcon(QPixmap::fromImage(buttonImages[0])));
    ui->restartButton->setIconSize(buttonImages[0].size());

    setNumMines(99);
    updateSecondsSlot(0);
    updateMinefield();
}


/// \brief MinesweeperWindow::~MinesweeperWindow
/// Destructor
MinesweeperWindow::~MinesweeperWindow()
{
    delete ui;
}

/// \brief MinesweeperWindow::restartButtonSlot
/// Gets signal from button.  Emits one for model.
void MinesweeperWindow::restartButtonSlot()
{
    emit restartButton();
}

/// \brief MinesweeperWindow::resetMinefieldSlot
/// Gets signal from model.
/// Shades all squares and changes face on button
/// \param numMines
void MinesweeperWindow::resetMinefieldSlot(int numMines)
{
    for(int i = 0; i < sizeX; i ++)
    {
        for(int j = 0; j < sizeY; j++)
        {
            shadeSquare(i, j);
        }
    }

    ui->restartButton->setIcon(QIcon(QPixmap::fromImage(buttonImages[0])));
    ui->restartButton->setIconSize(buttonImages[0].size());

    setNumMines(numMines);
    updateMinefield();
}

/// \brief MinesweeperWindow::updateSecondsSlot
/// Gets a signal from the model.
/// \param seconds
void MinesweeperWindow::updateSecondsSlot(int seconds)
{
    int hunds,tens,ones;
    hunds = seconds / 100;
    tens = seconds % 100 /10;
    ones = seconds % 10;

    QImage time(QImage(99, 64, QImage::Format_ARGB32));

    //hundreds
    for(int i = 0; i < 33; i++)
    {
        for(int j = 0; j < 64; j++)
        {
            time.setPixelColor(i, j, timeNums[hunds].pixelColor(QPoint(i,j)));
        }
    }

    //tens
    for(int i = 0; i < 33; i++)
    {
        for(int j = 0; j < 64; j++)
        {
            time.setPixelColor(i + 33, j, timeNums[tens].pixelColor(QPoint(i,j)));
        }
    }

    //ones
    for(int i = 0; i < 33; i++)
    {
        for(int j = 0; j < 64; j++)
        {
            time.setPixelColor(i + 66, j, timeNums[ones].pixelColor(QPoint(i,j)));
        }
    }

    ui->timerLabel->setPixmap(QPixmap::fromImage(time).scaled(99, 64, Qt::IgnoreAspectRatio, Qt::FastTransformation));
}

/// \brief MinesweeperWindow::mouseDraggingSLot
/// Gets signal from minefield.
/// \param x
/// \param y
void MinesweeperWindow::mouseDraggingSlot(int x, int y)
{
    emit mouseDrag(x/32, y/32);
}

/// \brief MinesweeperWindow::showSelectedSquareSlot
/// Gets signal from model.
/// Colors a square as flat and updates button face.
/// \param x
/// \param y
void MinesweeperWindow::showSelectedSquareSlot(int x, int y)
{
    QColor color = QColor(180, 180, 180);
    for(int i = 0; i < 32; i++)
    {
        for(int j = 0; j < 32; j++)
        {
            minefieldImage.setPixelColor(x * 32 + i, y * 32 + j, color);
        }
    }

    ui->restartButton->setIcon(QIcon(QPixmap::fromImage(buttonImages[1])));
    ui->restartButton->setIconSize(buttonImages[1].size());

    updateMinefield();
}

/// \brief MinesweeperWindow::unselectSquareSlot
/// Gets signal from model.
/// Shades a square that was previously selected and updates button face.
/// \param x
/// \param y
void MinesweeperWindow::unselectSquareSlot(int x, int y)
{
    shadeSquare(x, y);

    ui->restartButton->setIcon(QIcon(QPixmap::fromImage(buttonImages[0])));
    ui->restartButton->setIconSize(buttonImages[0].size());

    updateMinefield();
}

/// \brief MinesweeperWindow::squareClickedSlot
/// Gets signal from minefield.
/// \param x
/// \param y
void MinesweeperWindow::squareClickedSlot(int x, int y)
{
    emit squareClicked(x/32, y/32);
}

/// \brief MinesweeperWindow::validSquareSlot
/// Gets signal from the model.
/// Draws a revealed square.
/// \param numMines
void MinesweeperWindow::validSquareSlot(int numMines, int x, int y)
{
    for(int i = 0; i < 32; i++)
    {
        for(int j = 0; j < 32; j++)
        {
            minefieldImage.setPixelColor(x * 32 + i, y * 32 + j, images[numMines].pixelColor(QPoint(i,j)));
        }
    }

    ui->restartButton->setIcon(QIcon(QPixmap::fromImage(buttonImages[0])));
    ui->restartButton->setIconSize(buttonImages[0].size());

    updateMinefield();
}

/// \brief hitMineSlot
/// Gets a signal from the model.
/// Red color mine and death face.
/// \param x
/// \param y
 void MinesweeperWindow::hitMineSlot(int x, int y)
 {
    for(int i = 0; i < 32; i++)
     {
        for(int j = 0; j < 32; j++)
        {
           minefieldImage.setPixelColor(x * 32 + i, y * 32 + j, images[10].pixelColor(QPoint(i,j)));
        }
    }

    ui->restartButton->setIcon(QIcon(QPixmap::fromImage(buttonImages[2])));
    ui->restartButton->setIconSize(buttonImages[2].size());

    updateMinefield();
 }

/// \brief MinesweeperWindow::invalidSquareSlot
/// Gets signal from the model.
/// The user clicked on a mine.
/// \param x
/// \param y
void MinesweeperWindow::invalidSquareSlot(int x, int y)
{
    for(int i = 0; i < 32; i++)
    {
        for(int j = 0; j < 32; j++)
        {
            minefieldImage.setPixelColor(x * 32 + i, y * 32 + j, images[9].pixelColor(QPoint(i,j)));
        }
    }

    updateMinefield();
}

/// \brief MinesweeperWindow::rightCLickSlot
/// Gets signal from minefield.  Sends one to the model.
/// \param x
/// \param y
void MinesweeperWindow::rightCLickSlot(int x, int y)
{
    emit rightClicked(x/32, y/32);
}

/// \brief MinesweeperWindow::spaceHitSlot
/// Emits signal to the model.
/// \param x
/// \param y
void MinesweeperWindow::spaceHitSlot(int x, int y)
{
    emit spaceHit(x / 32, y / 32);
}

/// \brief MinesweeperWindow::displayFlagSlot
/// Gets signal from the model.
/// \param x
/// \param y
void MinesweeperWindow::displayFlagSlot(int minesLeft, int x, int y)
{
    for(int i = 0; i < 32; i++)
    {
        for(int j = 0; j < 32; j++)
        {
            minefieldImage.setPixelColor(x * 32 + i, y * 32 + j, images[11].pixelColor(QPoint(i,j)));
        }
    }

    setNumMines(minesLeft);
    updateMinefield();
}

/// \brief MinesweeperWindow::removeFlagSlot
/// Gets signal from the model.
/// \param x
/// \param y
void MinesweeperWindow::removeFlagSlot(int minesLeft, int x, int y)
{
    shadeSquare(x, y);

    setNumMines(minesLeft);
    updateMinefield();
}

/// \brief MinesweeperWindow::falseFlagSlot
/// Gets a signal from the model.
/// \param x
/// \param y
void MinesweeperWindow::falseFlagSlot(int x, int y)
{
    for(int i = 0; i < 32; i++)
    {
        for(int j = 0; j < 32; j++)
        {
            minefieldImage.setPixelColor(x * 32 + i, y * 32 + j, images[12].pixelColor(QPoint(i,j)));
        }
    }

    updateMinefield();
}

/// \brief MinesweeperWindow::playerWinsSlot
/// Updates the smiley face for a win.
void MinesweeperWindow::playerWinsSlot()
{
    ui->restartButton->setIcon(QIcon(QPixmap::fromImage(buttonImages[3])));
    ui->restartButton->setIconSize(buttonImages[3].size());

    updateMinefield();
}

/// \brief MinesweeperWindow::shadeSquare
/// Helper method to clean up readablility.  Draws a shaded square.
/// i is x-coordinate.  j is y-coordinate.
/// \param i
/// \param j
void MinesweeperWindow::shadeSquare(int i, int j)
{
    int lightness = 120;
    for(int x = 0; x < 32; x++)
    {
        for(int y = 0; y < 32; y++)
        {
            //minefieldImage.setPixelColor(i*32 + x, j * 32 + y, QColor(180, 180, 180));
            QColor color = QColor(180, 180, 180);
            // Top and left sides
            if ((x < 4 && y < 28) || (y < 4 && x < 28))
            {
                color = color.lighter(lightness); // Lighten the top and left sides of the square
            }
            // Bottom and right sides
            else if ((x > 28 && y > 4) || (y > 28 && x > 4))
            {
                color = color.darker(lightness); // Darken the bottom and right sides of the square
            }
            // Lighten top left and bottom right corners
            else if((x == 28 && y < 4) ||
                    (x == 29 && y < 3) ||
                    (x == 30 && y < 2) ||
                    (x == 31 && y < 1) ||
                    (y == 28 && x < 4) ||
                    (y == 29 && x < 3) ||
                    (y == 30 && x < 2) ||
                    (y == 31 && x < 1))
            {
                color = color.lighter(lightness);
            }
            // Darken top left and bottom right corners
            else if((x == 28 && y < 5) ||
                    (x == 29 && y < 5) ||
                    (x == 30 && y < 5) ||
                    (x == 31 && y < 5) ||
                    (y == 28 && x < 5) ||
                    (y == 29 && x < 5) ||
                    (y == 30 && x < 5) ||
                    (y == 31 && x < 5))
            {
                color = color.darker(lightness);
            }
            minefieldImage.setPixelColor(i*32 + x, j * 32 + y, color);
        }
    }
}

/// \brief MinesweeperWindow::updateMinefield
/// Called when the minefield needs to be redrawn.  Mostly at the end of other methods.
void MinesweeperWindow::updateMinefield()
{
    ui->label->setPixmap(QPixmap::fromImage(minefieldImage).scaled(960, 512, Qt::IgnoreAspectRatio, Qt::FastTransformation));
    ui->label->setFocus();
}

/// \brief MinesweeperWindow::setNumMines
/// \param numMines
void MinesweeperWindow::setNumMines(int numMines)
{
    int hunds,tens,ones;
    hunds = numMines / 100;
    tens = numMines % 100 /10;
    ones = numMines % 10;

    QImage number(QImage(99, 64, QImage::Format_ARGB32));

    //hundreds
    for(int i = 0; i < 33; i++)
    {
        for(int j = 0; j < 64; j++)
        {
            number.setPixelColor(i, j, timeNums[hunds].pixelColor(QPoint(i,j)));
        }
    }

    //tens
    for(int i = 0; i < 33; i++)
    {
        for(int j = 0; j < 64; j++)
        {
            number.setPixelColor(i + 33, j, timeNums[tens].pixelColor(QPoint(i,j)));
        }
    }

    //ones
    for(int i = 0; i < 33; i++)
    {
        for(int j = 0; j < 64; j++)
        {
            number.setPixelColor(i + 66, j, timeNums[ones].pixelColor(QPoint(i,j)));
        }
    }

    ui->numMinesLabel->setPixmap(QPixmap::fromImage(number).scaled(99, 64, Qt::IgnoreAspectRatio, Qt::FastTransformation));
}
