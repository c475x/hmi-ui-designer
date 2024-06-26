#include <QPushButton>

#include "mitempropertymodel.h"

#include "mgraph.h"

//
// ----------------------------------- MItemProperty -----------------------------------
//

/**
 * @brief MItemProperty::MItemProperty - создает набор свойств графического элемента
 * @param GuiType - тип графического элемента
 */
MItemProperty::MItemProperty(GuiType guiType)
{
	items.clear();
	items.append(new PropItem("X", PropNumber, 0));
	items.append(new PropItem("Y", PropNumber, 0));
	items.append(new PropItem("Width", PropNumber, 0));
	items.append(new PropItem("Height", PropNumber, 0));

	switch (guiType)
	{
		case GuiIconSet:
			items.append(new PropItem("Icons", PropFiles, QStringList()));
			items.append(new PropItem("CurIndex", PropNumber, -1));
		break;
		case GuiLabel:
			items.append(new PropItem("Text", PropString, ""));
			items.append(new PropItem("Font", PropNumber, 0));
		break;
		case GuiMenu:
			items.append(new PropItem("Items", PropList, QStringList()));
			items.append(new PropItem("StartPos", PropNumber, 0));
			items.append(new PropItem("CurPos", PropNumber, -1));
			items.append(new PropItem("Font", PropNumber, 0));
		break;
		case GuiProgress:
			items.append(new PropItem("Value", PropNumber, 0));
			items.append(new PropItem("Min", PropNumber, 0));
			items.append(new PropItem("Max", PropNumber, 0));
			items.append(new PropItem("Text", PropString, ""));
			items.append(new PropItem("Font", PropNumber, 0));
		break;
		case GuiCombo:
			items.append(new PropItem("Label", PropString, "combo"));
			items.append(new PropItem("Items", PropList, QStringList()));
			items.append(new PropItem("CurPos", PropNumber, -1));
			items.append(new PropItem("IsSelected", PropNumber, 0));
			items.append(new PropItem("Font", PropNumber, 0));
		break;
	}

	this->guiType = guiType;
}

MItemProperty::~MItemProperty()
{

}

/**
 * @brief MItemProperty::addProperty - добавляет свойство
 * @param type - тип свойства
 * @param name - имя свойства
 * @param data - значение свойства
 */
void MItemProperty::addProperty(PropType type, QString name, QVariant data)
{
	PropItem *pItem = new PropItem(name, type, data);
	items.append(pItem);
}

/**
 * @brief MItemProperty::getProperty - получает свойство по индексу
 * @param index - индекс свойства
 * @return указатель на PropItem, если индекс валидный, иначе NULL
 */
PropItem *MItemProperty::getProperty(int32_t index)
{
	if (index < items.size())
	{
		return items.at(index);
	}

	return NULL;
}

/**
 * @brief MItemProperty::setProperty - устанавливает значение свойства по индексу
 * @param index - индекс свойства
 * @param value - новое значение свойства
 */
void MItemProperty::setProperty(int32_t index, QVariant value)
{
	if (index < items.size())
	{
		items[index]->data = value;
	}
}

//
// ------------------------------------ MItemPropertyModel --------------------------------
//

/**
 * @brief MItemPropertyModel::rowCount - функция возвращает количество строк таблицы
 * @param parent - неведомо
 * @return - возвращаем количество свойств в таблице
 */
int MItemPropertyModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	if (pCurPropList)
		return pCurPropList->getSize();

	return 0;
}

/**
 * @brief MItemPropertyModel::columnCount - функция возвращает количество столбцов таблицы
 * @param parent - неведомо
 * @return - константно 2 (название свойства и его значение)
 */
int MItemPropertyModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return 2;
}

/**
 * @brief MItemPropertyModel::data - функция для получения данных ячейки
 * @param index - индекс ячейки
 * @param role - роль данных
 * @return - значение ячейки или пустой QVariant
 */
QVariant MItemPropertyModel::data(const QModelIndex &index, int role) const
{
	if (pCurPropList)
	{
		PropItem *pItem = pCurPropList->getProperty(index.row());

		switch (role)
		{
			case Qt::DisplayRole:
				if (pItem != NULL)
				{
					// Если первая колонка, то выводим имя параметра
					if (index.column() == 0)
					{
						return QVariant(pItem->name);
					}
					else
					{
						switch (pItem->type) // Смотрим, какого типа поле свойства
						{
							case PropString:	// Текстовая строка
								return QVariant(pItem->data);
							break;
							case PropNumber:	// Число
								return QVariant(pItem->data);
							break;
							break;
							case PropFiles:	 // Список имен файлов
							case PropList:	  // Список строк
							{
								QStringList temp = pItem->data.toStringList();
								QString info = QString("[%1]").arg(temp.size());
								return QVariant(info);
							}
							break;
						}
					}
				}
			break;
			case Qt::BackgroundRole:

			break;
		}
	}

	return QVariant();
}

