#include "dialog.h"
#include "ui_dialog.h"
#include "Qt/QTcpSocket.h"
#include "Qt/qtcpserver.h"
#include "QtGui/qmessagebox.h"
#include "Qt/qthread.h"
#include "Qt/qdatetime.h"
#include <iostream>
#include "field.h"
#include "QtGlobal"
const  QString Dialog::auto_bodyLength    =   "<AUTO_LENGTH>";
const  QString Dialog::auto_checkSum = "<AUTO_CHECKSUM>";
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    packets(),
    model(packets)
{
    ui->setupUi(this);
    ui->tabWidget->setTabText(0,"Logon");
    ui->tabWidget->setTabText(1,"Heartbeat");
    ui->tabWidget->setTabText(2,"Resend Request");
    ui->tabWidget->setTabText(3,"Reject");
    ui->tabWidget->setTabText(4,"Sequence Reset");
    ui->tabWidget->setTabText(5,"Test Request");
    ui->tabWidget->setTabText(6,"Logout");
    ui->tabWidget->setTabText(7,"New Order - Single");
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_3->setDisabled(true);
    ui->pb_stoplisten->setDisabled(true);
    senderMsgSeq    =   0;
    ui->recvFixLst->setModel(&model);
    fixServer = NULL;
    socket = NULL;
    workMode = undefined;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    QString addr = ui->lineEdit->text();
    QString port = ui->lineEdit_2->text();
    socket = new QTcpSocket(this);
    socket->connectToHost(addr,port.toShort());
    connect(socket, SIGNAL(readyRead()), this, SLOT(readPacket()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(connected()),this,SLOT(OnConnected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(OnDisconnected()));
    ui->pushButton->setDisabled(true);
    ui->pb_startListen->setDisabled(true);
    workMode = client;
}

void Dialog::readPacket()
{
    QString recv = socket->readAll();;
    recv.replace("\001","<SOH>");
    packets.append(recv);
    model.setStringList(packets);
    QModelIndex index = model.index(packets.size() - 1);
    ui->recvFixLst->setCurrentIndex(index);
}

void Dialog::on_pushButton_2_clicked()
{
    std::clog<<"Send Text"<<std::endl;
    QString text = ui->textEdit->toPlainText();
    text.replace("<SOH>","\001");
    std::string dat = text.toStdString();
    int ret = socket->write(dat.data(),dat.size());
    if(ret == -1){
        QMessageBox::critical(this,
                              "FIX Packet Sender"
                              ,"Write Error");
    }
    //sprintf(buffer,"wirte ret %d ",ret);
    //QMessageBox::information(this,tr("info"),tr(buffer) + text);
}

void Dialog::displayError(QAbstractSocket::SocketError socketError)
{
    if(workMode == client){
        ui->pushButton->setDisabled(false);
        ui->pb_startListen->setDisabled(false);
    }
    switch (socketError) {
       case QAbstractSocket::RemoteHostClosedError:
           break;
       case QAbstractSocket::HostNotFoundError:
           QMessageBox::information(this, tr("FIX Packet Sender"),
                                    tr("The host was not found. Please check the "
                                       "host name and port settings."));
           break;
       case QAbstractSocket::ConnectionRefusedError:
           QMessageBox::information(this, tr("FIX Packet Sender"),
                                    tr("The connection was refused by the peer. "
                                       "Make sure the fortune server is running, "
                                       "and check that the host name and port "
                                       "settings are correct."));
           break;
       default:
           QMessageBox::information(this, tr("FIX Packet Sender"),
                                    tr("The following error occurred: %1.")
                                    .arg(socket->errorString()));
    }

}

void Dialog::OnConnected()
{
    workMode = client;
    ui->pushButton->setDisabled(true);
    ui->pushButton_2->setDisabled(false);
    ui->pushButton_3->setDisabled(false);
    ui->pb_startListen->setDisabled(true);
}

void Dialog::on_pushButton_3_clicked()
{
    socket->disconnectFromHost();
    ui->pushButton->setDisabled(false);
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_3->setDisabled(true);
}
//Send
void Dialog::on_pushButton_4_clicked()
{
    QString fix = ui->textEdit->toPlainText();
    fix.replace("<SOH>","\001");
    int cs = Field::checkSum(fix);
    int bl = Field::bodyLength(fix);
    char buffer[BUFSIZ] = {0};
    if(cs <= 0 ||
            bl <= 0){
        QMessageBox::critical(this,
                              tr("Fix Packet Sender")
                              ,tr("Invalid Message"));
        return  ;
    }
    sprintf(buffer,"BodyLength : %d ,CheckSum : %d ",bl,cs%256);
    QMessageBox::information(this,
                             tr("FIX Packet Sender"),
                             tr(buffer));
}
/*
int Dialog::checkSum(QString fixData)
{
    int checkSum_b = fixData.indexOf("10=");
    int checkSum_e = fixData.indexOf("\001",checkSum_b);
    fixData.replace(checkSum_b,(checkSum_e - checkSum_b) + 1,"");
    std::string fix = fixData.toStdString();//.c_str();
    const unsigned char* iter =
          reinterpret_cast<const unsigned char*>( fix.data() );
    int m_total = std::accumulate( iter,iter + fix.size(), 0 );

    return  m_total;
}

int Dialog::bodyLength(QString fixData)

{
    int beginString_b = fixData.indexOf("8=");
    int beginString_e = fixData.indexOf("\001",beginString_b);
    fixData.replace(beginString_b,(beginString_e - beginString_b) + 1,"");
    int bodyLength_b = fixData.indexOf("9=");
    int bodyLength_e = fixData.indexOf("\001",bodyLength_b);
    fixData.replace(bodyLength_b,(bodyLength_e - bodyLength_b) + 1,"");
    int checkSum_b = fixData.indexOf("10=");
    int checkSum_e = fixData.indexOf("\001",checkSum_b);
    fixData.replace(checkSum_b,(checkSum_e - checkSum_b) + 1,"");
    if(beginString_b == -1 ||
            beginString_e == -1
            || bodyLength_b == -1
            || bodyLength_e == -1
            || checkSum_b == -1
            || checkSum_e == -1){
        QMessageBox::critical(this,tr("fix packet sender"),
                              "Invalid Fix Data :"+ fixData);
        return  -1;
    }
    return  fixData.length();
}
*/
void Dialog::OnDisconnected()
{
    if(workMode == client){
        QMessageBox::information(this,
                                 "Fix Packet Sender",
                                 "The Server Disconnect the Connection");
        ui->pushButton->setDisabled(false);
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_3->setDisabled(true);
        ui->pb_startListen->setDisabled(false);
    }
    else if(workMode == server){
        QMessageBox::information(this,
                                 tr("Fix Packet Sender"),
                                 tr("Connection Disconnected"));
        ui->pushButton_2->setDisabled(true);
    }
    else {

    }
    //workMode = undefined;

}

void Dialog::on_pb_HeartBtGenerator_clicked()
{
    const char* Msg_Type = "0";
    QString header = generatorHeader(Msg_Type);
    QString trailer = generatorTrailer();
    if(header.length() == 0||
            trailer.length() == 0){
        return  ;
    }
    bool enableTestReqID = ui->cb_testReqId->isChecked();
    QString body = header;
    if(enableTestReqID){
        QString testReqID = ui->le_testReqID->text();
        if(testReqID.length() == 0){
            QMessageBox::critical(this,
                                  tr("FIX Packet Sender"),
                                  tr("HeartBeat : TestReqID field can't be emtpy"));
            return  ;
        }
        body += Field::generatorField(Field::testReqID,testReqID);
    }
    else {

    }
    body += trailer;
    doBodyLength(body);
    doCheckSum(body);
    ui->textEdit->setText(body);
}

void Dialog::on_pb_logonGenerator_clicked()
{
    const char* Msg_Type = "A";
    QString header = generatorHeader(Msg_Type);
    QString trailer = generatorTrailer();
    if(header.length() == 0||
            trailer.length() == 0){
        return  ;
    }
    QString body = header;
    QString encryptMethod = ui->le_EncrypeMethod->text();
    QString HeartBtInt = ui->le_HeartBtInt->text();
    if(encryptMethod.length() == 0||
            HeartBtInt.length() == 0){
        QMessageBox::critical(this,
                              tr("FIX Packet Sender"),
                              tr("Logon : EnCryptMethod,HeartBtInt field can't be emtpy"));
        return  ;
    }
    body += Field::generatorField(Field::encryptMethod,encryptMethod);
    body += Field::generatorField(Field::heartBtInt,HeartBtInt);
    if(ui->cb_rawDataLength->isChecked()){
        QString RawDataLen = ui->le_rawDataLength->text();
        QString RawData = ui->te_RawData->toPlainText();
        if(RawDataLen.length() == 0||
                RawData.length() == 0){
            QMessageBox::critical(this,
                                  tr("FIX Packet Sender"),
                                  tr("Logon : RawDataLen,RawData field can't be emtpy"));
            return  ;

        }
        body += Field::generatorField(Field::rawDataLen,RawDataLen);
        body += Field::generatorField(Field::rawData,RawData);
    }
    if(ui->cb_ResetNumFlags->isChecked()){
        body += Field::generatorField(Field::resetSeqNumFlag,"Y");
    }
    if(ui->cb_message_size->isChecked()){
        QString maxMessageSize = ui->le_maxMessageSize->text();
        if(maxMessageSize.length() == 0){
            QMessageBox::critical(this,
                                  tr("FIX Packet Sender"),
                                  tr("Logon : MaxMessageSize field can't be emtpy"));
            return  ;
        }
        body += Field::generatorField(Field::maxMessageSize,maxMessageSize);
    }
    body += trailer;
    doBodyLength(body);
    doCheckSum(body);
    ui->textEdit->setText(body);
}

void Dialog::on_pb_TestRequestGenerator_clicked()
{
    const char* Msg_Type = "1";
    QString header = generatorHeader(Msg_Type);
    QString trailer = generatorTrailer();
    if(header.length() == 0||
            trailer.length() == 0){
        return  ;
    }
    QString TestReqID = ui->le_testReqID_2->text();
    if(TestReqID.length() == 0){
        QMessageBox::critical(this,
                              tr("FIX Packet Sender"),
                              tr("TestRequest : TestReqID field can't be emtpy"));
        return  ;
    }
    QString body = header;
    body += Field::generatorField(Field::testReqID,TestReqID);
    body += trailer;
    doBodyLength(body);
    doCheckSum(body);
    ui->textEdit->setText(body);
}

void Dialog::on_pb_LogoutGenerator_clicked()
{
    const char* Msg_Type = "5";
    QString header = generatorHeader(Msg_Type);
    QString trailer = generatorTrailer();
    if(header.length() == 0||
            trailer.length() == 0){
        return  ;
    }
    QString body = header;
    if(ui->cb_text->isChecked()){
        QString Text = ui->te_text->toPlainText();
        if(Text.length() == 0){
            QMessageBox::critical(this,
                                  tr("FIX Packet Sender"),
                                  tr("Logout : Text field can't be emtpy"));
            return  ;
        }
        body += Field::generatorField(Field::text,Text);
    }
    if(ui->cb_encodeTextLen->isChecked()){
        QString EncodedTextLen = ui->le_encodedTextLen->text();
        QString EncodedText = ui->te_EncodeText->toPlainText();
        if(EncodedTextLen.length() == 0||
                EncodedText.length() == 0){
            QMessageBox::critical(this,
                                  tr("FIX Packet Sender"),
                                  tr("Logout : EncodeTextLen ,EncodedText field can't be emtpy"));
            return  ;
        }
        body += Field::generatorField(Field::encodedTextLen,EncodedTextLen);
        body += Field::generatorField(Field::encodedText,EncodedText);
    }
    body += trailer;
    doBodyLength(body);
    doCheckSum(body);
    ui->textEdit->setText(body);
}

void Dialog::on_pb_seqResetGenerator_clicked()
{
    const char* Msg_Type = "4";
    QString header = generatorHeader(Msg_Type);
    QString trailer = generatorTrailer();
    if(header.length() == 0||
            trailer.length() == 0){
        return  ;
    }
    QString body = header;
    if(ui->cb_gapFillFlag->isChecked()){
        body += Field::generatorField(Field::gapFillFlag,"Y");
    }
    QString NewSeqNo = ui->le_newSeqNo->text();
    if(NewSeqNo.length() == 0){
        QMessageBox::critical(this,
                              tr("FIX Packet Sender"),
                              tr("Sequence Reset : NewSeqNo field can't be emtpy"));
        return  ;
    }
    body += Field::generatorField(Field::newSeqNo,NewSeqNo);
    body += trailer;
    doBodyLength(body);
    doCheckSum(body);
    ui->textEdit->setText(body);
}

void Dialog::on_pb_RejectGenerator_clicked()
{
    const char* Msg_Type = "3";
    QString header = generatorHeader(Msg_Type);
    QString trailer = generatorTrailer();
    if(header.length() == 0||
            trailer.length() == 0){
        return  ;
    }
    QString body = header;
    QString RefSeqNum = ui->le_refSeqNum->text();
    if(RefSeqNum.length() == 0){
        QMessageBox::critical(this,
                              tr("FIX Packet Sender"),
                              tr("Reject : RefSeqNum field can't be emtpy"));
        return  ;
    }
    body += Field::generatorField(Field::refSeqNum,RefSeqNum);
    if(ui->cb_refTagID->isChecked()){
        QString RefTagId = ui->le_refTagID->text();
        if(RefTagId.length() == 0){
            QMessageBox::critical(this,
                                  tr("FIX Packet Sender"),
                                  tr("Reject : RefTagID field can't be emtpy"));
            return  ;
        }
        body += Field::generatorField(Field::refTagID,RefTagId);
    }
    if(ui->cb_refMsgtype->isChecked()){
        QString refMsgType = ui->le_msgType->text();
        if(refMsgType.length() == 0){
            QMessageBox::critical(this,
                                  tr("FIX Packet Sender"),
                                  tr("Reject : RefMsgType field can't be emtpy"));
            return  ;
        }
    }
    if(ui->cb_sessionRejectReason->isChecked()){
        QString SessionRejectReason = ui->le_SessionRejectReason->text();
        if(SessionRejectReason.length() == 0){
            QMessageBox::critical(this,
                                  tr("FIX Packet Sender"),
                                  tr("Reject : SessionRejectReason field can't be emtpy"));
            return  ;
        }
    }
    if(ui->cb_text_2->isChecked()){
        QString Text = ui->te_text_2->toPlainText();
        if(Text.length() == 0){
            QMessageBox::critical(this,
                                  tr("FIX Packet Sender"),
                                  tr("Reject : Text field can't be emtpy"));
            return  ;
        }
        body += Field::generatorField(Field::text,Text);
    }
    if(ui->cb_encodeTextLen_2->isChecked()){
        QString EncodedTextLen = ui->le_encodedTextLen2->text();
        QString EncodedText = ui->te_EncodedText2->toPlainText();
        if(EncodedTextLen.length() == 0||
                EncodedText.length() == 0){
            QMessageBox::critical(this,
                                  tr("FIX Packet Sender"),
                                  tr("Reject : EncodeTextLen ,EncodedText field can't be emtpy"));
            return  ;
        }
        body += Field::generatorField(Field::encodedTextLen,EncodedTextLen);
        body += Field::generatorField(Field::encodedText,EncodedText);
    }
    body += trailer;
    doBodyLength(body);
    doCheckSum(body);
    ui->textEdit->setText(body);
}

void Dialog::on_pb_resendGenerator_clicked()
{
    const char* Msg_Type = "2";
    QString header = generatorHeader(Msg_Type);
    QString trailer = generatorTrailer();
    if(header.length() == 0||
            trailer.length() == 0){
        return  ;
    }
    QString body = header;
    QString BeginSeqNo = ui->le_beginSeqNo->text();
    QString endSeqNo = ui->le_endSeqNo->text();
    if(BeginSeqNo.length() == 0||
            endSeqNo.length() == 0){
        QMessageBox::critical(this,
                              tr("FIX Packet Sender"),
                              tr("Resend Request : BeginSeqNo ,EndSeqNo field can't be emtpy"));
        return  ;
    }
    body += Field::generatorField(Field::beginSeqNo,BeginSeqNo);
    body += Field::generatorField(Field::endSeqNo,endSeqNo);
    body += trailer;
    doBodyLength(body);
    doCheckSum(body);
    ui->textEdit->setText(body);
}

void Dialog::on_cb_signatrueLength_clicked(bool checked)
{
    if(checked){
        ui->le_signatrueLength->setDisabled(false);
        ui->te_signatrue->setDisabled(false);
    }
    else{
        ui->le_signatrueLength->setDisabled(true);
        ui->te_signatrue->setDisabled(true);
    }
}

void Dialog::on_cb_encodeTextLen_clicked(bool checked)
{
    if(checked){
        ui->le_encodedTextLen->setDisabled(false);
        ui->te_EncodeText->setDisabled(false);
    }
    else{
        ui->le_encodedTextLen->setDisabled(true);
        ui->te_EncodeText->setDisabled(true);
    }
}

void Dialog::on_cb_text_clicked(bool checked)
{
    if(checked){
        ui->te_text->setDisabled(false);
    }
    else {
        ui->te_text->setDisabled(true);
    }
}
void Dialog::on_cb_rawDataLength_clicked(bool checked)
{
    if(checked){
        ui->le_rawDataLength->setDisabled(false);
        ui->te_RawData->setDisabled(false);
    }
    else {
        ui->le_rawDataLength->setDisabled(true);
        ui->te_RawData->setDisabled(true);
    }
}

void Dialog::on_cb_message_size_clicked(bool checked)
{
    if(checked){
        ui->le_maxMessageSize->setDisabled(false);
    }
    else{
        ui->le_maxMessageSize->setDisabled(true);
    }
}

void Dialog::on_cb_testReqId_clicked(bool checked)
{
    if(checked){
        ui->le_testReqID->setDisabled(false);
    }
    else {
        ui->le_testReqID->setDisabled(true);

    }
}

void Dialog::on_cb_refTagID_clicked(bool checked)
{
    if(checked){
        ui->le_refTagID->setDisabled(false);

    }
    else {
        ui->le_refTagID->setDisabled(true);

    }
}

void Dialog::on_cb_refMsgtype_clicked(bool checked)
{
    if(checked){
        ui->le_refMsgType->setDisabled(false);
    }
    else {
        ui->le_refMsgType->setDisabled(true);
    }
}

void Dialog::on_cb_sessionRejectReason_clicked(bool checked)
{
    if(checked){
        ui->le_SessionRejectReason->setDisabled(false);

    }
    else {
        ui->le_SessionRejectReason->setDisabled(true);
    }
}

void Dialog::on_cb_encodeTextLen_2_clicked(bool checked)
{
    if(checked){
        ui->le_encodedTextLen2->setDisabled(false);
        ui->te_EncodedText2->setDisabled(false);
    }
    else {
        ui->le_encodedTextLen2->setDisabled(true);
        ui->te_EncodedText2->setDisabled(true);
    }
}

void Dialog::on_cb_text_2_clicked(bool checked)
{
    if(checked){
        ui->te_text_2->setDisabled(false);
    }
    else {
        ui->te_text_2->setDisabled(true);

    }

}

QString Dialog::generatorHeader(QString msg_type)
{
    QString beginString = ui->le_beginString->text();
    QString SenderCompID = ui->le_SendCompID->text();
    QString TargetCompID = ui->le_targetCompID->text();
    QString BodyLength = ui->le_bodyLength->text();
    //QString MsgType =
    QString MsgSeqNum = ui->le_msgSeqNum->text();
    bool possDupPoss= ui->cb_possDupFlag->isChecked();
    bool possResend = ui->cb_PossResend->isChecked();
    QString SendTime = ui->le_SendTime->text();
    QString OrigSendingTime = ui->le_OrigSendingTime->text();
    if(beginString.length()== 0||
            SenderCompID.length() == 0||
            TargetCompID.length() == 0){
        QMessageBox::information(this
                                 ,tr("Fix Packet Sender")
                                 ,tr("beginString ,senderCompID,TargetCompID can't be empty"));
        return  "";
    }
    if(SendTime.length() == 0){
        SendTime = QDateTime::currentDateTimeUtc().toString("yyyyMMdd-hh:mm:ss.zzz");
    }
    if(BodyLength.length() == 0){
        BodyLength = auto_bodyLength;
    }
    if(MsgSeqNum.length() == 0){
        MsgSeqNum = QString("%1").arg(++senderMsgSeq);
    }
    QString header = Field::generatorField(Field::beginString,beginString);
    header += Field::generatorField(Field::bodylength,BodyLength);
    header += Field::generatorField(Field::msyType,msg_type);
    header += Field::generatorField(Field::msgSeqNum,MsgSeqNum);
    header += Field::generatorField(Field::senderCompID,SenderCompID);
    header += Field::generatorField(Field::sendingTime,SendTime);
    header += Field::generatorField(Field::targetCompID,TargetCompID);
    if(possDupPoss){
        header += Field::generatorField(Field::possDupFlag,"Y");
    }
    if(OrigSendingTime.length() != 0){
        header += Field::generatorField(Field::OrigSendingTime,OrigSendingTime);
    }
    if(possResend){
        header += Field::generatorField(Field::PossResend,"Y");
    }
    return  header;
}

QString Dialog::generatorTrailer()
{
    QString trailer = "";
    QString CheckSum = ui->le_checkSum->text();
    if(ui->cb_signatrueLength->isChecked()){
        QString signatureLength = ui->le_signatrueLength->text();
        QString signatrue = ui->te_signatrue->toPlainText();
        if(signatureLength.length() == 0||
                signatrue.length() == 0){
            QMessageBox::critical(this,
                                  tr("Fix Packet Sender"),
                                  tr("Signatrue Length ,Signatrue can't be emtpy"));
            return  "";
        }
        trailer = Field::generatorField(Field::signatrueLength,signatureLength);
        trailer += Field::generatorField(Field::signatrue,signatrue);
    }
    else {

    }
    if(CheckSum.length() == 0){
        CheckSum = "<AUTO_CHECKSUM>";
    }
    trailer += Field::generatorField(Field::checksum,CheckSum);
    return  trailer;
}

int Dialog::doBodyLength(QString &body)
{
    int bodyLength = Field::bodyLength(body);
    body.replace(auto_bodyLength,QString("%1").arg(bodyLength));
    return  1;
}

int Dialog::doCheckSum(QString &body)
{
    int checkSum = Field::checkSum(body);
    body.replace(auto_checkSum,QString("%1").arg(checkSum,3,10,QLatin1Char('0')));
    return  0;
}


void Dialog::on_pushButton_5_clicked()
{
    senderMsgSeq = 0;
    ui->textEdit->setText("");
    //ui->textEdit_2->setText("");
    model.removeRows(0,model.rowCount());
    packets.clear();
}

void Dialog::on_pb_startListen_clicked()
{
    uint port = ui->le_listenPort->text().toUInt();
    if(port <= 0){
        QMessageBox::critical(this,
                              tr("Fix Packet Sender"),
                              tr("Invalid Port"));
        return  ;
    }
    fixServer = new QTcpServer(this);
    if(!fixServer->listen(QHostAddress::Any,port)){
        QMessageBox::critical(this,
                              tr("Fix Packet Sender"),
                              tr("Application can't bind the port"));
        return  ;
    }
    workMode = server;
    ui->pb_startListen->setDisabled(true);
    ui->pb_stoplisten->setDisabled(false);
    ui->pushButton->setDisabled(true);
    connect(fixServer,SIGNAL(newConnection()),this,SLOT(OnnewConnection()));
    //connect(socket, SIGNAL(readyRead()), this, SLOT(readPacket()));
    //connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)));
    //connect(socket,SIGNAL(connected()),this,SLOT(OnConnected()));
    //connect(socket,SIGNAL(disconnected()),this,SLOT(OnDisconnected()));

}

void Dialog::on_pb_stoplisten_clicked()
{
    fixServer->close();
    if(socket != NULL){
        socket->disconnectFromHost();
    }
    workMode = undefined;
    ui->pushButton->setDisabled(false);
    ui->pushButton_2->setDisabled(true);
    ui->pb_startListen->setDisabled(false);
    ui->pb_stoplisten->setDisabled(true);
}

void Dialog::OnnewConnection()
{
    QMessageBox::information(this,
                             tr("Fix Packet Sender")
                             ,tr("Acceptor New Connection"));
    ui->pushButton_2->setDisabled(false);
    socket = fixServer->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readPacket()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)));
    //connect(socket,SIGNAL(connected()),this,SLOT(OnConnected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(OnDisconnected()));
}

