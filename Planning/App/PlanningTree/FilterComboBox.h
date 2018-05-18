#ifndef FILTERCOMBOBOX_H
#define FILTERCOMBOBOX_H

#include <QComboBox>

class FilterComboBox : public QComboBox
{
    Q_OBJECT
    public:
        explicit FilterComboBox(QWidget *prent = nullptr);
    private:
        bool m_showFactor;
    protected:
        void showPopup()override;
        void hidePopup()override;
    signals:
        void popupHidden();
};

#endif // FILTERCOMBOBOX_H
