#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class UDPReceiver;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
     explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void printLog(const QString& line);
    void updatePasswords();

private:
    Ui::MainWindow *ui;

    UDPReceiver* _mainUDPReceiver = nullptr;
};

#endif // MAINWINDOW_H
