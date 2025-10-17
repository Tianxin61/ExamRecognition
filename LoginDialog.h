#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_pushButton_start_clicked();

    void on_pushButton_history_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
