#include "NE2048.h"
#include <ctime>

NE2048::NE2048(QObject *parent)
    : QObject(parent)
    , m_score(0)
    , m_bestScore(0) // native log needed
    , m_step(0)
    , m_totalStep(0)
{
    connect(this, SIGNAL(backed()), this, SLOT(goBack()));
    srand(time(0));
}

NE2048::~NE2048()
{
}

void NE2048::initialize()
{
    int num = 0;
    int index = 0;
    int indexMax = ROWS * COLUMNS;
    while (index < indexMax) {
        m_number.push_back(num);
        if (0 == num) {
            num = 2;
        }
        else {
            num *= 2;
        }
        index++;
    }
}

void NE2048::start()
{
    initNum();
}

void NE2048::move(Move_Direction direction)
{
    if (gameIsOver()) {
        emit gameOver();
        return;
    }
    added(direction);
    if (m_addedFlag && m_bestScore < m_score) {
        m_bestScore = m_score;
    }
    moved(direction);
    freshed(m_addedFlag || m_movedFlag);
}

QColor NE2048::bkgColor(const int &index)
{
    if (m_number.size() <= index || 0 > index) {
        return QColor(255, 255, 255);
    }
    QColor color;
    switch (m_number[index]) {
    case 0: color = QColor(255, 255, 255); break; // white
    case 2: color = QColor(245, 222, 179); break; // wheat
    case 4: color = QColor(238, 130, 238); break; // violet
    case 8: color = QColor(0, 255, 127); break; // springgreen
    case 16: color = QColor(255, 192, 203); break; // pink
    case 32: color = QColor(255, 165, 0); break; // orange
    case 64: color = QColor(173, 255, 47); break; // greenyellow
    case 128: color = QColor(255, 99, 71); break; // tomato
    case 256: color = QColor(154, 205, 50); break; // yellowgreen
    case 512: color = QColor(255, 215, 0); break; // gold
    case 1024: color = QColor(0, 255, 255); break; // cyan
    case 2048: color = QColor(0, 255, 0); break; // green
    case 4096: color = QColor(255, 255, 0); break; // yellow
    case 8192: color = QColor(255, 0, 0); break; // red
    default: color = QColor(0, 0, 0); break; // black
    }
    return color;
}

QColor NE2048::numColor(const int &index)
{
    if (m_number.size() <= index || 0 > index || 8192 < m_number[index]) {
        return QColor(255, 255, 255);
    }
    else {
        return QColor(0, 0, 0);
    }
}

int NE2048::score() const
{
    return m_score;
}

int NE2048::bestScore() const
{
    return m_bestScore;
}

int NE2048::step() const
{
    return m_step;
}

int NE2048::totalStep() const
{
    return m_totalStep;
}


int NE2048::show(const int &index)
{
    if (m_number.size() <= index || 0 > index) {
        return 0;
    }
    return m_number[index];
}

void NE2048::goBack()
{
    if (0 < m_step && m_state.size() >= m_step) {
        m_number = m_state[m_step - 1];
        m_state.pop_back();
        m_step -= 1;
    }
}

void NE2048::initNum()
{
    if (!m_number.empty()) {
        m_number.clear();
    }
    int count = ROWS * COLUMNS;
    m_number = NEPanel(count, 0);
    int firstNum = rand() % count;
    int secondNum = rand() % count;
    while (firstNum == secondNum) {
        secondNum = rand() % count;
    }
    m_number[firstNum] = 2;
    m_number[secondNum] = 2;
    m_score = 0;
    m_step = 0;
    m_totalStep = 0;
    if (!m_state.empty()) {
        m_state.clear();
    }
    m_state.push_back(m_number);
}

