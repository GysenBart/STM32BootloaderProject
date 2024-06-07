#ifndef CRC_H
#define CRC_H

#include <qmainwindow.h>

//Error in " Cpp.includeStatement('', Cpp.cxxHeaderSuffix(), ['QObject', 'QWidget', 'QMainWindow', 'QQuickItem', 'QSharedData'], 'D:/Bart/School/IoT 3 '23 - '24/Embedded Devices Advanced/ProjectBootloader/QTUartBinaryFlasher/UartBinaryFlasher')": SyntaxError: Expected token `)'
class crc
{
public:
    crc();
    uint16_t CRC16 (QByteArray* puchMsg, unsigned short usDataLen);
    quint16 crc16_ccitt(const QByteArray &data);
    quint16 calculateCRC16(QByteArray data, size_t length);
};

#endif // CRC_H
