#ifndef FIELD_H
#define FIELD_H
#include <QString>
class Field
{
public:
    Field();
    const static int msyType = 35;
    const static int beginString = 8;
    const static int bodylength = 9;
    const static int senderCompID = 49;
    const static int targetCompID = 56;
    const static int possDupFlag = 43;
    const static int PossResend = 97;
    const static int sendingTime = 52;
    const static int msgSeqNum = 34;
    const static int signatureLength = 93;
    const static int encryptMethod = 98;
    const static int heartBtInt = 108;
    const static int rawDataLen = 95;
    const static int rawData = 96;
    const static int resetSeqNumFlag = 141;
    const static int maxMessageSize = 383;
    const static int testReqID  =   112;
    const static int beginSeqNo = 7;
    const static int endSeqNo = 16;
    const static int refSeqNum = 45;
    const static int refTagID = 371;
    const static int refMsgType = 372;
    const static int sessionRejectReason = 373;
    const static int text = 58;
    const static int encodedTextLen = 354;
    const static int encodedText = 355;
    const static int gapFillFlag = 123;
    const static int newSeqNo   =   36;
    const static int signatrueLength = 93;
    const static int signatrue = 89;
    const static int checksum = 10;
    const static int OrigSendingTime = 122;
    const static int ClOrdID = 11;
    const static int HandlInst = 21;
    const static int Symbol = 55;
    const static int Side = 54;
    const static int TransactTime = 60;
    const static int OrdType = 40;

public:
    static int checkSum(QString );
    static int bodyLength(QString );
    static QString generatorField(int tag,QString value);
};

#endif // FIELD_H
