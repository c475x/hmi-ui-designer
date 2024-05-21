#ifndef MGUIITEMSMODEL_H
#define MGUIITEMSMODEL_H

#include <QObject>
#include <QVector>
#include <QStringListModel>
#include <QGraphicsScene>
#include "mgraph.h"

class MGuiItemsModel : public QStringListModel
{
public:
    MGuiItemsModel(QGraphicsScene *scene) {this->scene = scene;}
    //QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    //inline QObject *parent() const { return d_ptr->parent; }
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    //virtual int columnCount(const QModelIndex &parent = QModelIndex()) const = 0;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void addItem(GuiType type);
    //MBase *getItem(int32_t index) {if(index < items.size()) return items.at(index); else return NULL;}
    MBase *getItem(int32_t index);

protected:
    QVector<MBase *> items;
    QGraphicsScene *scene;

public slots:
    void newItem(GuiType id);
    void selectItem(int32_t id);
};

#endif // MGUIITEMSMODEL_H
