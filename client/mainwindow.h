#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../lib/unitsmanager.h"

class ConnectionDialog;
class BackendManager;
class UDPReceiver;
class RecordsManager;
class UnitsModel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum Mode : int
    {
        Init,
        Translation,
        Track
    };
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Mode mode() const;
    void setMode(Mode newMode);

private slots:
    void connectButtonClicked();
    void openTrackButtonClicked();
    void updateUnitData(UnitsManager::UnitObjPtr unit, bool isNewUnit);
    void unitSelected(const QModelIndex& index);
    void clearMap();
    void setNightMapMode(bool isNightMode);

private:
    Ui::MainWindow *ui;

    Mode _mode = Mode::Init;

    ConnectionDialog* _connectionDialog = nullptr;

    BackendManager* _backendManager = nullptr;
    UnitsManager* _unitsManager = nullptr;
    UDPReceiver* _udpReceiver = nullptr;
    RecordsManager* _recordsManager = nullptr;

    UnitsModel* _unitsModel = nullptr;

    int _loginTimerId = 0;
};
#endif // MAINWINDOW_H
