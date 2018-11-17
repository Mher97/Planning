#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include "ExplorerItem.h"

class ProjectItem : public ExplorerItem
{
    Q_OBJECT
    public:
        ProjectItem(BaseItem *parent = nullptr, const ItemState = ItemState::NEW);
        ProjectItem(const ProjectItem& src);
        virtual ~ProjectItem();
        virtual ProjectItem* clone() const override;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
        QIcon icon() const;
};

#endif // PROJECTITEM_H
