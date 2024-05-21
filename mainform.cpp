#include "mainform.h"
#include "ui_mainform.h"
#include <QFile>
#include <QtDebug>

MainForm::MainForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainForm)
{
    ui->setupUi(this);

    qApp->installEventFilter(this);

    scene = new MGraphicsScene();
    scene->resize(ui->screen->width(), ui->screen->height());
    guiModel = new MGuiItemsModel(scene);

    status = new QLabel(this);
    ui->statusbar->addWidget(status, 100);

    connect(ui->listGuiItems, SIGNAL(clicked(const QModelIndex)), this, SLOT(listItemClicked(QModelIndex)));

    propModel = new MItemPropertyModel(this);
	//propModel->initProperties();
    ui->tableItemProperties->setModel(propModel);
    ui->tableItemProperties->horizontalHeader()->hide();
    ui->tableItemProperties->verticalHeader()->hide();
/*TEMP+{*/
//    propModel->setProp(GuiIconSet, 0, 0);
//    propModel->setProp(GuiIconSet, 1, 0);
//    propModel->setProp(GuiLabel, 0, 0);
//    propModel->setProp(GuiLabel, 1, 0);
/*TEMP+}*/
    //propModel->setProperty()
	connect(propModel, SIGNAL(updateScene()), scene, SLOT(updateScene()));
    //connect(propModel, SIGNAL(setWidget(QPoint, QWidget *)), this, SLOT(setWidget(QPoint, QWidget *)));
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
    //guiIconsModel->eventFilter(this, QEvent::MouseButtonPress);

    //connect(guiIconsModel, SIGNAL(mouseMoveEvent(QGraphicsSceneMouseEvent *)), this, SLOT(onMouseMove(QGraphicsSceneMouseEvent *)));
    //connect(guiIconsModel, SIGNAL(mouseMoveEvent(QGraphicsSceneMouseEvent *)), this, SLOT(onMouseMove(QGraphicsSceneMouseEvent *)));

    //guiIconsModel->installEventFilter(this);
    //ui->listPalette->installEventFilter(this);
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
    //listPalette->addItem(item2);
    lay->addWidget(listPalette);
    ui->framePalette->setLayout(lay);
    //listPalette->insertItem(0, );
    //ui->listPalette->setModel(guiIconsModel);

    //ui->listPalette->setViewMode(QListView::IconMode);

/*
    // Иконка выходной мощности
    MIconSet *icon1 = new MIconSet(QPoint(0, 0), QSize(8*4, 32));
    QImage icon;
    icon.load("../../../hmi-prepare-gui/resources/icons/icon_power_low.png");
    icon1->addIcon(icon);
    icon.load("../../../hmi-prepare-gui/resources/icons/icon_power_hi.png");
    icon1->addIcon(icon);
    icon1->setCurrent(1);
    scene->addItem(icon1);
    guiModel->addGuiItem(icon1);

    // Иконка режима
    MIconSet *icon8 = new MIconSet(QPoint(10*4, 0), QSize(36*4, 32));
    icon.load("../../../hmi-prepare-gui/resources/icons/icon_mode_uhf_ff.png");
    icon8->addIcon(icon);
    icon8->setCurrent(0);
    scene->addItem(icon8);
    guiModel->addGuiItem(icon8);

    // Иконка изэрнета
    MIconSet *icon7 = new MIconSet(QPoint(48*4, 0), QSize(10*4, 32));
    icon.load("../../../hmi-prepare-gui/resources/icons/icon_ethernet.png");
    icon7->addIcon(icon);
    icon7->setCurrent(0);
    scene->addItem(icon7);
    guiModel->addGuiItem(icon7);

    // Иконка ППРЧ
    MIconSet *icon6 = new MIconSet(QPoint(60*4, 0), QSize(7*4, 32));
    icon.load("../../../hmi-prepare-gui/resources/icons/icon_fhss_idle.png");
    icon6->addIcon(icon);
    icon.load("../../../hmi-prepare-gui/resources/icons/icon_fhss_sync.png");
    icon6->addIcon(icon);
    icon6->setCurrent(0);
    scene->addItem(icon6);
    guiModel->addGuiItem(icon6);

    // Иконка гарнитуры
    MIconSet *icon5 = new MIconSet(QPoint(69*4, 0), QSize(4*4, 32));
    icon.load("../../../hmi-prepare-gui/resources/icons/icon_loudspk.png");
    icon5->addIcon(icon);
    icon5->setCurrent(0);
    scene->addItem(icon5);
    guiModel->addGuiItem(icon5);

    // Иконка ПЭВМ
    MIconSet *icon4 = new MIconSet(QPoint(75*4, 0), QSize(4*4, 32));
    icon.load("../../../hmi-prepare-gui/resources/icons/icon_pc.png");
    icon4->addIcon(icon);
    icon4->setCurrent(0);
    scene->addItem(icon4);
    guiModel->addGuiItem(icon4);

    // Иконка внешней периферии
    MIconSet *icon3 = new MIconSet(QPoint(80*4, 0), QSize(11*4, 32));
    icon.load("../../../hmi-prepare-gui/resources/icons/icon_ext_485.png");
    icon3->addIcon(icon);
    icon.load("../../../hmi-prepare-gui/resources/icons/icon_ext_um.png");
    icon3->addIcon(icon);
    icon3->setCurrent(0);
    scene->addItem(icon3);
    guiModel->addGuiItem(icon3);

    // Иконка батареи
    MIconSet *icon2 = new MIconSet(QPoint(93*4, 0), QSize(4*4, 32));
    icon.load("../../../hmi-prepare-gui/resources/icons/icon_battery_0.png");
    icon2->addIcon(icon);
    icon.load("../../../hmi-prepare-gui/resources/icons/icon_battery_1.png");
    icon2->addIcon(icon);
    icon.load("../../../hmi-prepare-gui/resources/icons/icon_battery_2.png");
    icon2->addIcon(icon);
    icon.load("../../../hmi-prepare-gui/resources/icons/icon_battery_3.png");
    icon2->addIcon(icon);
    icon2->setCurrent(1);
    scene->addItem(icon2);
    guiModel->addGuiItem(icon2);

    //MLabel *label1 = new MLabel(QPoint(392, 0), QSize(120, 32), "00:00");
    MLabel *label1 = new MLabel(QRectF(392, 0, 120, 32), "00:00");
    //MLabel *label1 = new MLabel(QPoint(0, 0), QSize(120, 32), "00:00");
    scene->addItem(label1);
    guiModel->addGuiItem(label1);

//    MProgress *progress = new MProgress(QRectF(0, 0, 512-120, 32));
//    progress->setValue(33);
//    scene->addItem(progress);

    MMenu *menu = new MMenu(QRectF(0, 64, 512, 160));
    menu->addMenuItem("Канал");
    menu->addMenuItem("Параметры");
    menu->addMenuItem("Информация");
    menu->addMenuItem("Внешняя периферия");
    menu->addMenuItem("Доступ");
    scene->addItem(menu);
    guiModel->addGuiItem(menu);
*/

    QList<QGraphicsItem *> a = scene->items();
    uint32_t b = a.length();
    asm("   nop");
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
    //if (obj == ui->listGuiItems->viewport() && event->type() == QEvent::MouseButtonPress)
    // Обработка кликов по списку объектов
    if(obj == ui->listGuiItems->viewport())
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *ev = static_cast<QMouseEvent *>(event);
            if (ev->buttons() & Qt::LeftButton)
            {
                int32_t pos = ui->listGuiItems->indexAt(ev->pos()).row();
//                if(pos >= 0)
                    emit selectItem(pos);
            }
        }
