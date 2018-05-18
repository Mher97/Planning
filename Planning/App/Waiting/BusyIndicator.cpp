#include "BusyIndicator.h"
#include <QPainter>
#include <QPen>

BusyIndicator::BusyIndicator(QWidget *parent) : QLabel(parent), m_activeColor(Qt::green),
    m_deactiveColor("#9b9b9b"), m_state(BusyIndicator::STATE::DEACTIVE)

{
    setWindowFlags(Qt::CustomizeWindowHint);
}

void BusyIndicator::setActiveColor(const QColor &newColor)
{
    m_activeColor = newColor;
}

void BusyIndicator::setDeactiveColor(const QColor &newColor)
{
    m_deactiveColor = newColor;
}

void BusyIndicator::activate()
{
    m_state = BusyIndicator::STATE::ACTIVE;
    repaint();
}

void BusyIndicator::deactivate()
{
    m_state = BusyIndicator::STATE::DEACTIVE;
    repaint();
}

void BusyIndicator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    QColor currentColor = m_activeColor;
    if (m_state == BusyIndicator::STATE::DEACTIVE){
        currentColor = m_deactiveColor;
    }
    p.setPen(QPen(QColor("#C4C4C3"), 2, Qt::NoPen, Qt::RoundCap));
    p.setBrush(QBrush(currentColor, Qt::SolidPattern));
    p.drawEllipse(0, 0, qMin(width(), height()), qMin(width(), height()));
}
