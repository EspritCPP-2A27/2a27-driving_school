#include "seance.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QRegularExpression>
#include <QMetaType>
#include <QDebug>
#include <utility>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QFileDialog>
#include <QTextTableFormat>

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
    if (p < 0.0) prix = -1.0;
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
    QSqlQuery q;
    q.prepare("SELECT COUNT(1) FROM SEANCE WHERE ID_SEANCE = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qWarning() << "[SEANCE existe] SQL:" << q.lastError().text();
        return false;
    }
    return (q.next() && q.value(0).toInt() > 0);
}

bool Seance::ajouter() const
{
    QSqlQuery q;
    q.prepare(
        "INSERT INTO SEANCE "
        "(ID_SEANCE, COLUMN2, DATE_SEANCE, HEURE_DEBUT, HEURE_ARRIVER, LIEU_DEPART, PRIX"
        ") VALUES "
        "(:id, :c2, TO_DATE(:d,'YYYY-MM-DD'), TO_DATE(:hd,'HH24:MI'), TO_DATE(:ha,'HH24:MI'), :lieu, :prix"
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
        q.bindValue(":prix", QVariant(QMetaType(QMetaType::Double)));
    }

    if (!q.exec()) {
        qWarning() << "[SEANCE ajouter] SQL:" << q.lastError().text();
        return false;
    }
    return true;
}

bool Seance::modifier() const
{
    QSqlQuery q;

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

    if (m->lastError().isValid()) {
        qWarning() << "[SEANCE afficher] SQL:" << m->lastError().text();
    }

    m->setHeaderData(0, Qt::Horizontal, "ID");
    m->setHeaderData(1, Qt::Horizontal, "Type");
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
        "SELECT ID_SEANCE, COLUMN2, "
        "       TO_CHAR(DATE_SEANCE,'DD/MM/YYYY') AS DATE_S, "
        "       TO_CHAR(HEURE_DEBUT,'HH24:MI') AS H_DEB, "
        "       TO_CHAR(HEURE_ARRIVER,'HH24:MI') AS H_ARR, "
        "       LIEU_DEPART, PRIX "
        "FROM SEANCE "
        "WHERE LOWER(ID_SEANCE) LIKE LOWER(:k) "
        "   OR LOWER(COLUMN2) LIKE LOWER(:k) "
        "   OR LOWER(LIEU_DEPART) LIKE LOWER(:k) "
        "   OR TO_CHAR(DATE_SEANCE,'DD/MM/YYYY') LIKE :k "
        "   OR TO_CHAR(HEURE_DEBUT,'HH24:MI') LIKE :k "
        "   OR TO_CHAR(HEURE_ARRIVER,'HH24:MI') LIKE :k "
        "   OR CAST(PRIX AS VARCHAR2(20)) LIKE :k"
        );
    q.bindValue(":k", like);

    if (!q.exec()) {
        qWarning() << "[SEANCE rechercher] SQL:" << q.lastError().text();
        delete m;
        return nullptr;
    }

    m->setQuery(std::move(q));

    m->setHeaderData(0, Qt::Horizontal, "ID");
    m->setHeaderData(1, Qt::Horizontal, "Type");
    m->setHeaderData(2, Qt::Horizontal, "Date");
    m->setHeaderData(3, Qt::Horizontal, "Heure début");
    m->setHeaderData(4, Qt::Horizontal, "Heure arrivée");
    m->setHeaderData(5, Qt::Horizontal, "Lieu");
    m->setHeaderData(6, Qt::Horizontal, "Prix");

    return m;
}

QSqlQueryModel* Seance::trier(const QString& colonne, bool asc)
{
    auto* m = new QSqlQueryModel();
    const QString order = asc ? "ASC" : "DESC";

    // Liste blanche pour prévenir les injections SQL
    QStringList colonnesAutorisees = {
        "ID_SEANCE", "COLUMN2", "DATE_SEANCE",
        "HEURE_DEBUT", "HEURE_ARRIVER", "LIEU_DEPART", "PRIX"
    };

    QString colonneSecurisee = "ID_SEANCE"; // Valeur par défaut
    if (colonnesAutorisees.contains(colonne.toUpper())) {
        colonneSecurisee = colonne;
    }

    QString queryString = QString(
                              "SELECT ID_SEANCE, COLUMN2, "
                              "       TO_CHAR(DATE_SEANCE,'DD/MM/YYYY') AS DATE_S, "
                              "       TO_CHAR(HEURE_DEBUT,'HH24:MI') AS H_DEB, "
                              "       TO_CHAR(HEURE_ARRIVER,'HH24:MI') AS H_ARR, "
                              "       LIEU_DEPART, PRIX "
                              "FROM SEANCE ORDER BY %1 %2").arg(colonneSecurisee, order);

    qDebug() << "[SEANCE trier] Exécution de la requête:" << queryString;

    QSqlQuery q;
    if (!q.exec(queryString)) {
        qWarning() << "[SEANCE trier] SQL:" << q.lastError().text();
        delete m;
        return nullptr;
    }

    m->setQuery(std::move(q));

    // Définir les en-têtes
    m->setHeaderData(0, Qt::Horizontal, "ID");
    m->setHeaderData(1, Qt::Horizontal, "Type");
    m->setHeaderData(2, Qt::Horizontal, "Date");
    m->setHeaderData(3, Qt::Horizontal, "Heure début");
    m->setHeaderData(4, Qt::Horizontal, "Heure arrivée");
    m->setHeaderData(5, Qt::Horizontal, "Lieu");
    m->setHeaderData(6, Qt::Horizontal, "Prix");

    qDebug() << "[SEANCE trier] Tri réussi, nombre de lignes:" << m->rowCount();
    return m;
}

