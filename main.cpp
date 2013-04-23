#include "cfrontend2.h"

#include <QtGui>
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QStringList dfmFileList;
    QApplication a(argc, argv);


    CFrontend2 w;
    w.show();
    dfmFileList = a.arguments();
    if(dfmFileList.count() > 1)  // more than the app path?
    {
        dfmFileList.removeAt(0);
        w.setDfmFilePathList(dfmFileList);
        w.convert();
    }
    else
        w.loadLastDfms();

    return a.exec();
}
