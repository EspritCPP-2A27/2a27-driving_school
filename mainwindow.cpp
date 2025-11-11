#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "seance.h"

#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDate>
#include <QTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QBrush>

// ---------------------------------------
static void setHeaders(QTableWidget* tabS)
{
    if (!tabS) return;
    QStringList headers;
    headers << "ID" << "Col2" << "Date" << "Heure début" << "Heure arrivée" << "Lieu" << "Prix";
    tabS->setColumnCount(headers.size());
    tabS->setHorizontalHeaderLabels(headers);
    tabS->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabS->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabS->setSelectionMode(QAbstractItemView::SingleSelection);
    tabS->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
// ---------------------------------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Table Séance : entêtes et clic
    initSeanceTable();
    if (ui->tabS)
        connect(ui->tabS, &QTableWidget::itemClicked, this, &MainWindow::on_tabS_itemClicked);

    // Recharge si la base est déjà ouverte par ta Connection
    if (QSqlDatabase::database().isOpen())
        rechargerTableSeancesDepuisDB();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::initSeanceTable()
{
    if (ui->tabS && ui->tabS->columnCount() == 0)
        setHeaders(ui->tabS);
}

// ===================== CRUD Séance =====================

void MainWindow::rechargerTableSeancesDepuisDB()
{
    if (!ui->tabS) return;
    setHeaders(ui->tabS);
    ui->tabS->setRowCount(0);

    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "BD", "Connexion BD fermée. Lance d'abord ta Connection.");
        return;
    }

    QSqlQuery q;
    if (!q.exec(
            "SELECT ID_SEANCE, COLUMN2, "
            "       TO_CHAR(DATE_SEANCE,'DD/MM/YYYY') AS DATE_S, "
            "       TO_CHAR(HEURE_DEBUT,'HH24:MI')   AS H_DEB, "
            "       TO_CHAR(HEURE_ARRIVER,'HH24:MI') AS H_ARR, "
            "       LIEU_DEPART, PRIX "
            "FROM SEANCE ORDER BY ID_SEANCE"))
    {
        QMessageBox::critical(this, "BD - SELECT", q.lastError().text());
        return;
    }

    int row = 0;
    while (q.next()) {
        ui->tabS->insertRow(row);
        ui->tabS->setItem(row, 0, new QTableWidgetItem(q.value(0).toString()));
        ui->tabS->setItem(row, 1, new QTableWidgetItem(q.value(1).toString()));
        ui->tabS->setItem(row, 2, new QTableWidgetItem(q.value(2).toString()));
        ui->tabS->setItem(row, 3, new QTableWidgetItem(q.value(3).toString()));
        ui->tabS->setItem(row, 4, new QTableWidgetItem(q.value(4).toString()));
        ui->tabS->setItem(row, 5, new QTableWidgetItem(q.value(5).toString()));
        ui->tabS->setItem(row, 6, new QTableWidgetItem(q.value(6).toString()));
        row++;
    }
}

void MainWindow::on_pushButton_Ajouter_2_clicked()
{
    if (!QSqlDatabase::database().isOpen()) { QMessageBox::critical(this,"BD","Connexion BD fermée."); return; }

    // Récup UI (noms exacts de ton .ui)
    const QString id   = ui->idS     ? ui->idS->text().trimmed()             : QString();
    const QString col2 = ui->typeS   ? ui->typeS->currentText().trimmed()    : QString();
    const QDate   d    = ui->dateS_2 ? ui->dateS_2->date()                   : QDate();
    const QTime   hd   = ui->hDeb    ? ui->hDeb->time()                       : QTime();
    const QTime   ha   = ui->hArr    ? ui->hArr->time()                       : QTime();
    const QString lieu = ui->lieuS   ? ui->lieuS->text().trimmed()           : QString();
    const QString prixStr = ui->prixS ? ui->prixS->text().trimmed()           : QString();

    double prix = -1.0; // -1 => NULL
    if (!prixStr.isEmpty()) {
        bool ok=false; double p=prixStr.toDouble(&ok);
        if (ok) prix = p;
    }

    Seance s(id, col2, d, hd, ha, lieu, prix);
    const QStringList errs = Seance::validate(s);
    if (!errs.isEmpty()) { QMessageBox::warning(this, "Saisie invalide", "- " + errs.join("\n- ")); return; }

    if (Seance::existe(id)) { QMessageBox::warning(this,"Ajout","ID déjà existant."); return; }

    if (!s.ajouter()) { QMessageBox::critical(this,"Ajout","Échec INSERT (voir logs)."); return; }

    rechargerTableSeancesDepuisDB();
    if (ui->idS) ui->idS->clear();
    if (ui->lieuS) ui->lieuS->clear();
    if (ui->prixS) ui->prixS->clear();
    QMessageBox::information(this, "Ajout", "Séance ajoutée.");
}

