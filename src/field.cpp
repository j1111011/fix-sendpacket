#include "field.h"
#include <numeric>
Field::Field()
{
}
int Field::checkSum(QString fixData)
{
    int checkSum_b = fixData.indexOf("\00110=");
    int checkSum_e = fixData.indexOf("\001",checkSum_b + 1);
    fixData.replace(checkSum_b + 1,(checkSum_e - checkSum_b),"");
    std::string fix = fixData.toStdString();//.c_str();
    const unsigned char* iter =
          reinterpret_cast<const unsigned char*>( fix.data() );
    int m_total = std::accumulate( iter,iter + fix.size(), 0 );

    return  m_total%256;
}

int Field::bodyLength(QString fixData)
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
        return  -1;
    }
    return  fixData.length();
}

QString Field::generatorField(int tag, QString value)
{
    QString field = QString("%1=%2\001").arg(tag,0,10).arg(value);
    return  field;
}
