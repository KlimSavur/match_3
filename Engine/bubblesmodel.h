#ifndef BUBLESMODEL_H
#define BUBLESMODEL_H

#include <QQuickItem>

class BublesModel : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(BublesModel)

public:
    explicit BublesModel(QQuickItem *parent = nullptr);
    ~BublesModel() override;
};

#endif // BUBLESMODEL_H
