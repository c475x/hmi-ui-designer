#include <QFileDialog>
#include <QDir>

#include "mlisteditor.h"

/**
 * @brief convertToMap - конвертирует данные типа QVariant в QMap<QString, QStringList> (для комбо)
 * @param data - данные
 * @return - данные, приведенные к типу QMap<QString, QStringList>
 */
QMap<QString, QStringList> convertToMap(const QVariant &data)
{
	QMap<QString, QStringList> dataMap;

	if (data.canConvert<QVariantMap>())
	{
		QMap<QString, QVariant> variantMap = data.toMap();
		for (auto it = variantMap.begin(); it != variantMap.end(); ++it)
		{
			QStringList stringList;
			if (it.value().canConvert<QStringList>())
			{
				stringList = it.value().toStringList();
			}
			else if (it.value().canConvert<QString>())
			{
				stringList.append(it.value().toString());
			}
			dataMap.insert(it.key(), stringList);
		}
	}

	return dataMap;
}

/**
 * @brief MListEditor::MListEditor - создает диалоговое окно редактирования списка строк
 * @param data - данные
 * @param isFiles - являются ли данные путями
 * @param parent - родитель
 */
MListEditor::MListEditor(QVariant data, PropType type, QWidget *parent) : QDialog(parent)
{
	this->type = type;

	if (type == PropCombo)
	{
		table = new QTableWidget(this);
		table->setColumnCount(2);
		table->setHorizontalHeaderLabels(QStringList() << "Название" << "Строки");
		FillTable(convertToMap(data));
	}
	else
	{
		list = new QListWidget(this);
		FillList(data.toStringList());
	}

	QVBoxLayout *layMain = new QVBoxLayout(this);
	QHBoxLayout *layKeys = new QHBoxLayout(this);
	QPushButton *btnOk = new QPushButton("OK", this);
	QPushButton *btnDel = new QPushButton("-", this);
	QPushButton *btnAdd = new QPushButton("+", this);

	connect(btnOk, &QPushButton::clicked, this, &MListEditor::btnOkPressed);
	connect(btnDel, &QPushButton::clicked, this, &MListEditor::btnDelPressed);
	connect(btnAdd, &QPushButton::clicked, this, &MListEditor::btnAddPressed);

	btnOk->setDefault(true);

	if (type != PropFiles)
	{
		edit = new QLineEdit(this);
		edit->setMaxLength(21);
		layKeys->addWidget(edit);
	}
	else
	{
		layKeys->addStretch();
	}

	layKeys->addWidget(btnAdd);
	layKeys->addWidget(btnDel);
	layKeys->addWidget(btnOk);

	if (type == PropCombo)
	{
		layMain->addWidget(table);
	}
	else
	{
		layMain->addWidget(list);
	}

	layMain->addLayout(layKeys);
	this->setLayout(layMain);
}

MListEditor::~MListEditor()
{
	delete list;
}

/**
 * @brief MListEditor::btnOkPressed - ивент нажатия кнопки "OK"
 */
void MListEditor::btnOkPressed()
{
	QStringList data;
	for (int32_t i = 0; i < list->count(); i++)
	{
		data += list->item(i)->text();
	}

	emit dialogResult(data);
	accept();
}

/**
 * @brief MListEditor::btnAddPressed - ивент нажатия кнопки "+"
 */
void MListEditor::btnAddPressed()
{
	if (type == PropFiles)
	{
		QStringList fnames = QFileDialog::getOpenFileNames(this, "Выберите нужные файлы");
		FillList(fnames);
	}
	else
	{
		// Проверяем, не пустая ли строка
		if (edit->text().length() > 0)
		{
			list->addItem(edit->text());
		}

		edit->clear();
	}
}

/**
 * @brief MListEditor::btnDelPressed - ивент нажатия кнопки "-"
 */
void MListEditor::btnDelPressed()
{
	auto ind = list->currentRow();
	list->takeItem(ind);
}

/**
 * @brief MListEditor::FillList - функция заполнения списка новыми элементами
 * @param items - записываемые элементы
 */
void MListEditor::FillList(QStringList items)
{
	list->addItems(items);
}

/**
 * @brief MListEditor::FillTable - функция заполнения таблицы новыми элементами (для комбо)
 * @param items - записываемые элементы
 */
void MListEditor::FillTable(QMap<QString, QStringList> items)
{
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		int row = table->rowCount();
		table->insertRow(row);
		table->setItem(row, 0, new QTableWidgetItem(it.key()));
		table->setItem(row, 1, new QTableWidgetItem(it.value().join(", ")));
	}
}
