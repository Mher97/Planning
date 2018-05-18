#ifndef BUSYDIALOG_H
#define BUSYDIALOG_H

#include <QDialog>

namespace Ui {
class BusyDialog;
}

class BusyIndicator;

class BusyDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit BusyDialog(const QString& text, QWidget *parent = nullptr);
        ~BusyDialog();
        void updateBusyIndicators();
    private:
        Ui::BusyDialog *ui;
        static constexpr int INDICATORS_COUNT = 6;
        int m_currentIndicatorIndex;
        BusyIndicator *m_currentIndicator;
};

#endif // WAITDIALOG_H
