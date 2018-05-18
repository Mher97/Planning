#ifndef PLANNINGITEM_H
#define PLANNINGITEM_H

#include "../BaseItem.h"

class PlanningItem : public BaseItem
{
    public:
        static const int ID_COLUMN = 0;
        static const int START_COLUMN = 1;
        static const int FINISH_COLUMN = 2;
        PlanningItem(const int phase, const QVector<QVariant>& data, BaseItem *parent = nullptr);
        PlanningItem(const int phase = 0, BaseItem *parent = nullptr);
        virtual ~PlanningItem();
        virtual PlanningItem* clone() const override;
        QVariant data(int column, int role) const override;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
        int getPhaseNumber()const;
    private:
        int m_phaseNumber;
};

#endif // PLANNINGITEM_H
