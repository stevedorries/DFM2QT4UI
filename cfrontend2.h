#ifndef CFRONTEND2_H
#define CFRONTEND2_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QDomDocument>
#include "formpreview.h"
#include "cdfmparser.h"

namespace Ui {
    class CFrontend2;
}

class CFrontend2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit CFrontend2(QWidget *parent = 0);
    ~CFrontend2();

    void setDfmFilePathList(QStringList list);
    void setRememberLastDfms(bool state);
    void loadLastDfms();
    void convert();
    void storeSettings();
    void loadStettings();

    CDfmParser *parser;


private:
    Ui::CFrontend2 *ui;
    FormPreview *mFormPreview;
    QStandardItemModel *mLogModel;
    QColor mPreviewBgColor;
    QString mLastDfmDirPath;
    bool mRememberLastDfms;

    void dumpDomDoc(QDomDocument *domDoc, const char *fileName);
    QStringList getListToConvert();
    void enablePreview(QString &filePath);
    void showPreview(QString fileName);


public slots:
    void logText(const QString &text);
    void parseFiles();
    void togleRememberLastDfms();

private slots:
    void on_listToConvert_doubleClicked(QModelIndex index);
    void on_btnPreview_clicked();
    void on_btnConvert_clicked();
    void on_listToConvert_itemSelectionChanged();
    void on_btnRemoveFile_clicked();
    void on_btnAddFile_clicked();
    void on_actionRemember_Last_Directory_triggered();
};

#endif // CFRONTEND2_H
