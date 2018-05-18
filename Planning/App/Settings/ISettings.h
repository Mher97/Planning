#ifndef ISETTINGS_H
#define ISETTINGS_H

#include <QMap>
#include <QVariant>
#include <QSettings>

class SettingsException : public std::exception
{
    public:
        virtual const char * what() const noexcept override
        {
            return "Incorrect Settings";
        }
};

class ISettings
{
    public:
        ISettings();
        virtual ~ISettings();
        virtual void loadSettings() = 0;
        virtual void saveSettings() = 0;
        virtual void updateContent() = 0;
        virtual const QSettings* getSettings()const;
        bool isDirty()const;
    protected:
        QSettings *m_settings;
        bool m_dirty;                       //if settings changed
};

#endif // SETTINGSINTERFACE_H
