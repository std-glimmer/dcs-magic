#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectionmanager.h"
#include "databasemanager.h"
#include "authmanager.h"
#include "udpreceiver.h"
#include "../lib/unitsmanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)//,
    //_mainUDPReceiver(new UDPReceiver(this, true))
{
    ui->setupUi(this);

    DatabaseManager::Instance(this);
    AuthManager::Instance(this);
    ConnectionManager::Instance(this);
    UnitsManager::Instance(this);

    UDPReceiver::Instance(this)->startListening();

    connect(ConnectionManager::Instance(), &ConnectionManager::printLog, this, &MainWindow::printLog);
    connect(AuthManager::Instance(), &AuthManager::updated, this, &MainWindow::updatePasswords);
    emit AuthManager::Instance()->updated();
}

MainWindow::~MainWindow()
{
    UDPReceiver::Instance()->stopListening();
    delete ui;
}

void MainWindow::printLog(const QString &message)
{
    ui->logsTextEdit->appendPlainText(message);
}

void MainWindow::updatePasswords()
{
    for (auto& i : AuthManager::Instance()->passwords())
    {
        switch(i.coalition)
        {
        case Coalition::GameMaster:
            ui->gameMasterLineEdit->setText(i.password);
            break;
        case Coalition::Red:
            ui->redLineEdit->setText(i.password);
            break;
        case Coalition::Blue:
            ui->blueLineEdit->setText(i.password);
            break;
        default:
            break;
        }
    }
}

