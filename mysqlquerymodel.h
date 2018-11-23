#ifndef MYSQLQUERYMODEL_H
#define MYSQLQUERYMODEL_H

#include <QObject>
#include "QModelIndex"
#include "QVariant"
#include "QSqlQueryModel"

class MySqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit MySqlQueryModel(QObject *parent = nullptr);
    Qt::ItemFlags flags(const QModelIndex& index) const;
    bool setData(const QModelIndex& index,const QVariant& value,int role);
    QVariant data(const QModelIndex& item,int role=Qt::DisplayRole) const;
private:
    bool setName(int studentId,const QString& name);
    void refresh();
signals:

public slots:
};

#endif // MYSQLQUERYMODEL_H
