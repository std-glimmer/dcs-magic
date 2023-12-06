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
    connect(ui->trackSlider, &QSlider::valueChanged, _recordsManager, &RecordsManager::selectMessage);

//    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectButtonClicked);

    connect(ui->connectButton, &QToolButton::clicked, ui->act_connect, &QAction::triggered);
    connect(ui->act_connect, &QAction::triggered, this, &MainWindow::connectButtonClicked);

    connect(ui->act_disconnect, &QAction::triggered, this, [this] () {
        _connectionDialog->disconnectFromServer();
    });

    connect(ui->act_trackStorage, &QAction::triggered, this, [this] () {
        _connectionDialog->disconnectFromServer();
    });

    connect(ui->act_nightMode, &QAction::triggered, this, &MainWindow::setNightMapMode);

    connect(BackendManager::Instance(), &BackendManager::disconnectedFromServer, this, [this] () {
        setMode(Mode::Init);
    });

    connect(ui->loadTrack, &QToolButton::clicked, this, &MainWindow::openTrackButtonClicked);

    QMetaObject::invokeMethod(ui->quickWidget->rootObject(), "init");
    QMetaObject::invokeMethod(ui->quickWidget->rootObject(), "moveCenter",
                              Q_ARG(QVariant, QVariant::fromValue(QGeoCoordinate(36.186267,37.597433))),
                              Q_ARG(QVariant, QVariant::fromValue(18)));

    ui->act_nightMode->setChecked(true);

    ui->stackedWidget->setCurrentIndex(Mode::Translation);


    QGeoCoordinate coordinate = QGeoCoordinate(36.186267,37.597433);
    QString testTitle = "test";
    QString imageSource = "qrc:///resources/images/test/test_rdr.png";

    QMetaObject::invokeMethod(  ui->quickWidget->rootObject(), "addRadarImage",
                              Q_ARG(QVariant, QVariant::fromValue(testTitle)),
                              Q_ARG(QVariant, QVariant::fromValue(testTitle)),
                              Q_ARG(QVariant, QVariant::fromValue(imageSource)),
                              Q_ARG(QVariant, QVariant::fromValue(coordinate)));

    testTitle = "test1";
    imageSource = "qrc:///resources/images/units/unknown.png";

    QMetaObject::invokeMethod(  ui->quickWidget->rootObject(), "updateUnitData",
                              Q_ARG(QVariant, QVariant::fromValue(testTitle)),
                              Q_ARG(QVariant, QVariant::fromValue(testTitle)),
                              Q_ARG(QVariant, QVariant::fromValue(imageSource)),
                              Q_ARG(QVariant, QVariant::fromValue(coordinate)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectButtonClicked()
{
    clearMap();

    _connectionDialog->clear();
    if (_connectionDialog->exec() == QDialog::Accepted)
    {
        setMode(Mode::Translation);
    }
    else
    {
        setMode(Mode::Init);
    }
}

void MainWindow::openTrackButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть запись", _recordsManager->getRecordsPath(), tr("Tracks (*.txt)"));

    if (QFile::exists(fileName))
    {
        _unitsManager->clear();
        _unitsModel->modelReset();
        clearMap();

        _recordsManager->loadRecord(fileName);

        ui->trackSlider->blockSignals(true);
        ui->trackSlider->setRange(0, _recordsManager->countLoadedRecordMessages());
        ui->trackSlider->blockSignals(false);

        setMode(Mode::Track);
        emit ui->trackSlider->valueChanged(0);
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

void MainWindow::setNightMapMode(bool isNightMode)
{
    QMetaObject::invokeMethod(ui->quickWidget->rootObject(), "setNightMode",
                              Q_ARG(QVariant, QVariant::fromValue(isNightMode)));
}

MainWindow::Mode MainWindow::mode() const
{
    return _mode;
}

void MainWindow::setMode(Mode newMode)
{
    switch(_mode = newMode)
    {
    case Mode::Init:
        ui->stackedWidget->setCurrentIndex(0);
        ui->trackSlider->hide();
        break;
    case Mode::Translation:
        ui->stackedWidget->setCurrentIndex(1);
        ui->trackSlider->hide();
        break;
    case Mode::Track:
        ui->stackedWidget->setCurrentIndex(1);
        ui->trackSlider->show();
        break;
    }
}
