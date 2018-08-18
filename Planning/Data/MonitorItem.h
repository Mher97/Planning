#ifndef MONITORITEM_H
#define MONITORITEM_H

#include "WorkSpaceItem.h"

class MonitorItem : public WorkSpaceItem
{
    Q_OBJECT
    public:
        enum class COLUMN_DATA_TYPE : int{
            NAME_COLUMN = 0,
            DURATION_COLUMN = 1,
            DEPENDENCY_COLUMN = 2
        };

        static const int IMPORTED_DATA_COLUMN = 1;
        MonitorItem(const QVector<QVariant>& data, BaseItem *parent = nullptr, const int state = WorkSpaceItem::ISNEW);
        MonitorItem(const MonitorItem& src);
        void resetDataMapping();
        virtual ~MonitorItem();
        virtual MonitorItem* clone() const;
        QIcon icon() const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
        void setCurrentData(const QByteArray &data);
        QByteArray getCurrentData()const;
        bool hasData()const;
        bool hasMapping()const;
        QMap<COLUMN_DATA_TYPE, int> getDataMapping()const;
        void changeMapping(const QList<QPair<COLUMN_DATA_TYPE,int>>& mappingList, bool fromLoad = true);
        bool isMappingChanged()const;
        void setMappingChanged(const bool edited);
        bool isDataChanged()const;
        void setDataChanged(const bool changed);
        void setFirstSelection(const bool value);
        bool isFirstSelection()const;
    private:
        QByteArray m_currentData;
        bool m_hasData;
        bool m_hasMapping;
        QMap<COLUMN_DATA_TYPE, int> m_dataMapping;
        bool m_isMappingChanged;
        bool m_isDataChanged;
        bool m_isFirstSelection;
};

#endif // MONITORITEM_H
