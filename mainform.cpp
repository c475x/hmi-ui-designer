#include "mainform.h"
#include "ui_mainform.h"

#include <QFile>
#include <QtDebug>

MainForm::MainForm(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainForm)
{
	ui->setupUi(this);

	setWindowTitle("HMI UI Designer");
	setMaximumWidth(window()->width());

	qApp->installEventFilter(this);

	// Графическая сцена для отображения элементов рабочего экрана, которая отображается компонентом ui->screen
	scene = new MGraphicsScene();
	scene->resize(ui->screen->width(), ui->screen->height());
	guiModel = new MGuiItemsModel(scene);

	// Статусбар для отображения информационных сообщений
	status = new QLabel(this);
	ui->statusbar->addWidget(status, 100);

	// Сигнал clicked() заменен на pressed(), так как сигнал clicked() приводил к некорректному поведению при выборе элемента в списке
	// Если мышь была нажата на элементе, а отпущена вне - элемент выделялся, но не появлялся редактор его свойств
	connect(ui->listGuiItems, SIGNAL(pressed(QModelIndex)), this, SLOT(listItemClicked(QModelIndex)));

	// Обработка переименования элемента
	connect(ui->listGuiItems->itemDelegate(), &QAbstractItemDelegate::commitData, this, [this](QWidget *editor) {
		QModelIndex index = ui->listGuiItems->currentIndex();
		guiModel->renameItem(index.row(), editor);
	});

	propModel = new MItemPropertyModel(this);
	ui->tableItemProperties->setModel(propModel);
	ui->tableItemProperties->horizontalHeader()->hide();
	ui->tableItemProperties->verticalHeader()->hide();

	connect(propModel, SIGNAL(updateScene()), scene, SLOT(updateScene()));
	auto res = connect(propModel, &MItemPropertyModel::setWidget, this, &MainForm::setWidget);
	qDebug() << "Connect propModel to MainForm: " << res;
	res = connect(scene, &MGraphicsScene::newItem, guiModel, &MGuiItemsModel::newItem);
	qDebug() << "Connect scene to guiModel: " << res;
	res = connect(this, &MainForm::selectItem, guiModel, &MGuiItemsModel::selectItem);
	qDebug() << "Connect mainform to guiModel: " << res;

	setAcceptDrops(true);
	ui->screen->setAcceptDrops(true);

	ui->listGuiItems->setModel(guiModel);
	ui->screen->setScene(scene);

	guiIconsModel = new QStandardItemModel();
	guiIconsModel->appendRow(new QStandardItem(QIcon(":/icons/resources/icons/icon_GUI_icon.png"), "Иконка"));
	guiIconsModel->appendRow(new QStandardItem(QIcon(":/icons/resources/icons/icon_GUI_label.png"), "Надпись"));
	guiIconsModel->appendRow(new QStandardItem(QIcon(":/icons/resources/icons/icon_GUI_menu.png"), "Меню"));
	guiIconsModel->appendRow(new QStandardItem(QIcon(":/icons/resources/icons/icon_GUI_progress.png"), "Прогресс"));

	QVBoxLayout *lay = new QVBoxLayout(ui->framePalette);
	listPalette = new MListWidget(ui->framePalette);
	listPalette->setIconSize(QSize(64, 64));
	QListWidgetItem *item = new QListWidgetItem(QIcon(":/icons/resources/icons/icon_GUI_icon.png"), "Иконка", NULL, GuiIconSet);
	listPalette->addItem(item);
	item = new QListWidgetItem(QIcon(":/icons/resources/icons/icon_GUI_label.png"), "Надпись", NULL, GuiLabel);
	listPalette->addItem(item);
	item = new QListWidgetItem(QIcon(":/icons/resources/icons/icon_GUI_menu.png"), "Меню", NULL, GuiMenu);
	listPalette->addItem(item);
	item = new QListWidgetItem(QIcon(":/icons/resources/icons/icon_GUI_progress.png"), "Прогресс", NULL, GuiProgress);
	listPalette->addItem(item);
	lay->addWidget(listPalette);
	ui->framePalette->setLayout(lay);
}

MainForm::~MainForm()
{
	delete status;
	delete propModel;
	delete guiModel;
	delete scene;
	delete ui;
}

/**
 * @brief MainForm::eventFilter - функция отслеживает события [нажатий кнопок] мыши по элементам типа QTableView и QListView
 * @param obj - элемент, на котором произошло событие
 * @param event - тип события
 * @return - для дефолтной обработки
 */
