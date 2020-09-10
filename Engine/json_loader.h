#pragma once
#include <QObject>
#include <QColor>
class JSON_loader : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE QList<QColor> avaliableColors() const;
    Q_INVOKABLE int rows() const;
    Q_INVOKABLE int columns() const;
    Q_INVOKABLE static JSON_loader* getInstace();
private:
    JSON_loader();
    JSON_loader(const JSON_loader&);
    JSON_loader &operator=(const JSON_loader&);
private:
    static JSON_loader* m_instance;
    QList<QColor> m_avaliableColors;
    int m_rows;
    int m_columns;
};

