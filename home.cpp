#include "home.h"
#include <QPainter>
#include <QTimer>
#include <QApplication>
#include <QFontDatabase>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <algorithm>

// Constructor To Initialize Home Screen And Core Components
Home::Home(QWidget *parent)
    : QWidget(parent)
    , m_currentDifficulty(1)
    , m_titleScale(1.0)
    , m_highScoreValue(nullptr)
{
    setFixedSize(600, 700);
    setWindowTitle("Snake Game");
    setWindowFlags(Qt::FramelessWindowHint);

    // Initialize User Interface And Background
    setupUI();
    createBackground();

    // Load And Display High Scores
    loadHighScores();
    updateHighScoresDisplay();

    // Configure Continuous Title Scaling Animation
    m_titleAnimation = new QPropertyAnimation(this, "titleScale");
    m_titleAnimation->setDuration(2000);
    m_titleAnimation->setKeyValueAt(0, 1.0);
    m_titleAnimation->setKeyValueAt(0.5, 1.1);
    m_titleAnimation->setKeyValueAt(1, 1.0);
    m_titleAnimation->setEasingCurve(QEasingCurve::InOutSine);
    m_titleAnimation->setLoopCount(-1);
    m_titleAnimation->start();
}

// Setup All UI Components And Layout Structure
void Home::setupUI()
{
    // Create Main Vertical Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(40, 30, 40, 30);

    // Configure Main Title Label
    m_titleLabel = new QLabel("SNAKE GAME");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet(
        "color: white;"
        "font-size: 52px;"
        "font-weight: bold;"
        "font-family: 'Arial Black';"
        "background: transparent;"
        "letter-spacing: 4px;"
        "margin-bottom: 10px;"
        );
    addNeonEffect(m_titleLabel);

    // Configure Subtitle Label
    m_subtitleLabel = new QLabel("CLASSIC • MODERN • FUN");
    m_subtitleLabel->setAlignment(Qt::AlignCenter);
    m_subtitleLabel->setStyleSheet(
        "color: rgba(255, 255, 255, 180);"
        "font-size: 16px;"
        "letter-spacing: 3px;"
        "font-family: 'Arial';"
        "background: transparent;"
        "margin-bottom: 20px;"
        );

    mainLayout->addWidget(m_titleLabel);
    mainLayout->addWidget(m_subtitleLabel);

    // Create Difficulty Selection Section
    QLabel *difficultyLabel = new QLabel("SELECT DIFFICULTY");
    difficultyLabel->setAlignment(Qt::AlignCenter);
    difficultyLabel->setStyleSheet(
        "color: #3498db;"
        "font-size: 18px;"
        "font-weight: bold;"
        "letter-spacing: 2px;"
        "margin-top: 20px;"
        "margin-bottom: 10px;"
        );
    mainLayout->addWidget(difficultyLabel);

    // Create Difficulty Radio Buttons Layout
    QHBoxLayout *difficultyLayout = new QHBoxLayout();
    difficultyLayout->setSpacing(30);
    difficultyLayout->setAlignment(Qt::AlignCenter);

    m_difficultyGroup = new QButtonGroup(this);

    // Configure Radio Button Appearance And Register With Button Group
    auto setupRadio = [&](QRadioButton* rb, QString color, int id) {
        rb->setStyleSheet(QString(
                              "QRadioButton { color: %1; font-size: 16px; font-weight: bold; spacing: 10px; }"
                              "QRadioButton::indicator { width: 18px; height: 18px; }"
                              "QRadioButton::indicator:checked { background-color: %1; border-radius: 9px; }"
                              ).arg(color));
        m_difficultyGroup->addButton(rb, id);
        difficultyLayout->addWidget(rb);
    };

    m_easyRadio = new QRadioButton("EASY");
    m_mediumRadio = new QRadioButton("MEDIUM");
    m_hardRadio = new QRadioButton("HARD");

    setupRadio(m_easyRadio, "#2ecc71", 0);
    setupRadio(m_mediumRadio, "#f39c12", 1);
    setupRadio(m_hardRadio, "#e74c3c", 2);
    m_mediumRadio->setChecked(true);

    mainLayout->addLayout(difficultyLayout);

    connect(m_difficultyGroup, QOverload<int>::of(&QButtonGroup::idClicked),
            this, &Home::onDifficultyChanged);

    // Configure High Score Title Label
    m_highScoresTitle = new QLabel("🏆 HIGH SCORE");
    m_highScoresTitle->setAlignment(Qt::AlignCenter);
    m_highScoresTitle->setStyleSheet(
        "color: #f1c40f; font-size: 24px; font-weight: bold; margin-top: 30px; background: transparent;"
        );
    mainLayout->addWidget(m_highScoresTitle);

    // Configure High Score Value Display Label
    m_highScoreValue = new QLabel("0");
    m_highScoreValue->setAlignment(Qt::AlignCenter);
    m_highScoreValue->setStyleSheet(
        "color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ffd700, stop:1 #ffa500);"
        "font-size: 72px; font-weight: 900; background: transparent;"
        );
    mainLayout->addWidget(m_highScoreValue);

    mainLayout->addStretch();

    // Configure Start Game Button
    m_startBtn = new QPushButton("START GAME");
    m_startBtn->setCursor(Qt::PointingHandCursor);
    m_startBtn->setFixedHeight(60);
    m_startBtn->setMinimumWidth(280);
    m_startBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2ecc71, stop:1 #27ae60);"
        "   color: white;"
        "   border-radius: 30px;"
        "   font-size: 22px;"
        "   font-weight: bold;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #27ae60, stop:1 #2ecc71);"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1e8449;"
        "}"
        );

    // Configure Exit Button
    m_exitBtn = new QPushButton("EXIT");
    m_exitBtn->setCursor(Qt::PointingHandCursor);
    m_exitBtn->setFixedHeight(50);
    m_exitBtn->setMinimumWidth(200);
    m_exitBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ff6b6b, stop:1 #ff4757);"
        "   color: white;"
        "   border-radius: 25px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ff4757, stop:1 #ff6b6b);"
        "}"
        "QPushButton:pressed {"
        "   background-color: #ff2222;"
        "}"
        );

    mainLayout->addWidget(m_startBtn, 0, Qt::AlignCenter);
    mainLayout->addWidget(m_exitBtn, 0, Qt::AlignCenter);

    // Configure Developer Credit Label
    m_creditLabel = new QLabel("Developed By Prasiddha Timalsina");
    m_creditLabel->setAlignment(Qt::AlignCenter);
    m_creditLabel->setStyleSheet(
        "color: rgba(255, 255, 255, 150); font-size: 14px; font-style: italic; margin-top: 20px;"
        );
    mainLayout->addWidget(m_creditLabel);

    // Connect Button Signals To Corresponding Slots
    connect(m_startBtn, &QPushButton::clicked, this, &Home::onStartClicked);
    connect(m_exitBtn, &QPushButton::clicked, this, &Home::onExitClicked);
}