void MainWindow::on_pushButton_Modifier2_clicked()
{
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this,"BD","Connexion BD fermée. Lance d'abord la connexion.");
        return;
    }

    const QString id = ui->idS ? ui->idS->text().trimmed() : QString();
    if (id.isEmpty()) {
        QMessageBox::warning(this,"Modifier","Saisis l'ID à modifier dans le champ idS.");
        return;
    }

    if (!Seance::existe(id)) {
        QMessageBox::warning(this,"Modifier","ID inexistant en base.");
        return;
    }

    const QString col2 = ui->typeS   ? ui->typeS->currentText().trimmed() : QString();
    const QDate   d    = ui->dateS_2 ? ui->dateS_2->date()                : QDate();
    const QTime   hd   = ui->hDeb    ? ui->hDeb->time()                    : QTime();
    const QTime   ha   = ui->hArr    ? ui->hArr->time()                    : QTime();
    const QString lieu = ui->lieuS   ? ui->lieuS->text().trimmed()        : QString();
    const QString prixStr = ui->prixS ? ui->prixS->text().trimmed()        : QString();

    double prix = -1.0; // -1 => PRIX = NULL
    if (!prixStr.isEmpty()) {
        bool ok=false; double p=prixStr.toDouble(&ok);
        if (ok) prix = p;
    }

    Seance s(id, col2, d, hd, ha, lieu, prix);

    const QStringList errs = Seance::validate(s);
    if (!errs.isEmpty()) {
        QMessageBox::warning(this, "Saisie invalide", "- " + errs.join("\n- "));
        return;
    }

    if (!s.modifier()) {
        // Dernière erreur SQL utile en popup
        QSqlQuery last; // juste pour lire l'erreur globale du driver
        QMessageBox::critical(this, "Modifier",
                              "Échec de la mise à jour.\n"
                              "Vérifie que la table/colonnes existent (ex: ID_SEANCE, COLUMN2, DATE_SEANCE, HEURE_DEBUT, HEURE_ARRIVER, LIEU_DEPART, PRIX)\n"
                              "et que l'ID est correct.");
        return;
    }

    rechargerTableSeancesDepuisDB();
    QMessageBox::information(this, "Modifier", "Séance modifiée avec succès.");
}


void MainWindow::on_pushButton_Supprimer2_clicked()
{
    if (!QSqlDatabase::database().isOpen()) { QMessageBox::critical(this,"BD","Connexion BD fermée."); return; }

    const QString id = ui->lineEdit_ID_Supprimer ? ui->lineEdit_ID_Supprimer->text().trimmed() : QString();
    if (id.isEmpty()) { QMessageBox::warning(this,"Suppression","Saisis un ID."); return; }

    if (!Seance::existe(id)) { QMessageBox::warning(this,"Suppression","ID inexistant."); return; }

    if (QMessageBox::question(this,"Confirmation","Supprimer cette séance ?",
                              QMessageBox::Yes|QMessageBox::No) != QMessageBox::Yes) return;

    if (!Seance::supprimer(id)) {
        QMessageBox::critical(this,"Suppression","Échec DELETE (voir logs).");
        return;
    }

    rechargerTableSeancesDepuisDB();
    if (ui->lineEdit_ID_Supprimer) ui->lineEdit_ID_Supprimer->clear();
    QMessageBox::information(this,"Suppression","Séance supprimée.");
}


void MainWindow::on_pushButton_Rechercher2_clicked()
{
    if (!ui->tabS) { QMessageBox::warning(this,"Séance","Table tabS introuvable."); return; }

    const QString key = ui->lineEdit_Recherche ? ui->lineEdit_Recherche->text().trimmed() : QString();
    if (key.isEmpty()) { QMessageBox::information(this,"Recherche","Saisis un mot-clé (ID/Lieu)."); return; }

    bool foundAny = false;
    for (int i = 0; i < ui->tabS->rowCount(); ++i) {
        bool match = false;
        if (auto *it = ui->tabS->item(i,0)) match |= it->text().contains(key, Qt::CaseInsensitive);
        if (auto *it = ui->tabS->item(i,5)) match |= it->text().contains(key, Qt::CaseInsensitive);

        for (int j=0;j<ui->tabS->columnCount();++j) {
            if (auto *cell = ui->tabS->item(i,j))
                cell->setBackground(match ? QBrush(Qt::yellow) : QBrush());
        }
        if (match) foundAny = true;
    }
    if (!foundAny) QMessageBox::information(this,"Recherche","Aucun résultat.");
}

