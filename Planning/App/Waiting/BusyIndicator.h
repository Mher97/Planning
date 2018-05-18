#ifndef BUSYINDICATOR_H
#define BUSYINDICATOR_H

#include <QLabel>

class BusyIndicator : public QLabel
{
    public:
        explicit BusyIndicator(QWidget* parent = nullptr);
        void setActiveColor(const QColor& newColor);
        void setDeactiveColor(const QColor& newColor);
        void activate();
        void deactivate();
    protected:
        void paintEvent(QPaintEvent *event) override;
    private:
        enum class STATE : int{
            ACTIVE = 0,
            DEACTIVE = 1,
        };
        QColor m_activeColor;
        QColor m_deactiveColor;
        BusyIndicator::STATE m_state;
};

#endif // BUSYINDICATOR_H