//        if(event->type() == QEvent::UpdateRequest)
//        {
//            asm("   nop");
//        }
    }
    // Обработка кликов по таблице свойств
    if(obj == ui->tableItemProperties->viewport())
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *ev = static_cast<QMouseEvent *>(event);
            if (ev->buttons() & Qt::RightButton)
            {
                // Получаем номер строки / номер свойства
                int32_t pos = ui->tableItemProperties->indexAt(ev->pos()).row();
                // Если у выбранного свойства тип "Список файлов"...
                if(propModel->getProp(pos)->type == PropFiles)
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
                    if(res)
                    {
                        propModel->setFileList(pos, tempPropertyList);  // Сохраняем новый список файлов в свойство
                        // Загружаем изображения иконок в графическое отображение
                        QImage tempImage;
                        // Получаем номер выбранного графического элемента в сцене
                        int32_t ind = ui->listGuiItems->selectionModel()->selectedIndexes()[0].row();
                        // Получаем указатель на графический объект иконки
                        MIconSet *icon = (MIconSet *)guiModel->getItem(ind);
                        // Очищаем старые иконки, чтобы не накапливались
                        if(tempPropertyList.size())
                            icon->clearIcons();
                        // Перебираем список файлов
                        foreach(QString fn, tempPropertyList)
                        {
                            tempImage.load(fn);         // Подгружаем изображение в память
                            icon->addIcon(tempImage);   // Сохраняем в объект набора иконок
                        }
                    }
                }
                // Если у выбранного свойства тип "Список строк"...
                if(propModel->getProp(pos)->type == PropList)
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
                    if(res)
                    {
                        propModel->setFileList(pos, tempPropertyList);  // Сохраняем новый список файлов в свойство
                    }
                }
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

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
    if(pItem)
    {
        //showStatus(QString("clicked on %1").arg(pItem->type));
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
    //ui->tableItemProperties->setIndexWidget(propModel->index(1, 1), wgt);
}

/**
 * @brief MainForm::dialogResult - слот вызывается, когда диалоговое окно редактирования списка возвращает новый список
 * @param data - новый список строк с именами файлов
 */
void MainForm::dialogResult(const QStringList &data)
{
    tempPropertyList = data;
}
