#include <qrect.h>
#include "mguiitemsmodel.h"

/**
 * @brief MGuiItemsModel::rowCount - функция возвращает количество строк списка
 * @param parent - неведомо
 * @return - возвращаем количество элементов в хранилище
 */
int MGuiItemsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return items.length();
}

/**
 * @brief MGuiItemsModel::data - функция возвращает представления элемента в хранилище
 * @param index - индекс выбранного элемента
 * @param role - роль отображения
 * @return - данные для отображения типа QVariant
 */
QVariant MGuiItemsModel::data(const QModelIndex &index, int role) const
{
    int x, y;
    switch(role)
    {
        case Qt::DisplayRole:
            x = index.column();
            y = index.row();
            if(x < items.length())
            {
                return QVariant("[" + QString(items.at(y)->type) + "]");
            }
        break;
        case Qt::BackgroundRole:

        break;
    }
    return QVariant();
}

/**
 * @brief MGuiItemsModel::addItem - функция добавляет в хранилище новый элемент управления
 * @param type
 */
void MGuiItemsModel::addItem(GuiType type)
{
    MBase *temp;
    switch(type)
    {
        case GuiIconSet:
            temp = new MIconSet(QPointF(0, 0), QSize(8, 8));
        break;
        case GuiLabel:
            temp = new MLabel(QRectF(QPointF(0, 0), QSizeF(5*6, 8)), "label");
        break;
        case GuiProgress:
            temp = new MProgress(QRectF(QPointF(0, (64-8)), QSizeF(128, 8)));
        break;
        case GuiMenu:
            temp = new MMenu(QRectF(QPointF(0, 8), QSizeF(128, (64-16))));
        break;
    }
    scene->addItem(temp);
    beginResetModel();
    items.append(temp);
    endResetModel();
}

/**
 * @brief MGuiItemsModel::newItem - слот, вызывается когда дропают иконку элемента управления в область экрана
 * @param id - тип элемента управления
 */
void MGuiItemsModel::newItem(GuiType id)
{
    addItem(id);
}

/**
 * @brief MGuiItemsModel::selectItem - слот, вызывается когда кликают мышкой по списку существующих элементов управления
 * @param id - номер выбранного элемента управления из списка
 */
void MGuiItemsModel::selectItem(int32_t id)
{
    // Перебираем все элементы из хранилища
    for(int32_t i=0; i<items.length(); i++)
        // Меняем всем признаки выбора
        items[i]->setSelected(i == id);
    scene->update();
}

MBase *MGuiItemsModel::getItem(int32_t index)
{
    if(index < items.size())
        return items.at(index);
    else
        return NULL;
}
