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

void BubblesModel::move(int from, int to)
{
    static bool moveBack = true;
    int offset = to - from;
    if (from < 0|| from >= m_elements.count()){
        return;
    }
    if (to < 0|| to >= m_elements.count()){
        return;
    }
    if (from == to){
        return;
    }
    if ((offset == 1) && (to % m_columns == 0)){
        return;
    }
    if ((offset == -1) && (from % m_columns == 0)){
        return;
    }
    applyMove(from, to);
    if ((simpleMatch() != QVector<int>({0, 0, 0})) ){
        remove();
    }
    else{
        if (moveBack){
            moveBack = false;
            applyMove(to, from);
            moveBack = true;
        }
    }
}


/*
    ***************
    **NEED REVIEW**
    ***************
*/
void BubblesModel::collapse(){
    if (m_collapse.count() >= 3){
        int j    = 0;
        int from = 0;
        int to   = 0;
        int end  = 0;
        end = (m_collapse[1] - m_collapse[0] == 1) ? (m_collapse.last()) : (m_collapse.first());
        while (m_collapse.first() - m_columns * (j + 1) >= 0){
            for (int i = m_collapse.first(); i <= end; ++i){
                from = i - m_columns - m_columns * j;
                to = (m_collapse[1] - m_collapse[0] == 1) ? (i - m_columns * j) : (m_collapse.last() - m_columns * j);
                emit beginMoveRows(QModelIndex(), from , from, QModelIndex(),to + 1);
                m_elements.move(from, to);
                emit endMoveRows();
                emit beginMoveRows(QModelIndex(), to - 1 , to - 1, QModelIndex(), from);
                m_elements.move(to - 1, from);
                emit endMoveRows();
            }
            j++;
        }
    }
    m_collapse.clear();
}

void BubblesModel::add()
{
    if (m_add.count() >= 3){
        qDebug() << m_add;
        if ((m_add[1] - m_add[0]) == 1){
            for (auto c: m_add){
                m_elements.remove(c % m_columns);
    //            update();
                emit beginInsertRows(QModelIndex(), c % m_columns, c % m_columns);
                m_elements.insert(c % m_columns, randomColor());
                emit endInsertRows();
            }
        }
        m_add.clear();
//        update();
    }
    m_add.clear();
}

void BubblesModel::update()
{
    emit beginResetModel();
    emit endResetModel();
    collapse();
    if ((m_add.last() / m_columns) < 1){
        add();
    }
}

void BubblesModel::remove(){
    if (simpleMatch() != QVector<int>({0, 0, 0})){
        QColor trans;
        update();
        trans.setAlpha(0);
        QVector<int> temp_vec = findMatch();
        if ((temp_vec[1] - temp_vec[0] == 1) || (temp_vec[1] - temp_vec[0] == m_columns)){
             for (auto i = temp_vec.count() - 1; i >= 0; --i){
                 emit beginRemoveRows(QModelIndex(), temp_vec[i], temp_vec[i]);
                 m_elements[temp_vec[i]] = trans;
                 emit endRemoveRows();
            }
        }
        m_add = temp_vec;
        m_collapse = temp_vec;
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
