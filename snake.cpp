#include "snake.h"
#include <QPainter>
#include <QTimer>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QPainterPath>
#include <QFontDatabase>
#include <QParallelAnimationGroup>
#include <QApplication>
#include <QDebug>
#include <algorithm>

/**
 * @brief Constructor For The Snake Game Widget
 * @param parent Parent Widget Pointer
 */
Snake::Snake(QWidget *parent)
    : QWidget(parent)
    , m_gameOver(false)
    , m_paused(false)
    , m_score(0)
    , m_gridSize(20)
    , m_cellSize(30)
    , m_headScale(1.0)
    , m_shadowOffset(0)
    , m_currentDifficulty(1)
{
    // Configure Window Properties
    setFixedSize(600, 600);
    setFocusPolicy(Qt::StrongFocus);
    setStyleSheet("background-color: transparent;");

    // Initialize Visual Components
    createGradientBackground();

    // Setup Game Timer
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Snake::gameLoop);

    // Initialize Head Animation
    m_headAnimation = new QPropertyAnimation(this, "headScale");
    m_headAnimation->setDuration(300);
    m_headAnimation->setKeyValueAt(0, 1.0);
    m_headAnimation->setKeyValueAt(0.5, 1.2);
    m_headAnimation->setKeyValueAt(1, 1.0);
    m_headAnimation->setEasingCurve(QEasingCurve::OutBounce);
    connect(m_headAnimation, &QPropertyAnimation::finished, this, &Snake::animateHead);

    // Initialize Shadow Animation
    m_shadowAnimation = new QPropertyAnimation(this, "shadowOffset");
    m_shadowAnimation->setDuration(200);
    m_shadowAnimation->setStartValue(0);
    m_shadowAnimation->setEndValue(5);
    m_shadowAnimation->setEasingCurve(QEasingCurve::OutQuad);

    // Setup Particle Effects Timer
    m_particleTimer = new QTimer(this);
    m_particleTimer->setInterval(50);
    connect(m_particleTimer, &QTimer::timeout, [this]() {
        // Update Particle Positions
        for (auto &particle : m_particles) {
            particle.ry() += 1;
        }

        // Remove Particles That Have Fallen Off Screen
        m_particles.erase(
            std::remove_if(m_particles.begin(), m_particles.end(),
                           [this](const QPointF &p) { return p.y() > height(); }),
            m_particles.end());
        update();
    });

    // Initialize Snake Color Palette
    for (int i = 0; i < 10; ++i) {
        int hue = (i * 36) % 360;
        m_snakeColors[i] = QColor::fromHsv(hue, 255, 200);
    }
}

/**
 * @brief Destructor Ensures Proper Cleanup Of Resources
 */
Snake::~Snake()
{
    m_particleTimer->stop();
}

/**
 * @brief Creates The Gradient Background For The Game Area
 */
void Snake::createGradientBackground()
{
    m_backgroundGradient = QLinearGradient(0, 0, width(), height());
    m_backgroundGradient.setColorAt(0.0, QColor(20, 30, 40));
    m_backgroundGradient.setColorAt(0.5, QColor(30, 40, 50));
    m_backgroundGradient.setColorAt(1.0, QColor(20, 30, 40));
}

/**
 * @brief Initializes A New Game With Specified Difficulty
 * @param difficulty Difficulty Level (0 = Easy, 1 = Medium, 2 = Hard)
 */
void Snake::initGame(int difficulty)
{
    m_currentDifficulty = difficulty;

    // Set Game Speed Based On Difficulty Level
    int speed;
    switch(difficulty) {
    case 0:
        speed = 200;
        break;
    case 1:
        speed = 120;
        break;
    case 2:
        speed = 70;
        break;
    default:
        speed = 120;
    }

    // Initialize Snake Starting Position
    m_snake.clear();
    m_snake.append(QPoint(10, 10));
    m_snake.append(QPoint(9, 10));
    m_snake.append(QPoint(8, 10));

    // Reset Game State
    m_direction = QPoint(1, 0);
    m_nextDirection = QPoint(1, 0);
    m_score = 0;
    m_gameOver = false;
    m_paused = false;

    // Start Game Components
    updateScore(0);
    spawnFood();
    m_timer->start(speed);
    m_headAnimation->start();

    emit gameStarted();
    update();
}

/**
 * @brief Pauses The Current Game
 */
