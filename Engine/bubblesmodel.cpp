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
    ushort area = m_rows * m_columns;
    for (int i = 0; i < (area)-2; ++i){
        if ((m_elements[i] == m_elements[i + 1]) && (m_elements[i] == m_elements[i + 2])){
            return QVector<int>({i, i + 1, i + 2});
        }
        if ((i + (2 * m_columns)) < area){
            if ((m_elements[i] == m_elements[i + m_columns]) && \
                    (m_elements[i] == m_elements[i + (2 * m_columns)])){
                return QVector<int>({i, i + m_columns, i + (2 * m_columns)});
            }
        }
    }
    return {0, 0, 0};
}

void BublesModel::move()
{

}

QVector<int> BublesModel::findMatch() const
{
    QVector<int> result = simpleMatch();

    if (result != QVector<int>({0, 0, 0})){
        int counter = 0;
        if ((result[1] - result[0]) == 1){
            counter = result[0] - 1;
            ushort low_pos = result[0] - result[0] % m_columns;
            while( (counter >= low_pos) && (m_elements[counter] == m_elements[result[0]])){
                result.push_front(counter--);
            }
            counter = result[result.count() - 1];
            while( (counter <= (low_pos + m_columns)) && (m_elements[counter] == m_elements[result[0]])){
                result.push_back(counter++);
            }
        } else {
            counter = result[0] - m_columns;
            while( (counter >= 0) && (m_elements[counter] == m_elements[result[0]])){
                result.push_front(counter);
                counter -= m_columns;
            }
            counter = result[result.count() - 1];
            while( (counter <= m_elements.count()) && (m_elements[counter] == m_elements[result[0]])){
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
    for (int i = 0 ; i < (m_rows * m_columns); ++i) {
        m_elements.push_back(randomColor());
    }
    checkBoard();
    emit endResetModel();
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