void NE2048::added(Move_Direction direction)
{
    m_addedFlag = false;

    if (Move_Down == direction) {
        for (int c = 0; c < COLUMNS; c++) {
            m_preIndex = c;
            m_nextIndex = m_preIndex + 4;
            while (m_nextIndex <= c + 12) {
                if (0 == m_number[m_preIndex]) {
                    m_preIndex = m_nextIndex;
                    m_nextIndex = m_preIndex + 4;
                    continue;
                }
                if (0 == m_number[m_nextIndex]) {
                    m_nextIndex += 4;
                    continue;
                }
                if (m_number[m_preIndex] != m_number[m_nextIndex]) {
                    m_preIndex = m_nextIndex;
                    m_nextIndex = m_preIndex + 4;
                }
                else {
                    m_number[m_preIndex] = 0;
                    m_number[m_nextIndex] += m_number[m_nextIndex];
                    m_score += m_number[m_nextIndex];
                    m_preIndex = m_nextIndex + 4;
                    m_nextIndex = m_preIndex + 4;
                    m_addedFlag = true;
                }
            }
        }
    }

    if (Move_Up == direction) {
        for (int c = 0; c < COLUMNS; c++) {
            m_preIndex = c + 12;
            m_nextIndex = m_preIndex - 4;
            while (m_nextIndex >= c) {
                if (0 == m_number[m_preIndex]) {
                    m_preIndex = m_nextIndex;
                    m_nextIndex = m_preIndex - 4;
                    continue;
                }
                if (0 == m_number[m_nextIndex]) {
                    m_nextIndex -= 4;
                    continue;
                }
                if (m_number[m_preIndex] != m_number[m_nextIndex]) {
                    m_preIndex = m_nextIndex;
                    m_nextIndex = m_preIndex - 4;
                }
                else {
                    m_number[m_preIndex] = 0;
                    m_number[m_nextIndex] += m_number[m_nextIndex];
                    m_score += m_number[m_nextIndex];
                    m_preIndex = m_nextIndex - 4;
                    m_nextIndex = m_preIndex - 4;
                    m_addedFlag = true;
                }
            }
        }
    }

    if (Move_Right == direction) {
        for (int r = 0; r < ROWS; r++) {
            m_preIndex = r * 4;
            m_nextIndex = m_preIndex + 1;
            while (m_nextIndex <= r * 4 + 3) {
                if (0 == m_number[m_preIndex]) {
                    m_preIndex = m_nextIndex;
                    m_nextIndex = m_preIndex + 1;
                    continue;
                }
                if (0 == m_number[m_nextIndex]) {
                    m_nextIndex += 1;
                    continue;
                }
                if (m_number[m_preIndex] != m_number[m_nextIndex]) {
                    m_preIndex = m_nextIndex;
                    m_nextIndex = m_preIndex + 1;
                }
                else {
                    m_number[m_preIndex] = 0;
                    m_number[m_nextIndex] += m_number[m_nextIndex];
                    m_score += m_number[m_nextIndex];
                    m_preIndex = m_nextIndex + 1;
                    m_nextIndex = m_preIndex + 1;
                    m_addedFlag = true;
                }
            }
        }
    }

    if (Move_Left == direction) {
        for (int r = 0; r < ROWS; r++) {
            m_preIndex = r * 4 + 3;
            m_nextIndex = m_preIndex - 1;
            while (m_nextIndex >= r * 4) {
                if (0 == m_number[m_preIndex]) {
                    m_preIndex = m_nextIndex;
                    m_nextIndex = m_preIndex - 1;
                    continue;
                }
                if (0 == m_number[m_nextIndex]) {
                    m_nextIndex -= 1;
                    continue;
                }
                if (m_number[m_preIndex] != m_number[m_nextIndex]) {
                    m_preIndex = m_nextIndex;
                    m_nextIndex = m_preIndex - 1;
                }
                else {
                    m_number[m_preIndex] = 0;
                    m_number[m_nextIndex] += m_number[m_nextIndex];
                    m_score += m_number[m_nextIndex];
                    m_preIndex = m_nextIndex - 1;
                    m_nextIndex = m_preIndex - 1;
                    m_addedFlag = true;
                }
            }
        }
    }
}