void Snake::pauseGame()
{
    if (!m_paused && !m_gameOver) {
        m_timer->stop();
        m_particleTimer->stop();
        m_paused = true;
        emit gamePaused();
        update();
    }
}

/**
 * @brief Resumes A Paused Game
 */
void Snake::resumeGame()
{
    if (m_paused) {
        m_timer->start();
        m_particleTimer->start();
        m_paused = false;
        emit gameResumed();
        update();
    }
}

/**
 * @brief Changes Game Difficulty During Runtime
 * @param difficulty New Difficulty Level
 */
void Snake::setDifficulty(int difficulty)
{
    m_currentDifficulty = difficulty;
    int speed;
    switch(difficulty) {
    case 0: speed = 200; break;
    case 1: speed = 120; break;
    case 2: speed = 70; break;
    default: speed = 120;
    }
    if (!m_gameOver && !m_paused) {
        m_timer->setInterval(speed);
    }
}

/**
 * @brief Spawns Food At A Random Valid Position On The Grid
 */
void Snake::spawnFood()
{
    int maxX = width() / m_cellSize - 1;
    int maxY = height() / m_cellSize - 1;

    if (maxX <= 0 || maxY <= 0) return;

    // Find Valid Position Not Occupied By Snake
    int attempts = 0;
    bool validPosition = false;

    while (!validPosition && attempts < 1000) {
        int x = QRandomGenerator::global()->bounded(0, maxX);
        int y = QRandomGenerator::global()->bounded(0, maxY);
        m_food = QPoint(x, y);

        if (!m_snake.contains(m_food)) {
            validPosition = true;
        }
        attempts++;
    }

    // Create Particle Effect For New Food
    for (int i = 0; i < 8; ++i) {
        m_particles.append(QPointF(
            m_food.x() * m_cellSize + m_cellSize/2,
            m_food.y() * m_cellSize + m_cellSize/2
            ));
    }
    m_particleTimer->start();
}

/**
 * @brief Moves The Snake In The Current Direction
 */
void Snake::moveSnake()
{
    if (m_gameOver || m_paused) return;

    // Update Direction If Valid (Prevent Reversing)
    if (m_nextDirection != -m_direction && m_nextDirection != QPoint(0,0)) {
        m_direction = m_nextDirection;
    }

    // Add New Head Position
    QPoint newHead = m_snake.first() + m_direction;
    m_snake.prepend(newHead);

    // Handle Food Collision
    if (newHead == m_food) {
        updateScore(m_score + 1);
        spawnFood();
        m_headAnimation->start();
        m_shadowAnimation->start();
    } else {
        m_snake.removeLast();
    }

    // Check For Collision After Movement
    if (checkCollision()) {
        m_timer->stop();
        m_particleTimer->stop();
        m_gameOver = true;
        showGameOverDialog();
        emit gameOver(m_score);
        update();
    }
}

/**
 * @brief Checks For Wall And Self Collision
 * @return true If Collision Detected, false Otherwise
 */
bool Snake::checkCollision()
{
    if (m_snake.isEmpty()) return true;

    QPoint head = m_snake.first();
    int maxX = width() / m_cellSize;
    int maxY = height() / m_cellSize;

    // Check Wall Collision
    if (head.x() < 0 || head.x() >= maxX || head.y() < 0 || head.y() >= maxY) {
        return true;
    }

    // Check Self Collision
    for (int i = 1; i < m_snake.size(); ++i) {
        if (m_snake[i] == head) return true;
    }

    return false;
}

/**
 * @brief Main Game Loop Called By Timer
 */
void Snake::gameLoop()
{
    moveSnake();
    update();
}

/**
 * @brief Resets Head Scale After Animation Completes
 */
void Snake::animateHead()
{
    m_headScale = 1.0;
}

/**
 * @brief Handles Keyboard Input For Game Control
 * @param event Key Press Event
 */
