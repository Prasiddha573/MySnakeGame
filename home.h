#ifndef HOME_H
#define HOME_H

// Include Required Qt Widget And UI Components
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QDateTime>
#include <QVector>
#include <QPair>

// Structure To Store Player High Score Information
struct ScoreEntry {
    QString playerName;   // Name Of The Player
    int score;            // Player Score Value
    QString date;         // Date When Score Was Achieved
};

// Home Screen Widget Class For Snake Game
class Home : public QWidget
{
    Q_OBJECT

    // Property Used For Title Scaling Animation
    Q_PROPERTY(float titleScale READ titleScale WRITE setTitleScale)

public:
    // Constructor To Initialize Home Screen
    explicit Home(QWidget *parent = nullptr);

    // Getter For Title Scale Animation Value
    float titleScale() const { return m_titleScale; }

    // Setter For Title Scale Animation Value
    void setTitleScale(float scale) { m_titleScale = scale; update(); }

    // Return Currently Selected Difficulty Level
    int getDifficulty() const { return m_currentDifficulty; }

    // Add A New Score To High Score List
    void addHighScore(int score);

signals:
    // Signal To Start Snake Game With Selected Difficulty
    void startSnakeGame(int difficulty);

    // Signal To Exit The Game Application
    void exitGame();

protected:
    // Handle Custom Painting Of Background And UI Effects
    void paintEvent(QPaintEvent *event) override;

    // Handle Responsive Layout Adjustments On Resize
    void resizeEvent(QResizeEvent *event) override;

    // Handle Keyboard Input Events
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    // Handle Start Button Click Event
    void onStartClicked();

    // Handle Exit Button Click Event
    void onExitClicked();

    // Handle Difficulty Selection Change
    void onDifficultyChanged(int id);

    // Update High Score Display Label
    void updateHighScoresDisplay();

private:
    // Setup All UI Components And Layouts
    void setupUI();

    // Create Animated Or Gradient Background
    void createBackground();

    // Apply Neon Glow Effect To Given Widget
    void addNeonEffect(QWidget *widget);

    // Load High Scores From Persistent Storage
    void loadHighScores();

    // Save High Scores To Persistent Storage
    void saveHighScores();

    // Convert Difficulty Level To Text Representation
    QString getDifficultyText(int diff);

    // UI Button Components
    QPushButton *m_startBtn;
    QPushButton *m_exitBtn;

    // Title And Informational Labels
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    QLabel *m_creditLabel;

    // Difficulty Selection Components
    QButtonGroup *m_difficultyGroup;
    QRadioButton *m_easyRadio;
    QRadioButton *m_mediumRadio;
    QRadioButton *m_hardRadio;
    int m_currentDifficulty;

    // High Score Display Components
    QLabel *m_highScoresTitle;
    QLabel *m_highScoreValue;
    QVector<ScoreEntry> m_highScores;

    // Animation Components For Title Scaling Effect
    QPropertyAnimation *m_titleAnimation;
    float m_titleScale;

    // Background Gradient For Modern UI Appearance
    QLinearGradient m_backgroundGradient;
};

#endif // HOME_H
