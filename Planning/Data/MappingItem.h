#ifndef MAPPINGITEM_H
#define MAPPINGITEM_H

#include "BaseItem.h"

class MappingItem : public BaseItem
{
    Q_OBJECT
    public:
        enum class DATA_TYPES : int
        {
            COLUMN_RAW_NAME = 0,
            DATA_TASK_CODE  = 1,
            DATA_DURATION = 2,
            DATA_DEPENDENCE = 3,
            DATA_NOT_MAPPED = 4
        };
        static QString dataTypesToString(MappingItem::DATA_TYPES type);
        MappingItem(BaseItem *parent = nullptr);
        virtual ~MappingItem();
        void setRawColName(const QString& colName);
        QString getRawColName() const;
        void setColumnNumber(const int number);
        int getColumnNumber()const;
        virtual QVariant data(int column, int role) const;
        virtual bool setData(int column, const QVariant &value, int role);
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
        void clearSelections();
    private:
        QString m_rawColName;  //Column name in initial CSV file.
        int m_columnNumber;     //Column number in initial CSV file
};

#endif // MAPPINGITEM_H