void Snake::keyPressEvent(QKeyEvent *event)
{
    if (m_gameOver) return;

    // Set Next Direction Based On Key Press
    if (event->key() == Qt::Key_Up && m_direction != QPoint(0, 1)) {
        m_nextDirection = QPoint(0, -1);
    }
    else if (event->key() == Qt::Key_Down && m_direction != QPoint(0, -1)) {
        m_nextDirection = QPoint(0, 1);
    }
    else if (event->key() == Qt::Key_Left && m_direction != QPoint(1, 0)) {
        m_nextDirection = QPoint(-1, 0);
    }
    else if (event->key() == Qt::Key_Right && m_direction != QPoint(-1, 0)) {
        m_nextDirection = QPoint(1, 0);
    }
    else if (event->key() == Qt::Key_Space) {
        if (m_paused) resumeGame();
        else pauseGame();
    }
    else if (event->key() == Qt::Key_Escape) {
        if (!m_gameOver) {
            m_timer->stop();
            m_particleTimer->stop();
            hide();
            emit backToHome();
        }
    }

    QWidget::keyPressEvent(event);
}

/**
 * @brief Handles Widget Resize Events
 * @param event Resize Event
 */
void Snake::resizeEvent(QResizeEvent *event)
{
    createGradientBackground();
    QWidget::resizeEvent(event);
}

/**
 * @brief Renders The Game Graphics
 * @param event Paint Event (Unused)
 */
