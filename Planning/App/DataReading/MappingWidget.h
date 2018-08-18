#ifndef MAPPINGWIDGET_H
#define MAPPINGWIDGET_H

#include <QWidget>
#include <QTreeView>
#include "../Data/MappingItem.h"
#include <QHash>
#include <QPair>
#include "../Data/MonitorItem.h"

class MappingModel;
class MappingProxyModel;
class CheckBoxItemDelegate;
class DataHandler;

namespace Ui {
class MappingWidget;
}

class MappingWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit MappingWidget(QWidget *parent = nullptr);
        ~MappingWidget();
        void reset();
        void handleData(const QByteArray& data, const QMap<MonitorItem::COLUMN_DATA_TYPE, int> monitorMapping);
        QHash<QString, QPair<int, QStringList>> readData();
        QMap<MappingItem::DATA_TYPES, int> getMapping()const;
    private:
        Ui::MappingWidget *ui;
        MappingModel* m_mappingModel;
        MappingProxyModel* m_proxyModel;
        CheckBoxItemDelegate* m_checkBoxItemDelegate;
        DataHandler *m_dataHandler;
        QMap<MappingItem::DATA_TYPES, int> m_mapping;
        void initModel();
        void initUi();
        void initConnections();
        bool mappedColumnIsValid(const int columnNumber);
    signals:
        void onImportButtonClicked();
    private slots:
        void onExportButtonClicked();
};

#endif // MAPPINGWIDGET_H
