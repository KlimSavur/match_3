#include "bubblesmodel.h"

void BubblesModel::loadFromJSON()
{
    QString text;
    QFile file;
    file.setFileName("cfg.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    text = file.readAll();
    file.close();
    QJsonDocument JSON_doc = QJsonDocument::fromJson(text.toUtf8());
    for(const auto& i: JSON_doc["colors"].toArray()){
         m_avaliableColors.push_back(QColor(i.toString()));
    }
    m_columns = JSON_doc["columns"].toInt();
    m_rows = JSON_doc["rows"].toInt();
}

QColor BubblesModel::randomColor() const
{
    return m_avaliableColors[QRandomGenerator::system()->bounded(m_avaliableColors.count())];
}

void BubblesModel::checkBoard()
{
    QVector<int> vec;
    while((vec = simpleMatch()) != QVector<int>({0, 0, 0})){
        QColor old_color = m_elements[vec[1]];
        while ((m_elements[vec[1]] = randomColor()) == old_color) {;}
    }
}

QVector<int> BubblesModel::simpleMatch() const{
    ushort counter = 0;
    for (int i = 0; i < m_rows; ++i){
        for (int j = 0; j < m_columns; ++j){
            counter = m_columns * i + j;
            if (counter >= m_elements.count()) return QVector<int>({0, 0, 0});
            if (j < m_columns - 2){
                if ((m_elements[counter] == m_elements[counter + 1]) && \
                        (m_elements[counter] == m_elements[counter + 2])){
                    return QVector<int>({counter, counter + 1, counter + 2});
                }
            }
            if (counter + (2 * m_columns) < m_elements.count()){
                if ((m_elements[counter] == m_elements[counter + m_columns]) && \
                        (m_elements[counter] == m_elements[counter + (2 * m_columns)])){
                    return QVector<int>({counter, counter + m_columns, counter + (2 * m_columns)});
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
    checkBoard();
    emit endResetModel();
}


/*
    ***************
    **NEED REVIEW**
    ***************
*/
QVector<int> BubblesModel::findMatch() const
{
    QVector<int> result = simpleMatch();
    if (result != QVector<int>({0, 0, 0})){
        int counter = 0;
        if ((result[1] - result[0]) == 1){
            ushort low_pos = result[0] - result[0] % m_columns;
            counter = result[0] - 1;
            while ((counter > low_pos) && (m_elements[counter] == m_elements[result[0]])){
                result.push_front(counter--);
            }
            counter = result.last() + 1;
            while ((counter < (low_pos + m_columns)) && (m_elements[counter] == m_elements[result[0]])){
                result.push_back(counter++);
            }
        } else {
            counter = result[0] - m_columns;
            while ((counter >= 0) && (m_elements[counter] == m_elements[result[0]])){
                result.push_front(counter);
                counter -= m_columns;
            }
            counter = result.last() + m_columns;
            while ((counter < m_elements.count()) && (m_elements[counter] == m_elements[result[0]])){
                result.push_back(counter);
                counter += m_columns;
            }
        }
        for (auto c: result){
            if ((c - 2 * m_columns) < 0) return result;
            if ((c + 2 * m_columns) >= m_elements.count()) return result;
            if ((m_elements[c] == m_elements[c - m_columns]) && (m_elements[c] == m_elements[c - 2*m_columns]) ) {
                result.push_back(c - m_columns);
                result.push_back(c - 2 * m_columns);
                counter = c - 2 * m_columns;
                while ((counter -= m_columns) >= 0){
                    if (m_elements[0] == m_elements[counter]){
                        result.push_back(counter);
                    }
                    else{
                        counter = -1;
                    }
                }
            }
            if ((m_elements[c] == m_elements[c + m_columns]) && (m_elements[c] == m_elements[c + 2*m_columns]) ) {
                result.push_back(c + m_columns);
                result.push_back(c + 2 * m_columns);
                counter = c + 2 * m_columns;
                while ((counter += m_columns) < m_elements.count()){
                    if (m_elements[0] == m_elements[counter]){
                        result.push_back(counter);
                    }
                    else{
                        counter = m_elements.count();
                    }
                }
            }
            if ((c - m_columns >= 0) &&(c + m_columns < m_elements.count())){
                if ((m_elements[c] == m_elements[c + m_columns]) && (m_elements[c] == m_elements[c - m_columns]) ) {
                    result.push_back(c + m_columns);
                    result.push_back(c - m_columns);
                }
            }
        }
    }
    return result;
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


/*
    ***************
    **NEED REVIEW**
    ***************
*/

void BubblesModel::remove(){
    if (simpleMatch() != QVector<int>({0, 0, 0})){
        QVector<int> temp_vec = findMatch();
        if ((temp_vec[1] - temp_vec[0] == 1) || (temp_vec[1] - temp_vec[0] == m_columns)){
             for (auto i = 0; i <= temp_vec.count() - 1; ++i){
                 emit beginRemoveRows(QModelIndex(), temp_vec[i], temp_vec[i]);
                 m_elements.remove(temp_vec[i]);
                 emit endRemoveRows();
                 emit beginInsertRows(QModelIndex(), temp_vec[i] % m_columns, temp_vec[i] % m_columns);
                 m_elements.insert(temp_vec[i] % m_columns, randomColor());
                 emit endInsertRows();
                 for(int j = temp_vec[i] % m_columns + 1; j  < temp_vec[i]; j += m_columns){
                     qDebug() << "from " << j << " to " << j + m_columns -1 ;
                     emit beginMoveRows(QModelIndex(), j, j, QModelIndex(), j + m_columns);
                     m_elements.move(j, j + m_columns - 1);
                     emit endMoveRows();
                 }
            }
        }
    }
}


BubblesModel::BubblesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    loadFromJSON();
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