void Snake::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // Draw Background
    painter.fillRect(rect(), m_backgroundGradient);

    // Draw Grid Overlay
    painter.setPen(QPen(QColor(255, 255, 255, 20), 1));
    for (int x = 0; x < width(); x += m_cellSize) {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y < height(); y += m_cellSize) {
        painter.drawLine(0, y, width(), y);
    }

    int blockSize = m_cellSize - 2;

    // Draw Food
    if (m_food.x() >= 0 && m_food.y() >= 0) {
        QRadialGradient foodGradient(
            m_food.x() * m_cellSize + m_cellSize/2,
            m_food.y() * m_cellSize + m_cellSize/2,
            m_cellSize * 0.8
            );
        foodGradient.setColorAt(0, Qt::yellow);
        foodGradient.setColorAt(0.7, QColor(255, 200, 0));
        foodGradient.setColorAt(1, QColor(255, 100, 0));

        painter.setBrush(foodGradient);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(
            m_food.x() * m_cellSize + 2,
            m_food.y() * m_cellSize + 2,
            blockSize - 4,
            blockSize - 4
            );
    }

    // Draw Particle Effects
    painter.setBrush(QColor(255, 255, 255, 100));
    for (const auto &particle : m_particles) {
        painter.drawEllipse(particle, 3, 3);
    }

    // Draw Snake Body
    for (int i = 0; i < m_snake.size(); ++i) {
        QPoint part = m_snake[i];
        QRectF rect(
            part.x() * m_cellSize + 2,
            part.y() * m_cellSize + 2,
            blockSize - 4,
            blockSize - 4
            );

        if (i == 0) {
            // Draw Snake Head With Special Effects
            QRadialGradient headGradient(rect.center(), rect.width());
            headGradient.setColorAt(0, Qt::red);
            headGradient.setColorAt(0.7, QColor(200, 0, 0));
            headGradient.setColorAt(1, QColor(150, 0, 0));

            painter.setBrush(headGradient);

            // Apply Head Scale Animation
            if (m_headScale != 1.0) {
                rect = QRectF(
                    rect.x() - (rect.width() * (m_headScale - 1)) / 2,
                    rect.y() - (rect.height() * (m_headScale - 1)) / 2,
                    rect.width() * m_headScale,
                    rect.height() * m_headScale
                    );
            }

            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(rect, 10, 10);

            // Draw Eyes Based On Direction
            painter.setBrush(Qt::white);
            float eyeSize = rect.width() * 0.2;
            float pupilSize = eyeSize * 0.5;

            if (m_direction.x() > 0) {
                // Right Direction
                painter.drawEllipse(rect.right() - eyeSize * 2.5, rect.top() + eyeSize, eyeSize, eyeSize);
                painter.drawEllipse(rect.right() - eyeSize * 2.5, rect.bottom() - eyeSize * 2, eyeSize, eyeSize);

                painter.setBrush(Qt::black);
                painter.drawEllipse(rect.right() - eyeSize * 2.5 + pupilSize/2, rect.top() + eyeSize + pupilSize/2, pupilSize, pupilSize);
                painter.drawEllipse(rect.right() - eyeSize * 2.5 + pupilSize/2, rect.bottom() - eyeSize * 2 + pupilSize/2, pupilSize, pupilSize);
            }
            else if (m_direction.x() < 0) {
                // Left Direction
                painter.drawEllipse(rect.left() + eyeSize * 1.5, rect.top() + eyeSize, eyeSize, eyeSize);
                painter.drawEllipse(rect.left() + eyeSize * 1.5, rect.bottom() - eyeSize * 2, eyeSize, eyeSize);

                painter.setBrush(Qt::black);
                painter.drawEllipse(rect.left() + eyeSize * 1.5 + pupilSize/2, rect.top() + eyeSize + pupilSize/2, pupilSize, pupilSize);
                painter.drawEllipse(rect.left() + eyeSize * 1.5 + pupilSize/2, rect.bottom() - eyeSize * 2 + pupilSize/2, pupilSize, pupilSize);
            }
            else if (m_direction.y() > 0) {
                // Down Direction
                painter.drawEllipse(rect.left() + eyeSize, rect.bottom() - eyeSize * 2.5, eyeSize, eyeSize);
                painter.drawEllipse(rect.right() - eyeSize * 2, rect.bottom() - eyeSize * 2.5, eyeSize, eyeSize);

                painter.setBrush(Qt::black);
                painter.drawEllipse(rect.left() + eyeSize + pupilSize/2, rect.bottom() - eyeSize * 2.5 + pupilSize/2, pupilSize, pupilSize);
                painter.drawEllipse(rect.right() - eyeSize * 2 + pupilSize/2, rect.bottom() - eyeSize * 2.5 + pupilSize/2, pupilSize, pupilSize);
            }
            else {
                // Up Direction
                painter.drawEllipse(rect.left() + eyeSize, rect.top() + eyeSize * 1.5, eyeSize, eyeSize);
                painter.drawEllipse(rect.right() - eyeSize * 2, rect.top() + eyeSize * 1.5, eyeSize, eyeSize);

                painter.setBrush(Qt::black);
                painter.drawEllipse(rect.left() + eyeSize + pupilSize/2, rect.top() + eyeSize * 1.5 + pupilSize/2, pupilSize, pupilSize);
                painter.drawEllipse(rect.right() - eyeSize * 2 + pupilSize/2, rect.top() + eyeSize * 1.5 + pupilSize/2, pupilSize, pupilSize);
            }

        } else {
            // Draw Snake Body Segments With Gradient Colors
            int colorIndex = i % 10;
            QColor bodyColor = m_snakeColors.contains(colorIndex) ?
                                   m_snakeColors[colorIndex] :
                                   QColor(0, 100, 200);

            QLinearGradient bodyGradient(rect.topLeft(), rect.bottomRight());
            bodyGradient.setColorAt(0, bodyColor.lighter(120));
            bodyGradient.setColorAt(1, bodyColor.darker(120));

            painter.setBrush(bodyGradient);
            painter.setPen(QPen(QColor(255, 255, 255, 50), 1));
            painter.drawRoundedRect(rect, 8, 8);
        }
    }

    // Draw Shadow Effect On Food
    if (m_shadowOffset > 0 && m_food.x() >= 0 && m_food.y() >= 0) {
        painter.setBrush(QColor(0, 0, 0, 50));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(m_food.x() * m_cellSize + m_cellSize/2 + m_shadowOffset,
                            m_food.y() * m_cellSize + m_cellSize/2 + m_shadowOffset,
                            m_cellSize/2, m_cellSize/2);
    }

    // Draw Current Score
    painter.setPen(Qt::white);
    QFont scoreFont = QFont("Arial", 24, QFont::Bold);
    scoreFont.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    painter.setFont(scoreFont);

    QRect scoreRect(0, height() - 60, width(), 40);
    painter.drawText(scoreRect, Qt::AlignCenter, QString("%1").arg(m_score));

    // Draw Pause Indicator
    if (m_paused) {
        painter.setPen(QPen(QColor(255, 255, 255, 200), 3));
        painter.setFont(QFont("Arial", 32, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "PAUSED");
    }
}

/**
 * @brief Updates The Game Score
 * @param newScore New Score Value
 */
void Snake::updateScore(int newScore)
{
    if (newScore != m_score) {
        m_score = newScore;
    }
}

/**
 * @brief Displays The Game Over Dialog With Score And Options
 */
