#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <Qt/qabstractsocket.h>
//#include <Qt/qtcpserver.h>
//#include <Qt/qtcpsocket.h>
#include <Qt/qstringlistmodel.h>

namespace Ui {
    class Dialog;

}
class QTcpSocket;
class QTcpServer;
enum WorkMode{
    undefined = 0,
    client = 1,
    server = 2
};

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QString generatorHeader(QString msg_type);
    QString generatorTrailer();
    int doBodyLength(QString &);
    int doCheckSum(QString &);
private slots:
    void on_pushButton_clicked();
    void readPacket();
    void OnConnected();
    void OnDisconnected();
    void OnnewConnection();
    void displayError(QAbstractSocket::SocketError);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pb_HeartBtGenerator_clicked();

    void on_pb_logonGenerator_clicked();

    void on_pb_TestRequestGenerator_clicked();

    void on_pb_LogoutGenerator_clicked();

    void on_pb_seqResetGenerator_clicked();

    void on_pb_RejectGenerator_clicked();

    void on_pb_resendGenerator_clicked();

    void on_cb_signatrueLength_clicked(bool checked);

    void on_cb_encodeTextLen_clicked(bool checked);

    void on_cb_text_clicked(bool checked);

    void on_cb_rawDataLength_clicked(bool checked);

    void on_cb_message_size_clicked(bool checked);

    void on_cb_testReqId_clicked(bool checked);

    void on_cb_refTagID_clicked(bool checked);

    void on_cb_refMsgtype_clicked(bool checked);

    void on_cb_sessionRejectReason_clicked(bool checked);

    void on_cb_encodeTextLen_2_clicked(bool checked);

    void on_cb_text_2_clicked(bool checked);


    void on_pushButton_5_clicked();

    void on_pb_startListen_clicked();

    void on_pb_stoplisten_clicked();

    void on_pb_generatorNewOrder_clicked();

private:
    Ui::Dialog *ui;
    QTcpSocket* socket;
    QTcpServer *fixServer;
    int senderMsgSeq;
    QStringList packets;
    QStringListModel    model;
    const static QString auto_bodyLength;//    =   "<AUTO_LENGTH>";
    const static QString auto_checkSum;// = "<AUTO_CHECKSUM>";
    WorkMode workMode;
};

#endif // DIALOG_H
