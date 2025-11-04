#include "connection.h"
#include <QDebug>

Connection::Connection() {

}
bool Connection :: connect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase ("QODBC")  ;
    db.setDatabaseName("projet_qt");
    db.setUserName("Khalil");
    db.setPassword("khalil123");

    if (db.open()) {

        test=true;
        // qDebug() <<"felicitation vous etes connecte a la base de donnes" ;
    }

    return test ;


}
