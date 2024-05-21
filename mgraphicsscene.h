#ifndef MGRAPHICSSCENE_H
#define MGRAPHICSSCENE_H

#include <QDragEnterEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsScene>
#include <QMimeData>
#include <QGraphicsRectItem>
//#include "mgraph.h"
#include "mguiitemsmodel.h"

class MGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    MGraphicsScene() : QGraphicsScene()
    {
        this->addItem( this->background = new QGraphicsRectItem(0, 0, 1, 1));
        background->setAcceptDrops(true);
    }

    virtual void resize(int width, int height)
    {
        this->background->setRect(0, 0, width - 1, height - 1);
    }

protected:
    QGraphicsRectItem* background = nullptr;
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    virtual void dropEvent(QGraphicsSceneDragDropEvent* event) override;

signals:
    void newItem(GuiType id);

public slots:
    void updateScene();
};

#endif // MGRAPHICSSCENE_H
