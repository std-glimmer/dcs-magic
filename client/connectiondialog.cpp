#include "connectiondialog.h"
#include "ui_connectiondialog.h"

#include "backendmanager.h"

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog
                   | Qt::WindowTitleHint
                   | Qt::WindowStaysOnTopHint
                   | Qt::CustomizeWindowHint
                   | Qt::FramelessWindowHint
                   );

    connect(ui->connectButton, &QPushButton::clicked, this, [this] ()
    {
        ui->reporter->clear();

        if (getPassword().isEmpty() || getAddress().isEmpty() || getAddress().split(":").size() != 2)
        {
            ui->reporter->setText("Введите ip, порт и пароль.");
            return;
        }

        auto address = getAddress().split(":");

        if (BackendManager::Instance()->connectToHost(address.first(), address.last().toInt(), getPassword(), getCoalition()))
        {
            accept();
        }
        else
        {
            ui->reporter->setText("Ошибка подключения.");
        }
    });

    connect(ui->exitButton, &QPushButton::clicked, this, &QDialog::reject);
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

QString ConnectionDialog::getAddress() const
{
    return ui->ipLineEdit->text().trimmed();
}

QString ConnectionDialog::getPassword() const
{
    return ui->passwordLineEdit->text().trimmed();
}

Coalition ConnectionDialog::getCoalition() const
{
    switch(ui->comboBox->currentIndex())
    {
    case 1:
        return Coalition::Red;
        break;
    case 2:
        return Coalition::Blue;
        break;
    case 3:
        return Coalition::Neutral;
        break;
    }

    return Coalition::GameMaster;
}

void ConnectionDialog::clear()
{
    ui->ipLineEdit->clear();
    ui->passwordLineEdit->clear();

    ui->ipLineEdit->setText("127.0.0.1:9085");
    ui->passwordLineEdit->setText("1234");
}
