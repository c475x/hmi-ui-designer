#ifndef MLISTEDITOR_H
#define MLISTEDITOR_H

#include <QWidget>
#include <QDialog>
#include <QListWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include "mtypes.h"

/**
 * @brief MListEditor - класс диалогового окна редактора строк
 */
class MListEditor : public QDialog
{
	Q_OBJECT
public:
	explicit MListEditor(QVariant data, PropType type, QWidget *parent = nullptr);
	~MListEditor();

protected:
	PropType type;
	QLineEdit *edit;
	QListWidget *list;
	QTableWidget *table;
	void FillList(QStringList items);
	void FillTable(QMap<QString, QStringList> items);

signals:
	void dialogResult(const QStringList &data);

protected slots:
	void btnOkPressed();
	void btnAddPressed();
	void btnDelPressed();
};

#endif // MLISTEDITOR_H
