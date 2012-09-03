#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QVector<QString> generatorAcceptor(int count = 1);
    QVector<QString> generatorInitiator(int count = 1);
    QString generatorPair(QString key,QString vlaue);
    void createCfg(QString fname,QVector<QString>& cfg);
    void updateCfg(QString fname,QVector<QString>& cfg);


private slots:
    void on_cb_multiClient_clicked(bool checked);

    void on_cb_enableXML_clicked(bool checked);

    void on_pb_create_clicked();

    void on_cb_enableLog_clicked(bool checked);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