bool MainForm::eventFilter(QObject *obj, QEvent *event)
{
	// Обработка кликов по списку объектов
	if (obj == ui->listGuiItems->viewport())
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent *ev = static_cast<QMouseEvent *>(event);
			if (ev->buttons() & Qt::LeftButton)
			{
				int32_t pos = ui->listGuiItems->indexAt(ev->pos()).row();
					emit selectItem(pos);
			}
		}
	}

	// Обработка кликов по таблице свойств
	if (obj == ui->tableItemProperties->viewport())
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent *ev = static_cast<QMouseEvent *>(event);
			if (ev->buttons() & Qt::RightButton)
			{
				// Получаем номер строки / номер свойства
				int32_t pos = ui->tableItemProperties->indexAt(ev->pos()).row();

				// Если у выбранного свойства тип "Список файлов"...
				if (propModel->getProp(pos)->type == PropFiles)
				{
					// Создаем объект формы диалога редактора
					MListEditor *pLe = new MListEditor(propModel->getProp(pos)->data.toStringList(), true);
					pLe->resize(640, 480);
					pLe->setModal(true);
					pLe->setWindowTitle("Редактор списка файлов");

					// Соединяем сигнал-слот для получения результата от формы диалога редактора
					connect(pLe, &MListEditor::dialogResult, this, &MainForm::dialogResult);

					// Вызываем редактор модально
					int res = pLe->exec();

					// Если нажата кнопка "Ok", то значит в переменной tempPropertyList уже
					// лежит новый список строк, сформированный редактором
					if (res)
					{
						// Сохраняем новый список файлов в свойство
						propModel->setFileList(pos, tempPropertyList);

						// Загружаем изображения иконок в графическое отображение
						QImage tempImage;

						// Получаем номер выбранного графического элемента в сцене
						int32_t ind = ui->listGuiItems->selectionModel()->selectedIndexes()[0].row();

						// Получаем указатель на графический объект иконки
						MIconSet *icon = (MIconSet *)guiModel->getItem(ind);

						// Очищаем старые иконки, чтобы не накапливались
						if (tempPropertyList.size())
						{
							icon->clearIcons();
						}

						// Перебираем список файлов
						foreach(QString fn, tempPropertyList)
						{
							tempImage.load(fn);		 // Подгружаем изображение в память
							icon->addIcon(tempImage);   // Сохраняем в объект набора иконок
						}
					}
				}

				// Если у выбранного свойства тип "Список строк"...
				if (propModel->getProp(pos)->type == PropList)
				{
					// Создаем объект формы диалога редактора
					MListEditor *pLe = new MListEditor(propModel->getProp(pos)->data.toStringList(), false);
					pLe->resize(640, 480);
					pLe->setModal(true);
					pLe->setWindowTitle("Редактор списка");

					// Соединяем сигнал-слот для получения результата от формы диалога редактора
					connect(pLe, &MListEditor::dialogResult, this, &MainForm::dialogResult);

					// Вызываем редактор модально
					int res = pLe->exec();

					// Если нажата кнопка "Ok", то значит в переменной tempPropertyList уже
					// лежит новый список строк, сформированный редактором
					if (res)
					{
						propModel->setFileList(pos, tempPropertyList);  // Сохраняем новый список файлов в свойство
					}
				}
			}
		}
	}

	return QObject::eventFilter(obj, event);
}

/**
 * @brief MainForm::showStatus - обновляет текст статусбара
 * @param text - текст статусы
 */
void MainForm::showStatus(QString text)
{
	status->setText(text);
}

/**
 * @brief MainForm::listItemClicked - слот обрабатывает клик по элементу списка
 * @param index - выбранный элемент
 */
void MainForm::listItemClicked(QModelIndex index)
{
	MBase *pItem = guiModel->getItem(index.row());
	if (pItem)
	{
		showStatus(QString("clicked on %1").arg(pItem->name));
		propModel->setList(pItem->props);
	}
}

/**
 * @brief MainForm::setWidget - слот для добавления в таблицу свойств виджетов из внешних классов
 * @param pos - ячейка таблицы
 * @param wgt - указатель на виджет
 */
void MainForm::setWidget(QPoint pos, QWidget *wgt)
{
	ui->tableItemProperties->setIndexWidget(propModel->index(pos.x(), pos.y()), wgt);
}

/**
 * @brief MainForm::dialogResult - слот вызывается, когда диалоговое окно редактирования списка возвращает новый список
 * @param data - новый список строк с именами файлов
 */
void MainForm::dialogResult(const QStringList &data)
{
	tempPropertyList = data;
}
