#ifndef _CHECKBOX_ITEM_DELEGATE_H
#define _CHECKBOX_ITEM_DELEGATE_H

#include <QStyledItemDelegate>
#include <QStyleOptionButton>

class CheckBoxItemDelegate :public QStyledItemDelegate
{
	Q_OBJECT
    public:
        explicit CheckBoxItemDelegate(QObject *parent = nullptr);
        virtual ~CheckBoxItemDelegate();
        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
        virtual bool editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option,
                                 const QModelIndex & index) override;
};

#endif
