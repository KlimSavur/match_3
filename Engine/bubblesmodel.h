#ifndef BUBLESMODEL_H
#define BUBLESMODEL_H

#include <QAbstractListModel>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QColor>
#include <QRandomGenerator>

class BublesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    BublesModel(QObject *parent = nullptr);
    ~BublesModel() override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE void generateBoard();
    Q_INVOKABLE void move(int from, int to);
    Q_INVOKABLE void remove();
private:
    QVector<int> simpleMatch() const;
    void move();
    void loadFromJSON();
    void checkBoard();
    QVector<int> findMatch() const;
    QColor randomColor() const;
    QList<QColor> m_avaliableColors;
    QVector<QColor> m_elements;
    ushort m_rows;
    ushort m_columns;
};

#endif // BUBLESMODEL_H
