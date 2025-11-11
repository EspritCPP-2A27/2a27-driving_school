#include "mainwindow.h"
#include "connection.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Connexion avec TA méthode existante
    Connection c;
    bool ok = c.connect();

    if (!ok) {
        QMessageBox::critical(nullptr, "Échec BD",
                              "Impossible d'ouvrir la connexion ODBC.\n"
                              "Vérifie DSN / utilisateur / mot de passe.");
        return 1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
