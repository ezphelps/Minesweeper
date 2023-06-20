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

    //button
    connect(ui->restartButton, &QPushButton::released, this, &MinesweeperWindow::restartButtonSlot);
    connect(this, &MinesweeperWindow::restartButton, &model, &Model::restartButton);
    connect(&model, &Model::resetMinefield, this, &MinesweeperWindow::resetMinefieldSlot);

    //mouse press
    connect(ui->label, &MineField::mousePressed, this, &MinesweeperWindow::mousePressedSlot);
    connect(this, &MinesweeperWindow::mouseDrag, &model, &Model::mouseDrag);
    connect(&model, &Model::selectSquare, this, &MinesweeperWindow::selectSquareSlot);
    connect(&model, &Model::unselectSquare, this, &MinesweeperWindow::unselectSquareSlot);


    //left click
    connect(ui->label, &MineField::mouseRelease, this, &MinesweeperWindow::squareClickedSlot);
    connect(this, &MinesweeperWindow::squareClicked, &model, &Model::squareClicked);
    connect(&model, &Model::validSquare, this, &MinesweeperWindow::validSquareSlot);
    connect(&model, &Model::invalidSquare, this, &MinesweeperWindow::invalidSquareSlot);

    //right click
    connect(ui->label, &MineField::rightClick, this, &MinesweeperWindow::rightCLickSlot);
    connect(this, &MinesweeperWindow::rightClicked, &model, &Model::rightClicked);
    connect(&model, &Model::displayFlag, this, &MinesweeperWindow::displayFlagSlot);
    connect(&model, &Model::removeFlag, this, &MinesweeperWindow::removeFlagSlot);

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
    images[10] = deserializer.deserializeSSP(":/squareImages/flag.ssp");

    buttonImages[0] = deserializer.deserializeSSP(":/squareImages/smileyFace.ssp");
    buttonImages[1] = deserializer.deserializeSSP(":/squareImages/ohFace.ssp");
    buttonImages[2] = deserializer.deserializeSSP(":/squareImages/deathFace.ssp");

    ui->restartButton->setIcon(QIcon(QPixmap::fromImage(buttonImages[0])));
    ui->restartButton->setIconSize(buttonImages[0].size());

    updateMinefield();
}


/// \brief MinesweeperWindow::~MinesweeperWindow
/// Destructor
MinesweeperWindow::~MinesweeperWindow()
{
    delete ui;
}

/// \brief MinesweeperWindow::restartButtonSlot
/// Translates a button click to a signal for the model.
void MinesweeperWindow::restartButtonSlot()
{
    emit restartButton();
}

/// \brief MinesweeperWindow::resetMinefieldSlot
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

    updateMinefield();
}

/// \brief MinesweeperWindow::mousePressedSlot
/// \param x
/// \param y
void MinesweeperWindow::mousePressedSlot(int x, int y)
{
    //ui->restartButton->setIcon(QIcon(QPixmap::fromImage(buttonImages[1])));
    //ui->restartButton->setIconSize(buttonImages[1].size());

    emit mouseDrag(x/32, y/32);
}

/// \brief MinesweeperWindow::showSelectedSquareSlot
/// Colors a square as flat.
/// \param x
/// \param y
void MinesweeperWindow::selectSquareSlot(int x, int y)
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
/// Recieves signal from model.
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
/// Recieves a mouse click signal from the minefield label.
/// \param x
/// \param y
void MinesweeperWindow::squareClickedSlot(int x, int y)
{
    emit squareClicked(x/32, y/32);
}

/// \brief MinesweeperWindow::validSquareSlot
/// Changes a square.
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

/// \brief MinesweeperWindow::invalidSquareSlot
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

    ui->restartButton->setIcon(QIcon(QPixmap::fromImage(buttonImages[2])));
    ui->restartButton->setIconSize(buttonImages[2].size());

    updateMinefield();
}

/// \brief MinesweeperWindow::rightCLickSlot
/// Recieves a right click signal from the minefield label.
/// \param x
/// \param y
void MinesweeperWindow::rightCLickSlot(int x, int y)
{
    emit rightClicked(x/32, y/32);
}

/// \brief MinesweeperWindow::displayFlagSlot
/// \param x
/// \param y
void MinesweeperWindow::displayFlagSlot(int x, int y)
{
    for(int i = 0; i < 32; i++)
    {
        for(int j = 0; j < 32; j++)
        {
            minefieldImage.setPixelColor(x * 32 + i, y * 32 + j, images[10].pixelColor(QPoint(i,j)));
        }
    }

    updateMinefield();
}

/// \brief MinesweeperWindow::removeFlagSlot
/// \param x
/// \param y
void MinesweeperWindow::removeFlagSlot(int x, int y)
{
    shadeSquare(x, y);

    updateMinefield();
}

/// \brief MinesweeperWindow::shadeSquare
/// Helper method to clean up readablility.  i is x.  j is y.
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
void MinesweeperWindow::updateMinefield()
{
    ui->label->setPixmap(QPixmap::fromImage(minefieldImage).scaled(960, 512, Qt::IgnoreAspectRatio, Qt::FastTransformation));
    ui->label->setFocus();
}
