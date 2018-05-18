#ifndef WORKSPACEITEM_H
#define WORKSPACEITEM_H

#include <BaseItem.h>

class WorkSpaceItem : public BaseItem
{
    public:
        static const int ISNEW = 1;
        static const int FROMBASE = 2;
        static const int DELETEDFROMBASE = 3;
        static const int NAME_COLUMN = 0;
        static const int ID_COLUMN = 1;
        static const int PARENT_ID_COLUMN = 2;
        WorkSpaceItem(const QVector<QVariant>& data, BaseItem* parent = nullptr, const int state = WorkSpaceItem::ISNEW);
        WorkSpaceItem(const WorkSpaceItem& src);
        virtual ~WorkSpaceItem();
        virtual WorkSpaceItem* clone() const override;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
        void setState(const int state);
        int getState()const;
        void setEdited(const bool edited);
        bool getEdited() const;
        void setId(const int id);
        int getId() const;
        void setParentId(const int id);
        int getParentId() const;
        void setDeleted(const int deleted);
        bool isDeleted()const;
    protected:
        int m_state;               //read from base, or new, or deleted from base
        bool m_dirty;
        bool m_deleted;
};

#endif // WORKSPACEITEM_H