void Dialog::on_pb_generatorNewOrder_clicked()
{
    const char* Msg_Type = "D";
    QString header = generatorHeader(Msg_Type);
    QString trailer = generatorTrailer();
    if(header.length() == 0||
            trailer.length() == 0){
        return  ;
    }
    QString body = header;
    QString ClOrdID = ui->le_clordid->text();
    QString HandlInst = ui->le_handlInst->text();
    QString Symbol = ui->le_symbol->text();
    QString Side = ui->le_side->text();
    QString TransactTime = ui->le_transactTime->text();
    QString OrdType = ui->le_ordtype->text();
    if(ClOrdID.length() == 0 ||
            HandlInst.length() == 0||
            Symbol.length() == 0||
            Side.length() == 0||
            OrdType.length() == 0){
        QMessageBox::critical(this,
                              tr("FIX Packet Sender"),
                              tr("New Order - Single : ClOrdID,HandlInst,Symbol,Side,TransactTime,OrdType field can't be emtpy"));
        return  ;
    }
    if(TransactTime.length() == 0){
        TransactTime = QDateTime::currentDateTimeUtc().toString("yyyyMMdd-hh:mm:ss.zzz");
    }
    body += Field::generatorField(Field::ClOrdID,ClOrdID);
    body += Field::generatorField(Field::HandlInst,HandlInst);
    body += Field::generatorField(Field::Symbol,Symbol);
    body += Field::generatorField(Field::Side,Side);
    body += Field::generatorField(Field::TransactTime,TransactTime);
    body += Field::generatorField(Field::OrdType,OrdType);
    body += trailer;
    doBodyLength(body);
    doCheckSum(body);
    ui->textEdit->setText(body);
}
