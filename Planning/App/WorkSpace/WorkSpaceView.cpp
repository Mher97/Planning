#include "WorkSpaceView.h"
#include "Globals.h"
#include <QMouseEvent>
#include <QDebug>

WorkSpaceView::WorkSpaceView(QWidget *parent) : QTreeView(parent)
{

}

void WorkSpaceView::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<"mouseDoubleClickEvent";
    QModelIndex selectedIndex = indexAt(event->pos());
    try{
        qDebug()<<"click p1";
        edit(selectedIndex);
        qDebug()<<"click p2";
    }catch(std::exception& ex){
        Globals::showMessage(QMessageBox::Warning, "WorkSpace", ex.what());
    }
}
