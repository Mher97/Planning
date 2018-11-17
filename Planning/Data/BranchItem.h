#ifndef BRANCHITEM_H
#define BRANCHITEM_H

#include "ExplorerItem.h"

class BranchItem : public ExplorerItem
{
    Q_OBJECT
    public:
        BranchItem(BaseItem *parent = nullptr, const ItemState = ItemState::NEW);
        BranchItem(const BranchItem& src);
        virtual ~BranchItem();
        virtual BranchItem* clone() const override;
        virtual QIcon icon() const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // PROJECTITEM_H
