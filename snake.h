#ifndef SNAKE_H
#define SNAKE_H

// Include Required Qt Core And GUI Components
#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QPoint>
#include <QKeyEvent>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QMap>
#include <QPointF>
#include <QColor>
#include <QPainter>
#include <QResizeEvent>

// Snake Game Widget Class Responsible For Core Gameplay Logic And Rendering
class Snake : public QWidget
{
    Q_OBJECT

    // Property For Head Scaling Animation Effect
    Q_PROPERTY(float headScale READ headScale WRITE setHeadScale)

    // Property For Shadow Offset Animation Effect
    Q_PROPERTY(int shadowOffset READ shadowOffset WRITE setShadowOffset)

    // Property To Control Game Pause State
    Q_PROPERTY(bool paused READ isPaused WRITE setPaused)

public:
    // Constructor To Initialize Snake Game Widget
    explicit Snake(QWidget *parent = nullptr);

    // Destructor To Release Allocated Resources
    ~Snake();

    // Initialize Or Reset Game With Selected Difficulty
    void initGame(int difficulty = 1);

    // Pause Current Game Session
    void pauseGame();

    // Resume Paused Game Session
    void resumeGame();

    // Set Current Difficulty Level
    void setDifficulty(int difficulty);

    // Getter And Setter For Head Scale Property
    float headScale() const { return m_headScale; }
    void setHeadScale(float scale) { m_headScale = scale; update(); }

    // Getter And Setter For Shadow Offset Property
    int shadowOffset() const { return m_shadowOffset; }
    void setShadowOffset(int offset) { m_shadowOffset = offset; update(); }

    // Getter And Setter For Pause State Property
    bool isPaused() const { return m_paused; }
    void setPaused(bool paused) {
        if (paused) pauseGame();
        else resumeGame();
    }

signals:
    // Signal Emitted When Game Starts
    void gameStarted();

    // Signal Emitted When Game Is Paused
    void gamePaused();

    // Signal Emitted When Game Is Resumed
    void gameResumed();

    // Signal Emitted When Game Ends With Final Score
    void gameOver(int finalScore);

    // Signal Emitted To Navigate Back To Home Screen
    void backToHome();

protected:
    // Handle Keyboard Input For Snake Movement And Controls
    void keyPressEvent(QKeyEvent *event) override;

    // Render Snake, Food, Background, And Visual Effects
    void paintEvent(QPaintEvent *event) override;

    // Adjust Layout And Grid When Window Is Resized
    void resizeEvent(QResizeEvent *event) override;

private slots:
    // Main Game Loop Executed On Timer Timeout
    void gameLoop();

    // Animate Snake Head For Visual Enhancement
    void animateHead();

private:
    // Move Snake According To Current Direction
    void moveSnake();

    // Check For Wall Or Self Collision
    bool checkCollision();

    // Spawn Food At Random Valid Position
    void spawnFood();

    // Display Game Over Dialog With Final Score
    void showGameOverDialog();

    // Update Score And Trigger UI Changes
    void updateScore(int newScore);

    // Create Gradient Background For Game Area
    void createGradientBackground();

    // Game State Variables
    QVector<QPoint> m_snake;        // Stores Snake Body Segments
    QPoint m_food;                  // Current Food Position
    QPoint m_direction;             // Current Movement Direction
    QPoint m_nextDirection;         // Buffered Next Direction
    QTimer *m_timer;                // Main Game Loop Timer
    bool m_gameOver;                // Game Over Flag
    bool m_paused;                  // Pause State Flag
    int m_score;                    // Current Player Score
    int m_gridSize;                 // Grid Dimension Count
    int m_cellSize;                 // Pixel Size Of Each Grid Cell
    int m_currentDifficulty;        // Selected Difficulty Level

    // Visual Enhancement Variables
    float m_headScale;              // Head Animation Scaling Factor
    int m_shadowOffset;             // Shadow Offset For Visual Depth
    QPropertyAnimation *m_headAnimation;   // Animation For Head Scaling
    QPropertyAnimation *m_shadowAnimation; // Animation For Shadow Effect
    QLinearGradient m_backgroundGradient;  // Gradient Background
    QMap<int, QColor> m_snakeColors;       // Color Map For Snake Body

    // Particle Effect Variables
    QVector<QPointF> m_particles;   // Decorative Particle Positions
    QTimer *m_particleTimer;        // Timer For Particle Animation
};

#endif // SNAKE_H
