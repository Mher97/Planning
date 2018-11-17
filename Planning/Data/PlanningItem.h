#ifndef PLANNINGITEM_H
#define PLANNINGITEM_H

#include "BaseItem.h"

class PlanningItem : public BaseItem
{
    public:
        static const int NAME_COLUMN = 0;
        static const int START_COLUMN = 1;
        static const int FINISH_COLUMN = 2;
        PlanningItem(const QString& taskName,
                     const int startTime = 0,
                     const int finishTime = 0,
                     const int phaseNumber = 0,
                     QColor phaseColor = Qt::black,
                     BaseItem *parent = nullptr);
        virtual ~PlanningItem();
        virtual PlanningItem* clone() const override;
        virtual int columnCount() const override;
        virtual bool setData(int column, const QVariant &value, int role);
        virtual QVariant data(int column, int role) const override;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
        void setPhaseNumber(const int phaseNumber);
        int getPhaseNumber()const;
        void setPhaseColor(const QColor& phaseColor);
        QColor getPhaseColor()const;
        int getStartTime() const;
        void setStartTime(const int startTime);
        int getFinishTime() const;
        void setFinishTime(const int finishTime);
    private:
        int m_startTime;
        int m_finishTime;
        int m_phaseNumber;
        QColor m_phaseColor;
};

#endif // PLANNINGITEM_H