// Add New Score Entry And Maintain Top Five Scores
void Home::addHighScore(int score)
{
    ScoreEntry entry;
    entry.playerName = "Player";
    entry.score = score;
    entry.date = QDateTime::currentDateTime().toString("dd/MM/yy");

    m_highScores.append(entry);

    std::sort(m_highScores.begin(), m_highScores.end(),
              [](const ScoreEntry &a, const ScoreEntry &b) {
                  return a.score > b.score;
              });

    while (m_highScores.size() > 5) {
        m_highScores.removeLast();
    }

    saveHighScores();
    updateHighScoresDisplay();
}

// Update High Score Label With Highest Score Value
void Home::updateHighScoresDisplay()
{
    if (m_highScoreValue) {
        if (!m_highScores.isEmpty()) {
            m_highScoreValue->setText(QString::number(m_highScores.first().score));
        } else {
            m_highScoreValue->setText("0");
        }
    }
}

// Load High Scores From Persistent Text File
void Home::loadHighScores()
{
    m_highScores.clear();
    QFile file("highscores.txt");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");

            if (parts.size() == 3) {
                ScoreEntry entry;
                entry.playerName = parts[0];
                entry.score = parts[1].toInt();
                entry.date = parts[2];
                m_highScores.append(entry);
            }
        }
        file.close();
    }

    std::sort(m_highScores.begin(), m_highScores.end(),
              [](const ScoreEntry &a, const ScoreEntry &b) {
                  return a.score > b.score;
              });
}

// Save High Scores To Persistent Text File
void Home::saveHighScores()
{
    QFile file("highscores.txt");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        for (const auto &entry : m_highScores) {
            out << entry.playerName << "," << entry.score << "," << entry.date << "\n";
        }

        file.close();
    }
}

// Create Gradient Background For Home Screen
void Home::createBackground()
{
    m_backgroundGradient = QLinearGradient(0, 0, 600, 700);
    m_backgroundGradient.setColorAt(0.0, QColor(20, 30, 40));
    m_backgroundGradient.setColorAt(0.3, QColor(30, 40, 60));
    m_backgroundGradient.setColorAt(0.6, QColor(40, 30, 60));
    m_backgroundGradient.setColorAt(1.0, QColor(20, 20, 40));
}

// Apply Neon Glow Effect To Widget Using Drop Shadow
void Home::addNeonEffect(QWidget *widget)
{
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(30);
    effect->setColor(QColor(0, 255, 255, 100));
    effect->setOffset(0);
    widget->setGraphicsEffect(effect);
}

// Handle Difficulty Selection Change Event
void Home::onDifficultyChanged(int id)
{
    m_currentDifficulty = id;

    QString difficultyText = (id == 0) ? "EASY"
                             : (id == 1) ? "MEDIUM"
                                         : "HARD";

    m_subtitleLabel->setText(QString("DIFFICULTY: %1").arg(difficultyText));

    QTimer::singleShot(2000, [this]() {
        m_subtitleLabel->setText("CLASSIC • MODERN • FUN");
    });
}

// Emit Signal To Start Snake Game
void Home::onStartClicked()
{
    emit startSnakeGame(m_currentDifficulty);
}

// Exit Application
void Home::onExitClicked()
{
    QApplication::quit();
}

// Paint Custom Background And Decorative Particles
void Home::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), m_backgroundGradient);

    painter.setPen(Qt::NoPen);

    for (int i = 0; i < 8; ++i) {
        int x = (i * 100 + QDateTime::currentMSecsSinceEpoch() / 20) % width();
        int y = (i * 70) % height();

        painter.setBrush(QColor(52, 152, 219, 20));
        painter.drawEllipse(x, y, 40, 40);
    }
}

// Update Background Gradient On Window Resize
void Home::resizeEvent(QResizeEvent *event)
{
    m_backgroundGradient.setFinalStop(width(), height());
    QWidget::resizeEvent(event);
}

// Handle Keyboard Shortcuts For Start And Exit Actions
void Home::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        onStartClicked();
    } else if (event->key() == Qt::Key_Escape) {
        onExitClicked();
    }

    QWidget::keyPressEvent(event);
}
