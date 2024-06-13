#ifndef MGUIITEMSMODEL_H
#define MGUIITEMSMODEL_H

#include <QObject>
#include <QVector>
#include <QStringListModel>
#include <QGraphicsScene>

#include "mgraph.h"

/**
 * @brief MGuiItemsModel - класс модели управления элементами
 */
class MGuiItemsModel : public QStringListModel
{
public:
	MGuiItemsModel(QGraphicsScene *scene) { this->scene = scene; }
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
	void renameItem(int32_t id, QString newName);
	void deleteItem(int32_t id);
};

#endif // MGUIITEMSMODEL_H
