#ifndef PLANNINGPROXYMODEL_H
#define PLANNINGPROXYMODEL_H

#include "../BaseProxyModel.h"

class PlanningProxyModel : public BaseProxyModel
{
    public:
        static const int COLUMN_COUNT = 3;
        explicit PlanningProxyModel(QObject *parent = nullptr);
        virtual ~PlanningProxyModel();
        void setFilterVector(const QVector<bool>& filterVector);
    private:
        QVector<bool> m_filterVector;
    protected:
        virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
        virtual bool filterAcceptsColumn(int source_col, const QModelIndex &source_parent) const override;
};

#endif // PLANNINGPROXYMODEL_H
