#ifndef MLISTEDITOR_H
#define MLISTEDITOR_H

#include <QWidget>
#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

class MListEditor : public QDialog
{
    Q_OBJECT
public:
    explicit MListEditor(QStringList data, bool isFiles, QWidget *parent = nullptr);
    ~MListEditor();

protected:
    bool isFiles;
    QLineEdit *edit;
    QListWidget *list;
    void FillList(QStringList items);

signals:
    void dialogResult(const QStringList &data);

protected slots:
    void btnOkPressed();
    void btnAddPressed();
    void btnDelPressed();
};

#endif // MLISTEDITOR_H
