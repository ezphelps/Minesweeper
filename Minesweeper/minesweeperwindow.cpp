/// MinesweeperWindow represents the ui of the minesweeper gameplay window.
/// Written by: ezphelps

#include "minesweeperwindow.h"
#include "ui_minesweeperwindow.h"

/// \brief MinesweeperWindow::MinesweeperWindow
/// Constructor
/// \param parent
MinesweeperWindow::MinesweeperWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MinesweeperWindow),
      sizeX(30),
      sizeY(16),
      minefieldImage(QImage(sizeX*32, sizeY*32, QImage::Format_ARGB32))
{
    ui->setupUi(this);

    connect(ui->label, &MineField::mousePressed, this, &MinesweeperWindow::squareClicked);

    //Drawing the minefield.
    minefieldImage.fill(QColor(180, 180, 180));
    int lightness = 120;
    for(int i = 0; i < sizeX; i++) // Rows of squares
    {
        for(int j = 0; j < sizeY; j++) // Collumns of squares
        {
            //Each square
            for(int x = 0; x < 32; x++)
            {
                for(int y = 0; y < 32; y++)
                {
                    QColor color = minefieldImage.pixelColor(i*32 + x, j * 32 + y);
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
    }



    ui->label->setPixmap(QPixmap::fromImage(minefieldImage).scaled(960, 512, Qt::IgnoreAspectRatio, Qt::FastTransformation));
}


/// \brief MinesweeperWindow::~MinesweeperWindow
/// Destructor
MinesweeperWindow::~MinesweeperWindow()
{
    delete ui;
}

/// \brief MinesweeperWindow::squareClicked
/// Recieves a mouse press signal from the minefield label.
/// \param x
/// \param y
void MinesweeperWindow::squareClicked(int x, int y)
{
    int squareX = x / 32;
    int squareY = y / 32;

    for(int i = 0; i < 32; i++)
    {
        for(int j = 0; j < 32; j++)
        {
            minefieldImage.setPixelColor(squareX * 32 + i, squareY * 32 + j, QColor(180,180,180));
        }
    }

    ui->label->setPixmap(QPixmap::fromImage(minefieldImage).scaled(960, 512, Qt::IgnoreAspectRatio, Qt::FastTransformation));
}

