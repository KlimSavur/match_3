#include "bubblesmodel.h"
#include "json_loader.h"

#include <QRandomGenerator>

QColor BubblesModel::randomColor() const
{
    return m_avaliableColors[QRandomGenerator::system()->bounded(m_avaliableColors.count())];
}

void BubblesModel::checkBoard()
{
    QVector<int> vec;
    while((vec = simpleMatch()) != QVector<int>({0, 0, 0})){
        QColor old_color = m_elements[vec[1]];
        while ((m_elements[vec[1]] = randomColor()) == old_color);
    }
}

QVector<int> BubblesModel::simpleMatch() const{
    ushort counter = 0;
    for (int i = 0; i < m_rows; ++i){
        for (int j = 0; j < m_columns; ++j){
            counter = m_columns * i + j;
            if (counter >= m_elements.count()) return QVector<int>({0, 0, 0});
            if (j < m_columns - 2){
                if ((m_elements[counter] == m_elements[counter + 1]) && (m_elements[counter] == m_elements[counter + 2])){
                    return QVector<int>({counter, counter + 1, counter + 2});
                }
            }
            if (counter + 2 * m_columns < m_elements.count()){
                if ((m_elements[counter] == m_elements[counter + m_columns]) && (m_elements[counter] == m_elements[counter + 2 * m_columns])){
                    return QVector<int>({counter, counter + m_columns, counter + 2 * m_columns});
                }
            }
        }
    }
    return QVector<int>({0, 0, 0});
}

void BubblesModel::generateBoard()
{
    emit beginResetModel();
    m_elements.clear();
    for (int i = 0 ; i < m_columns * m_rows; ++i) {
        m_elements.push_back(randomColor());
    }
    while (isLoss())
        generateBoard();
    checkBoard();
    emit endResetModel();
}

int BubblesModel::setCell(QScopedArrayPointer<int>& matchColumns, int index, int matched) {
    if (index % m_columns >= m_columns - 1){
        matchColumns[index] = matched;
        return matched;
    }
    int res = 0;
    if (m_elements[index] == m_elements[index + 1]){
        res = setCell(matchColumns, index + 1, matched + 1);
        matchColumns[index] = res;
        return res;
    }
    else {
        setCell(matchColumns, index + 1, 1);
        matchColumns[index] = matched;
        return matched;
    }
}
int BubblesModel::setCellRow(QScopedArrayPointer<int>& matchRows, int index, int matched) {
    if (index / m_columns >= m_rows - 1){
        matchRows[index] = matched;
        return matched;
    }
    int res = 0;
    if (m_elements[index] == m_elements[index + m_columns]){
        res = setCellRow(matchRows, index + m_columns, matched + 1);
        matchRows[index] = res;
        return res;
    }
    else {
        setCellRow(matchRows, index + m_columns, 1);
        matchRows[index] = matched;
        return matched;
    }
}
void BubblesModel::findMatch()
{
    if (simpleMatch() != QVector<int>({0, 0, 0})){
        QVector<int> result;
        QScopedArrayPointer<int> matchRows (new int[m_rows * m_columns]);
        QScopedArrayPointer<int> matchColumns (new int[m_rows * m_columns]);
        for (int i = 0; i < m_rows * m_columns; ++i){
            matchRows[i] = 0;
        }
        for (int i = 0; i < m_rows * m_columns; ++i){
            matchColumns[i] = 0;
        }

        for (int i = 0; i < m_rows * m_columns; ++i){
            matchRows[i] = 0;
            matchColumns[i] = 0;
        }
        for (int i = 0; i < m_rows * m_columns; i += m_columns){
            if (m_elements[i] == m_elements[i + 1]){
                matchColumns[i] = setCell(matchColumns, i, 1);
            } else {
                setCell(matchColumns, i, 1);
                matchColumns[i] = 1;
            }
        }
        for (int i = 0; i < m_columns; ++i){
            if (m_elements[i] == m_elements[i + m_columns]){
                matchRows[i] = setCellRow(matchRows, i, 1);
            } else {
                setCellRow(matchRows, i, 1);
                matchRows[i] = 1;
            }
        }
        for (int i = 0; i < m_columns * m_rows; ++i){
            if (matchRows[i] >= 3 || matchColumns[i] >= 3){
                result.push_back(i);
            }
        }
        remove(result);
    }
}

