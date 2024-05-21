#include <QFileDialog>
#include <QDir>
#include "mlisteditor.h"

MListEditor::MListEditor(QStringList data, bool isFiles, QWidget *parent) : QDialog(parent)
{
    list = new QListWidget(this);

    this->isFiles = isFiles;

    FillList(data);

    QVBoxLayout *layMain = new QVBoxLayout(this);
    QHBoxLayout *layKeys = new QHBoxLayout(this);
    QPushButton *btnOk = new QPushButton("Ok", this);
    QPushButton *btnDel = new QPushButton("-", this);
    QPushButton *btnAdd = new QPushButton("+", this);
    if(!isFiles)
    {
        edit = new QLineEdit(this);
        edit->setMaxLength(21);
    }

    connect(btnOk, &QPushButton::clicked, this, &MListEditor::btnOkPressed);
    connect(btnDel, &QPushButton::clicked, this, &MListEditor::btnDelPressed);
    connect(btnAdd, &QPushButton::clicked, this, &MListEditor::btnAddPressed);

    btnOk->setDefault(true);

    if(isFiles)
        layKeys->addStretch();
    else
        layKeys->addWidget(edit);
    layKeys->addWidget(btnAdd);
    layKeys->addWidget(btnDel);
    layKeys->addWidget(btnOk);
    layMain->addWidget(list);
    layMain->addLayout(layKeys);
    this->setLayout(layMain);
}

MListEditor::~MListEditor()
{
    delete list;
}

void MListEditor::btnOkPressed()
{
    QStringList data;
    for(int32_t i=0; i<list->count(); i++)
    {
        data += list->item(i)->text();
    }
    emit dialogResult(data);
    accept();
}

void MListEditor::btnAddPressed()
{
    if(isFiles)
    {
        QStringList fnames = QFileDialog::getOpenFileNames(this, "Выберите нужные файлы", "D:/git/hmi-prepare-gui/resources/icons");
        FillList(fnames);
    }
    else
    {
        list->addItem(edit->text());
        edit->clear();
    }
}

void MListEditor::btnDelPressed()
{
    auto ind = list->currentRow();
    list->takeItem(ind);
}

void MListEditor::FillList(QStringList items)
{
    list->addItems(items);
}
