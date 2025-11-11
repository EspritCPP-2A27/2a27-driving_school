#include "seance.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QRegularExpression>
#include <QMetaType>
#include <QDebug>
#include <utility> // std::move

Seance::Seance() : prix(-1.0) {}

Seance::Seance(QString id, QString c2, QDate dateS, QTime hDeb,
               QTime hArr, QString lieu, double p, QString emp)
    : id_seance(std::move(id)),
    column2(std::move(c2)),
    date_seance(dateS),
    heure_debut(hDeb),
    heure_arriver(hArr),
    lieu_depart(std::move(lieu)),
    prix(p),
    id_employe(std::move(emp))
{
    if (p < 0.0) prix = -1.0; // -1 => sera stocké comme NULL
}

QStringList Seance::validate(const Seance& s)
{
    QStringList errs;

    static const QRegularExpression reId(R"(^[A-Za-z0-9_-]{1,32}$)");
    if (s.id_seance.isEmpty() || !reId.match(s.id_seance).hasMatch())
        errs << "ID invalide (lettres, chiffres, '_' ou '-', 1..32).";

    if (s.column2.trimmed().isEmpty())
        errs << "Type (col2) obligatoire.";

    if (!s.date_seance.isValid())
        errs << "Date invalide.";

    if (!s.heure_debut.isValid()) errs << "Heure de début invalide.";
    if (!s.heure_arriver.isValid()) errs << "Heure d'arrivée invalide.";
    if (s.heure_debut.isValid() && s.heure_arriver.isValid() && s.heure_arriver <= s.heure_debut)
        errs << "Heure d'arrivée doit être strictement après l'heure de début.";

    static const QRegularExpression reLieu(R"(^[A-Za-z0-9À-ÿ '._-]{2,100}$)");
    if (s.lieu_depart.trimmed().isEmpty() || !reLieu.match(s.lieu_depart).hasMatch())
        errs << "Lieu invalide (2–100 caractères autorisés).";

    if (s.hasPrix()) {
        if (!(s.prix >= 0.0) || s.prix > 1e6)
            errs << "Prix invalide (nombre positif, raisonnable).";
    }
    return errs;
}

bool Seance::existe(const QString& id)
{
    QSqlQuery q; // connexion par défaut (ta Connection)
    q.prepare("SELECT COUNT(1) FROM SEANCE WHERE ID_SEANCE = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qWarning() << "[SEANCE existe] SQL:" << q.lastError().text();
        return false; // prudence
    }
    return (q.next() && q.value(0).toInt() > 0);
}

bool Seance::ajouter() const
{
    QSqlQuery q;
    q.prepare(
        "INSERT INTO SEANCE "
        "(ID_SEANCE, COLUMN2, DATE_SEANCE, HEURE_DEBUT, HEURE_ARRIVER, LIEU_DEPART, PRIX"
        // ", ID_EMPLOYE" // décommente si la colonne existe dans ton schéma
        ") VALUES "
        "(:id, :c2, TO_DATE(:d,'YYYY-MM-DD'), TO_DATE(:hd,'HH24:MI'), TO_DATE(:ha,'HH24:MI'), :lieu, :prix"
        // ", :emp"
        ")"
        );

    q.bindValue(":id",  id_seance);
    q.bindValue(":c2",  column2);
    q.bindValue(":d",   date_seance.toString("yyyy-MM-dd"));
    q.bindValue(":hd",  heure_debut.toString("HH:mm"));
    q.bindValue(":ha",  heure_arriver.toString("HH:mm"));
    q.bindValue(":lieu", lieu_depart);

    if (hasPrix()) {
        q.bindValue(":prix", prix);
    } else {
        // Qt6 : créer un QVariant NULL du type voulu via QMetaType objet
        q.bindValue(":prix", QVariant(QMetaType(QMetaType::Double)));
    }

    // Si ID_EMPLOYE existe :
    // if (id_employe.trimmed().isEmpty()) q.bindValue(":emp", QVariant(QMetaType(QMetaType::QString)));
    // else                                 q.bindValue(":emp", id_employe);

    if (!q.exec()) {
        qWarning() << "[SEANCE ajouter] SQL:" << q.lastError().text();
        return false;
    }
    return true;
}

