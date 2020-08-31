#include "bubblesmodel.h"

void BublesModel::loadFromJSON()
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
    m_columns = JSON_doc["width"].toInt();
    m_rows = JSON_doc["height"].toInt();
}

QColor BublesModel::randomColor() const
{
    return m_avaliableColors[QRandomGenerator::system()->bounded(m_avaliableColors.count())];
}

void BublesModel::checkBoard()
{
    QVector<int> vec;
    while((vec = simpleMatch()) != QVector<int>({0, 0, 0})){
        QColor old_color = m_elements[vec[1]];
        while ((m_elements[vec[1]] = randomColor()) == old_color) {;}
    }

}

QVector<int> BublesModel::simpleMatch() const{
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

QVector<int> BublesModel::findMatch() const
{
    QVector<int> result = simpleMatch();
    qDebug() << "result " << (result != QVector<int>({0, 0, 0}));
    if (result != QVector<int>({0, 0, 0})){
        int counter = 0;
        if ((result[1] - result[0]) == 1){
            ushort low_pos = result[0] - result[0] % m_columns;
            counter = result[0] - 1;
            while( (counter > low_pos) && (m_elements[counter] == m_elements[result[0]])){
                result.push_front(counter--);
            }
            counter = result.last() + 1;
            while( (counter < (low_pos + m_columns)) && (m_elements[counter] == m_elements[result[0]])){
                result.push_back(counter++);
            }
        } else {
            counter = result[0] - m_columns;
            while( (counter >= 0) && (m_elements[counter] == m_elements[result[0]])){
                result.push_front(counter);
                counter -= m_columns;
            }
            counter = result.last() + m_columns;
            while( (counter < m_elements.count()) && (m_elements[counter] == m_elements[result[0]])){
                result.push_back(counter);
                counter += m_columns;
            }
        }
    }
    return result;
}

void BublesModel::generateBoard()
{
    emit beginResetModel();
    m_elements.clear();
    for (int i = 0 ; i < m_columns * m_rows; ++i) {
        m_elements.push_back(randomColor());
    }
    checkBoard();
    emit endResetModel();
}

void BublesModel::move(int from, int to)
{
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
    if (abs(offset) == 1 || abs(offset) == m_columns){
        emit beginMoveRows(QModelIndex(), from, from, QModelIndex(), to + ((offset > 0) ? 1 : 0));
        m_elements.move(from, to);
        emit endMoveRows();
    }
    if (abs(offset) == m_columns){
        emit beginMoveRows(QModelIndex(), to + ((offset < 0) ? 1 : -1) , to + ((offset < 0) ? 1 : -1), QModelIndex(), from + ((offset < 0)? 1 : 0));
        m_elements.move(to + ((offset < 0) ? 1 : -1), from);
        emit endMoveRows();
    }
    if (simpleMatch() == QVector<int>({0, 0, 0})){
        move(to, from);
    }
}
void BublesModel::colapce(){
    qDebug() << m_colapse;
        if (m_colapse.count() >= 3){
            int j = 0;
            int from = 0, to = 0;
            int end = 0;
            for(const auto& c : m_colapse){
                emit beginInsertRows(QModelIndex(), c, c);
                m_elements.insert(c, randomColor());
                emit endInsertRows();
            }
            end = (m_colapse[1] - m_colapse[0] == 1) ? (m_colapse.last()) : (m_colapse.first());
            while (m_colapse.first() - m_columns * (j + 1) >= 0){
                for (int i = m_colapse.first(); i <= end; ++i){
                    from = i - m_columns - m_columns * j;
                    to = (m_colapse[1] - m_colapse[0] == 1) ? (i - m_columns * j) : (m_colapse.last());
                    qDebug() << "from " << from << " to " << to;
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
    m_colapse.clear();
}



void BublesModel::remove(){
//        while (simpleMatch() != QVector<int>({0, 0, 0})){
        QVector<int> temp_vec = findMatch();
        if (temp_vec[1] - temp_vec[0] == 1){
            int deleted = 0;
            for (const auto& i : temp_vec){
                m_elements.remove(i - deleted);
                emit beginRemoveRows(QModelIndex(), i - deleted, i - deleted);
                deleted++;
                emit endRemoveRows();
            }
        }
        else if (temp_vec[1] - temp_vec[0] == m_columns){
            for (auto i = temp_vec.count() - 1; i >= 0; --i){
                emit beginRemoveRows(QModelIndex(), temp_vec[i], temp_vec[i]);
                qDebug() << "removing " << temp_vec[i] << "length is NE TO" ;
                m_elements.remove(temp_vec[i]);
                emit endRemoveRows();
            }
        }
        m_colapse = temp_vec;
//    }
}

BublesModel::BublesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    loadFromJSON();
    generateBoard();
}

BublesModel::~BublesModel()
{
}

int BublesModel::rowCount(const QModelIndex &) const
{
    return m_elements.count();
}

QVariant BublesModel::data(const QModelIndex &index, int role) const
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
