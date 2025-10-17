#include "MainWindow.h"
#include "ui_MainWindow.h"
#include"SubjectInfo.h"
#include<QLabel>
#include <QFile>
#include <QMessageBox>
#include"DBFactory.h"
#include"ExamDao.h"
#include"HistoryDialog.h"
#define FACE_MODEL_NAME "face.xml"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(time(0));
    addLabels();

    cc.load("haarcascade_frontalface_alt2.xml");
    initRecongnizer();

    connect(&timer, &QTimer::timeout, this, &MainWindow::onTimeout);
    timer.start(50);
    vc.open(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTimeout()
{
    vc >> frame;
    flip(frame, frame, 1);//翻转视频

    findFace();
    showImage();
}


void MainWindow::addLabels()
{
    int num=SubjectInfo::getInstance()->getNum();
    const int lineNum=8;
    for(int i=0;i<num;i++)
    {
        QLabel* label=new QLabel(this);//创建QLabel对象
        label->setMaximumSize(QSize(50, 50));
        label->setMinimumSize(QSize(50, 50));        
        label->setScaledContents(true);
        QPixmap pixmap(":/img.png");//资源文件中的图片文件都是以:开头
        label->setPixmap(pixmap);//显示图片
        int y=i/lineNum;
        int x=i%lineNum;
        ui->gridLayout->addWidget(label,y,x);
    }
}

void MainWindow::initRecongnizer()
{
    QFile file(FACE_MODEL_NAME);
    if(file.exists())
    {
        recognizer = FaceRecognizer::load<LBPHFaceRecognizer>("face.xml");
    }
    else
    {
        recognizer = LBPHFaceRecognizer::create();
    }

    recognizer->setThreshold(80);//设置阈值
}

void MainWindow::findFace()
{
    Mat gray;
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    //最后两个参数降低了搜索的轮次提升查找效率
    cc.detectMultiScale(gray, faces, 1.1, 5, 0, Size(300, 200), Size(200, 200));

    for(auto i : faces)
    {
        rectangle(frame, i, Scalar(0, 0, 255), 2);
    }
}

void MainWindow::showImage()
{
    Mat rgb;
    cvtColor(frame, rgb, COLOR_BGR2RGB);

    // 将OpenCV的Mat转换为QImage
    QImage img(rgb.data, rgb.cols, rgb.rows, rgb.cols * rgb.channels(), QImage::Format_RGB888);

    // 将QImage转换为QPixmap并按label大小缩放，保持纵横比
    QPixmap pixmap = QPixmap::fromImage(img);
    pixmap = pixmap.scaled(ui->label_show->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 设置label属性，确保图像能正确缩放显示
    ui->label_show->setScaledContents(true);  // 让内容适应label大小
    ui->label_show->setPixmap(pixmap);
}

void MainWindow::studyFace(int id)
{
    Mat face = frame(faces[0]);
    cvtColor(face, face, COLOR_BGR2GRAY);

    cv::resize(face, face, Size(128, 128));
    vector<Mat> studyFaces;

    studyFaces.push_back(face);
    vector<int> labels;
    labels.push_back(id);

    recognizer->update(studyFaces, labels);
    recognizer->save(FACE_MODEL_NAME);

}

int MainWindow::checkFace()
{
    if(recognizer->empty())
    {
        return -1;//人脸识别器为空不查找
    }
    Mat face=frame(faces[0]);
    cvtColor(face,face,COLOR_BGR2GRAY);
    cv::resize(face,face,Size(128,128));

    int label;
    double cfds;
    recognizer->predict(face,label,cfds);
    return label;
}

void MainWindow::randSeet(int id)
{
    QLabel* label;
    do
    {
        int pos=rand()%SubjectInfo::getInstance()->getNum();
        QWidget* w=ui->gridLayout->itemAt(pos)->widget();
        label=dynamic_cast<QLabel*>(w);
    }while(repeatSeet(label));//循环判断随即座位是否有人

    Mat face=frame(faces[0]);
    Mat rgb;
    cvtColor(face,rgb,COLOR_BGR2RGB);
    QImage img(rgb.data, rgb.cols, rgb.rows, rgb.cols * rgb.channels(), QImage::Format_RGB888);
    label->setPixmap(QPixmap::fromImage(img));
    //保存学生和作为的信息
    stus[id] = label;

}

bool MainWindow::repeatSeet(QLabel *label)
{
    for(auto i:stus)
    {
        if(i.second==label)
        {
            return true;
        }
    }
    return false;
}

void MainWindow::on_pushButton_in_clicked()
{
     QString strid = ui->lineEdit_StudentId->text();
     bool ok;
     int stuid = strid.toInt(&ok);
     if(!ok)
     {
         QMessageBox::warning(this, "警告", "请输入正确考号");
         return;
     }
     if(stus.count(stuid)>0)
     {
         QMessageBox::warning(this,"警告","该考生已经进入考场");
         return ;
     }
     if(faces.size() == 0)
     {
         QMessageBox::warning(this, "警告", "请摘掉口罩");
         return;
     }
      //人脸学习
     studyFace(stuid);
     //随机一个座位
     randSeet(stuid);
     ExamEntity ee;
     ee.subjectId=SubjectInfo::getInstance()->getSubjectId();
     ee.subjectName=SubjectInfo::getInstance()->getSubjectName();
     ee.stuid=stuid;
     DBFactory::getInstance()->getExamDao()->addHistory(ee);
}

void MainWindow::on_pushButton_out_clicked()
{
    //是否有人脸
    if(faces.size() == 0)
    {
        QMessageBox::warning(this, "警告", "请摘掉口罩");
        return;
    }
    int stuid=checkFace();
    if(stuid==-1)
    {
        QMessageBox::warning(this, "警告", "离开考场失败");
        return;
    }
    if(stus.count(stuid)==0)
    {
        QMessageBox::warning(this, "警告", "考场中没有你");
        return;
    }
    QLabel* label=stus[stuid];
    label->setPixmap(QPixmap(":/img.png"));
    stus.erase(stuid);//从数据中删除学生
    //后续这里有数据库
    ExamEntity ee;
    ee.subjectId=SubjectInfo::getInstance()->getSubjectId();
    ee.subjectName=SubjectInfo::getInstance()->getSubjectName();
    ee.stuid=stuid;
    DBFactory::getInstance()->getExamDao()->updateHistory(ee);

    QMessageBox::information(this, "成功", QString("考生%1已离开考场").arg(stuid));
}

