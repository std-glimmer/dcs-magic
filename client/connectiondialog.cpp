#include "connectiondialog.h"
#include "ui_connectiondialog.h"

#include "backendmanager.h"

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);

    connect(ui->connectButton, &QPushButton::clicked, this, [this] ()
    {
        ui->reporter->clear();

        if (getUsername().isEmpty() || getPassword().isEmpty() || getIP().isEmpty())
        {
            ui->reporter->setText("Введите ip, порт, имя пользователя и пароль.");
            return;
        }

        if (BackendManager::Instance()->connectToHost(getIP(), getPort(), getUsername(), getPassword(), getCoalition()))
        {
            accept();
        }
        else
        {
            ui->reporter->setText("Ошибка подключения.");
        }
    });
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

QString ConnectionDialog::getIP() const
{
    return ui->ipLineEdit->text().trimmed();
}

int ConnectionDialog::getPort() const
{
    return ui->portLineEdit->text().trimmed().toInt();
}

QString ConnectionDialog::getUsername() const
{
    return ui->userLineEdit->text().trimmed();
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
    ui->portLineEdit->clear();
    ui->userLineEdit->clear();
    ui->passwordLineEdit->clear();
}
