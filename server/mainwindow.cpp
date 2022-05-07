#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectionmanager.h"

#include "../lib/unitsmanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ConnectionManager::Instance(this);
    UnitsManager::Instance(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

