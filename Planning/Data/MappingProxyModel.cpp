#include "MappingProxyModel.h"
#include "BaseModel.h"
#include "MappingItem.h"

MappingProxyModel::MappingProxyModel(QObject *parent) : BaseProxyModel(parent)
{
    int columnCount = static_cast<int>(MappingItem::DATA_TYPES::DATA_NOT_MAPPED);
    for (int i = 0 ; i < columnCount; ++i){
       m_headerData << MappingItem::dataTypesToString(MappingItem::DATA_TYPES(i));
    }
}

MappingProxyModel::~MappingProxyModel()
{

}

bool MappingProxyModel::filterAcceptsColumn(int source_col, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    return (source_col < sourceModel()->columnCount());
}
