#include <QApplication>
#include "home.h"
#include "snake.h"

/**
 * @brief Main Application Entry Point
 * @param argc Number Of Command Line Arguments
 * @param argv Array Of Command Line Arguments
 * @return Application Exit Code
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set Application-Wide Style
    a.setStyle("Fusion");

    // Create Main Windows
    Home home;
    Snake snake;

    /**
     * @brief Connect Home Screen Start Game Signal
     * When User Clicks Start Game Button, Hide Home And Show Snake With Selected Difficulty
     */
    QObject::connect(&home, &Home::startSnakeGame, [&](int difficulty){
        home.hide();
        snake.show();
        snake.initGame(difficulty);
    });

    /**
     * @brief Connect Game Over Signal
     * When Game Ends, Update High Scores List In Home Screen
     * Game Over Dialog Is Handled By Snake Class Itself
     */
    QObject::connect(&snake, &Snake::gameOver, [&](int finalScore){
        home.addHighScore(finalScore);
    });

    /**
     * @brief Connect Back To Home Signal
     * When User Chooses To Return Home From Game Over Dialog,
     * Hide Snake And Show Home Screen
     */
    QObject::connect(&snake, &Snake::backToHome, [&](){
        snake.hide();
        home.show();
    });

    // Display Home Screen And Start Application Event Loop
    home.show();
    return a.exec();
}
