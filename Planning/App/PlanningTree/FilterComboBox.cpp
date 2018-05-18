#include "FilterComboBox.h"

FilterComboBox::FilterComboBox(QWidget *parent) : QComboBox(parent), m_showFactor(false)
{

}

void FilterComboBox::showPopup()
{
    m_showFactor = true;
    QComboBox::showPopup();
}

void FilterComboBox::hidePopup()
{
    QComboBox::hidePopup();
    if (m_showFactor){
        emit popupHidden();
    }
    m_showFactor = false;
}
