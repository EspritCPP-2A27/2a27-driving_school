#include "mainwindow.h"
#include <QApplication>
#include "connection.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection c;
    bool test = c.connect();

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Base de donnees ouverte"),QObject::tr(" Vous etes connecte a  la base de donnees.\n""Cliquez sur OK pour continuer."), QMessageBox::Ok);

        MainWindow w;
        w.show();
        return a.exec();
    }
    else {
        QMessageBox::critical(nullptr, QObject::tr("Echec"), QObject::tr("Echec de connexion a la base de donnees.\n""Cliquez sur OK pour quitter."),
         QMessageBox::Cancel);
        return -1;
    }
}

