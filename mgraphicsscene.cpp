#include "mgraphicsscene.h"

void MGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    //if ( event->mimeData()->hasUrls() )
        event->acceptProposedAction();
}

void MGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasText())
    {
        QString a = mimeData->text();
        GuiType b = (GuiType)a.toInt();
        emit newItem(b);
//        switch(b)
//        {
//            case 0:
//                MIconSet *icon1 = new MIconSet(QPoint(0, 0), QSize(8*4, 32));
//                addItem(icon1);
//            break;
//        }
    }
    event->acceptProposedAction();
}

void MGraphicsScene::updateScene()
{
    update();
}