void NE2048::moved(Move_Direction direction)
{
    m_movedFlag = false;

    if (Move_Down == direction) {
        for (int c = 0; c < COLUMNS; c++) {
            m_preIndex = c + 12;
            m_nextIndex = m_preIndex - 4;
            while (m_nextIndex >= c) {
                if (0 != m_number[m_preIndex]) {
                    m_preIndex = m_nextIndex;
                    m_nextIndex = m_preIndex - 4;
                    continue;
                }
                if (0 == m_number[m_nextIndex]) {
                    m_nextIndex -= 4;
                    continue;
                }
                else {
                    m_number[m_preIndex] = m_number[m_nextIndex];
                    m_number[m_nextIndex] = 0;
                    m_preIndex = m_preIndex - 4;
                    m_nextIndex = m_nextIndex - 4;
                    m_movedFlag = true;
                }
            }
        }
    }

    if (Move_Up == direction) {
        for (int c = 0; c < COLUMNS; c++) {
            m_preIndex = c;
            m_nextIndex = m_preIndex + 4;
            while (m_nextIndex <= c + 12) {
                if(0 != m_number[m_preIndex]) {
                    m_preIndex = m_nextIndex;
                    m_nextIndex = m_preIndex + 4;
                    continue;
                }
                if(0 == m_number[m_nextIndex]) {
                    m_nextIndex += 4;
                    continue;
                }
                else {
                    m_number[m_preIndex] = m_number[m_nextIndex];
                    m_number[m_nextIndex] = 0;
                    m_preIndex = m_preIndex + 4;
                    m_nextIndex = m_nextIndex + 4;
                    m_movedFlag = true;
                }
            }
        }
    }

    if (Move_Right == direction) {
        for (int r = 0; r < ROWS; r++) {
            m_preIndex = r * 4 + 3;
            m_nextIndex = m_preIndex - 1;
            while (m_nextIndex >= r * 4) {
                if (0 != m_number[m_preIndex]) {
                    m_preIndex = m_nextIndex;
                    m_nextIndex = m_preIndex - 1;
                    continue;
                }
                if (0 == m_number[m_nextIndex]) {
                    m_nextIndex -= 1;
                    continue;
                }
                else {
                    m_number[m_preIndex] = m_number[m_nextIndex];
                    m_number[m_nextIndex] = 0;
                    m_preIndex = m_preIndex - 1;
                    m_nextIndex = m_nextIndex - 1;
                    m_movedFlag = true;
                }
            }
        }
    }

    if (Move_Left == direction) {
        for (int r = 0; r < ROWS; r++) {
            m_preIndex = r * 4;
            m_nextIndex = m_preIndex + 1;
            while (m_nextIndex <= r * 4 + 3) {
                if (0 != m_number[m_preIndex]) {
                    m_preIndex = m_nextIndex;
                    m_nextIndex = m_preIndex + 1;
                    continue;
                }
                if (0 == m_number[m_nextIndex]) {
                    m_nextIndex += 1;
                    continue;
                }
                else {
                    m_number[m_preIndex] = m_number[m_nextIndex];
                    m_number[m_nextIndex] = 0;
                    m_preIndex = m_preIndex + 1;
                    m_nextIndex = m_nextIndex + 1;
                    m_movedFlag = true;
                }
            }
        }
    }
}

void NE2048::freshed(bool fresh)
{
    if (fresh) {
        m_step += 1;
        m_totalStep = m_step;
        if (!m_index.empty()) {
            m_index.clear();
        }
        for (size_t s = 0; s < m_number.size(); s++) {
            if (!m_number[s]) {
                m_index.push_back(s);
            }
        }
        int randIndex = rand() % m_index.size();
        m_number[m_index[randIndex]] = 2;
        m_state.push_back(m_number);
    }
}

bool NE2048::gameIsOver()
{
    // null check
    for (size_t i = 0; i < m_number.size(); i++) {
        if (0 == m_number[i]) {
            return false;
        }
    }
    int preIndex = 0;
    int nextIndex = 0;
    // row check
    for (int row = 0; row < 4 ; row++) {
        preIndex = row * 4;
        for (int col = 1; col < 4; col++) {
            nextIndex = preIndex + 1;
            if (m_number[preIndex] == m_number[nextIndex]) {
                return false;
            }
            preIndex = nextIndex;
        }
    }
    // column check
    for (int col = 0; col < 4; col++) {
        preIndex = col;
        for (int row = 1; row < 3; row++) {
            nextIndex = preIndex + 4;
            if (m_number[preIndex] == m_number[nextIndex]) {
                return false;
            }
            preIndex = nextIndex;
        }
    }
    // game over
    return true;
}