/**
 * @brief MItemPropertyModel::setData - функция вызывается по окончании редактирования ячейки
 * @param index - позиция ячейки
 * @param value - значение, установленное в ячейке после редактирования
 * @param role - роль
 * @return - true, если все ок, иначе false
 */
bool MItemPropertyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	Q_UNUSED(role)

	if (pCurPropList)
	{
		PropItem *pItem = pCurPropList->getProperty(index.row());

		// Если в числовое поле была введена строка
		if (pItem->type == PropNumber)
		{
			bool ok;
			value.toInt(&ok);

			if (!ok)
			{
				return false;
			}
		}

		// При изменении полей-списков сам список изменяться не должен
		if (pItem->type == PropList || pItem->type == PropFiles)
		{
			return false;
		}

		// Поле StartPos элемента типа GuiMenu предназначено только для чтения
		if (pCurPropList->getGuiType() == GuiMenu && pItem->name == "StartPos")
		{
			return false;
		}

		// Поле IsSelected элемента типа GuiCombo может иметь значения 0 и 1
		if (pCurPropList->getGuiType() == GuiCombo && pItem->name == "IsSelected")
		{
			if (value != 0 && value != 1)
			{
				return false;
			}
		}

		if (pItem)
		{
			beginResetModel();
			pItem->data = value;
			endResetModel();
			emit updateScene();
			return true;
		}
	}

	return false;
}

/**
 * @brief MItemPropertyModel::flags - имплементация флагов для возможности редактирования ячеек
 * @param index - индекс выбранной ячейки
 * @return - флаги
 */
Qt::ItemFlags MItemPropertyModel::flags(const QModelIndex &index) const
{
	// Если выбрана вторая колонка, то можно редактировать
	if (index.column() > 0)
	{
		return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

/**
 * @brief MItemPropertyModel::setPropCur - устанавливает значение свойства по индексу для текущего набора свойств
 * @param index - индекс свойства
 * @param value - новое значение свойства
 */
void MItemPropertyModel::setPropCur(int32_t index, QVariant value)
{
	if (pCurPropList)
	{
		PropItem *pItem = pCurPropList->getProperty(index);
		if (pItem)
		{
			beginResetModel();
			pItem->data = value;
			endResetModel();
		}
	}
}

/**
 * @brief MItemPropertyModel::setProp - устанавливает значение свойства по индексу для указанного типа GUI
 * @param gui - тип графического элемента
 * @param index - индекс свойства
 * @param value - новое значение свойства
 */
void MItemPropertyModel::setProp(GuiType gui, int32_t index, QVariant value)
{
	PropItem *pItem = pCurPropList->getProperty(index);
	if (pItem)
	{
		beginResetModel();
		pItem->data = value;
		endResetModel();
	}
}

/**
 * @brief MItemPropertyModel::setList - устанавливает текущий набор свойств
 * @param list - указатель на новый набор свойств
 */
void MItemPropertyModel::setList(MItemProperty *list)
{
	beginResetModel();
	pCurPropList = list;
	endResetModel();
}

/**
 * @brief MItemPropertyModel::getProp - получает свойство по индексу
 * @param index - индекс свойства
 * @return указатель на PropItem, если индекс валидный, иначе NULL
 */
PropItem *MItemPropertyModel::getProp(int32_t index)
{
	return pCurPropList->getProperty(index);
}

/**
 * @brief MItemPropertyModel::setListData - устанавливает список строк по индексу
 * @param index - индекс свойства
 * @param data - список строк
 * @return true, если установка прошла успешно, иначе false
 */
bool MItemPropertyModel::setListData(int32_t index, QVariant data)
{
	PropItem *pItem = pCurPropList->getProperty(index);
	if (pItem)
	{
		if ((pItem->type == PropFiles) || (pItem->type == PropList))
		{
			beginResetModel();
			pItem->data = data;
			endResetModel();
			return true;
		}
	}

	return false;
}
