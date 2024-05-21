#ifndef MLISTWIDGET_H
#define MLISTWIDGET_H

#include <QWidget>
#include <QListWidget>

class MListWidget : public QListWidget
{
public:
    MListWidget(QWidget *parent = 0);
    //static QString puzzleMimeType() { return QStringLiteral("image/x-puzzle-piece"); }

protected:
    //void dragEnterEvent(QDragEnterEvent *event) override;
    //void dragMoveEvent(QDragMoveEvent *event) override;
    //void dropEvent(QDropEvent *event) override;
    void startDrag(Qt::DropActions acts) override;
};

#endif // MLISTWIDGET_H
