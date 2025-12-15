#ifndef ARDUINO_H
#define ARDUINO_H

#include <QtSerialPort/QSerialPort>

class Arduino
{
public:
    Arduino();
    int connect_arduino();       // connecter le PC à Arduino
    int close_arduino();         // fermer la connexion
    void write_to_arduino(QByteArray);   // envoyer vers Arduino
    QByteArray read_from_arduino();      // lire depuis Arduino
    QSerialPort* getserial();    // accesseur
    QString getarduino_port_name();
    QString getLastRFIDCard() const { return lastRFIDCard; }
    void setLastRFIDCard(const QString &card) { lastRFIDCard = card; }
    bool isWaitingForCard() const { return waitingForCard; }
    void setWaitingForCard(bool waiting) { waitingForCard = waiting; }
    void resetRFIDCard() { lastRFIDCard.clear(); }

private:
    QSerialPort * serial;
    QString lastRFIDCard;
    bool waitingForCard;

    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;

    QString arduino_port_name;
    bool arduino_is_available;

    QByteArray data;
};

#endif // ARDUINO_H
