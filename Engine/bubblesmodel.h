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
    Q_PROPERTY(int cScore READ cScore WRITE setCScore NOTIFY cScoreChanged)
    Q_INVOKABLE void generateBoard();
    Q_INVOKABLE bool move(int from, int to);
    Q_INVOKABLE void remove();
    Q_INVOKABLE void moveCompleted();
    void setCScore(const int&val);
    int cScore() const;
signals:
    void cScoreChanged();
private:
    QVector<int> simpleMatch() const;
    void loadFromJSON();
    void checkBoard();
    QVector<int> findMatch() const;
    QColor randomColor() const;
    void applyMove(int from, int to);
    bool isLoss();
    QList<QColor> m_avaliableColors;
    QVector<QColor> m_elements;
    ushort m_rows;
    ushort m_columns;
    int m_cScore;
};
