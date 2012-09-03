#include "dialog.h"
#include "ui_dialog.h"
#include "QtGui/QMessageBox"
#include "Qt/qfile.h"
#include <sstream>
const char BEGINSTRING[] = "BeginString";
const char SENDERCOMPID[] = "SenderCompID";
const char TARGETCOMPID[] = "TargetCompID";
const char SESSION_QUALIFIER[] = "SessionQualifier";
const char DEFAULT_APPLVERID[] = "DefaultApplVerID";
const char CONNECTION_TYPE[] = "ConnectionType";
const char USE_DATA_DICTIONARY[] = "UseDataDictionary";
const char SEND_RESETSEQNUMFLAG[] = "SendResetSeqNumFlag";
const char SEND_REDUNDANT_RESENDREQUESTS[] = "SendRedundantResendRequests";
const char DATA_DICTIONARY[] = "DataDictionary";
const char TRANSPORT_DATA_DICTIONARY[] = "TransportDataDictionary";
const char APP_DATA_DICTIONARY[] = "AppDataDictionary";
const char USE_LOCAL_TIME[] = "UseLocalTime";
const char START_TIME[] = "StartTime";
const char END_TIME[] = "EndTime";
const char START_DAY[] = "StartDay";
const char END_DAY[] = "EndDay";
const char LOGON_TIME[] = "LogonTime";
const char LOGOUT_TIME[] = "LogoutTime";
const char LOGON_DAY[] = "LogonDay";
const char LOGOUT_DAY[] = "LogoutDay";
const char CHECK_COMPID[] = "CheckCompID";
const char CHECK_LATENCY[] = "CheckLatency";
const char MAX_LATENCY[] = "MaxLatency";
const char HEARTBTINT[] = "HeartBtInt";
const char SOCKET_ACCEPT_PORT[] = "SocketAcceptPort";
const char SOCKET_REUSE_ADDRESS[] = "SocketReuseAddress";
const char SOCKET_CONNECT_HOST[] = "SocketConnectHost";
const char SOCKET_CONNECT_PORT[] = "SocketConnectPort";
const char SOCKET_NODELAY[] = "SocketNodelay";
const char SOCKET_SEND_BUFFER_SIZE[] = "SendBufferSize";
const char SOCKET_RECEIVE_BUFFER_SIZE[] = "ReceiveBufferSize";
const char RECONNECT_INTERVAL[] = "ReconnectInterval";
const char VALIDATE_LENGTH_AND_CHECKSUM[] = "ValidateLengthAndChecksum";
const char VALIDATE_FIELDS_OUT_OF_ORDER[] = "ValidateFieldsOutOfOrder";
const char VALIDATE_FIELDS_HAVE_VALUES[] = "ValidateFieldsHaveValues";
const char VALIDATE_USER_DEFINED_FIELDS[] = "ValidateUserDefinedFields";
const char LOGON_TIMEOUT[] = "LogonTimeout";
const char LOGOUT_TIMEOUT[] = "LogoutTimeout";
const char FILE_STORE_PATH[] = "FileStorePath";
const char MYSQL_STORE_USECONNECTIONPOOL[] = "MySQLStoreUseConnectionPool";
const char MYSQL_STORE_DATABASE[] = "MySQLStoreDatabase";
const char MYSQL_STORE_USER[] = "MySQLStoreUser";
const char MYSQL_STORE_PASSWORD[] = "MySQLStorePassword";
const char MYSQL_STORE_HOST[] = "MySQLStoreHost";
const char MYSQL_STORE_PORT[] = "MySQLStorePort";
const char POSTGRESQL_STORE_USECONNECTIONPOOL[] = "PostgreSQLStoreUseConnectionPool";
const char POSTGRESQL_STORE_DATABASE[] = "PostgreSQLStoreDatabase";
const char POSTGRESQL_STORE_USER[] = "PostgreSQLStoreUser";
const char POSTGRESQL_STORE_PASSWORD[] = "PostgreSQLStorePassword";
const char POSTGRESQL_STORE_HOST[] = "PostgreSQLStoreHost";
const char POSTGRESQL_STORE_PORT[] = "PostgreSQLStorePort";
const char ODBC_STORE_USER[] = "OdbcStoreUser";
const char ODBC_STORE_PASSWORD[] = "OdbcStorePassword";
const char ODBC_STORE_CONNECTION_STRING[] = "OdbcStoreConnectionString";
const char FILE_LOG_PATH[] = "FileLogPath";
const char FILE_LOG_BACKUP_PATH[] = "FileLogBackupPath";
const char SCREEN_LOG_SHOW_INCOMING[] = "ScreenLogShowIncoming";
const char SCREEN_LOG_SHOW_OUTGOING[] = "ScreenLogShowOutgoing";
const char SCREEN_LOG_SHOW_EVENTS[] = "ScreenLogShowEvents";
const char MYSQL_LOG_USECONNECTIONPOOL[] = "MySQLLogUseConnectionPool";
const char MYSQL_LOG_DATABASE[] = "MySQLLogDatabase";
const char MYSQL_LOG_USER[] = "MySQLLogUser";
const char MYSQL_LOG_PASSWORD[] = "MySQLLogPassword";
const char MYSQL_LOG_HOST[] = "MySQLLogHost";
const char MYSQL_LOG_PORT[] = "MySQLLogPort";
const char MYSQL_LOG_INCOMING_TABLE[] = "MySQLLogIncomingTable";
const char MYSQL_LOG_OUTGOING_TABLE[] = "MySQLLogOutgoingTable";
const char MYSQL_LOG_EVENT_TABLE[] = "MySQLLogEventTable";
const char POSTGRESQL_LOG_USECONNECTIONPOOL[] = "PostgreSQLLogUseConnectionPool";
const char POSTGRESQL_LOG_DATABASE[] = "PostgreSQLLogDatabase";
const char POSTGRESQL_LOG_USER[] = "PostgreSQLLogUser";
const char POSTGRESQL_LOG_PASSWORD[] = "PostgreSQLLogPassword";
const char POSTGRESQL_LOG_HOST[] = "PostgreSQLLogHost";
const char POSTGRESQL_LOG_PORT[] = "PostgreSQLLogPort";
const char POSTGRESQL_LOG_INCOMING_TABLE[] = "PostgreSQLLogIncomingTable";
const char POSTGRESQL_LOG_OUTGOING_TABLE[] = "PostgreSQLLogOutgoingTable";
const char POSTGRESQL_LOG_EVENT_TABLE[] = "PostgreSQLLogEventTable";
const char ODBC_LOG_USER[] = "OdbcLogUser";
const char ODBC_LOG_PASSWORD[] = "OdbcLogPassword";
const char ODBC_LOG_CONNECTION_STRING[] = "OdbcLogConnectionString";
const char ODBC_LOG_INCOMING_TABLE[] = "OdbcLogIncomingTable";
const char ODBC_LOG_OUTGOING_TABLE[] = "OdbcLogOutgoingTable";
const char ODBC_LOG_EVENT_TABLE[] = "OdbcLogEventTable";
const char RESET_ON_LOGON[] = "ResetOnLogon";
const char RESET_ON_LOGOUT[] = "ResetOnLogout";
const char RESET_ON_DISCONNECT[] = "ResetOnDisconnect";
const char REFRESH_ON_LOGON[] = "RefreshOnLogon";
const char MILLISECONDS_IN_TIMESTAMP[] = "MillisecondsInTimeStamp";
const char HTTP_ACCEPT_PORT[] = "HttpAcceptPort";
const char PERSIST_MESSAGES[] = "PersistMessages";
const char DEFAULT[] = "[DEFAULT]\n";
const char SESSION[] = "[SESSION]\n";
const char ACCEPTOR[] = "acceptor";
const char INITIATOR[] = "initiator";
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_cb_multiClient_clicked(bool checked)
{
    if(checked){
        ui->le_clientCount->setDisabled(false);
    }
    else {
        ui->le_clientCount->setDisabled(true);
    }
}

