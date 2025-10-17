#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <opencv.hpp>
#include <opencv2/face.hpp>
#include <vector>
#include <map>

using namespace cv;
using namespace cv::face;
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onTimeout();

    void on_pushButton_in_clicked();

    void on_pushButton_out_clicked();

private:
    void addLabels();//添加头像QLabel对象
    void initRecongnizer();//初始化人脸识别器
    void findFace();
    void showImage();//显示图像
    void studyFace(int id);
    int checkFace();//返回学生考号
    void randSeet(int id);
    bool repeatSeet(QLabel* label);
private:
    Ui::MainWindow *ui;
    cv::CascadeClassifier cc;
    Ptr<FaceRecognizer> recognizer;
    VideoCapture vc;//读取摄像头
    Mat frame;
    QTimer timer;
    vector<Rect> faces;
    //保存学生ID和座位信息
    map<int, QLabel*> stus;
};
#endif // MAINWINDOW_H
