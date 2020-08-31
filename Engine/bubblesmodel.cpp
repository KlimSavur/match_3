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
    static bool reset = true;
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
    if (reset){
        if (simpleMatch() == QVector<int>({0, 0, 0})){
            reset = false;
            move(to, from);
            reset = true;
        }
        else{
            remove();
        }
    }
}
/*void BublesModel::colapce(QVector<int> i){
    for (int counter = 0; counter <= i[0] % m_columns; ++counter){
        emit beginMoveRows(QModelIndex(), i[0]- m_columns*counter, i[i.count() - 1]- m_columns*counter, QModelIndex(), i[0] - m_columns*(counter+1));
        for (const auto& k: i ){
            move(k - m_columns*counter, k - m_columns*(counter + 1));
            int offset = m_columns - i.count();

        }
        emit endMoveRows();
//        emit beginResetModel();
//        for (const auto& k: i ){
//            move(k+i.size(), k - i.size());
//        }
//        emit endResetModel();

    }
//    for (int k = 0; k < counter; k++){
//        emit beginInsertRows(QModelIndex(), i - k*m_columns, i - k*m_columns);
//        m_elements.insert(i - k*m_columns, randomColor());
//        emit endInsertRows();
//    }
}*/



void BublesModel::remove(){
//        while (simpleMatch() != QVector<int>({0, 0, 0})){
        QVector<int> temp_vec = findMatch();
        qDebug() << temp_vec;
        if (temp_vec[1] - temp_vec[0] == 1){
            emit beginRemoveRows(QModelIndex(), temp_vec.first(), temp_vec.last());
            for (auto i = temp_vec.count() - 1; i >= 0; --i){
                qDebug() << "removing " << temp_vec[i]<< "length is" << i;
                m_elements.remove(temp_vec[i]);
//                emit beginInsertRows(QModelIndex(),temp_vec[i],temp_vec[i]);
//                m_elements.insert(temp_vec[i], QColor("white"));
//                emit endInsertRows();
            }
            emit endRemoveRows();
//            emit beginResetModel();
//            emit endResetModel();
//            colapce(temp_vec);
        }
        else{
            for (auto i = temp_vec.count() - 1; i >= 0; --i){
                emit beginRemoveRows(QModelIndex(), temp_vec[i], temp_vec[i]);
                qDebug() << "removing " << temp_vec[i] << "length is NE TO" ;
                m_elements.remove(temp_vec[i]);
                emit endRemoveRows();
            }
        }
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
