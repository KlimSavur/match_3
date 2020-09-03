#pragma once

#include <QAbstractListModel>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QColor>
#include <QRandomGenerator>

class BubblesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    BubblesModel(QObject *parent = nullptr);
    ~BubblesModel() override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE void generateBoard();
    Q_INVOKABLE void move(int from, int to);
    Q_INVOKABLE void remove();
    Q_INVOKABLE void collapse();
    Q_INVOKABLE void add();
    Q_INVOKABLE void update();
private:
    QVector<int> simpleMatch() const;
    void loadFromJSON();
    void checkBoard();
    QVector<int> findMatch() const;
    QColor randomColor() const;
    void applyMove(int from, int to);
    QList<QColor> m_avaliableColors;
    QVector<QColor> m_elements;
    ushort m_rows;
    ushort m_columns;
    QVector<int> m_collapse;
    QVector<int> m_add;

};
