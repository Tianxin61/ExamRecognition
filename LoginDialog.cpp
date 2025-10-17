#include "LoginDialog.h"
#include "ui_LoginDialog.h"
#include"SubjectInfo.h"
#include"MainWindow.h"
#include"HistoryDialog.h"
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pushButton_start_clicked()
{
    QString subjectName=ui->lineEdit_subject->text();
    int subjectId=ui->spinBox_id->value();
    int num=ui->spinBox_num->value();
    //保存考场信息
    SubjectInfo::getInstance()->setSubjectName(subjectName);
    SubjectInfo::getInstance()->setSubjectId(subjectId);
    SubjectInfo::getInstance()->setNum(num);

    MainWindow* mw=new MainWindow;
    mw->show();
    close();
}

void LoginDialog::on_pushButton_history_clicked()
{
    HistoryDialog* hd=new HistoryDialog(this);
    hd->exec();
}