// Méthodes de statistiques
int Seance::getTotalSeances()
{
    QSqlQuery q;
    if (q.exec("SELECT COUNT(*) FROM SEANCE") && q.next()) {
        return q.value(0).toInt();
    }
    qWarning() << "[SEANCE getTotalSeances] Erreur:" << q.lastError().text();
    return 0;
}

double Seance::getRevenuTotal()
{
    QSqlQuery q;
    if (q.exec("SELECT SUM(PRIX) FROM SEANCE WHERE PRIX IS NOT NULL") && q.next()) {
        return q.value(0).toDouble();
    }
    qWarning() << "[SEANCE getRevenuTotal] Erreur:" << q.lastError().text();
    return 0.0;
}

QMap<QString, int> Seance::getSeancesParType()
{
    QMap<QString, int> result;
    QSqlQuery q;
    if (q.exec("SELECT COLUMN2, COUNT(*) FROM SEANCE GROUP BY COLUMN2")) {
        while (q.next()) {
            result[q.value(0).toString()] = q.value(1).toInt();
        }
    } else {
        qWarning() << "[SEANCE getSeancesParType] Erreur:" << q.lastError().text();
    }
    return result;
}

QMap<QString, int> Seance::getSeancesParMois()
{
    QMap<QString, int> result;
    QSqlQuery q;
    if (q.exec("SELECT TO_CHAR(DATE_SEANCE,'YYYY-MM'), COUNT(*) FROM SEANCE GROUP BY TO_CHAR(DATE_SEANCE,'YYYY-MM') ORDER BY TO_CHAR(DATE_SEANCE,'YYYY-MM')")) {
        while (q.next()) {
            result[q.value(0).toString()] = q.value(1).toInt();
        }
    } else {
        qWarning() << "[SEANCE getSeancesParMois] Erreur:" << q.lastError().text();
    }
    return result;
}

// Méthode d'export PDF
bool Seance::exporterPDF(const QString& filename)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);
    printer.setPageSize(QPageSize(QPageSize::A4));

    QTextDocument document;
    QTextCursor cursor(&document);

    // En-tête du document
    QTextCharFormat titleFormat;
    titleFormat.setFontPointSize(16);
    titleFormat.setFontWeight(QFont::Bold);
    cursor.setCharFormat(titleFormat);
    cursor.insertText("LISTE DES SÉANCES - DRIVING SCHOOL\n");

    QTextCharFormat normalFormat;
    normalFormat.setFontPointSize(10);
    cursor.setCharFormat(normalFormat);
    cursor.insertText("====================================\n\n");
    cursor.insertText("Date d'export : " + QDate::currentDate().toString("dd/MM/yyyy") + "\n\n");

    // Récupérer les données
    QSqlQueryModel* model = afficher();
    if (!model) {
        qWarning() << "[SEANCE exporterPDF] Impossible de récupérer les données";
        return false;
    }

    if (model->rowCount() == 0) {
        cursor.insertText("Aucune séance à exporter.\n");
    } else {
        // Créer le tableau
        QTextTableFormat tableFormat;
        tableFormat.setHeaderRowCount(1);
        tableFormat.setAlignment(Qt::AlignCenter);
        tableFormat.setCellSpacing(2);
        tableFormat.setCellPadding(6);
        tableFormat.setBorder(1);
        tableFormat.setBorderStyle(QTextTableFormat::BorderStyle_Solid);
        tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));

        QTextTable *table = cursor.insertTable(model->rowCount() + 1, model->columnCount(), tableFormat);

        // Format pour l'en-tête du tableau
        QTextCharFormat headerFormat;
        headerFormat.setFontWeight(QFont::Bold);
        headerFormat.setBackground(Qt::lightGray);

        // En-têtes du tableau
        for (int col = 0; col < model->columnCount(); ++col) {
            QTextTableCell cell = table->cellAt(0, col);
            QTextCursor cellCursor = cell.firstCursorPosition();
            cellCursor.setCharFormat(headerFormat);
            cellCursor.insertText(model->headerData(col, Qt::Horizontal).toString());
        }

        // Données du tableau
        for (int row = 0; row < model->rowCount(); ++row) {
            for (int col = 0; col < model->columnCount(); ++col) {
                QTextTableCell cell = table->cellAt(row + 1, col);
                QTextCursor cellCursor = cell.firstCursorPosition();
                QVariant data = model->data(model->index(row, col));
                cellCursor.insertText(data.isValid() ? data.toString() : "");
            }
        }
    }

    cursor.movePosition(QTextCursor::End);
    cursor.insertText("\n\n--- Fin du rapport ---");

    // Générer le PDF
    document.print(&printer);
    delete model;

    qDebug() << "[SEANCE exporterPDF] Export PDF réussi vers:" << filename;
    return true;
}
