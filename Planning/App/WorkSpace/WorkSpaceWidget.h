#ifndef WORKSPACEWIDGET_H
#define WORKSPACEWIDGET_H

#include <QWidget>
#include <array>
#include "ItemType.h"
#include <QModelIndex>

namespace Ui {
class WorkSpaceWidget;
}

class ExplorerModel;
class ExplorerProxyModel;
class ExplorerItem;
class MonitorItem;

class WorkSpaceWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit WorkSpaceWidget(QWidget *parent = nullptr);
        ~WorkSpaceWidget();
        void saveImportedFile(const QString &fileName);
    private:
        Ui::WorkSpaceWidget *ui;
        static const int TYPES_COUNT = 2;
        static const QList<ItemType::Type> TYPES_ARRAY;
        ExplorerModel *m_model;
        ExplorerProxyModel *m_proxyModel;
        std::array<QList<int>, TYPES_COUNT> m_deletedFromBase;
        QModelIndex m_currentMonitoringIndex;
        QByteArray m_monitoringData;

        void initUi();
        void initModel();
        void initConnections();
        void deleteHelper(ExplorerItem *item);

    signals:
        void workSpaceCurrentItemChanged(bool isMonitoring, bool isChangedMonitoring);
        void monitoringDataChanged(MonitorItem* monitoritem);

    private slots:
        void onTbNewClicked();
        void onTbDeleteClicked();
        void onTbLoadClicked();
        void onTbSaveClicked();
        void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
        void onDetectedMultipleNameDefinition(const QString& message);
        void currentItemChanged();
        void getMonitoringData(const QModelIndex& index);
};

#endif // WORKSPACEWIDGET_H
