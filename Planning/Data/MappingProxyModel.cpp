#include "MappingProxyModel.h"
#include "BaseModel.h"
#include "MappingItem.h"

MappingProxyModel::MappingProxyModel(QObject *parent) : BaseProxyModel(parent)
{
    for(int i = 0; i < MappingProxyModel::COLUMN_COUNT; ++i){
        m_headerData<<MappingItem::dataTypesToString(MappingItem::DATA_TYPES(i));
    }
}

MappingProxyModel::~MappingProxyModel()
{

}

bool MappingProxyModel::filterAcceptsColumn(int source_col, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    return (source_col < COLUMN_COUNT);
}
