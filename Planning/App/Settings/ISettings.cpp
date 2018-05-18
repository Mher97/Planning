#include "ISettings.h"
#include <QApplication>

ISettings::ISettings(): m_settings(nullptr), m_dirty(false)
{

}

ISettings::~ISettings()
{
    delete m_settings;
    m_settings = nullptr;
}

const QSettings* ISettings::getSettings()const
{
    return m_settings;
}

bool ISettings::isDirty() const
{
    return m_dirty;
}