void Dialog::on_cb_enableXML_clicked(bool checked)
{
    if(checked){
        ui->le_xmlcfgPath->setDisabled(false);
    }
    else {
        ui->le_xmlcfgPath->setDisabled(true);
    }
}

void Dialog::on_pb_create_clicked()
{
    int count = 1;
    if(ui->cb_multiClient->isChecked()){
        count = ui->le_clientCount->text().toUInt();
        count == 0?count = 1:count;
    }
    QVector<QString> acceptor = generatorAcceptor(count);
    QVector<QString> initiator = generatorInitiator(count);
    QString afile = ui->le_acceptorCfgName->text();
    QString ifile = ui->le_initiatorCfgName->text();
    createCfg(afile,acceptor);
    createCfg(ifile,initiator);
}

QVector<QString> Dialog::generatorAcceptor(int count)
{
    QVector<QString> cfg;
    QString Port = ui->le_port->text();
    if(Port.toUInt() <= 0){
        QMessageBox::critical(this,
                              tr("FixEngine Configure Gen"),
                              tr("Port Can't be emtpy"));
    }
    QString log = ui->le_logPath->text();
    bool enableLog = ui->cb_enableLog->isChecked();
    cfg.push_back(DEFAULT);
    cfg.push_back(generatorPair(CONNECTION_TYPE,ACCEPTOR));
    cfg.push_back(generatorPair(SOCKET_ACCEPT_PORT,Port));
    cfg.push_back(generatorPair(START_TIME,"00:00:00"));
    cfg.push_back(generatorPair(END_TIME,"00:00:00"));
    if(enableLog){
        cfg.push_back(generatorPair(FILE_LOG_PATH,log));
    }
    QString BeginString = ui->le_beginStringA->text();
    QString SenderCompID = ui->le_CompIDA->text();
    QString TargetCompID = ui->le_CompIDI->text();
    QString prefix = "";
    QString store = ui->le_storePath->text();
    bool enableXML= ui->cb_enableXML->isChecked();
    QString XML = ui->le_xmlcfgPath->text();
    for(int it = 0;it != count; it++){
        cfg.push_back(SESSION);
        cfg.push_back(generatorPair(BEGINSTRING,BeginString));
        cfg.push_back(generatorPair(SENDERCOMPID,SenderCompID));
        cfg.push_back(generatorPair(TARGETCOMPID,TargetCompID + prefix));
        prefix = QString("P%1").arg(it,3,10,QChar('0'));
        cfg.push_back(generatorPair(FILE_STORE_PATH,store));
        if(enableXML){
            cfg.push_back(generatorPair(USE_DATA_DICTIONARY,"Y"));
            cfg.push_back(generatorPair(DATA_DICTIONARY,XML));
        }
        else {
            cfg.push_back(generatorPair(USE_DATA_DICTIONARY,"N"));
        }
    }
    return cfg;
}
QVector<QString> Dialog::generatorInitiator(int count)
{
    QVector<QString> cfg;
    QString log = ui->le_logPath->text();
    cfg.push_back(DEFAULT);
    cfg.push_back(generatorPair(CONNECTION_TYPE,INITIATOR));
    cfg.push_back(generatorPair(START_TIME,"00:00:00"));
    cfg.push_back(generatorPair(END_TIME,"00:00:00"));
    cfg.push_back(generatorPair(RECONNECT_INTERVAL,ui->le_reconnectInterval->text()));
    bool enableLog = ui->cb_enableLog->isChecked();
    if(enableLog){
        cfg.push_back(generatorPair(FILE_LOG_PATH,log));
    }
    QString BeginString = ui->le_beginStringI->text();
    QString SenderCompID = ui->le_CompIDI->text();
    QString TargetCompID = ui->le_CompIDA->text();
    QString prefix = "";
    QString store = ui->le_storePath->text();
    cfg.push_back(generatorPair(FILE_STORE_PATH,store));
    bool enableXML= ui->cb_enableXML->isChecked();
    QString XML = ui->le_xmlcfgPath->text();
    QString Port = ui->le_port->text();
    QString Host = ui->le_address->text();
    QString heartBt = ui->le_heartBt->text();
    bool resetOnLogon = ui->cb_resetonLogon->isChecked();
    if(enableXML){
        cfg.push_back(generatorPair(USE_DATA_DICTIONARY,"Y"));
        cfg.push_back(generatorPair(DATA_DICTIONARY,XML));
    }
    else {
        cfg.push_back(generatorPair(USE_DATA_DICTIONARY,"N"));
    }
    for(int it = 0;it != count; it++){
        cfg.push_back(SESSION);
        cfg.push_back(generatorPair(BEGINSTRING,BeginString));
        cfg.push_back(generatorPair(SENDERCOMPID,SenderCompID + prefix));
        cfg.push_back(generatorPair(TARGETCOMPID,TargetCompID ));
        prefix = QString("P%1").arg(it,3,10,QChar('0'));
        cfg.push_back(generatorPair(SOCKET_CONNECT_HOST,Host));
        cfg.push_back(generatorPair(SOCKET_CONNECT_PORT,Port));
        cfg.push_back(generatorPair(HEARTBTINT,heartBt));
        if(resetOnLogon){
            cfg.push_back(generatorPair(RESET_ON_LOGON,"Y"));
        }
    }
    return  cfg;
}

QString Dialog::generatorPair(QString key, QString value)
{
    return  key + "=" + value + "\n";
}

void Dialog::createCfg(QString fname, QVector<QString> &cfg)
{
    QFile   file(fname);
    if(!file.open(QFile::WriteOnly)){
        QMessageBox::critical(this,
                              tr("FixEngine Configure Gen"),
                              tr("Create Configure Fail!"));
    }
    for(QVector<QString>::const_iterator iter = cfg.begin();iter != cfg.end();iter ++){
        file.write(iter->toStdString().c_str());
    }
    file.close();
}

void Dialog::updateCfg(QString fname, QVector<QString> &cfg)
{
}

void Dialog::on_cb_enableLog_clicked(bool checked)
{
    if(checked){
        ui->le_logPath->setDisabled(false);
    }
    else {
        ui->le_logPath->setDisabled(true);
    }
}
