#include <QPushButton>

#include "mitempropertymodel.h"

//
// ----------------------------------- MItemProperty -----------------------------------
//

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
			items.append(new PropItem("IsVisible", PropNumber, 1));
		break;
		case GuiCombo:
		break;
	}

	this->guiType = guiType;
}

MItemProperty::~MItemProperty()
{

}

void MItemProperty::addProperty(PropType type, QString name, QVariant data)
{
	PropItem *pItem = new PropItem(name, type, data);
	items.append(pItem);
}

PropItem *MItemProperty::getProperty(int32_t index)
{
	if (index < items.size())
	{
		return items.at(index);
	}

	return NULL;
}

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

int MItemPropertyModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	if (pCurPropList)
		return pCurPropList->getSize();

	return 0;
}

int MItemPropertyModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return 2;
}

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
					if (index.column() == 0) // Если первая колонка, то выводим имя параметра
						return QVariant(pItem->name);
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
							case PropCombo:	 // Комбобокс
								return QVariant("-");
							break;
							case PropFiles:	 // Список имен файлов
							case PropList:	  // Список строк
							{
								QStringList temp = pItem->data.toStringList();
								QString info = QString("[%1]").arg(temp.size());
								//return QVariant(QString("[%1]").arg(temp.size()));
								//auto cellData = this->data(index, role);
								//auto item = this->index(index.row(), index.column());
								//QPushButton *cartButton = new QPushButton("+");
								//ui->table_view->setIndexWidget(item, cartButton);
								//tableView->setIndexWidget(model->index(position,COLUMN_NUMBER), helpButton);
								//emit setWidget(QPoint(index.row(), index.column()), cartButton);
								//emit setWidget(QPoint(), NULL);
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

bool validate(const QString &value)
{

	return true;
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
			int val = value.toInt(&ok);

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
	if (index.column() > 0)  // Если выбрана вторая колонка, то можно редактировать
		return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

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

void MItemPropertyModel::setList(MItemProperty *list)
{
	beginResetModel();
	pCurPropList = list;
	endResetModel();
}

PropItem *MItemPropertyModel::getProp(int32_t index)
{
	return pCurPropList->getProperty(index);
}

bool MItemPropertyModel::setFileList(int32_t index, QStringList data)
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