void BubblesModel::applyMove(int from, int to){
    int offset = to - from;
    if (abs(offset) == 1 || abs(offset) == m_columns){
        emit beginMoveRows(QModelIndex(), from, from, QModelIndex(), to + ((offset > 0) ? 1 : 0));
        m_elements.move(from, to);
        emit endMoveRows();
    }
    if (abs(offset) == m_columns){
        emit beginMoveRows(QModelIndex(), to + ((offset < 0) ? 1 : -1) , to + ((offset < 0) ? 1 : -1), QModelIndex(), from + ((offset < 0) ? 1 : 0));
        m_elements.move(to + ((offset < 0) ? 1 : -1), from);
        emit endMoveRows();
    }
}

bool BubblesModel::isLoss()
{
    QScopedArrayPointer<int> matchRows (new int[m_rows * m_columns]);
    QScopedArrayPointer<int> matchColumns (new int[m_rows * m_columns]);
    for (int i = 0; i < m_rows * m_columns; ++i){
        matchRows[i] = 0;
    }
    for (int i = 0; i < m_rows * m_columns; ++i){
        matchColumns[i] = 0;
    }

    for (int i = 0; i < m_rows * m_columns; ++i){
        matchRows[i] = 0;
        matchColumns[i] = 0;
    }
    for (int i = 0; i < m_rows * m_columns; i += m_columns){
        if (m_elements[i] == m_elements[i + 1]){
            matchColumns[i] = setCell(matchColumns, i, 1);
        } else {
            setCell(matchColumns, i, 1);
            matchColumns[i] = 1;
        }
    }
    for (int i = 0; i < m_columns; ++i){
        if (m_elements[i] == m_elements[i + m_columns]){
            matchRows[i] = setCellRow(matchRows, i, 1);
        } else {
            setCellRow(matchRows, i, 1);
            matchRows[i] = 1;
        }
    }
    for (int i = 0; i < m_rows * m_columns; i++){
        if (matchColumns[i] == 2){
            if (i % m_columns + 2 < m_columns){
                if (i - m_columns >= 0){
                    if (m_elements[i] == m_elements[i + 2 - m_columns])
                        return false;
                }
                if (i + m_columns < m_columns * m_rows){
                    if (m_elements[i] == m_elements[i + 2 + m_columns])
                        return false;
                }
                if (i % m_columns + 3 < m_columns){
                    if (m_elements[i] == m_elements[i + 3])
                        return false;
                }
            }
            if (i % m_columns - 1 >= 0){
                if (i - m_columns >= 0){
                    if (m_elements[i] == m_elements[i - 1 - m_columns])
                        return false;
                }
                if (i + m_columns < m_columns * m_rows){
                    if (m_elements[i] == m_elements[i - 1 + m_columns])
                        return false;
                }
                if (i % m_columns - 2 >= 0){
                    if (m_elements[i] == m_elements[i - 2])
                        return false;
                }
            }
            i += 2;
        }
        if (matchColumns[i] == 1){
            if (i % m_columns + 2 < m_columns){
                if (m_elements[i] == m_elements[i + 2]){
                    if (i - m_columns >= 0){
                        if (m_elements[i] == m_elements[i + 1 - m_columns])
                            return false;
                    }
                    if (i + m_columns < m_columns * m_rows){
                        if (m_elements[i] == m_elements[i + 1 + m_columns])
                            return false;
                    }
                }
            }
        }
    }
    for (int i = 0; i < m_columns * m_rows; ++i){
        if (matchRows[i] == 2){
            if ((i + m_columns < m_columns * m_rows) && (matchRows[i + m_columns] == 2)){
                if (i + 2 * m_columns < m_columns * m_rows){
                    if (i % m_columns + 1 < m_columns )
                        if (m_elements[i] == m_elements[i + 2 * m_columns + 1])
                            return false;
                    if (i % m_columns - 1 >= 0)
                        if (m_elements[i] == m_elements[i + 2 * m_columns - 1])
                            return false;
                    if (i + 3 * m_columns < m_columns * m_rows)
                        if (m_elements[i] == m_elements[i + 3 * m_columns])
                            return false;
                }

                if (i - m_columns >= 0){
                    if (i % m_columns - 1 >= 0){
                        if (m_elements[i] == m_elements[i - m_columns - 1])
                            return false;
                    }
                    if (i % m_columns + 1 < m_columns){
                        if (m_elements[i] == m_elements[i - m_columns + 1])
                            return false;
                    }
                    if (i - 2 * m_columns  >= 0){
                        if (m_elements[i] == m_elements[i - 2 * m_columns])
                            return false;
                    }
                }
            }
        }
        if (matchRows[i] == 1){
            if (i + 2 * m_columns < m_columns * m_rows){
                if (m_elements[i] == m_elements[i + 2 * m_columns]){
                    if (i % m_columns - 1 >= 0){
                        if (m_elements[i] == m_elements[i + m_columns - 1])
                            return false;
                    }
                    if (i % m_columns + 1 < m_columns){
                        if (m_elements[i] == m_elements[i + m_columns + 1])
                            return false;
                    }
                }
            }
        }
    }
    return true;
}

