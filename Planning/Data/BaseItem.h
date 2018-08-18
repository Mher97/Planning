#ifndef BASEITEM_H
#define BASEITEM_H

#include <QObject>
#include <QList>
#include <QVariant>
#include <QVector>
#include <QModelIndex>
#include <QIcon>
#include "ItemType.h"

class BaseItem : public QObject
{
    Q_OBJECT
    public:
        BaseItem(const QVector<QVariant>& data, BaseItem *parent = nullptr);
        BaseItem(BaseItem *parent = nullptr);
        BaseItem(const BaseItem& src);

        virtual ~BaseItem();
        virtual BaseItem* clone() const;

        BaseItem *child(int number) const;
        int childCount() const;
        int columnCount() const;
        virtual QVariant data(int column, int role) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
        virtual QIcon icon() const;
        bool insertChildren(int position, int count, int columns);
        bool insertChild(int position, BaseItem * child);
        QList<BaseItem*> childList(const ItemType::Type childType = ItemType::Type::Default, bool byDepth = false) const;
        bool insertColumns(int position, int columns);
        BaseItem *parent() const;
        bool removeChildren(int position, int count);
        bool removeColumns(int position, int columns);
        int childNumber() const;
        virtual bool setData(int column, const QVariant &value, int role);
        void setParentItem(BaseItem * item);
        bool isChecked() const;

        ItemType::Type itemType() const;
        QString tagName() const;

        virtual void setName(const QString &name);
        virtual QString getName() const;

        bool nameIsAllowed(const QString& name) const;
        bool nameIsAllowedForChild(const QString& name, const ItemType::Type childType) const;
        QString uniqueNameForChild(const QString& name, const ItemType::Type childType) const;
        QString baseName(const QString& name) const;
        void setItemData(const QVector<QVariant>& itemDataVector);

    protected:
        QList<BaseItem*> m_childItems;
        QVector<QVariant> m_itemData;
        BaseItem *m_parentItem;
        ItemType::Type m_itemType;
};

#endif // BASEITEM_H
