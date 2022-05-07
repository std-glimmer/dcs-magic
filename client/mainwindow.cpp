#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "backendmanager.h"

MainWindow::MainWindow(QWidget *parent)
    :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    BackendManager::Instance(this)->connect("localhost", 9085, "Glimmer", "666");
}

MainWindow::~MainWindow()
{
    delete ui;
}

