#ifndef MONITORITEM_H
#define MONITORITEM_H

#include "ExplorerItem.h"

class MonitorItem : public ExplorerItem
{
    Q_OBJECT
    public:
        enum class COLUMN_DATA_TYPE : int{
            NAME_COLUMN = 0,
            DURATION_COLUMN = 1,
            DEPENDENCY_COLUMN = 2
        };

        static const int IMPORTED_DATA_COLUMN = 1;
        MonitorItem(BaseItem *parent = nullptr, const ItemState = ItemState::NEW);
        MonitorItem(const MonitorItem& src);
        void resetDataMapping();
        virtual ~MonitorItem();
        virtual MonitorItem* clone() const override;
        virtual QIcon icon() const override;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
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
