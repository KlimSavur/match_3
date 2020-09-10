#include "json_loader.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>


JSON_loader* JSON_loader::m_instance = 0;

int JSON_loader::columns() const
{
    return m_columns;
}

JSON_loader* JSON_loader::getInstace()
{
    if (!m_instance){
        m_instance = new JSON_loader();
        QString text;
        QFile file;
        file.setFileName("cfg.json");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        text = file.readAll();
        file.close();
        QJsonDocument JSON_doc = QJsonDocument::fromJson(text.toUtf8());
        for(auto i: JSON_doc["colors"].toArray()){
             m_instance->m_avaliableColors.push_back(QColor(i.toString()));
        }
        m_instance->m_columns = JSON_doc["columns"].toInt();
        m_instance->m_rows = JSON_doc["rows"].toInt();
    }
    return m_instance;
}

int JSON_loader::rows() const
{
    return m_rows;
}

QList<QColor> JSON_loader::avaliableColors() const
{
    return m_avaliableColors;
}
JSON_loader::JSON_loader()
{

}
