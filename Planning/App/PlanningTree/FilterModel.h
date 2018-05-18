#ifndef FILTERMODEL_H
#define FILTERMODEL_H

#include <QStandardItemModel>

class FilterModel : public QStandardItemModel
{
    Q_OBJECT
    public:
        template <typename ...Args>
        FilterModel(Args&& ...args): QStandardItemModel(std::forward<Args>(args)...){}
        virtual bool setData(const QModelIndex& index, const QVariant &value, int role = Qt::EditRole)override;
};

#endif // FILTERMODEL_H

