#ifndef SEANCE_H
#define SEANCE_H

#include <QString>
#include <QDate>
#include <QTime>
#include <QSqlQueryModel>
#include <QMap>

class Seance
{
public:
    Seance();
    Seance(QString id, QString column2, QDate dateSeance, QTime heureDebut,
           QTime heureArriver, QString lieuDepart, double prix, QString idEmploye = QString());

    // CRUD
    bool ajouter() const;
    bool modifier() const;
    static bool supprimer(const QString& id);

    // Affichage / utilitaires
    static QSqlQueryModel* afficher();
    static QSqlQueryModel* rechercher(const QString& motCle);
    static QSqlQueryModel* trier(const QString& colonne, bool asc = true);

    // Validation
    static QStringList validate(const Seance& s);

    // Existence
    static bool existe(const QString& id);

    // Statistiques
    static int getTotalSeances();
    static double getRevenuTotal();
    static QMap<QString, int> getSeancesParType();
    static QMap<QString, int> getSeancesParMois();

    // Export PDF
    static bool exporterPDF(const QString& filename);

    // Validation pour recherche et tri
    static bool isValidSearchCriteria(const QString& critere);
    static bool isValidSortCriteria(const QString& critere);
    static QString sanitizeSearchInput(const QString& input);
    static bool isValidSearchInput(const QString& input);

    // Validation spécifique pour la recherche
    static bool isValidDateSearch(const QString& dateStr);
    static bool isValidLieuSearch(const QString& lieu);
    static QSqlQueryModel* rechercherParCritere(const QString& critere, const QString& valeur);

    // Données
    QString id_seance;
    QString column2;     // "Code"/"Conduite"…
    QDate   date_seance;
    QTime   heure_debut;
    QTime   heure_arriver;
    QString lieu_depart;
    double  prix;        // <0 => NULL
    QString id_employe;  // optionnel

    bool hasPrix() const { return prix >= 0.0; }
};

#endif // SEANCE_H
