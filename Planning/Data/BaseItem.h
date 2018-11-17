#ifndef BASEITEM_H
#define BASEITEM_H

#include <QObject>
#include <QList>
#include <QVariant>
#include <QModelIndex>
#include <QIcon>
#include "ItemType.h"

class BaseItem : public QObject
{
    Q_OBJECT
    public:
        BaseItem(BaseItem *parent = nullptr);
        BaseItem(const BaseItem& src);

        virtual ~BaseItem();
        virtual BaseItem* clone() const;

        BaseItem *child(int number) const;
        int childCount() const;
        virtual int columnCount() const;
        virtual QVariant data(int column, int role) const;
        virtual bool setData(int column, const QVariant &value, int role);
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
        virtual QIcon icon() const;
        bool insertChildren(int position, int count);
        bool insertChild(int position, BaseItem * child);
        QList<BaseItem*> childList(const ItemType::Type childType = ItemType::Type::Default, bool byDepth = false) const;
        bool insertColumns(int position, int columns);
        bool removeColumns(int position, int columns);
        BaseItem *parent() const;
        bool removeChildren(int position, int count);

        int childNumber() const;
        void setParentItem(BaseItem * item);

        ItemType::Type itemType() const;
        QString tagName() const;

        virtual void setName(const QString &name);
        virtual QString getName() const;

    protected:
        QList<BaseItem*> m_childItems;
        BaseItem *m_parentItem;
        /*QVector<QVariant> m_itemData;*/
        int m_columnCount;
        QString m_name;
        ItemType::Type m_itemType;
};

#endif // BASEITEM_H
