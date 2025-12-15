#include "arduino.h"
#include <QSerialPortInfo>
#include <QDebug>

Arduino::Arduino()
{
    serial = new QSerialPort;
    arduino_is_available = false;
    arduino_port_name = "";
    data.clear();
}

int Arduino::connect_arduino()
{
    // Chercher la carte avec les bons vendor/product id
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            if ((serial_port_info.vendorIdentifier() == arduino_uno_vendor_id) &&
                (serial_port_info.productIdentifier() == arduino_uno_product_id)) {

                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
            }
        }
    }

    qDebug() << "arduino_port_name is :" << arduino_port_name;

    if (arduino_is_available) {
        serial->setPortName(arduino_port_name);

        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            return 0;  // OK
        }
    }

    return 1;  // erreur
}

int Arduino::close_arduino()
{
    if (serial->isOpen()) {
        serial->close();
        return 0;
    }
    return 1;
}

void Arduino::write_to_arduino(QByteArray d)
{
    if (!serial) {
        qDebug() << "❌ serial is NULL!";
        return;
    }

    if (!serial->isOpen()) {
        qDebug() << "❌ serial is not open!";
        return;
    }

    if (serial->isWritable()) {
        serial->write(d);
        qDebug() << "→ Arduino << " << d;
    } else {
        qDebug() << "❌ serial not writable!";
    }
}
QByteArray Arduino::read_from_arduino()
{
    if (serial->isReadable()) {
        QByteArray data = serial->readAll();
        qDebug() << "← Arduino → Qt:" << data;
        return data;
    }
    return QByteArray();
}



QSerialPort* Arduino::getserial()
{
    return serial;
}

QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}
