#ifndef MGUIITEMSMODEL_H
#define MGUIITEMSMODEL_H

#include <QObject>
#include <QVector>
#include <QStringListModel>
#include <QGraphicsScene>
#include "mgraph.h"

/**
 * @brief MGuiItemsModel - класс графической сцены
 */
class MGuiItemsModel : public QStringListModel
{
public:
    MGuiItemsModel(QGraphicsScene *scene) {this->scene = scene;}
    //QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void addItem(GuiType type);
    MBase *getItem(int32_t index);

protected:
    QVector<MBase *> items;
    QGraphicsScene *scene;

public slots:
    void newItem(GuiType id);
    void selectItem(int32_t id);
};

#endif // MGUIITEMSMODEL_H
