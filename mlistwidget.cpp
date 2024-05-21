#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include "mlistwidget.h"

MListWidget::MListWidget(QWidget *parent) :
    QListWidget(parent)
{
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(DragOnly);
}

void MListWidget::startDrag(Qt::DropActions acts)
{
    QListWidgetItem *item = currentItem();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

//    QStringList tmp = item->text().split(",");
//    foreach(QString str, tmp)
//        dataStream << str.toLocal8Bit();
    int32_t type = item->type();

    QMimeData *mimeData = new QMimeData;
    //mimeData->setData(MListWidget::puzzleMimeType(), itemData);
    mimeData->setText(QString::number(type));

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction);
}
