#include "mysqlquerymodel.h"
#include "QSqlQuery"
#include "QColor"
#include "QString"

MySqlQueryModel::MySqlQueryModel(QObject *parent) : QSqlQueryModel(parent)
{

}

//返回表格是否可更改的标志
/*
Returns the item flags for the given index.
The base class implementation returns a combination of flags
 that enables the item (ItemIsEnabled) and allows it to
be selected (ItemIsSelectable).
 */
Qt::ItemFlags MySqlQueryModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags= QSqlQueryModel::flags(index);//返回一个flag
    if(index.column()==1)//如果是第二个属性，表示可以修改，只有主键key不能被更改
        flags|=Qt::ItemIsEditable;
    return flags;//我猜测应该是我们再处理这个单元格的时候自己触发了这个函数
}

//添加数据
/*
 [virtual] void QSqlQueryModel::clear()
 Clears the model and releases any acquired resource.
 */
bool MySqlQueryModel::setData(const QModelIndex& index,const QVariant& value,int /*value*/)
{
    if(index.column()<1||index.column()>2)//我们的数据只有两列
        return false;
    QModelIndex primaryKeyIndex=QSqlQueryModel::index(index.row(),0);//获取属性主键的index，主要看输入的index
    int id=data(primaryKeyIndex).toInt();//根据index获取data
    clear();//释放model申请的所有资源
    bool ok;
    if(index.column()==1)
        ok=setName(id,value.toString());
    refresh();
    return ok;
}

//刷新的时候需要调用的内容
void MySqlQueryModel::refresh()
{
    setQuery("select * from student");
    setHeaderData(0,Qt::Horizontal,QString("id"));
    setHeaderData(1,Qt::Horizontal,QString("name"));
}

bool MySqlQueryModel::setName(int studentId, const QString &name)
{
    QSqlQuery query;
    query.prepare("update student set name = ? where id = ?");
    query.addBindValue(name);
    query.addBindValue(studentId);
    return query.exec();
}

QVariant MySqlQueryModel::data(const QModelIndex &index, int role) const
{
    QVariant value=QSqlQueryModel::data(index,role);//暂时没有理解role，难道是显示模式？
    //第一个属性的字体颜色为红色
    if((role==Qt::TextColorRole)&&(index.column()==0))
    {
        return qVariantFromValue(QColor(Qt::red));//把QColor转换为QVariant
    }
    return value;
}
