#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include "WorkSpaceItem.h"

class ProjectItem : public WorkSpaceItem
{
    Q_OBJECT
    public:
        ProjectItem(const QVector<QVariant>& data, BaseItem *parent = nullptr, const int state = WorkSpaceItem::ISNEW);
        ProjectItem(const ProjectItem& src);
        virtual ~ProjectItem();
        virtual ProjectItem* clone() const override;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
        QIcon icon() const;
};

#endif // PROJECTITEM_H
