#ifndef NE2048_H
#define NE2048_H

#include <QObject>
#include <QColor>

#define ROWS 4
#define COLUMNS 4

class NE2048 : public QObject
{
    Q_OBJECT
    Q_ENUMS(Move_Direction)
    Q_PROPERTY(int score READ score)
    Q_PROPERTY(int bestScore READ bestScore)
    Q_PROPERTY(int step READ step)
    Q_PROPERTY(int totalStep READ totalStep)

public:
    NE2048(QObject *parent = 0);
    ~NE2048();

    enum Move_Direction {
        Move_Up = 0,
        Move_Down,
        Move_Left,
        Move_Right,
        Move_Invalid
    };

    Q_INVOKABLE void start();
    Q_INVOKABLE void move(Move_Direction direction);
    Q_INVOKABLE QColor color(const int &index);
    Q_INVOKABLE QColor numColor(const int &index);

    int score() const;
    int bestScore() const;
    int step() const;
    int totalStep() const;

signals:
    void backed();

public slots:
    int show(const int &index);
    void goBack();

private:
    void initNum();
    void added(Move_Direction direction);
    void moved(Move_Direction direction);
    void freshed(bool fresh);

    int m_score;
    int m_bestScore;
    int m_step;
    int m_totalStep;

    typedef std::vector<int> NEPanel;
    typedef std::vector<NEPanel> NEState;
    NEPanel m_number;
    NEPanel m_index;
    NEState m_state;
    int m_preIndex;
    int m_nextIndex;
    bool m_addedFlag;
    bool m_movedFlag;
};

#endif // NE2048_H