bool BubblesModel::move(int from, int to)
{
    int offset = to - from;
    if (from < 0|| from >= m_elements.count()){
        return false;
    }
    if (to < 0|| to >= m_elements.count()){
        return false;
    }
    if (from == to){
        return false;
    }
    if ((offset == 1) && (to % m_columns == 0)){
        return false;
    }
    if ((offset == -1) && (from % m_columns == 0)){
        return false;
    }
    applyMove(from,to);
    if (simpleMatch() != QVector<int>({0, 0, 0})){
        return true;
    }
    applyMove(to, from);
    return false;
}


void BubblesModel::remove(const QVector<int>& temp_vec){
    for (auto i = 0; i <= temp_vec.count() - 1; ++i){
        emit beginRemoveRows(QModelIndex(), temp_vec[i], temp_vec[i]);
        m_elements.remove(temp_vec[i]);
        emit endRemoveRows();
        emit beginInsertRows(QModelIndex(), temp_vec[i] % m_columns, temp_vec[i] % m_columns);
        m_elements.insert(temp_vec[i] % m_columns, randomColor());
        emit endInsertRows();
        for(int j = temp_vec[i] % m_columns + 1; j <= temp_vec[i]; j += m_columns){
            emit beginMoveRows(QModelIndex(), j, j, QModelIndex(), j + m_columns);
            m_elements.move(j, j + m_columns - 1);
            emit endMoveRows();
        }
    }
    m_cScore += (temp_vec.count() * 10);
    emit cScoreChanged();
    if (simpleMatch() == QVector<int>({0, 0, 0}))
        if (isLoss())
            emit openPopup();
}

void BubblesModel::checkMatch(){
    findMatch();
}

void BubblesModel::setCScore(int val)
{
    if (val != m_cScore){
        m_cScore = val;
        emit cScoreChanged();
    }
}

int BubblesModel::cScore() const
{
    return m_cScore;
}


BubblesModel::BubblesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_cScore = 0;
    m_columns = JSON_loader::getInstace()->columns();
    m_rows = JSON_loader::getInstace()->rows();
    m_avaliableColors = JSON_loader::getInstace()->avaliableColors();
    generateBoard();
}

BubblesModel::~BubblesModel()
{
}

int BubblesModel::rowCount(const QModelIndex &) const
{
    return m_elements.count();
}

QVariant BubblesModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= m_elements.count()){
        return QVariant();
    }
    if (role == Qt::DisplayRole){
        return m_elements[row];
    }
    return QVariant();
}
