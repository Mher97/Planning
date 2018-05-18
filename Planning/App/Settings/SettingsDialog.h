#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class DbSettingsWidget;
class ISettings;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    public:
        explicit SettingsDialog(QWidget *parent = nullptr);
        ~SettingsDialog();
        void updateContent();
        void loadSettings();
        const ISettings* getDbSettings();
    private:
        Ui::SettingsDialog *ui;
        DbSettingsWidget* m_dbSettingsWidget;
        void initUi();
        void initConnections();
    private slots:
        void onSaveButtonClicked();
};

#endif // SETTINGSDIALOG_H
