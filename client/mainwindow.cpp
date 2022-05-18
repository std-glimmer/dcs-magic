#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "backendmanager.h"
#include "connectiondialog.h"

#include <QGeoCoordinate>
#include <QUrl>
#include <QQuickItem>
#include <QDateTime>
#include <QFileDialog>

#include "../lib/unitsmanager.h"
#include "../lib/recordsmanager.h"
#include "unitsmodel.h"
#include "backendmanager.h"

MainWindow::MainWindow(QWidget *parent)
    :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _connectionDialog(new ConnectionDialog(this)),
    _backendManager(BackendManager::Instance(this)),
    _unitsManager(UnitsManager::Instance(this)),
    _recordsManager(RecordsManager::Instance(this)),
    _unitsModel(new UnitsModel(this))
{
    ui->setupUi(this);

//    ui->listView->setModel(_unitsModel);

    ui->quickWidget->setSource(QStringLiteral("qrc:/main.qml"));

    connect(_backendManager, &BackendManager::sendData, _unitsManager, &UnitsManager::receiveInfo);
    connect(_backendManager, &BackendManager::sendData, _recordsManager, &RecordsManager::receiveInfo);
    connect(_recordsManager, &RecordsManager::sendMessage, _unitsManager, &UnitsManager::receiveInfo);
    connect(_unitsManager, &UnitsManager::sendUnitData, this, &MainWindow::updateUnitData);
//    connect(ui->listView, &QListView::clicked, this, &MainWindow::unitSelected);
//    connect(ui->trackSlider, &QSlider::valueChanged, _recordsManager, &RecordsManager::selectMessage);

//    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectButtonClicked);
//    connect(ui->openTrackButton, &QPushButton::clicked, this, &MainWindow::openTrackButtonClicked);

    QMetaObject::invokeMethod(ui->quickWidget->rootObject(), "init");
    QMetaObject::invokeMethod(ui->quickWidget->rootObject(), "moveCenter",
                              Q_ARG(QVariant, QVariant::fromValue(QGeoCoordinate(43.21, 38.49))),
                              Q_ARG(QVariant, QVariant::fromValue(7)));

    _loginTimerId = startTimer(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::connectButtonClicked()
//{
//    clearMap();

//    if (_udpReceiver->isConnected())
//    {
//        _udpReceiver->stopListening();
//        _recordsManager->stopRecording();
//        ui->connectButton->setText("Подключиться");
//        ui->openTrackButton->setEnabled(true);
//    }
//    else
//    {
//        _udpReceiver->startListening();
//        _recordsManager->initNewRecord(QDateTime::currentDateTimeUtc().toString("hhmmssddMMyyyy"));
//        ui->connectButton->setText("Отключиться");
//        ui->openTrackButton->setEnabled(false);
//    }
//}

void MainWindow::openTrackButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Track", _recordsManager->getRecordsPath(), tr("Tracks (*.txt)"));

    if (QFile::exists(fileName))
    {
        _unitsManager->clear();
        _unitsModel->modelReset();
        clearMap();

        _recordsManager->loadRecord(fileName);

//        ui->trackSlider->blockSignals(true);
//        ui->trackSlider->setRange(0, _recordsManager->countLoadedRecordMessages());
//        ui->trackSlider->blockSignals(false);

//        emit ui->trackSlider->valueChanged(0);
    }
}

void MainWindow::updateUnitData(UnitsManager::UnitObjPtr unit, bool isNewUnit)
{
    if (unit.isNull())
    {
        return;
    }

    if (isNewUnit)
    {
        _unitsModel->modelReset();
    }

    QGeoCoordinate coordinate = QGeoCoordinate(unit->latitude(), unit->longitude(), unit->altitude());

    QMetaObject::invokeMethod(  ui->quickWidget->rootObject(), "updateUnitData",
                              Q_ARG(QVariant, QVariant::fromValue(unit->unitName())),
                              Q_ARG(QVariant, QVariant::fromValue(unit->unitTitle())),
                              Q_ARG(QVariant, QVariant::fromValue(unit->getImageSource())),
                              Q_ARG(QVariant, QVariant::fromValue(coordinate)));
}

void MainWindow::unitSelected(const QModelIndex &index)
{
    auto unit = _unitsModel->getUnit(index);

    QGeoCoordinate coordinate = QGeoCoordinate(unit->latitude(), unit->longitude(), unit->altitude());

    QMetaObject::invokeMethod(ui->quickWidget->rootObject(), "moveCenter",
                              Q_ARG(QVariant, QVariant::fromValue(coordinate)),
                              Q_ARG(QVariant, QVariant::fromValue(12)));
}

void MainWindow::clearMap()
{
    QMetaObject::invokeMethod( ui->quickWidget->rootObject(), "clearMap" );
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (_loginTimerId != event->timerId())
    {
        return;
    }

    _connectionDialog->clear();
    if (_connectionDialog->exec() == QDialog::Rejected)
    {
        _connectionDialog->close();
        qApp->exit();
    }
    else
    {
        killTimer(_loginTimerId);
    }
}
