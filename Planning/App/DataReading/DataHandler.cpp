#include "DataHandler.h"

const QString ValidationFailed::message = QString("Validation Failed.\n");

DataHandler::DataHandler()
{

}

void DataHandler::setData(const QByteArray &data)
{
    m_data = qUncompress(data);
    m_data = data.fromBase64(m_data);
}

const QByteArray &DataHandler::getData()const
{
    return m_data;
}

QStringList DataHandler::getHeaders() const
{
    QByteArray line;
    for(int i = 0; i < m_data.size();++i){
        const auto& byte = m_data.at(i);
        if (byte == '\n'){
            break;
        }
        line.push_back(byte);
    }
    line = line.trimmed();
    return splitLine(line);
}

QStringList DataHandler::splitLine(const QByteArray &line)const
{
    bool underQuote = false;
    QString columnExp;
    QStringList expressions;
    foreach(const auto& byte, line){
        if (byte == '"'){
            underQuote = 1 - underQuote;
        }else{
            if (underQuote){
                columnExp.push_back(byte);
            }
            else{
                if (byte == ','){
                    expressions<<columnExp.trimmed();
                    columnExp.clear();
                }
                else{
                    columnExp.push_back(byte);
                }
            }
        }
    }
    if (!line.isEmpty())
        expressions<<columnExp.trimmed();
    return expressions;
}

QHash<QString, QPair<int, QStringList> > DataHandler::readMappedData(const QMap<MappingItem::DATA_TYPES, int> &mapping)
{
    QHash<QString, QPair<int, QStringList> > result;
    int idColumn = mapping.value(MappingItem::DATA_TYPES::DATA_TASK_CODE);
    int durColumn = mapping.value(MappingItem::DATA_TYPES::DATA_DURATION);
    int dependColumn = mapping.value(MappingItem::DATA_TYPES::DATA_DEPENDENCE);
    auto rows = m_data.split('\n');
    rows.removeFirst();
    for(int i = 0; i < rows.size(); ++i){
        auto row = rows[i];
        if (row.isEmpty()){
            continue;
        }
        auto values = splitLine(row);
        if (idColumn >= values.size()){
            throw ValidationFailed(QString("Task Id not in row " + QString::number(i + 1)));
        }
        auto taskId = values.at(idColumn).trimmed();
        if (taskId.isEmpty()){
            throw ValidationFailed(QString("Empty task Id in row " + QString::number(i + 1)));
        }
        if (result.find(taskId) != result.end()){
            throw ValidationFailed(QString("Multiple Task Id " + taskId + " use in row " + QString::number(i + 1)));
        }
        bool ok;
        if (durColumn >= values.size()){
            throw ValidationFailed(QString("Duration value not in row " + QString::number(i + 1)));
        }
        auto duration = values.at(durColumn).trimmed().toInt(&ok);
        if (!ok){
            throw ValidationFailed(QString("Duration must be integer in row " + QString::number(i + 1)));
        }
        if (duration <= 0){
            throw ValidationFailed(QString("Duration must be positive in row " + QString::number(i + 1)));
        }
        auto dependedTasks = QStringList();
        if (dependColumn < values.size()){
            auto dependStr = values.at(dependColumn).trimmed();
            auto tempList = dependStr.split(',');
            foreach(auto it, tempList){
                it = it.trimmed();
                dependedTasks.push_back(it);
            }
            if (dependStr.isEmpty()){
                dependedTasks.clear();
            }
        }
        result[taskId] = qMakePair(duration, dependedTasks);
    }
    foreach(const auto& key, result.keys()){
        foreach(const auto & depId, result.value(key).second){
            if (result.find(depId) == result.end()){
                throw ValidationFailed(QString("Invalid dependence from " + key + " to " + depId));
            }
        }
    }
    return result;
}


