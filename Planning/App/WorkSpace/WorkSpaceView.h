#ifndef WORKSPACEVIEW_H
#define WORKSPACEVIEW_H

#include <QTreeView>

class WorkSpaceView : public QTreeView
{
    public:
        WorkSpaceView(QWidget *parent = nullptr);
    protected:
        void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // WORKSPACEVIEW_H
