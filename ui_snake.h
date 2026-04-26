/********************************************************************************
** Custom UI for Snake game
**
** Updated: Head and Tail colors, no timer label
********************************************************************************/

#ifndef UI_SNAKE_H
#define UI_SNAKE_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFrame>
#include <QtGui/QFont>
#include <QtCore/QString>

QT_BEGIN_NAMESPACE

class Ui_Snake
{
public:
    QLabel *labelScore;
    QFrame *gameArea; // The playing area

    void setupUi(QWidget *Snake)
    {
        if (Snake->objectName().isEmpty())
            Snake->setObjectName(QString::fromUtf8("Snake"));
        Snake->resize(500, 500);

        // Score label
        labelScore = new QLabel(Snake);
        labelScore->setObjectName(QString::fromUtf8("labelScore"));
        labelScore->setGeometry(0, 0, 500, 30);
        QFont scoreFont;
        scoreFont.setPointSize(14);
        scoreFont.setBold(true);
        labelScore->setFont(scoreFont);
        labelScore->setAlignment(Qt::AlignCenter);
        labelScore->setStyleSheet("color: white; background-color: black;");

        // Game area
        gameArea = new QFrame(Snake);
        gameArea->setObjectName(QString::fromUtf8("gameArea"));
        gameArea->setGeometry(10, 40, 480, 450);
        gameArea->setStyleSheet("background-color: #222; border: 2px solid #555;");
        gameArea->setFrameStyle(QFrame::Box | QFrame::Plain);

        retranslateUi(Snake);
    }

    void retranslateUi(QWidget *Snake)
    {
        Snake->setWindowTitle("Snake Game");
        labelScore->setText("Score: 0");
    }
};

namespace Ui {
class Snake : public Ui_Snake {};
}

QT_END_NAMESPACE

#endif // UI_SNAKE_H
