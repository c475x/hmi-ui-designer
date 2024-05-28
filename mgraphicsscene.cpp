#include "mgraphicsscene.h"

#include <QMimeData>

/**
 * @brief MGraphicsScene::dragEnterEvent - функция начала события перетягивания
 * @param event - указатель на драг-н-дроп ивент
 */
void MGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	event->acceptProposedAction();
}

/**
 * @brief MGraphicsScene::dragEnterEvent - функция конца события перетягивания
 * @param event - указатель на драг-н-дроп ивент
 */
void MGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	const QMimeData *mimeData = event->mimeData();

	if (mimeData->hasText())
	{
		QString a = mimeData->text();
		GuiType b = (GuiType)a.toInt();

		emit newItem(b);
	}

	event->acceptProposedAction();
}

/**
 * @brief MGraphicsScene::updateScene - функция обновления сцены
 */
void MGraphicsScene::updateScene()
{
	update();
}
