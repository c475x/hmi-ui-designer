#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>

#include "mlistwidget.h"

/**
 * @brief MListWidget::MListWidget - создает объект списка элементов, доступных для добавления
 * @param parent - неведомо
 */
MListWidget::MListWidget(QWidget *parent) : QListWidget(parent)
{
	setDragEnabled(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
	setDragDropMode(DragOnly);
}

/**
 * @brief MListWidget::startDrag - начало события перетягивания
 * @param acts - флаги
 */
void MListWidget::startDrag(Qt::DropActions acts)
{
	QListWidgetItem *item = currentItem();

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);

	int32_t type = item->type();

	QMimeData *mimeData = new QMimeData;
	mimeData->setText(QString::number(type));

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->exec(Qt::CopyAction);
}
