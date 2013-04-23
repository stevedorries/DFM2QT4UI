#ifndef CLOGGER_H
#define CLOGGER_H

#include <QListView>
#include <QStandardItemModel>

class CLogger : public QListView
{
    Q_OBJECT

    QStandardItemModel *mLogModel;

public:
    explicit CLogger(QWidget *parent = 0);
    int init();

signals:

public slots:

};

#endif // CLOGGER_H
