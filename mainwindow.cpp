#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QSqlQuery"
#include "QDebug"
#include "QSqlRecord"
#include "QString"
#include "QSqlError"
#include "QSqlQueryModel"
#include "QTableView"
#include "mysqlquerymodel.h"
#include "QSqlRelationalDelegate"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //relation table 与ralation database,感受一下
    model=new QSqlRelationalTableModel(this);//this，父窗口指针

    //OnFiledChange
    //All changes to the model will
    //be applied immediately to the database.
    //表格的变化会立即修改database
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setTable("student");//和student table绑定

    //这里course代表外键所代表主健的table,这里的"id"表示model代表table的"属性"
    //这里"name"表示外键table的属性"name"
    model->setRelation(2,QSqlRelation("course","id","name"));
    model->setHeaderData(0,Qt::Horizontal,QString("ID"));
    model->setHeaderData(1,Qt::Horizontal,QString("Name"));

    //前两个都好理解，其实这里也是一样，因为每个tbale都需要一个元组，所以
    //"course"不代表元组，只是第三列属性的一个别名
    //这个别名会作为Header Data
    //但是第三列的值已经被替换了
    model->setHeaderData(2,Qt::Horizontal,QString("Course"));

    model->select();//这里完成索引

    //把model需要处理的事务处理完了，开始装载
    ui->tableView->setModel(model);
    //Qt提供了一个委托类，对于委托这个概念我理解不深，欢迎讨论
    //它为QSqlRelationTableModel显示和编辑提供数据
    //这个委托为一个外键提供了一个QComboBox部件来显示所有可选的数据
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));

    ui->pushButton_2->setDisabled(true);
    ui->pushButton_3->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QSqlQuery query;

    //不加变量
    query.exec("select * from student");
    qDebug()<<"exec next()";
    if(query.next())
    {
        //获取query所指向的记录在结果集中的编号
        int rowNum=query.at();//at()query所指向的record在result中的编号
        //获取每条记录中属性（即列）的个数
        int columnNum=query.record().count();
        //获取"name"属性所在列的编号，列从左向右编号，最左边编号为0
        int fieldNo=query.record().indexOf("name");//通过属性名获取相应编号
        //获取id的属性值，并转换为int型
        int id=query.value(0).toInt();//value()表示查找的第n个属性
        //获取name属性的值
        QString name=query.value(fieldNo).toString();

        qDebug()<<"rowNum is:"<<rowNum
               <<"id is:"<<id
              <<"name is:"<<name
             <<"columnNum is:"<<columnNum;
    }
    //定位到结果集中编号为2的记录，即第三条记录，因为第一条记录的编号为0
    qDebug()<<"exec seek(2):";
    if(query.seek(2))
    {
        qDebug()<<"row Num is:"<<query.at()
             <<"id is:"<<query.value(0).toInt()
            <<"name is :"<<query.value(1).toString();
    }
    //定位到结果的最后一条
    qDebug()<<"exec last()";
    if(query.last())
    {
        qDebug()<<"rowNum is:"<<query.at()
               <<"id is:"<<query.value(0).toInt()
              <<"name is :"<<query.value(0).toString();
    }
    //在query语句中添加变量
    int id=ui->spinBox->value();
    query.exec(QString("select name from student where id =%1").arg(id));
    query.next();
    QString name=query.value(0).toString();//这里只获取了name
    qDebug()<<name;

    //在query里面添加占位符
    query.prepare("insert into stdent(id,name)""values(:id,:name)");
    //这是odbc的占位写法，还有oracle的写法“？” values(?,?)
    query.bindValue(0,5);
    query.bindValue(1,"sixth");
    query.exec();//一定要执行exec()
    query.exec("select * from student");
    query.last();
    id=query.value(0).toInt();
    name=query.value(1).toString();
    qDebug()<<id<<name;
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);

}

void MainWindow::on_pushButton_2_clicked()
{
    QSqlQuery q;
    q.prepare("insert into student values (?,?)");
    QVariantList ints;
    ints<<10<<11<<12<<13;//注意这种赋值方式
    q.addBindValue(ints);//注意这种按顺序赋值的方式
    QVariantList names;
    names<<"xiaoming"<<"xiaoliang"<<"xiaogang"<<QVariant(QVariant::String);//最后一个是空字符串
    q.addBindValue(names);
    if(!q.execBatch())//这里进行批处理
        qDebug()<<q.lastError();
    //下面输出整张表
    QSqlQuery query;
    query.exec("select * from student");
    while(query.next())
    {
        int id=query.value(0).toInt();
        QString name=query.value(1).toString();
        qDebug()<<id<<name;
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    //QSqlQueryModel模型默认是只读的，所以在生成的表格中是不能进行修改的
    QSqlQueryModel *model=new QSqlQueryModel;
    model->setQuery("select * from student");//注意这里没有exec()
    model->setHeaderData(0,Qt::Horizontal,QString("id"));//直接设置显示的属性的position
    model->setHeaderData(1,Qt::Horizontal,QString("name"));
    QTableView *view=new QTableView;
    view->setModel(model);
    view->show();

    int column=model->columnCount();//获得列数
    int row=model->rowCount();//获得行数
    QSqlRecord record=model->record(1);//获得第一条记录
    QModelIndex index=model->index(1,1);//获得一条记录的一个属性值
    qDebug()<<"column num is:"<<column<<endl
           <<"row num is:"<<row<<endl
          <<"the second record is:"<<record<<endl//注意一下这种输出方式
         <<"the data of index(1,1) is:"<<index.data();

    MySqlQueryModel* myModel=new MySqlQueryModel;
    myModel->setQuery("select * from student");
    myModel->setHeaderData(0,Qt::Horizontal,QString("id"));
    myModel->setHeaderData(1,Qt::Horizontal,QString("name"));
    QTableView* view1=new QTableView;
    view1->setModel(myModel);
    view1->show();
}