void Snake::showGameOverDialog()
{
    // Create Overlay Widget
    QWidget *overlay = new QWidget(this);
    overlay->setFixedSize(400, 480);
    overlay->move((width() - 400) / 2, (height() - 480) / 2);

    overlay->setStyleSheet(
        "background-color: #1a1e2b;"
        "border-radius: 40px;"
        "border: 1px solid #2a2f3c;"
        );

    // Add Shadow Effect
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setBlurRadius(40);
    shadowEffect->setColor(QColor(0, 0, 0, 150));
    shadowEffect->setOffset(0, 8);
    overlay->setGraphicsEffect(shadowEffect);

    // Setup Main Layout
    QVBoxLayout *layout = new QVBoxLayout(overlay);
    layout->setSpacing(15);
    layout->setContentsMargins(40, 35, 40, 35);

    // Create Game Over Title
    QLabel *gameOverLabel = new QLabel("GAME OVER");
    gameOverLabel->setFixedHeight(60);
    gameOverLabel->setAlignment(Qt::AlignCenter);
    gameOverLabel->setStyleSheet(
        "QLabel {"
        "   background-color: #2a2f3c;"
        "   color: #ff5e5e;"
        "   border-radius: 30px;"
        "   font-size: 28px;"
        "   font-weight: 900;"
        "   font-family: 'Arial Black';"
        "   letter-spacing: 2px;"
        "}"
        );
    layout->addWidget(gameOverLabel);

    // Add Decorative Line
    QFrame *line = new QFrame();
    line->setFixedHeight(4);
    line->setFixedWidth(200);
    line->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 transparent, stop:0.5 #3498db, stop:1 transparent);"
        "border-radius: 2px;"
        );
    layout->addWidget(line, 0, Qt::AlignCenter);

    layout->addSpacing(5);

    // Create Score Display Card
    QWidget *scoreCard = new QWidget();
    scoreCard->setFixedHeight(110);
    scoreCard->setStyleSheet(
        "QWidget {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #232837, stop:1 #1c212d);"
        "   border-radius: 55px;"
        "   border: 1px solid #2a2f3c;"
        "}"
        );

    QVBoxLayout *scoreLayout = new QVBoxLayout(scoreCard);
    scoreLayout->setContentsMargins(0, 15, 0, 15);
    scoreLayout->setSpacing(0);

    QLabel *scoreTitle = new QLabel("YOUR SCORE");
    scoreTitle->setAlignment(Qt::AlignCenter);
    scoreTitle->setStyleSheet("color: #8f9bb5; font-size: 13px; font-weight: 700; border: none; background: transparent;");

    QLabel *scoreValue = new QLabel(QString("%1").arg(m_score));
    scoreValue->setAlignment(Qt::AlignCenter);
    scoreValue->setStyleSheet("color: #ffffff; font-size: 46px; font-weight: 800; border: none; background: transparent;");

    scoreLayout->addWidget(scoreTitle);
    scoreLayout->addWidget(scoreValue);
    layout->addWidget(scoreCard);

    layout->addSpacing(15);

    // Create Action Buttons
    QPushButton *restartBtn = new QPushButton("PLAY AGAIN");
    restartBtn->setCursor(Qt::PointingHandCursor);
    restartBtn->setFixedHeight(56);
    restartBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2ecc71, stop:1 #27ae60);"
        "   color: white;"
        "   border-radius: 28px;"
        "   font-size: 16px;"
        "   font-weight: 700;"
        "   border: none;"
        "}"
        "QPushButton:hover { background-color: #27ae60; }"
        );

    QPushButton *homeBtn = new QPushButton("HOME");
    homeBtn->setCursor(Qt::PointingHandCursor);
    homeBtn->setFixedHeight(56);
    homeBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #2f3545;"
        "   color: #e5e7eb;"
        "   border-radius: 28px;"
        "   font-size: 16px;"
        "   font-weight: 700;"
        "   border: none;"
        "}"
        "QPushButton:hover { background-color: #3f4559; }"
        );

    layout->addWidget(restartBtn);
    layout->addWidget(homeBtn);

    // Display Overlay With Fade Animation
    overlay->show();
    overlay->raise();

    QPropertyAnimation *fadeAnim = new QPropertyAnimation(overlay, "windowOpacity");
    fadeAnim->setDuration(350);
    fadeAnim->setStartValue(0);
    fadeAnim->setEndValue(1);
    fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);

    // Connect Button Signals
    connect(restartBtn, &QPushButton::clicked, [this, overlay](){
        overlay->deleteLater();
        initGame(m_currentDifficulty);
    });

    connect(homeBtn, &QPushButton::clicked, [this, overlay](){
        overlay->deleteLater();
        this->hide();
        emit backToHome();
    });
}
