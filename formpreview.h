#ifndef FORMPREVIEW_H
#define FORMPREVIEW_H

#include <QWidget>
#include <QtUiTools>

namespace Ui {
    class FormPreview;
}

class FormPreview : public QWidget
{
    Q_OBJECT

public:
    explicit FormPreview(QWidget *parent = 0);
    ~FormPreview();

    int loadUiFile(QString &aFileName, QString *aErrorMsg);

private:
    Ui::FormPreview *ui;
    QWidget *mLoadWidget;
};

#endif // FORMPREVIEW_H
