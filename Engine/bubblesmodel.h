#pragma once

#include <QAbstractListModel>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QColor>

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
    Q_INVOKABLE void checkMatch();
    void setCScore(int val);
    int cScore() const;
signals:
    void cScoreChanged();
    void openPopup();
private:
    QVector<int> simpleMatch() const;
    void loadFromJSON();
    void checkBoard();
    void findMatch();
    void remove(const QVector<int>& temp_vec);
    QColor randomColor() const;
    void applyMove(int from, int to);
    bool isLoss();
    int setCell(int* matchColumns, int index, int matched);
    int setCellRow(int* matchRows, int index, int matched);
private:
    QList<QColor> m_avaliableColors;
    QVector<QColor> m_elements;
    int m_rows;
    int m_columns;
    int m_cScore;
};
