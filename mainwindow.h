#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDebug>
#include <QMainWindow>
#include <QLabel>
#include <termios.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void ShowConfigure();
private slots:
    void on_cbps_currentIndexChanged(const QString &arg1);

    void on_action_L_triggered();

    void dev_host(int val, QString addname);
    void on_send_clicked();
    void addtext(QString);
private:
    void GetConfigure();
    QStringList GetDev();
    Ui::MainWindow *ui;
    QString dev;
    QLabel *linkstatus;

    int bps;
    int databit;
    int checkbit;
    int stopbit;
    int flowctrl;
    bool link;
};
#endif // MAINWINDOW_H
