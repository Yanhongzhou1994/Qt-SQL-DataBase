#include "mainwindow.h"
#include <QApplication>
#include "connection.h"
#include "connection_2.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!createConnection())
    {
        return 1;
    }
    if(!createConnection_2())
    {
        return 1;
    }
    MainWindow w;
    w.show();

    return a.exec();
}
