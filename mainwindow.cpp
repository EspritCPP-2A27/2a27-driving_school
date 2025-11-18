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
#include <QFileDialog>
#include <QComboBox>
#include <QPushButton>
#include <QDebug>
#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFont>
#include <QMap>
// ---------------------------------------
static void setHeaders(QTableWidget* tabS)
{
    if (!tabS) return;
    QStringList headers;
    headers << "ID" << "Type" << "Date" << "Heure début" << "Heure arrivée" << "Lieu" << "Prix";
    tabS->setColumnCount(headers.size());
    tabS->setHorizontalHeaderLabels(headers);
    tabS->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabS->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabS->setSelectionMode(QAbstractItemView::SingleSelection);
    tabS->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabS->setSortingEnabled(true);
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

    // CONNEXION MANUELLE POUR LES STATISTIQUES - SOLUTION GARANTIE
    QPushButton* statsBtn = nullptr;

    // Essayer différents noms possibles
    QStringList possibleNames = {"btnStats", "pushButton_Stats", "statsButton", "pushButton_76", "pushButton_4"};

    for (const QString& name : possibleNames) {
        statsBtn = findChild<QPushButton*>(name);
        if (statsBtn) {
            qDebug() << "Bouton stats trouvé avec le nom:" << name;
            break;
        }
    }

    // Si aucun bouton trouvé par nom, chercher par texte
    if (!statsBtn) {
        QList<QPushButton*> allButtons = findChildren<QPushButton*>();
        for (QPushButton* btn : allButtons) {
            if (btn->text().contains("stat", Qt::CaseInsensitive) ||
                btn->text().contains("📊", Qt::CaseInsensitive)) {
                statsBtn = btn;
                qDebug() << "Bouton stats trouvé par texte:" << btn->text();
                break;
            }
        }
    }

    if (statsBtn) {
        connect(statsBtn, &QPushButton::clicked, this, &MainWindow::showStats);
        qDebug() << "✅ Connexion stats réussie!";
    } else {
        qDebug() << "❌ AUCUN bouton stats trouvé! Création d'un bouton manuel...";
        // Créer un bouton manuel en cas d'urgence
        QPushButton* manualStatsBtn = new QPushButton("Stats Manuel", this);
        manualStatsBtn->setGeometry(10, 10, 100, 30);
        connect(manualStatsBtn, &QPushButton::clicked, this, &MainWindow::showStats);
        manualStatsBtn->show();
    }

    // Connexion pour le bouton de tri
    QPushButton* btnTri = findChild<QPushButton*>("pushButton_3");
    if (btnTri) {
        connect(btnTri, &QPushButton::clicked, this, &MainWindow::on_pushButton_TrierSeance_clicked);
        qDebug() << "Bouton de tri connecté";
    }

    // Recharge si la base est déjà ouverte
    if (QSqlDatabase::database().isOpen())
        rechargerTableSeancesDepuisDB();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::initSeanceTable()
{
    if (ui->tabS && ui->tabS->columnCount() == 0)
        setHeaders(ui->tabS);

    // Activer le tri par clic sur les colonnes
    if (ui->tabS) {
        ui->tabS->setSortingEnabled(true);
        connect(ui->tabS->horizontalHeader(), &QHeaderView::sectionClicked,
                this, &MainWindow::on_headerSectionClicked);
    }
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

    const QString id   = ui->idS     ? ui->idS->text().trimmed()             : QString();
    const QString col2 = ui->typeS   ? ui->typeS->currentText().trimmed()    : QString();
    const QDate   d    = ui->dateS_2 ? ui->dateS_2->date()                   : QDate();
    const QTime   hd   = ui->hDeb    ? ui->hDeb->time()                       : QTime();
    const QTime   ha   = ui->hArr    ? ui->hArr->time()                       : QTime();
    const QString lieu = ui->lieuS   ? ui->lieuS->text().trimmed()           : QString();
    const QString prixStr = ui->prixS ? ui->prixS->text().trimmed()           : QString();

    double prix = -1.0;
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

    double prix = -1.0;
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
        QSqlQuery last;
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
    if (!ui->tabS) {
        QMessageBox::warning(this, "Séance", "Table tabS introuvable.");
        return;
    }

    const QString critere = ui->rechS ? ui->rechS->currentText() : QString();
    const QString key = ui->lineEdit_Recherche_2 ? ui->lineEdit_Recherche_2->text().trimmed() : QString();

    if (key.isEmpty()) {
        QMessageBox::information(this, "Recherche", "Saisis un mot-clé.");
        return;
    }

    if (critere.isEmpty()) {
        QMessageBox::information(this, "Recherche", "Sélectionne un critère de recherche.");
        return;
    }

    QSqlQueryModel* model = Seance::rechercher(key);

    if (!model) {
        QMessageBox::critical(this, "Recherche", "Erreur lors de la recherche.");
        return;
    }

    ui->tabS->setRowCount(0);
    setHeaders(ui->tabS);

    int rowCount = model->rowCount();
    for (int row = 0; row < rowCount; ++row) {
        ui->tabS->insertRow(row);
        for (int col = 0; col < 7; ++col) {
            QModelIndex index = model->index(row, col);
            QString value = model->data(index).toString();
            ui->tabS->setItem(row, col, new QTableWidgetItem(value));
        }
    }

    delete model;

    if (rowCount == 0) {
        QMessageBox::information(this, "Recherche", "Aucun résultat trouvé pour \"" + key + "\".");
    }
}

void MainWindow::on_pushButton_TrierSeance_clicked()
{
    if (!ui->tabS) {
        QMessageBox::warning(this, "Erreur", "Tableau non disponible.");
        return;
    }

    if (ui->tabS->rowCount() == 0) {
        QMessageBox::information(this, "Tri", "Aucune donnée à trier.");
        return;
    }

    QComboBox* comboBoxTri = findChild<QComboBox*>("pushButton_TrierSeance");
    if (!comboBoxTri) {
        QMessageBox::warning(this, "Erreur", "Liste de tri non trouvée.");
        return;
    }

    QString critere = comboBoxTri->currentText();
    if (critere.isEmpty()) {
        QMessageBox::information(this, "Tri", "Veuillez sélectionner un critère de tri.");
        return;
    }

    qDebug() << "Tri demandé avec critère:" << critere;

    QMap<QString, int> mapping;
    mapping["Heure_Debut"] = 3;
    mapping["Heure_Arriver"] = 4;
    mapping["ID"] = 0;
    mapping["Date"] = 2;
    mapping["Lieu"] = 5;
    mapping["Prix"] = 6;

    if (mapping.contains(critere)) {
        int colonne = mapping[critere];
        ui->tabS->setSortingEnabled(true);
        ui->tabS->sortByColumn(colonne, Qt::AscendingOrder);
        qDebug() << "Tri effectué sur la colonne:" << colonne;
        QMessageBox::information(this, "Succès", "Tri effectué avec succès !");
    } else {
        QMessageBox::warning(this, "Erreur", "Critère de tri non reconnu: " + critere);
    }
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

// ===== STATISTIQUES - SOLUTION GARANTIE =====

void MainWindow::showStats()
{
    qDebug() << "📊 Méthode showStats() appelée!";

    // Récupérer les statistiques
    int totalSeances = Seance::getTotalSeances();
    double revenuTotal = Seance::getRevenuTotal();
    QMap<QString, int> seancesParType = Seance::getSeancesParType();
    QMap<QString, int> seancesParMois = Seance::getSeancesParMois();

    if (totalSeances == 0) {
        QMessageBox::information(this, "Statistiques", "📊 Aucune séance trouvée !");
        return;
    }

    QString message = "📊 STATISTIQUES DES SÉANCES\n\n";
    message += QString("• Nombre total de séances : %1\n").arg(totalSeances);
    message += QString("• Revenu total : %1 €\n\n").arg(QString::number(revenuTotal, 'f', 2));

    if (!seancesParType.isEmpty()) {
        message += "SÉANCES PAR TYPE :\n";
        for (auto it = seancesParType.begin(); it != seancesParType.end(); ++it) {
            double pourcentage = (it.value() * 100.0) / totalSeances;
            message += QString("  - %1 : %2 (%3%)\n").arg(it.key()).arg(it.value()).arg(QString::number(pourcentage, 'f', 1));
        }
        message += "\n";
    }

    if (!seancesParMois.isEmpty()) {
        message += "SÉANCES PAR MOIS :\n";
        for (auto it = seancesParMois.begin(); it != seancesParMois.end(); ++it) {
            double pourcentage = (it.value() * 100.0) / totalSeances;
            message += QString("  - %1 : %2 séances (%3%)\n").arg(it.key()).arg(it.value()).arg(QString::number(pourcentage, 'f', 1));
        }
    }

    // Afficher dans une fenêtre
    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("Statistiques des Séances - Driving School");
    statsDialog->setMinimumSize(500, 400);

    QVBoxLayout *layout = new QVBoxLayout(statsDialog);

    QTextEdit *textEdit = new QTextEdit();
    textEdit->setPlainText(message);
    textEdit->setReadOnly(true);
    textEdit->setFont(QFont("Arial", 10));

    QPushButton *closeButton = new QPushButton("Fermer");
    connect(closeButton, &QPushButton::clicked, statsDialog, &QDialog::close);

    layout->addWidget(textEdit);
    layout->addWidget(closeButton);

    statsDialog->exec();
}

// Méthode originale gardée pour compatibilité
void MainWindow::on_btnStats_2_clicked()
{
    qDebug() << "Méthode on_btnStats_2_clicked() appelée";
    showStats(); // Appeler la même fonction
}

void MainWindow::on_headerSectionClicked(int logicalIndex)
{
    if (!ui->tabS) return;

    QString colonne;
    switch(logicalIndex) {
    case 0: colonne = "ID_SEANCE"; break;
    case 1: colonne = "COLUMN2"; break;
    case 2: colonne = "DATE_SEANCE"; break;
    case 3: colonne = "HEURE_DEBUT"; break;
    case 4: colonne = "HEURE_ARRIVER"; break;
    case 5: colonne = "LIEU_DEPART"; break;
    case 6: colonne = "PRIX"; break;
    default: colonne = "ID_SEANCE"; break;
    }

    rechargerTableSeancesAvecTri(colonne);
}

void MainWindow::rechargerTableSeancesAvecTri(const QString& colonneTri)
{
    if (!ui->tabS) return;

    QSqlQueryModel* model = Seance::trier(colonneTri, true);

    if (!model) {
        QMessageBox::critical(this, "Tri", "Erreur lors du tri.");
        return;
    }

    ui->tabS->setRowCount(0);
    setHeaders(ui->tabS);

    int rowCount = model->rowCount();
    for (int row = 0; row < rowCount; ++row) {
        ui->tabS->insertRow(row);
        for (int col = 0; col < 7; ++col) {
            QModelIndex index = model->index(row, col);
            QString value = model->data(index).toString();
            ui->tabS->setItem(row, col, new QTableWidgetItem(value));
        }
    }

    delete model;
}

void MainWindow::on_pushButton_76_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF",
                                                    "seances_export.pdf",
                                                    "Fichiers PDF (*.pdf)");

    if (fileName.isEmpty()) return;

    if (Seance::exporterPDF(fileName)) {
        QMessageBox::information(this, "Export PDF", "Exportation PDF réussie !\nFichier : " + fileName);
    } else {
        QMessageBox::critical(this, "Export PDF", "Erreur lors de l'exportation PDF.");
    }
}

