#ifndef BASEPROXYMODEL_H
#define BASEPROXYMODEL_H

#include <QSortFilterProxyModel>

class BaseProxyModel : public QSortFilterProxyModel
{
    public:
        explicit BaseProxyModel(QObject *parent = nullptr);
        virtual ~BaseProxyModel();
        virtual bool setData(const QModelIndex& index, const QVariant& value, int role)override;
        virtual QVariant data(const QModelIndex &index, int role) const override;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    protected:
        QStringList m_headerData;
        virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
        virtual bool filterAcceptsColumn(int source_col, const QModelIndex &source_parent) const override;
};

#endif // BASEPROXYMODEL_H
