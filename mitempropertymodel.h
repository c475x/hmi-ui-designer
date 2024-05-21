/*
 * Класс описывает свойства графических элементов
 *
 */
#ifndef MITEMPROPERTY_H
#define MITEMPROPERTY_H

#include <stdint.h>
#include <QAbstractTableModel>
#include <QVector>
#include <QPoint>
#include "mtypes.h"

// Структура свойства
typedef struct _PropItem
{
    _PropItem() {this->name = ""; this->type = PropString; this->data = QVariant();}
    _PropItem(QString name, PropType type, QVariant data) {this->name = name; this->type = type; this->data = data;}
    QString name;       // Имя свойства
    PropType type;      // Тип свойства
    QVariant data;      // Значение свойства
} PropItem;

/**
 * @brief The MItemProperty class - класс набора свойств определенного объекта
 */
class MItemProperty
{
public:
    MItemProperty(GuiType gui_type);
    ~MItemProperty();
    void addProperty(PropType type, QString name, QVariant data = QVariant());
    int32_t getSize() {return items.size();}
    PropItem *getProperty(int32_t index);
    void setProperty(int32_t index, QVariant value);

protected:
    GuiType gui_type;
    QVector<PropItem *> items;
};

/**
 * @brief The MItemPropertyModel class - класс модели отображения свойств объекта на экране
 */
class MItemPropertyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MItemPropertyModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {pCurPropList = NULL;}
    ~MItemPropertyModel() {}
    //void setGuiType(GuiType gui_type);
    //void setGuiType(QString str);
    void setPropCur(int32_t index, QVariant value);
    void setProp(GuiType gui, int32_t index, QVariant value);
    void setList(MItemProperty *list);
    PropItem *getProp(int32_t index);
    //QStringList getFullPropList();
    bool setFileList(int32_t index, QStringList data);

    //static QVector<MItemProperty *> glProperty;
    //static void initProperties();

protected:
    MItemProperty *pCurPropList;    // Указатель на отображаемый объект свойств
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

signals:
    void updateScene();
    void setWidget(QPoint pos, QWidget *wgt) const;
};

#endif // MITEMPROPERTY_H
