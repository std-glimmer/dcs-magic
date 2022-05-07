#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include "utils.h"

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectionDialog(QWidget *parent = nullptr);
    ~ConnectionDialog();

    QString getIP() const;
    int getPort() const;
    QString getUsername() const;
    QString getPassword() const;
    Coalition getCoalition() const;

    void clear();

private:
    Ui::ConnectionDialog *ui;
};

#endif // CONNECTIONDIALOG_H
