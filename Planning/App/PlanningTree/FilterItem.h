#ifndef FILTERITEM_H
#define FILTERITEM_H

#include <QStandardItem>

class FilterItem : public QObject, public QStandardItem
{
    Q_OBJECT
    public:
        enum class DATA_CHANGE_TYPE : int{
            AUTO = 1,
            MANUAL =2
        };
        template <typename ...Args>
        FilterItem(Args&& ...args) : QObject(), QStandardItem(std::forward<Args>(args)...){}
        virtual void setData(const QVariant &value, int role, const DATA_CHANGE_TYPE type = DATA_CHANGE_TYPE::MANUAL);
    signals:
        void manualDataChanged(FilterItem* item);
};

#endif // FILTERITEM_H
