#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QStandardItemModel>
#include <QEvent>
#include <QLabel>

#include "mguiitemsmodel.h"
#include "mgraph.h"
#include "mgraphicsscene.h"
#include "mlistwidget.h"
#include "mitempropertymodel.h"
#include "mlisteditor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainForm; }
QT_END_NAMESPACE

/**
 * @brief MainForm - класс главной формы приложения
 */
class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    MainForm(QWidget *parent = nullptr);
    ~MainForm();

private:
    Ui::MainForm *ui;
    MGuiItemsModel *guiModel;
    MGraphicsScene *scene;
    MItemPropertyModel *propModel;
    QStandardItemModel *guiIconsModel;
    MListWidget *listPalette;
    QLabel *status;
    QStringList tempPropertyList;

    //void onMouseMove(QGraphicsSceneMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    //void on_listPalette_pressed(const QModelIndex &index);
    //void mousePressEvent(QMouseEvent *event);
    //bool eventFilter(QObject *object, QEvent *event);
    //void on_listGuiItems_entered(const QModelIndex &index);

public slots:
    void showStatus(QString text);
    void listItemClicked(QModelIndex index);
    void setWidget(QPoint pos, QWidget *wgt);
    void dialogResult(const QStringList &data);

signals:
    void selectItem(int32_t id);
    //void selectProp(int32_t id);
};
#endif // MAINFORM_H
