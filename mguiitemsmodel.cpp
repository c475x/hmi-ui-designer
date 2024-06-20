#include <qrect.h>
#include <QLineEdit>
#include <QSet>

#include "mguiitemsmodel.h"
#include "mtypes.h"

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
	switch (role)
	{
		case Qt::DisplayRole:
			x = index.column();
			y = index.row();
			if (x < items.length())
			{
				return items.at(y)->name;
			}
		break;
		case Qt::BackgroundRole:
			x = index.column();
			y = index.row();
			if (x < items.length())
			{
				GuiType type = items.at(y)->props->getGuiType();
				switch (type)
				{
					case GuiIconSet:
						return "icon";
					break;
					case GuiLabel:
						return "label";
					break;
					case GuiMenu:
						return "menu";
					break;
					case GuiProgress:
						return "progress";
					break;
					case GuiCombo:
						return "combo";
					break;
					default:
						return "";
					break;
				}
			}
		break;
	}

	return QVariant();
}

/**
 * @brief generateElementName - вспомогательная функция, генерирующая уникальное имя элемента на основе базового имени
 * @param baseName - базовое имя элемента
 * @param items - вектор существующих имен
 * @return - уникальное имя элемента
 */
QString generateElementName(QString baseName, const QVector<MBase *> &items)
{
	QSet<QString> names;
	for (MBase *item : items)
	{
		names.insert(item->name);
	}

	// Если имя уникальное, возвращаем его напрямую
	if (!names.contains(baseName))
	{
		return baseName;
	}

	// Ищем уникальное имя
	int count = 1;
	QString newName;
	do {
		newName = QString("%1%2").arg(baseName).arg(count);
		count++;
	} while (names.contains(newName));

	return newName;
}

/**
 * @brief MGuiItemsModel::addItem - функция добавляет в хранилище новый элемент управления
 * @param type - тип элемента управления
 */
void MGuiItemsModel::addItem(GuiType type)
{
	MBase *temp;

	switch (type)
	{
		case GuiIconSet:
			temp = new MIconSet(generateElementName("icon", items), QPointF(0, 0), QSize(8, 8));
		break;
		case GuiLabel:
			temp = new MLabel(generateElementName("label", items), "label", QRectF(QPointF(0, 0), QSizeF(5*6, 8)));
		break;
		case GuiProgress:
			temp = new MProgress(generateElementName("progress", items), "", QRectF(QPointF(0, (64-8)), QSizeF(128, 8)));
		break;
		case GuiMenu:
			temp = new MMenu(generateElementName("menu", items), QRectF(QPointF(0, 8), QSizeF(128, (64-16))));
		break;
		case GuiCombo:
			temp = new MCombo(generateElementName("combo", items), QRectF(QPointF(0, 8), QSizeF(128, (64-16))));
		break;
	}

	scene->addItem(temp);
	beginResetModel();
	items.append(temp);
	endResetModel();
}

/**
 * @brief MGuiItemsModel::getItem - функция получения элемента по его индексу
 * @param index - номер получаемого элемента
 * @return - сам элемент
 */
MBase *MGuiItemsModel::getItem(int32_t index)
{
	if (index < items.size())
		return items.at(index);
	else
		return NULL;
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
	for (int32_t i = 0; i < items.length(); i++)
	{
		// Меняем всем признаки выбора
		items[i]->setSelected(i == id);
	}

	scene->update();
}

/**
 * @brief MGuiItemsModel::renameItem - слот, вызывается при двойном клике по элементу (событии переименования)
 * @param id - номер переименовываемого элемента
 * @param newName - новое имя переименовываемого элемента
 */
void MGuiItemsModel::renameItem(int32_t id, QString newName)
{
	// Меняем название только если элемент существует и новое имя не является пустой строкой
	if (id < 0 || id >= items.size() || newName.isEmpty())
	{
		return;
	}

	// Максимальное количество символов в названии - 20
	items[id]->name = newName.left(20);
}

/**
 * @brief MGuiItemsModel::deleteItem - слот, вызывается при нажатии клавиши Del и удаляет выбранный элемент
 * @param id - номер удаляемого элемента
 */
void MGuiItemsModel::deleteItem(int32_t id)
{
	if (id < 0 || id >= items.size())
	{
		return;
	}

	beginRemoveRows(QModelIndex(), id, id);
	delete items[id];
	items.removeAt(id);
	endRemoveRows();
}
