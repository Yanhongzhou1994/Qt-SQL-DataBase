#ifndef CONNECTION_H
#define CONNECTION_H

#include "QMessageLogger"
#include "QtWidgets/QMessageBox"
#include "QSqlDatabase"
#include "QSqlQuery"
#include "QString"

static bool createConnection()
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName(":memory:");表明是写在内存里面，程序运行完后就没了
    db.setDatabaseName("database.db");//最后会写到硬盘上
    if(!db.open())
    {
        QMessageBox::critical(0,QString("cannot open database"),
                              QString("Unable to establisha database"
                                      "connection"),QMessageBox::Cancel);
        return false;
    }
    QSqlQuery query;//必须在连接上database driver之后才能创建query
    query.exec("create table student (id int primary key, "
               "name varchar(20))");
    query.exec("insert into student values(0, 'first')");
    query.exec("insert into student values(1, 'second')");
    query.exec("insert into student values(2, 'third')");
    query.exec("insert into student values(3, 'fourth')");
    query.exec("insert into student values(4, 'fifth')");
    return true;
}



#endif // CONNECTION_H
