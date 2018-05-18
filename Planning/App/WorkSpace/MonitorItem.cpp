#include "MonitorItem.h"

MonitorItem::MonitorItem(const QVector<QVariant>& data, BaseItem *parent, const int state)
    : WorkSpaceItem(data, parent, state), m_hasData(false), m_hasMapping(false),
      m_isMappingChanged(false), m_isDataChanged(false), m_isFirstSelection(true)
{
    m_itemType = ItemType::Type::MonitorItem;
    if (m_state == WorkSpaceItem::ISNEW){
        m_itemData[WorkSpaceItem::NAME_COLUMN] = ItemType::ItemTypetoString(ItemType::Type::MonitorItem);
    }
    resetDataMapping();
}

MonitorItem::MonitorItem(const MonitorItem& src) : WorkSpaceItem(src)
{
    m_currentData = src.m_currentData;
}

void MonitorItem::resetDataMapping()
{
    m_dataMapping[MonitorItem::COLUMN_DATA_TYPE::NAME_COLUMN] = -1;
    m_dataMapping[MonitorItem::COLUMN_DATA_TYPE::DURATION_COLUMN] = -1;
    m_dataMapping[MonitorItem::COLUMN_DATA_TYPE::DEPENDENCY_COLUMN] = -1;
    m_hasMapping = false;
}

MonitorItem* MonitorItem::clone() const
{
    return new MonitorItem(*this);
}

MonitorItem::~MonitorItem()
{

}

QIcon MonitorItem::icon() const
{
    return QIcon(":/Resources/Monitoring.png");
}

Qt::ItemFlags MonitorItem::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void MonitorItem::setCurrentData(const QByteArray &data)
{
    resetDataMapping();
    m_currentData = data;
    m_hasData = true;
}

QByteArray MonitorItem::getCurrentData() const
{
    return m_currentData;
}

bool MonitorItem::hasData() const
{
    return m_hasData;
}

bool MonitorItem::hasMapping() const
{
    return m_hasMapping;
}

QMap<MonitorItem::COLUMN_DATA_TYPE, int> MonitorItem::getDataMapping() const
{
    return m_dataMapping;
}

void MonitorItem::changeMapping(const QList<QPair<MonitorItem::COLUMN_DATA_TYPE, int> > &mappingList, bool fromLoad)
{
    foreach(const auto value, mappingList){
        auto it = m_dataMapping.find(value.first);
        if (it != m_dataMapping.end() && it.value() != value.second){
            it.value() = value.second;
            if (!fromLoad)
                setMappingChanged(true);
        }
    }
    if (fromLoad){
        m_hasMapping = true;
    }
}

bool MonitorItem::isMappingChanged() const
{
    return m_isMappingChanged;
}

void MonitorItem::setMappingChanged(const bool edited)
{
    m_isMappingChanged = edited;
    m_hasMapping = true;
}

bool MonitorItem::isDataChanged() const
{
    return m_isDataChanged;
}

void MonitorItem::setDataChanged(const bool changed)
{
    m_isDataChanged = changed;
}

void MonitorItem::setFirstSelection(const bool value)
{
    m_isFirstSelection = value;
}

bool MonitorItem::isFirstSelection() const
{
    return m_isFirstSelection;
}

