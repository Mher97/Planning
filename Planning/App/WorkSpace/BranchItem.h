#ifndef BRANCHITEM_H
#define BRANCHITEM_H

#include "WorkSpaceItem.h"

class BranchItem : public WorkSpaceItem
{
    Q_OBJECT
    public:
        BranchItem(QVector<QVariant> data, BaseItem *parent = nullptr, const int state = WorkSpaceItem::ISNEW);
        BranchItem(const BranchItem& src);
        virtual ~BranchItem();
        virtual BranchItem* clone() const override;
        QIcon icon() const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // PROJECTITEM_H
