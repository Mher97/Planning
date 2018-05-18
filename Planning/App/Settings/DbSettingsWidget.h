#ifndef DBSETTINGSWIDGET_H
#define DBSETTINGSWIDGET_H

#include <QWidget>
#include "ISettings.h"

namespace Ui {
class DbSettingsWidget;
}

class DbSettingsWidget : public QWidget, public ISettings
{
    Q_OBJECT
    public:
        enum class DB_SETTINGS : int
        {
            CONNECTION_NAME = 0,
            HOST_NAME = 1,
            DB_NAME = 2,
            USER_NAME = 3,
            PASSWORD = 4,
            PORT_NUMBER = 5
        };
        static QString settingsNamesToString(const DB_SETTINGS settingsName);
        explicit DbSettingsWidget(QWidget *parent = nullptr);
        ~DbSettingsWidget();
        void loadSettings()override;
        void saveSettings()override;
        void updateContent()override;
    private:
        Ui::DbSettingsWidget *ui;
        static const int SETTINGS_COUNT = 5;
        void initUi();
        void initConnections();
};

#endif // DBSETTINGSWIDGET_H
