#include "HistoryDialog.h"
#include "ui_HistoryDialog.h"
#include"DBFactory.h"
#include<vector>
#include<QFileDialog>
#include<QFile>
HistoryDialog::HistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistoryDialog)
{
    ui->setupUi(this);
    on_pushButton_update_clicked();
}

HistoryDialog::~HistoryDialog()
{
    delete ui;
}

void HistoryDialog::on_pushButton_update_clicked()
{
    vector<ExamEntity> v;
    DBFactory::getInstance()->getExamDao()->selectAll(v);
    for(auto i:v)
    {
        QString data;
        data+=QString().setNum(i.subjectId);
        data+=",";
        data+=i.subjectName;
        data+=",";
        data+=QString().setNum(i.stuid);
        data+=",";
        data+=i.enterTime.toString();
        data+=",";
        data+=i.leaveTime.toString();

        ui->textBrowser_history->append(data);
    }
}

void HistoryDialog::on_pushButton_export_clicked()
{
    QString path=QFileDialog::getExistingDirectory(this,"选择导出路径",".");
    if(path.isEmpty())
    {
        return ;
    }
    //根据当前时间起一个名字
    QFile file(path+"/"+QDateTime::currentDateTime().toString()+".txt");
    file.open(QIODevice::WriteOnly);//打开文件

    vector<ExamEntity> v;
    DBFactory::getInstance()->getExamDao()->selectAll(v);
    for(auto i:v)
    {
        QString data;
        data+=QString().setNum(i.subjectId);
        data+=",";
        data+=i.subjectName;
        data+=",";
        data+=QString().setNum(i.stuid);
        data+=",";
        data+=i.enterTime.toString();
        data+=",";
        data+=i.leaveTime.toString();
        data+="\n";//每条数据占一行
        file.write(data.toUtf8());
    }
    file.close();
}