void MainWindow::on_pushButton_TrierSeance_clicked()
{
    if (ui->tabS) ui->tabS->sortItems(0, Qt::AscendingOrder); // tri par ID
}

void MainWindow::on_tabS_itemClicked(QTableWidgetItem *item)
{
    Q_UNUSED(item);
    if (!ui->tabS) return;
    const int row = ui->tabS->currentRow();
    if (row < 0) return;

    if (ui->idS)   ui->idS->setText(ui->tabS->item(row,0)->text());
    if (ui->typeS) ui->typeS->setCurrentText(ui->tabS->item(row,1)->text());

    if (ui->dateS_2) {
        QDate d = QDate::fromString(ui->tabS->item(row,2)->text(), "dd/MM/yyyy");
        if (d.isValid()) ui->dateS_2->setDate(d);
    }
    if (ui->hDeb) {
        QTime t = QTime::fromString(ui->tabS->item(row,3)->text(), "HH:mm");
        if (t.isValid()) ui->hDeb->setTime(t);
    }
    if (ui->hArr) {
        QTime t = QTime::fromString(ui->tabS->item(row,4)->text(), "HH:mm");
        if (t.isValid()) ui->hArr->setTime(t);
    }
    if (ui->lieuS) ui->lieuS->setText(ui->tabS->item(row,5)->text());
    if (ui->prixS) ui->prixS->setText(ui->tabS->item(row,6)->text());
}

void MainWindow::on_pb_annuler_ajout_materiel_clicked()
{
    if (ui->idS) ui->idS->clear();
    if (ui->lieuS) ui->lieuS->clear();
    if (ui->prixS) ui->prixS->clear();
    if (ui->typeS) ui->typeS->setCurrentIndex(0);
    if (ui->dateS_2) ui->dateS_2->setDate(QDate::currentDate());
    if (ui->hDeb) ui->hDeb->setTime(QTime(8,0));
    if (ui->hArr) ui->hArr->setTime(QTime(9,0));
}

// ===== Stubs vides pour compat (si ton .ui les utilise) =====
void MainWindow::on_candidat_clicked(){}         void MainWindow::on_candidat_2_clicked(){}
void MainWindow::on_candidat_3_clicked(){}       void MainWindow::on_candidat_4_clicked(){}
void MainWindow::on_candidat_5_clicked(){}       void MainWindow::on_vehicule_clicked(){}
void MainWindow::on_vehicule_2_clicked(){}       void MainWindow::on_vehicule_3_clicked(){}
void MainWindow::on_vehicule_4_clicked(){}       void MainWindow::on_vehicule_5_clicked(){}
void MainWindow::on_employee_clicked(){}         void MainWindow::on_employee_2_clicked(){}
void MainWindow::on_employee_3_clicked(){}       void MainWindow::on_employee_5_clicked(){}
void MainWindow::on_employee_6_clicked(){}       void MainWindow::on_seance_clicked(){}
void MainWindow::on_seance_2_clicked(){}         void MainWindow::on_seance_3_clicked(){}
void MainWindow::on_seance_5_clicked(){}         void MainWindow::on_seance_6_clicked(){}
void MainWindow::on_examen_clicked(){}           void MainWindow::on_examen_2_clicked(){}
void MainWindow::on_examen_3_clicked(){}         void MainWindow::on_examen_4_clicked(){}
void MainWindow::on_examen_5_clicked(){}         void MainWindow::on_pushButton_clicked(){}
void MainWindow::on_pushButton_2_clicked(){}     void MainWindow::on_pushButton_9_clicked(){}
void MainWindow::on_pushButton_Ajouter_clicked(){} void MainWindow::on_pushButton_supprimer_clicked(){}
void MainWindow::on_pushButton_54_clicked(){}    void MainWindow::on_btnStats_clicked(){}
void MainWindow::on_pushButton_Rechercher_clicked(){}