// ===== Stubs vides =====
void MainWindow::on_candidat_clicked(){} void MainWindow::on_candidat_2_clicked(){}
void MainWindow::on_candidat_3_clicked(){} void MainWindow::on_candidat_4_clicked(){}
void MainWindow::on_candidat_5_clicked(){} void MainWindow::on_vehicule_clicked(){}
void MainWindow::on_vehicule_2_clicked(){} void MainWindow::on_vehicule_3_clicked(){}
void MainWindow::on_vehicule_4_clicked(){} void MainWindow::on_vehicule_5_clicked(){}
void MainWindow::on_employee_clicked(){} void MainWindow::on_employee_2_clicked(){}
void MainWindow::on_employee_3_clicked(){} void MainWindow::on_employee_5_clicked(){}
void MainWindow::on_employee_6_clicked(){} void MainWindow::on_seance_clicked(){}
void MainWindow::on_seance_2_clicked(){} void MainWindow::on_seance_3_clicked(){}
void MainWindow::on_seance_5_clicked(){} void MainWindow::on_seance_6_clicked(){}
void MainWindow::on_examen_clicked(){} void MainWindow::on_examen_2_clicked(){}
void MainWindow::on_examen_3_clicked(){} void MainWindow::on_examen_4_clicked(){}
void MainWindow::on_examen_5_clicked(){} void MainWindow::on_pushButton_clicked(){}
void MainWindow::on_pushButton_2_clicked(){} void MainWindow::on_pushButton_9_clicked(){}
void MainWindow::on_pushButton_Ajouter_clicked(){} void MainWindow::on_pushButton_supprimer_clicked(){}
void MainWindow::on_pushButton_54_clicked(){} void MainWindow::on_pushButton_Rechercher_clicked(){}
