#include "connection.h"
#include <QDebug>

Connection::Connection() {

}

bool Connection::connect()
{
    bool test = false;

    // Pilote ODBC par défaut
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    // >>> Paramètres EXACTS que tu utilises <<<
    db.setDatabaseName("projet_qt");  // DSN ODBC
    db.setUserName("moutia1");        // utilisateur
    db.setPassword("13038854");       // mot de passe

    if (db.open()) {
        test = true;
        // qDebug() << "Félicitations, connexion à la base réussie.";
    }

    return test;
}
