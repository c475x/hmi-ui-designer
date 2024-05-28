#ifndef MLISTWIDGET_H
#define MLISTWIDGET_H

#include <QWidget>
#include <QListWidget>

/**
 * @brief MListWidget - класс списка элементов, доступных для добавления
 */
class MListWidget : public QListWidget
{
public:
	MListWidget(QWidget *parent = 0);

protected:
	//void dragEnterEvent(QDragEnterEvent *event) override;
	//void dragMoveEvent(QDragMoveEvent *event) override;
	//void dropEvent(QDropEvent *event) override;
	void startDrag(Qt::DropActions acts) override;
};

#endif // MLISTWIDGET_H
