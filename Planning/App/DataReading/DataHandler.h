#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QByteArray>
#include <QStringList>
#include "MappingItem.h"
#include <QHash>

class ValidationFailed : public std::exception
{
    public:
        template <typename T>
        ValidationFailed(T&& errorText) : std::exception(), m_errorText(std::forward<T>(errorText))
        {

        }
        virtual const char* what() const noexcept override
        {
            auto stdStr = (message + m_errorText).toStdString();
            char *result = new char[stdStr.length() + 1]();
            memcpy(result, stdStr.c_str(), stdStr.length());
            return result;
        }
    private:
        static const QString message;
        QString m_errorText;
};

class DataHandler
{
    public:
        DataHandler();
        void setData(const QByteArray& data);
        const QByteArray& getData()const;
        QStringList getHeaders()const;
        QHash<QString, QPair<int, QStringList>> readMappedData(const QMap<MappingItem::DATA_TYPES, int>& mapping);
    private:
        QByteArray m_data;
        QStringList splitLine(const QByteArray& line)const;
};

#endif // DATAHANDLER_H
