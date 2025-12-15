#ifndef VEHICULES_H
#define VEHICULES_H

#include <QObject>
#include <QSqlQuery>
#include <QTableWidget>
#include <QDate>
#include <QCalendarWidget>

public:
explicit VehiculeManager(QObject *parent = nullptr);

void chargerVehicules(QTableWidget *tableWidget);
void ajouterVehicule(const QString &matricule, const QString &type, const QString &etat,
                     const QString &dispo, const QString &km, const QString &assurance,
                     const QDate &dateMaintenance, const QDate &dateAssurance, QTableWidget *tableWidget);
void supprimerVehicule(const QString &matricule, QTableWidget *tableWidget);
void rechercherVehicule(const QString &matricule, QTableWidget *tableWidget);
void trierVehicules(const QString &critere, QTableWidget *tableWidget);
void modifierVehicule(const QString &matricule, const QString &type, const QString &etat,
                      const QString &dispo, const QString &km, const QString &assurance,
                      const QDate &dateMaintenance, const QDate &dateAssurance, QTableWidget *tableWidget);
void exporterPDF(QTableWidget *tableWidget, QWidget *parent);
void marquerDatesCalendrier(QCalendarWidget *calendarWidget);
void verifierAssurancesDepuisBase();
void verifierDateAssuranceEtNotifier(const QString &matricule, const QDate &dateAssurance);

void afficherToast(const QString &message, QWidget *parent);
void jouerSonNotification();

QString getVehiculeInfoAtDate(const QDate &date);
void remplirChampsDepuisTableau(int row, QTableWidget *tableWidget,
                                QLineEdit *matriculeEdit, QComboBox *typeCombo,
                                QRadioButton *radioPanne, QRadioButton *radioFonctionnelle,
                                QLineEdit *kmEdit, QDateEdit *dateMaintenanceEdit,
                                QRadioButton *radioTousRisque, QRadioButton *radioIntermediaire,
                                QRadioButton *radioRC, QComboBox *dispoCombo, QDateEdit *dateAssuranceEdit);

signals:
void vehiculeAjoute();
void vehiculeSupprime();
void vehiculeModifie();
void notification(const QString &message);

private:
bool verifierMatriculeValide(const QString &matricule);
bool verifierVehiculeExiste(const QString &matricule);
};
#endif // VEHICULES_H