bool Seance::modifier() const
{
    QSqlQuery q;

    // On n’inclut PAS ID_EMPLOYE pour éviter l’erreur si la colonne n’existe pas
    QString sql =
        "UPDATE SEANCE SET "
        "  COLUMN2 = :c2, "
        "  DATE_SEANCE = TO_DATE(:d,'YYYY-MM-DD'), "
        "  HEURE_DEBUT = TO_DATE(:hd,'HH24:MI'), "
        "  HEURE_ARRIVER = TO_DATE(:ha,'HH24:MI'), "
        "  LIEU_DEPART = :lieu, ";

    if (hasPrix()) {
        sql += "  PRIX = :prix ";
    } else {
        sql += "  PRIX = NULL ";
    }
    sql += "WHERE ID_SEANCE = :id";

    if (!q.prepare(sql)) {
        qWarning() << "[SEANCE modifier] prepare:" << q.lastError().text();
        return false;
    }

    q.bindValue(":id",  id_seance);
    q.bindValue(":c2",  column2);
    q.bindValue(":d",   date_seance.toString("yyyy-MM-dd"));
    q.bindValue(":hd",  heure_debut.toString("HH:mm"));
    q.bindValue(":ha",  heure_arriver.toString("HH:mm"));
    q.bindValue(":lieu", lieu_depart);
    if (hasPrix()) q.bindValue(":prix", prix);

    if (!q.exec()) {
        qWarning() << "[SEANCE modifier] exec:" << q.lastError().text();
        return false;
    }
    if (q.numRowsAffected() <= 0) {
        qWarning() << "[SEANCE modifier] aucune ligne modifiée (ID introuvable ?)";
        return false;
    }
    return true;
}

bool Seance::supprimer(const QString& id)
{
    QSqlQuery q;
    if (!q.prepare("DELETE FROM SEANCE WHERE ID_SEANCE = :id")) {
        qWarning() << "[SEANCE supprimer] prepare:" << q.lastError().text();
        return false;
    }
    q.bindValue(":id", id);
    if (!q.exec()) {
        qWarning() << "[SEANCE supprimer] exec:" << q.lastError().text();
        return false;
    }
    if (q.numRowsAffected() <= 0) {
        qWarning() << "[SEANCE supprimer] aucune ligne supprimée (ID introuvable ?)";
        return false;
    }
    return true;
}

QSqlQueryModel* Seance::afficher()
{
    auto* m = new QSqlQueryModel();
    m->setQuery(
        "SELECT ID_SEANCE, COLUMN2, "
        "       TO_CHAR(DATE_SEANCE,'DD/MM/YYYY') AS DATE_S, "
        "       TO_CHAR(HEURE_DEBUT,'HH24:MI')   AS H_DEB, "
        "       TO_CHAR(HEURE_ARRIVER,'HH24:MI') AS H_ARR, "
        "       LIEU_DEPART, PRIX "
        "FROM SEANCE"
        );
    m->setHeaderData(0, Qt::Horizontal, "ID");
    m->setHeaderData(1, Qt::Horizontal, "Col2");
    m->setHeaderData(2, Qt::Horizontal, "Date");
    m->setHeaderData(3, Qt::Horizontal, "Heure début");
    m->setHeaderData(4, Qt::Horizontal, "Heure arrivée");
    m->setHeaderData(5, Qt::Horizontal, "Lieu");
    m->setHeaderData(6, Qt::Horizontal, "Prix");
    return m;
}

QSqlQueryModel* Seance::rechercher(const QString& motCle)
{
    auto* m = new QSqlQueryModel();
    const QString like = "%" + motCle + "%";
    QSqlQuery q;
    q.prepare(
        "SELECT ID_SEANCE, COLUMN2, TO_CHAR(DATE_SEANCE,'DD/MM/YYYY'), "
        "       TO_CHAR(HEURE_DEBUT,'HH24:MI'), TO_CHAR(HEURE_ARRIVER,'HH24:MI'), "
        "       LIEU_DEPART, PRIX "
        "FROM SEANCE "
        "WHERE LOWER(ID_SEANCE)   LIKE LOWER(:k) "
        "   OR LOWER(COLUMN2)     LIKE LOWER(:k) "
        "   OR LOWER(LIEU_DEPART) LIKE LOWER(:k)"
        );
    q.bindValue(":k", like);
    q.exec();
    m->setQuery(std::move(q)); // Qt6 : passer la requête par move, pas par copie
    return m;
}

QSqlQueryModel* Seance::trier(const QString& colonne, bool asc)
{
    auto* m = new QSqlQueryModel();
    const QString order = asc ? "ASC" : "DESC";
    m->setQuery(QString(
                    "SELECT ID_SEANCE, COLUMN2, TO_CHAR(DATE_SEANCE,'DD/MM/YYYY'), "
                    "       TO_CHAR(HEURE_DEBUT,'HH24:MI'), TO_CHAR(HEURE_ARRIVER,'HH24:MI'), "
                    "       LIEU_DEPART, PRIX "
                    "FROM SEANCE ORDER BY %1 %2").arg(colonne, order));
    return m;
}
