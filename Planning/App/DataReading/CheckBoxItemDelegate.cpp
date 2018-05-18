#include "CheckBoxItemDelegate.h"
#include <QPainter>
#include <QMouseEvent>
#include <QCheckBox>
#include <QApplication>
#include "../BaseModel.h"

CheckBoxItemDelegate::CheckBoxItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
	
}


CheckBoxItemDelegate::~CheckBoxItemDelegate()
{

}

void CheckBoxItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if(index.isValid())
    {
        QVariant variantData = index.data(Qt::CheckStateRole);
        if (!variantData.isNull()) {
            QPixmap controlImage;
            int value = variantData.toInt();
            QStyleOptionButton checkbox_indicator;
            checkbox_indicator.state |= QStyle::State_Enabled;
            checkbox_indicator.state |= (value) ? QStyle::State_On : QStyle::State_Off;
            checkbox_indicator.rect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator,
                                                                             &checkbox_indicator, nullptr );
            const int x = option.rect.center().x() - checkbox_indicator.rect.width() / 2;
            const int y = option.rect.center().y() - checkbox_indicator.rect.height() / 2;
            checkbox_indicator.rect.moveTo( x, y );

            if (option.state & QStyle::State_Selected){
                painter->fillRect(option.rect, QBrush(QColor(21, 153, 255)));
            }else if (option.state & QStyle::State_MouseOver){
                painter->fillRect(option.rect, QBrush(QColor(229, 243, 255)));
            }else{
                auto backgroundColor = index.data(Qt::BackgroundRole);
                if (backgroundColor.isValid()) {
                    painter->fillRect(option.rect, QBrush(backgroundColor.value<QColor>()));
                }
            }
            switch (value){
                case Qt::Checked :{
                    controlImage = QPixmap(":/Resources/checked");
                    break;
                }
                case Qt::PartiallyChecked :{
                    break;
                }
                case Qt::Unchecked :{
                    controlImage = QPixmap(":/Resources/unchecked");
                    break;
                }
            }
            QApplication::style()->drawItemPixmap(painter,checkbox_indicator.rect,
                                                  Qt::AlignCenter,controlImage.scaled(checkbox_indicator.rect.size()));
            return;
        }
    }
    QStyledItemDelegate::paint(painter,option,index);
}


bool CheckBoxItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                       const QStyleOptionViewItem &option, const QModelIndex &index)
{

    QMouseEvent *mouse_event = dynamic_cast<QMouseEvent*>(event);
    if((mouse_event != nullptr) &&(event->type() == QEvent::MouseButtonPress) && (mouse_event->button() == Qt::LeftButton))
    {
        QVariant variantData = index.data(Qt::CheckStateRole);
        if (!variantData.isNull()) {
            QStyleOptionButton checkbox_indicator;
            checkbox_indicator.rect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator,
                                                                             &checkbox_indicator, nullptr);
            const int x = option.rect.center().x() - checkbox_indicator.rect.width() / 2;
            const int y = option.rect.center().y() - checkbox_indicator.rect.height() / 2;
            checkbox_indicator.rect.moveTo( x, y );
            if(checkbox_indicator.rect.contains(mouse_event->pos())){
                Qt::CheckState checkState = variantData.toBool() ? Qt::Unchecked : Qt::Checked;
                model->setData(index, checkState, Qt::CheckStateRole);
                return true;
            }
        }
    }
    return QStyledItemDelegate::editorEvent(event,model,option,index);
}
