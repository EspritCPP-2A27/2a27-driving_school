#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QLabel>

#include <QPixmap>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <QMessageBox>
#include <QBrush>
#include <QColor>

#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>

#include <QLineEdit>

#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QVBoxLayout>


#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>



#include <QSoundEffect>


#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableFormat>

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>


#include <QInputDialog>
#include <QTimer>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialog>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isRegisteringMode(false)
    , employeeToRegister("")
    , rfidWaitDialog(nullptr)
{
    ui->setupUi(this);

   chargerVehicules();
    rechargerTableSeancesDepuisDB();
   afficherEmployes();
    marquerDatesCalendrier();
   verifierAssurancesDepuisBase();
    // Connexion Arduino via TA classe Arduino
   int ret = A.connect_arduino();
   if (ret == 0) {
       qDebug() << "✔ Arduino connecté sur :" << A.getarduino_port_name();
       // Connecter le signal readyRead
       QObject::connect(A.getserial(), &QSerialPort::readyRead,
                        this, &MainWindow::readArduino);
   } else {
       qDebug() << "❌ Arduino non trouvé";
   }
   connect(ui->tableWidget_Employe, &QTableWidget::cellClicked,
           this, &MainWindow::on_tableWidget_Employe_cellClicked);

   // Pour double-clic aussi (optionnel)
   connect(ui->tableWidget_Employe, &QTableWidget::cellDoubleClicked,
           this, &MainWindow::on_tableWidget_Employe_cellClicked);
}







MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_candidat_clicked()
{


    ui->tabWidget->setCurrentIndex(2);

}


void MainWindow::on_vehicule_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}


void MainWindow::on_employee_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}


void MainWindow::on_seance_clicked()
{
    ui->tabWidget->setCurrentIndex(5);
}


void MainWindow::on_examen_clicked()
{
    ui->tabWidget->setCurrentIndex(6);
}


void MainWindow::on_candidat_2_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}



void MainWindow::on_vehicule_2_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}


void MainWindow::on_employee_2_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}


void MainWindow::on_seance_2_clicked()
{
    ui->tabWidget->setCurrentIndex(5);
}


void MainWindow::on_examen_2_clicked()
{
    ui->tabWidget->setCurrentIndex(6);
}


void MainWindow::on_candidat_3_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}


void MainWindow::on_vehicule_3_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}


void MainWindow::on_employee_3_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}


void MainWindow::on_seance_3_clicked()
{
    ui->tabWidget->setCurrentIndex(5);
}


void MainWindow::on_examen_3_clicked()
{
    ui->tabWidget->setCurrentIndex(6);
}


void MainWindow::on_candidat_4_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}


void MainWindow::on_vehicule_4_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}


void MainWindow::on_employee_6_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}


void MainWindow::on_seance_6_clicked()
{
    ui->tabWidget->setCurrentIndex(5);
}


void MainWindow::on_examen_4_clicked()
{
    ui->tabWidget->setCurrentIndex(6);
}


void MainWindow::on_candidat_5_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}


void MainWindow::on_vehicule_5_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}


void MainWindow::on_employee_5_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}


void MainWindow::on_seance_5_clicked()
{
    ui->tabWidget->setCurrentIndex(5);
}


void MainWindow::on_examen_5_clicked()
{
    ui->tabWidget->setCurrentIndex(6);
}


void MainWindow::on_pushButton_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_9_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}


void MainWindow::chargerVehicules()
{
    ui->tableWidget_Vehicule->setRowCount(0);

    QSqlQuery query("SELECT MATRICULE, TYPE, ETAT_VOITURE, KILOMETRAGE, "  "TO_CHAR(DATE_MAINTENANCE, 'DD/MM/YYYY'), ASSURANCE, DISPONIBILITE, "  "TO_CHAR(DATE_ASSURANCE, 'DD/MM/YYYY') " "FROM VEHICULE");

    int row = 0;
    while (query.next()) {

        ui->tableWidget_Vehicule->insertRow(row);
        for (int col = 0; col < 8; ++col) {

            ui->tableWidget_Vehicule->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
}

void MainWindow::rechargerTableSeancesDepuisDB()
{
    if (!ui->tabS) return;
    ui->tabS->setRowCount(0);

    QSqlQuery query("SELECT ID_SEANCE, TYPE_SEANCE, TO_CHAR(DATE_SEANCE,'DD/MM/YYYY'), ""TO_CHAR(HEURE_DEBUT,'HH24:MI'), TO_CHAR(HEURE_ARRIVER,'HH24:MI'), ""LIEU_DEPART, PRIX FROM SEANCE");

    int row = 0;
    while (query.next()) {
        ui->tabS->insertRow(row);
        for (int col = 0; col < 7; ++col) {
            ui->tabS->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
}

void MainWindow::afficherEmployes()
{
    // Modifiez la requête pour inclure entree et sortie
    QSqlQuery query("SELECT ID_EMPLOYE, NOM, PRENOM, ROLE, NUMTEL, EMAIL, SPECIALITE, SALAIRE, MDP, RFID_UID, entree, sortie FROM EMPLOYE");

    ui->tableWidget_Employe->setRowCount(0);
    ui->tableWidget_Employe->setColumnCount(12); // 12 colonnes maintenant

    int row = 0;
    while (query.next()) {
        ui->tableWidget_Employe->insertRow(row);

        // Colonnes 0-8: Informations de base
        for (int col = 0; col < 9; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            item->setFlags(item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableWidget_Employe->setItem(row, col, item);
        }

        // Colonne 9: RFID UID
        QString rfidUID = query.value(9).toString();
        QTableWidgetItem *rfidItem = new QTableWidgetItem(
            rfidUID.isEmpty() ? "Non enregistré" : rfidUID
            );
        if (!rfidUID.isEmpty()) {
            rfidItem->setBackground(QColor(200, 255, 200)); // Vert clair
        }
        rfidItem->setFlags(rfidItem->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget_Employe->setItem(row, 9, rfidItem);

        // Colonne 10: Entrée
        int entree = query.value(10).toInt();
        QTableWidgetItem *entreeItem = new QTableWidgetItem(QString::number(entree));
        entreeItem->setTextAlignment(Qt::AlignCenter);
        entreeItem->setFlags(entreeItem->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        // Colorer en vert si > 0
        if (entree > 0) {
            entreeItem->setBackground(QColor(220, 255, 220)); // Vert très clair
        }
        ui->tableWidget_Employe->setItem(row, 10, entreeItem);

        // Colonne 11: Sortie
        int sortie = query.value(11).toInt();
        QTableWidgetItem *sortieItem = new QTableWidgetItem(QString::number(sortie));
        sortieItem->setTextAlignment(Qt::AlignCenter);
        sortieItem->setFlags(sortieItem->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        // Colorer en rouge si > 0
        if (sortie > 0) {
            sortieItem->setBackground(QColor(255, 220, 220)); // Rouge très clair
        }
        ui->tableWidget_Employe->setItem(row, 11, sortieItem);

        row++;
    }

    // Définir les en-têtes
    QStringList headers;
    headers << "ID" << "Nom" << "Prénom" << "Rôle" << "Téléphone"
            << "Email" << "Spécialité" << "Salaire" << "Mot de passe"
            << "RFID UID" << "Entrée" << "Sortie";
    ui->tableWidget_Employe->setHorizontalHeaderLabels(headers);

    // Ajuster la largeur des colonnes
    ui->tableWidget_Employe->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    qDebug() << "Tableau employés mis à jour avec" << row << "employés";
}

void MainWindow::chargerCandidats()
{
    QSqlQuery query("SELECT * FROM CANDIDAT");
    ui->tableWidget_Candidat->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->tableWidget_Candidat->insertRow(row);
        for (int col = 0; col < 6; ++col) {
            ui->tableWidget_Candidat->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
}


void MainWindow::verifierAssurancesDepuisBase()
{
    QSqlQuery query("SELECT MATRICULE, DATE_ASSURANCE FROM VEHICULE");

    while (query.next()) {
        QString matricule = query.value(0).toString();
        QDate dateAssurance = query.value(1).toDate();

        verifierDateAssuranceEtNotifier(matricule, dateAssurance);
    }
}



void MainWindow::on_pushButton_Ajouter_clicked()
{

        QString matricule = ui->matricule->text().trimmed();
        QString type = ui->comboBox_TypeVehicule->currentText();
 QString etat;
        QString dispo = ui->comboBox_dipo->currentText();
        QString km = ui->lineEdit_Kilometrage->text().trimmed();
        QString assurance;
        QDate dateMaintenance = ui->dateEdit_Maintenance->date();
        QDate dateAssurance = ui->date_assurance->date();
        QDate today = QDate::currentDate();

        if (ui->radioButton_EnPanne->isChecked())
            etat = "En panne";
        else
            etat = "Fonctionnelle";

        if (ui->radioButton_TousRisque->isChecked())
            assurance = "Tous Risque";
        else if (ui->radioButton_Intermediaire->isChecked())
            assurance = "Intermediaire";
        else
            assurance = "Responsabilite civile RC";

        if (matricule.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer un matricule.");
            return;
        }
        QRegularExpression regex("^[0-9]+ nt [0-9]+$");
        if (!regex.match(matricule).hasMatch()) {
            QMessageBox::warning(this, "Erreur", "Le matricule doit être au format : '123 nt 6450'.");
            return;
        }


        if (dateMaintenance < today) {
            QMessageBox::warning(this, "Erreur", "La date de maintenance ne peut pas être avant aujourd'hui.");
            return;
        }
        if (dateAssurance < today) {
            QMessageBox::warning(this, "Erreur", "La date d'assurance ne peut pas être avant aujourd'hui.");
            return;
        }


        bool kmOk;
        int kmNum = km.toInt(&kmOk);
        if (!kmOk) {
            QMessageBox::warning(this, "Erreur", "Le kilométrage doit être un nombre.");
            return;
        }


        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM VEHICULE WHERE MATRICULE = :mat");
        checkQuery.bindValue(":mat", matricule);

        if (!checkQuery.exec()) {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la vérification : " + checkQuery.lastError().text());
            return;
        }

        checkQuery.next();
        if (checkQuery.value(0).toInt() > 0) {
            QMessageBox::warning(this, "Erreur", "Un véhicule avec ce matricule existe déjà !");
            return;
        }


        QSqlQuery query;
        query.prepare(R"( INSERT INTO VEHICULE (MATRICULE, TYPE, ETAT_VOITURE, KILOMETRAGE, DATE_MAINTENANCE, ASSURANCE, DISPONIBILITE, DATE_ASSURANCE)VALUES (:mat, :type, :etat, :km, TO_DATE(:dateM, 'DD/MM/YYYY'), :ass, :dispo, TO_DATE(:dateA, 'DD/MM/YYYY')))");

        query.bindValue(":mat", matricule);
        query.bindValue(":type", type);
        query.bindValue(":etat", etat);
        query.bindValue(":km", kmNum);
        query.bindValue(":dateM", dateMaintenance.toString("dd/MM/yyyy"));
        query.bindValue(":ass", assurance);
        query.bindValue(":dispo", dispo);
        query.bindValue(":dateA", dateAssurance.toString("dd/MM/yyyy"));

        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Véhicule ajouté avec succès !");

            int row = ui->tableWidget_Vehicule->rowCount();
            ui->tableWidget_Vehicule->insertRow(row);
            ui->tableWidget_Vehicule->setItem(row, 0, new QTableWidgetItem(matricule));

            ui->tableWidget_Vehicule->setItem(row, 1, new QTableWidgetItem(type));

            ui->tableWidget_Vehicule->setItem(row, 2, new QTableWidgetItem(etat));
            ui->tableWidget_Vehicule->setItem(row, 3, new QTableWidgetItem(QString::number(kmNum)));

            ui->tableWidget_Vehicule->setItem(row, 4, new QTableWidgetItem(dateMaintenance.toString("dd/MM/yyyy")));

            ui->tableWidget_Vehicule->setItem(row, 5, new QTableWidgetItem(assurance));

            ui->tableWidget_Vehicule->setItem(row, 6, new QTableWidgetItem(dispo));

            ui->tableWidget_Vehicule->setItem(row, 7, new QTableWidgetItem(dateAssurance.toString("dd/MM/yyyy")));

            verifierDateAssuranceEtNotifier(matricule,dateAssurance);

            marquerDatesCalendrier();


            ui->lineEdit_Kilometrage->clear();
            ui->matricule->clear();
            ui->matricule->setReadOnly(false);
        } else {
            QMessageBox::critical(this, "Erreur", "Insertion échouée : " + query.lastError().text());
        }
}





void MainWindow::on_pushButton_supprimer_clicked()
{
        QString matriculeASupprimer = ui->lineEdit_supprimer->text().trimmed();
        if (matriculeASupprimer.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer un matricule a supprimer ");
            return;
        }

        QSqlQuery query;
        query.prepare("DELETE FROM VEHICULE WHERE MATRICULE = :mat");
        query.bindValue(":mat", matriculeASupprimer);

        if (!query.exec()) {

            QMessageBox::critical(this, "Erreur", "Erreur SQL : " + query.lastError().text());
            return;
        }

        if (query.numRowsAffected() == 0) {

            QMessageBox::information(this, "Info", "Aucun vehicule trouve avec ce matricule ");
            return;
        }


        for (int row = 0; row < ui->tableWidget_Vehicule->rowCount(); ++row) {

            QTableWidgetItem *item = ui->tableWidget_Vehicule->item(row, 0);

            if (item && item->text() == matriculeASupprimer) {

                ui->tableWidget_Vehicule->removeRow(row);
                break;
            }
        }

        QMessageBox::information(this, "Succes", "Vehicule supprime avec succes ");
        ui->lineEdit_supprimer->clear();


}



void MainWindow::on_pushButton_54_clicked()
{
    ui->tabWidget->setCurrentIndex(0);

}


void MainWindow::on_btnStats_clicked()
{
        int total = ui->tableWidget_Vehicule->rowCount();
        if (total == 0) {
            QMessageBox::information(this, "Statistiques", "🚗 Aucun véhicule trouvé !");
            return;
        }

        int fonctionnelles = 0;
        int enPanne = 0;
        int colEtat = 2;

        for (int i = 0; i < total; ++i) {
            QString etat = ui->tableWidget_Vehicule->item(i, colEtat)->text().trimmed();

            if (etat.compare("Fonctionnelle", Qt::CaseInsensitive) == 0)
                fonctionnelles++;
            else if (etat.compare("En panne", Qt::CaseInsensitive) == 0)
                enPanne++;
        }


        double pourcFonct = (fonctionnelles * 100.0) / total;
        double pourcPanne = (enPanne * 100.0) / total;

        QPieSeries *series = new QPieSeries();

        series->append(QString("Fonctionnelles : %1 (%2%)").arg(fonctionnelles).arg(QString::number(pourcFonct, 'f', 1)), fonctionnelles);
        series->append(QString("En panne : %1 (%2%)").arg(enPanne).arg(QString::number(pourcPanne, 'f', 1)), enPanne);


        QPieSlice *slice1 = series->slices().at(0);
        slice1->setExploded(true);
        slice1->setLabelVisible(true);
        slice1->setPen(QPen(Qt::black));
        slice1->setBrush(Qt::green);

        QPieSlice *slice2 = series->slices().at(1);
        slice2->setLabelVisible(true);
        slice2->setPen(QPen(Qt::black));
        slice2->setBrush(Qt::red);


        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle(QString("📊 Statistiques des véhicules — Total : %1").arg(total));
        chart->legend()->setAlignment(Qt::AlignBottom);
        chart->setAnimationOptions(QChart::SeriesAnimations);


        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->resize(600, 400);


        QWidget *window = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout(window);
        layout->addWidget(chartView);
        window->setLayout(layout);
        window->setWindowTitle("Statistiques véhicules");
        window->resize(700, 500);
        window->show();
    }



void MainWindow::on_pushButton_Rechercher_clicked()
{
    QString searchMatricule = ui->lineEdit_Recherche->text().trimmed();

    if (searchMatricule.isEmpty()) {

        QMessageBox::warning(this, "Erreur", "Veuillez entrer un matricule a rechercher ");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT MATRICULE, TYPE, ETAT_VOITURE, KILOMETRAGE, " "TO_CHAR(DATE_MAINTENANCE, 'DD/MM/YYYY'), ASSURANCE, DISPONIBILITE, " "TO_CHAR(DATE_ASSURANCE, 'DD/MM/YYYY') " "FROM VEHICULE WHERE UPPER(MATRICULE) = UPPER(:mat)");

    query.bindValue(":mat", searchMatricule);

    if (!query.exec()) {

        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    if (!query.next()) {

        QMessageBox::information(this, "Resultat", "aucun vehicule trouve avec ce matricule ");
        return;
    }

    chargerVehicules();

    bool found = false;
    for (int i = 0; i < ui->tableWidget_Vehicule->rowCount(); ++i) {
        QTableWidgetItem *item = ui->tableWidget_Vehicule->item(i, 0);

        if (item && item->text().compare(searchMatricule, Qt::CaseInsensitive) == 0) {
            found = true;

            for (int j = 0; j < ui->tableWidget_Vehicule->columnCount(); ++j) {
                ui->tableWidget_Vehicule->item(i, j)->setBackground(QBrush(QColor("lightgreen")));
            }


            ui->tableWidget_Vehicule->scrollToItem(item);

            QTimer::singleShot(20000, [this, i]() {
                for (int j = 0; j < ui->tableWidget_Vehicule->columnCount(); ++j) {
                    ui->tableWidget_Vehicule->item(i, j)->setBackground(Qt::white);
                }
            });

            break;
        }
    }

    if (!found)
        QMessageBox::information(this, "Résultat", "Véhicule trouvé dans la base mais non affiché dans le tableau.");

    ui->lineEdit_Recherche->clear();

}

void MainWindow::on_pushButton_trier_clicked()
{
    QString critere = ui->comboBox_Trier->currentText();

    if (critere != "Etat" && critere != "Disponibilite") {
        QMessageBox::warning(this, "Erreur", "Veuillez choisir un critère de tri.");
        return;
    }

    QString requete;

    if (critere == "Etat") {
        requete ="SELECT MATRICULE, TYPE, ETAT_VOITURE, KILOMETRAGE, ""TO_CHAR(DATE_MAINTENANCE,'DD/MM/YYYY'), ASSURANCE, DISPONIBILITE, ""TO_CHAR(DATE_ASSURANCE,'DD/MM/YYYY') ""FROM VEHICULE WHERE ETAT_VOITURE = 'Fonctionnelle'";
    }
    else if (critere == "Disponibilite") {
        requete ="SELECT MATRICULE, TYPE, ETAT_VOITURE, KILOMETRAGE, ""TO_CHAR(DATE_MAINTENANCE,'DD/MM/YYYY'), ASSURANCE, DISPONIBILITE, ""TO_CHAR(DATE_ASSURANCE,'DD/MM/YYYY') ""FROM VEHICULE WHERE DISPONIBILITE = 'OUI'";
    }


    QSqlQuery query(QSqlDatabase::database());

    if (!query.exec(requete)) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    ui->tableWidget_Vehicule->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->tableWidget_Vehicule->insertRow(row);

        for (int col = 0; col < 8; col++) {
            ui->tableWidget_Vehicule->setItem(
                row, col,
                new QTableWidgetItem(query.value(col).toString())
                );
        }

        row++;
    }

    QMessageBox::information(this, "Tri appliqué",QString("Affichage filtré selon : %1").arg(critere));


    QTimer::singleShot(20000, this, [this]() {
        chargerVehicules();
    });
}







void MainWindow::on_pushButton_55_clicked()
{
     ui->tabWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_56_clicked()
{
     ui->tabWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_57_clicked()
{
     ui->tabWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_58_clicked()
{
     ui->tabWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_105_clicked()
{
    QString matricule = ui->matricule->text().trimmed();
    if (matricule.isEmpty()) {

        QMessageBox::warning(this, "Erreur", "veuillez entrer le matricule du vehicule a modifier ");
        return;
    }
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM VEHICULE WHERE UPPER(MATRICULE) = UPPER(:mat)");
    checkQuery.bindValue(":mat", matricule);
    if (!checkQuery.exec()) {

        QMessageBox::critical(this, "Erreur SQL", checkQuery.lastError().text());
        return;
    }
    checkQuery.next();

    if (checkQuery.value(0).toInt() == 0) {

        QMessageBox::warning(this, "Erreur", "Aucun véhicule trouvé avec ce matricule !");
        return;
    }

    QString type = ui->comboBox_TypeVehicule->currentText();

    QString etat = ui->radioButton_EnPanne->isChecked() ? "En panne" : "Fonctionnelle";

    QString dispo =   ui->comboBox_dipo->currentText();
    QString kilometrage = ui->lineEdit_Kilometrage->text();

    QString dateMaintenance = ui->dateEdit_Maintenance->date().toString("dd/MM/yyyy");

    QString assurance;
    if (ui->radioButton_TousRisque->isChecked()) assurance = "Tous Risque";

    else if (ui->radioButton_Intermediaire->isChecked()) assurance = "Intermediaire";

    else assurance = "Responsabilite civile RC";

    QString dateAssurance = ui->date_assurance->date().toString("dd/MM/yyyy");

    QSqlQuery query;
    query.prepare("UPDATE VEHICULE SET TYPE=:type, ETAT_VOITURE=:etat, ""KILOMETRAGE=:km, DATE_MAINTENANCE=TO_DATE(:dm, 'DD/MM/YYYY'), ""ASSURANCE=:ass, DISPONIBILITE=:dispo, DATE_ASSURANCE=TO_DATE(:da, 'DD/MM/YYYY') ""WHERE MATRICULE=:mat");

    query.bindValue(":mat", matricule);

    query.bindValue(":type", type);

    query.bindValue(":etat", etat);

    query.bindValue(":km", kilometrage);

    query.bindValue(":dm", dateMaintenance);
    query.bindValue(":ass", assurance);

    query.bindValue(":dispo", dispo);

    query.bindValue(":da", dateAssurance);

    if (query.exec()) {

        QMessageBox::information(this, "Succès", "Véhicule modifié avec succès !");
        chargerVehicules();

    } else {

        QMessageBox::critical(this, "Erreur", "Échec de la modification : " + query.lastError().text());
    }
}


void MainWindow::on_pushButton_Annuler_clicked()
{

        ui->matricule->clear();
       ui->matricule->setReadOnly(false);

        ui->lineEdit_Kilometrage->clear();
        ui->comboBox_TypeVehicule->setCurrentIndex(0);

        ui->comboBox_dipo->setCurrentIndex(0);

        ui->radioButton_Fonctionnelle->setChecked(true);

        ui->radioButton_TousRisque->setChecked(true);

        ui->dateEdit_Maintenance->setDate(QDate::currentDate());

        ui->date_assurance->setDate(QDate::currentDate());


}


void MainWindow::on_pushButton_53_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
    if (filePath.isEmpty()) return;

    QPdfWriter pdf(filePath);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);

    QPainter painter(&pdf);
    painter.setRenderHint(QPainter::Antialiasing);

    // ---- Layout values ----
    int marginLeft = 60;
    int marginTop = 100;
    int rowHeight = 70;
    int headerHeight = 40;
    int tableWidth = pdf.width() - marginLeft * 3;

    // ---- Column widths ----
    QVector<int> colWidths;
    int colCount = ui->tableWidget_Vehicule->columnCount();
    int equalWidth = tableWidth / colCount;

    for (int i = 0; i < colCount; i++)
        colWidths.push_back(equalWidth);

    // ---- TITLE (FULLY VISIBLE NOW) ----
    painter.setFont(QFont("Arial", 28, QFont::Bold));
    painter.drawText(
        QRect(0, marginTop, pdf.width(), 120),  // MUCH BIGGER rectangle
        Qt::AlignCenter,
        "Liste des véhicules"
        );

    // spacing (~4 lines)
    marginTop += 120;   // +120 px below title (equivalent ~4 lines)

    // ---- DATE ----
    painter.setFont(QFont("Arial", 12));
    painter.drawText(marginLeft, marginTop, "Date d’export : " + QDate::currentDate().toString("dd/MM/yyyy"));

    // extra space before table (more clean)
    marginTop += 40;

    // ---- HEADER BACKGROUND ----
    painter.setBrush(QColor("#E8E8E8"));
    painter.setPen(Qt::NoPen);
    painter.drawRect(marginLeft, marginTop, tableWidth, headerHeight);

    // ---- HEADER TEXT ----
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 11, QFont::Bold));

    int x = marginLeft;
    for (int j = 0; j < colCount; j++) {
        painter.drawText(
            QRect(x + 3, marginTop + 5, colWidths[j], headerHeight),
            Qt::AlignLeft | Qt::AlignVCenter,
            ui->tableWidget_Vehicule->horizontalHeaderItem(j)->text()
            );
        x += colWidths[j];
    }

    // line under header
    painter.setPen(QPen(Qt::black, 1));
    painter.drawLine(marginLeft, marginTop + headerHeight, marginLeft + tableWidth, marginTop + headerHeight);

    marginTop += headerHeight;

    // ---- TABLE ROWS ----
    painter.setFont(QFont("Arial", 10));

    for (int i = 0; i < ui->tableWidget_Vehicule->rowCount(); i++)
    {
        // Zebra shading
        if (i % 2 == 0)
        {
            painter.setBrush(QColor("#F7F7F7"));
            painter.setPen(Qt::NoPen);
            painter.drawRect(marginLeft, marginTop, tableWidth, rowHeight);
        }

        painter.setPen(Qt::black);

        int xPos = marginLeft;
        for (int j = 0; j < colCount; j++)
        {
            QString text = ui->tableWidget_Vehicule->item(i, j)
            ? ui->tableWidget_Vehicule->item(i, j)->text()
            : "";

            painter.drawText(
                QRect(xPos + 3, marginTop + 5, colWidths[j], rowHeight),
                Qt::AlignLeft | Qt::AlignVCenter,
                text
                );

            xPos += colWidths[j];
        }

        // Border line under row
        painter.setPen(QPen(Qt::gray, 1));
        painter.drawLine(marginLeft, marginTop + rowHeight, marginLeft + tableWidth, marginTop + rowHeight);

        marginTop += rowHeight;

        // New page if needed
        if (marginTop > pdf.height() - 100)
        {
            pdf.newPage();
            marginTop = 100;
        }
    }

    painter.end();
    QMessageBox::information(this, "PDF", "Le fichier a été exporté avec succès !");
}



void MainWindow::on_tableWidget_Vehicule_cellClicked(int row, int column)
{

        QString matricule = ui->tableWidget_Vehicule->item(row, 0)->text();

        QString type = ui->tableWidget_Vehicule->item(row, 1)->text();

        QString etat = ui->tableWidget_Vehicule->item(row, 2)->text();

        QString kilometrage = ui->tableWidget_Vehicule->item(row, 3)->text();

        QString dateMaintenance = ui->tableWidget_Vehicule->item(row, 4)->text();

        QString assurance = ui->tableWidget_Vehicule->item(row, 5)->text();

        QString dispo = ui->tableWidget_Vehicule->item(row, 6)->text();

        QString dateAssurance = ui->tableWidget_Vehicule->item(row, 7)->text();

        ui->matricule->setText(matricule);
        ui->matricule->setReadOnly(true);

        int typeIndex = ui->comboBox_TypeVehicule->findText(type);
        if (typeIndex != -1) ui->comboBox_TypeVehicule->setCurrentIndex(typeIndex);

        if (etat == "En panne")
            ui->radioButton_EnPanne->setChecked(true);
        else
            ui->radioButton_Fonctionnelle->setChecked(true);

        ui->lineEdit_Kilometrage->setText(kilometrage);

        QDate dateM = QDate::fromString(dateMaintenance, "dd/MM/yyyy");
        ui->dateEdit_Maintenance->setDate(dateM);

        if (assurance == "Tous Risque")
            ui->radioButton_TousRisque->setChecked(true);
        else if (assurance == "Intermediaire")
            ui->radioButton_Intermediaire->setChecked(true);
        else
            ui->radioButton_ResponsabiliteCivile->setChecked(true);

        int dispoIndex = ui->comboBox_dipo->findText(dispo);
        if (dispoIndex != -1) ui->comboBox_dipo->setCurrentIndex(dispoIndex);

        QDate dateA = QDate::fromString(dateAssurance, "dd/MM/yyyy");
        ui->date_assurance->setDate(dateA);


}


void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    QString dateStr = date.toString("dd/MM/yyyy");

    QSqlQuery query;
    query.prepare(R"( SELECT MATRICULE, TO_CHAR(DATE_MAINTENANCE, 'DD/MM/YYYY'), TO_CHAR(DATE_ASSURANCE, 'DD/MM/YYYY')FROM VEHICULE)");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la lecture de la base:\n" + query.lastError().text());
        return;
    }

    QStringList infoTrouvee;
    while (query.next()) {
        QString matricule = query.value(0).toString();
        QString dateMaintenance = query.value(1).toString();
        QString dateAssurance = query.value(2).toString();

        if (dateMaintenance == dateStr)
            infoTrouvee << QString("🚗 %1 → Date de maintenance").arg(matricule);

        if (dateAssurance == dateStr)
            infoTrouvee << QString("🚙 %1 → Date d’assurance").arg(matricule);
    }

    if (infoTrouvee.isEmpty()) {
        QMessageBox::information(this, "Info", QString("Aucun véhicule trouvé le %1").arg(dateStr));
    } else {
        QMessageBox::information(this, "Véhicules trouvés", infoTrouvee.join("\n"));
    }
}


void MainWindow::marquerDatesCalendrier()
{
    QSqlQuery query("SELECT TO_CHAR(DATE_MAINTENANCE, 'DD/MM/YYYY'), TO_CHAR(DATE_ASSURANCE, 'DD/MM/YYYY') FROM VEHICULE");

    QTextCharFormat format;
    format.setBackground(Qt::green);
    format.setForeground(Qt::black);

    while (query.next()) {
        QString dateMaintenance = query.value(0).toString();
        QString dateAssurance = query.value(1).toString();

        QDate d1 = QDate::fromString(dateMaintenance, "dd/MM/yyyy");
        QDate d2 = QDate::fromString(dateAssurance, "dd/MM/yyyy");

        if (d1.isValid())
            ui->calendarWidget->setDateTextFormat(d1, format);

        if (d2.isValid())
            ui->calendarWidget->setDateTextFormat(d2, format);
    }
}

void MainWindow::afficherToast(QString message)
{
    QWidget *toast = new QWidget(this);

    toast->setStyleSheet("background-color: white;""color: black;""padding: 20px;""border-color: rgb(0, 0, 0);""border-radius: 15px;""font-size: 14px;");

    QLabel *label = new QLabel(message, toast);
    QVBoxLayout *layout = new QVBoxLayout(toast);
    layout->addWidget(label);
    toast->setLayout(layout);

    toast->adjustSize();
    toast->move(width()/2 - toast->width()/2, height() - 120);
    toast->show();


    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(toast);
    toast->setGraphicsEffect(effect);

    QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity");
    anim->setDuration(10000);
    anim->setStartValue(0.0);
    anim->setKeyValueAt(0.1, 1.0);
    anim->setKeyValueAt(0.8, 1.0);
    anim->setEndValue(0.0);

    connect(anim, &QPropertyAnimation::finished, toast, &QWidget::deleteLater);
    anim->start(QPropertyAnimation::DeleteWhenStopped);
}
void MainWindow::jouerSonNotification()
{
    QSoundEffect *son = new QSoundEffect(this);
    son->setSource(QUrl("qrc:/sons/notif.wav"));
    son->setVolume(0.50);
    son->play();
}
void MainWindow::verifierDateAssuranceEtNotifier(const QString &matricule, const QDate &dateAssurance)
{
    QDate today = QDate::currentDate();
    int joursRestants = today.daysTo(dateAssurance);

    if (joursRestants <= 7 && joursRestants >= 0) {
        QString msg = QString("⚠️ L’assurance du véhicule %1 doit être renouvelée dans %2 jour(s) !").arg(matricule).arg(joursRestants);

        afficherToast(msg);
        jouerSonNotification();
    }
}


//ajouter seance

void MainWindow::on_pushButton_75_clicked()
{

        QString id    = ui->idS->text().trimmed();
        QString type  = ui->typeS->currentText().trimmed();
        QDate   date  = ui->dateS_2->date();
        QTime   hDeb  = ui->hDeb->time();
        QTime   hArr  = ui->hArr->time();
        QString lieu  = ui->lieuS->text().trimmed();
        QString prixS = ui->prixS->text().trimmed();

        if (id.isEmpty()) { QMessageBox::warning(this,"Erreur","ID requis."); return; }
        if (lieu.isEmpty()) { QMessageBox::warning(this,"Erreur","Lieu requis."); return; }

        double prix = prixS.toDouble();

        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM SEANCE WHERE ID_SEANCE = :id");
        checkQuery.bindValue(":id", id);
        if (!checkQuery.exec() || checkQuery.next() && checkQuery.value(0).toInt() > 0) {
            QMessageBox::warning(this,"Erreur","ID déjà existant."); return;
        }

        QSqlQuery query;
        // SOLUTION : Utiliser le format DATE Oracle standard ou format ISO
        query.prepare(R"(INSERT INTO SEANCE (ID_SEANCE, TYPE_SEANCE, DATE_SEANCE, HEURE_DEBUT, HEURE_ARRIVER, LIEU_DEPART, PRIX)
                     VALUES (:id, :type, TO_DATE(:date, 'YYYY-MM-DD'), TO_DATE(:hDeb, 'HH24:MI'), TO_DATE(:hArr, 'HH24:MI'), :lieu, :prix))");
        query.bindValue(":id", id);
        query.bindValue(":type", type);
        query.bindValue(":date", date.toString("yyyy-MM-dd")); // Format ISO
        query.bindValue(":hDeb", hDeb.toString("HH:mm"));
        query.bindValue(":hArr", hArr.toString("HH:mm"));
        query.bindValue(":lieu", lieu);
        query.bindValue(":prix", prix);

        if (query.exec()) {
            QMessageBox::information(this,"Succès","Séance ajoutée !");
            rechargerTableSeancesDepuisDB();
            ui->idS->clear();
            ui->lieuS->clear();
            ui->prixS->clear();
        } else {
            QMessageBox::critical(this,"Erreur","Échec INSERT : " + query.lastError().text());
        }

}


// Modifier séance
void MainWindow::on_pushButton_146_clicked()
{
        QString id    = ui->idS->text().trimmed();
        if (id.isEmpty()) { QMessageBox::warning(this,"Erreur","ID requis."); return; }

        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM SEANCE WHERE ID_SEANCE = :id");
        checkQuery.bindValue(":id", id);
        checkQuery.exec(); checkQuery.next();
        if (checkQuery.value(0).toInt() == 0) { QMessageBox::warning(this,"Erreur","ID inexistant."); return; }

        QString type  = ui->typeS->currentText().trimmed();
        QDate date    = ui->dateS_2->date();
        QTime hDeb    = ui->hDeb->time();
        QTime hArr    = ui->hArr->time();
        QString lieu  = ui->lieuS->text().trimmed();
        double prix   = ui->prixS->text().toDouble();

        QSqlQuery query;
        // Même correction pour UPDATE
        query.prepare(R"(UPDATE SEANCE SET TYPE_SEANCE=:type, DATE_SEANCE=TO_DATE(:date,'YYYY-MM-DD'),
                     HEURE_DEBUT=TO_DATE(:hDeb,'HH24:MI'), HEURE_ARRIVER=TO_DATE(:hArr,'HH24:MI'),
                     LIEU_DEPART=:lieu, PRIX=:prix WHERE ID_SEANCE=:id)");
        query.bindValue(":type", type);
        query.bindValue(":date", date.toString("yyyy-MM-dd")); // Format ISO
        query.bindValue(":hDeb", hDeb.toString("HH:mm"));
        query.bindValue(":hArr", hArr.toString("HH:mm"));
        query.bindValue(":lieu", lieu);
        query.bindValue(":prix", prix);
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox::information(this,"Succès","Séance modifiée !");
            rechargerTableSeancesDepuisDB();
        } else {
            QMessageBox::critical(this,"Erreur","Échec UPDATE : " + query.lastError().text());
        }

}

// Supprimer séance
void MainWindow::on_pushButton_12_clicked()
{
    QString id = ui->lineEdit_ID_Supprimer->text().trimmed();
    if (id.isEmpty()) { QMessageBox::warning(this,"Erreur","ID requis."); return; }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM SEANCE WHERE ID_SEANCE=:id");
    checkQuery.bindValue(":id", id); checkQuery.exec(); checkQuery.next();
    if (checkQuery.value(0).toInt() == 0) { QMessageBox::warning(this,"Erreur","ID inexistant."); return; }

    if (QMessageBox::question(this,"Confirmation","Supprimer cette séance ?",QMessageBox::Yes|QMessageBox::No)!=QMessageBox::Yes) return;

    QSqlQuery query;
    query.prepare("DELETE FROM SEANCE WHERE ID_SEANCE=:id");
    query.bindValue(":id", id);

    if (query.exec()) {
        QMessageBox::information(this,"Succès","Séance supprimée !");
        rechargerTableSeancesDepuisDB();
        ui->lineEdit_ID_Supprimer->clear();
    } else {
        QMessageBox::critical(this,"Erreur","Échec DELETE : " + query.lastError().text());
    }
}

// Cliquer sur ligne du tableau séance
void MainWindow::on_tableWidget_11_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    if (row < 0 || row >= ui->tabS->rowCount()) return;

    ui->idS->setText(ui->tabS->item(row,0)->text());
    ui->typeS->setCurrentText(ui->tabS->item(row,1)->text());
    ui->dateS_2->setDate(QDate::fromString(ui->tabS->item(row,2)->text(),"dd/MM/yyyy"));
    ui->hDeb->setTime(QTime::fromString(ui->tabS->item(row,3)->text(),"HH:mm"));
    ui->hArr->setTime(QTime::fromString(ui->tabS->item(row,4)->text(),"HH:mm"));
    ui->lieuS->setText(ui->tabS->item(row,5)->text());
    ui->prixS->setText(ui->tabS->item(row,6)->text());
}

void MainWindow::on_pushButton_13_clicked()
{


        if (!ui->tabS) return;

        // Save the original table content
        QVector<QVector<QString>> originalData;
        int originalRows = ui->tabS->rowCount();
        int originalCols = ui->tabS->columnCount();
        for (int r = 0; r < originalRows; ++r) {
            QVector<QString> rowData;
            for (int c = 0; c < originalCols; ++c) {
                QTableWidgetItem* item = ui->tabS->item(r, c);
                rowData.append(item ? item->text() : "");
            }
            originalData.append(rowData);
        }

        QString searchType = ui->comboBox_9->currentText().trimmed();
        if (searchType.isEmpty()) return;

        QSqlQuery query;
        bool ok = false;

        if (searchType == "Date") {
            QDate date = ui->dateS_2->date();
            QString dateStr = date.toString("dd/MM/yyyy");

            query.prepare("SELECT ID_SEANCE, TYPE_SEANCE, TO_CHAR(DATE_SEANCE,'DD/MM/YYYY'), "
                          "TO_CHAR(HEURE_DEBUT,'HH24:MI'), TO_CHAR(HEURE_ARRIVER,'HH24:MI'), "
                          "LIEU_DEPART, PRIX FROM SEANCE WHERE TO_CHAR(DATE_SEANCE,'DD/MM/YYYY') = :date");
            query.bindValue(":date", dateStr);
            ok = query.exec();
        }
        else if (searchType == "Lieu de départ") {
            QString lieu = ui->lieuS->text().trimmed();
            if (lieu.isEmpty()) {
                QMessageBox::warning(this, "Recherche", "Saisis le lieu de départ.");
                return;
            }

            query.prepare("SELECT ID_SEANCE, TYPE_SEANCE, TO_CHAR(DATE_SEANCE,'DD/MM/YYYY'), "
                          "TO_CHAR(HEURE_DEBUT,'HH24:MI'), TO_CHAR(HEURE_ARRIVER,'HH24:MI'), "
                          "LIEU_DEPART, PRIX FROM SEANCE WHERE UPPER(LIEU_DEPART) LIKE UPPER(:lieu)");
            query.bindValue(":lieu", "%" + lieu + "%");
            ok = query.exec();
        }

        if (!ok) {
            QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
            return;
        }

        // Clear and fill table with search results
        ui->tabS->setRowCount(0);
        int row = 0;
        while (query.next()) {
            ui->tabS->insertRow(row);
            for (int col = 0; col < 7; ++col) {
                ui->tabS->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
            row++;
        }

        if (row == 0) {
            QMessageBox::information(this, "Recherche", "Aucune séance trouvée.");
        }

        // Restore original table after 20 seconds
        QTimer::singleShot(10000, this, [this, originalData]() {
            ui->tabS->setRowCount(0);
            for (int r = 0; r < originalData.size(); ++r) {
                ui->tabS->insertRow(r);
                for (int c = 0; c < originalData[r].size(); ++c) {
                    ui->tabS->setItem(r, c, new QTableWidgetItem(originalData[r][c]));
                }
            }
        });



}




//EMPLOYE
void MainWindow::on_pushButton_Rechercher_2_clicked()
{
    QString searchID = ui->lineEdit_Rechercheemp->text().trimmed();
    bool found = false;

    for (int i = 0; i < ui->tableWidget_Employe->rowCount(); ++i)
    {
        QTableWidgetItem *item = ui->tableWidget_Employe->item(i, 0);

        if (item && item->text().compare(searchID, Qt::CaseInsensitive) == 0)
        {
            found = true;

            for (int j = 0; j < ui->tableWidget_Employe->columnCount(); ++j)
            {
                ui->tableWidget_Employe->item(i, j)->setBackground(QBrush(QColor("green")));
            }

            ui->tableWidget_Employe->scrollToItem(item);

            QTimer::singleShot(20000, [this, i]() {
                for (int j = 0; j < ui->tableWidget_Employe->columnCount(); ++j)
                {
                    ui->tableWidget_Employe->item(i, j)->setBackground(Qt::white);
                }
            });

            break;
        }
    }

    if (!found)
    {
        QMessageBox::information(this, "Erreur", "Aucun employé trouvé avec cet ID !");
    }

    ui->lineEdit_Rechercheemp->clear();
}


void MainWindow::on_stats_emp_clicked()
{
    int total = ui->tableWidget_Employe->rowCount();
    if (total == 0) {
        QMessageBox::information(this, "Statistiques", "👷 Aucun employé trouvé !");
        return;
    }

    int chauffeurs = 0;
    int assistants = 0;

    // Supposons que la colonne Spécialité = colonne 5
    int colSpecialite = 5;

    for (int i = 0; i < total; ++i) {
        QString spec = ui->tableWidget_Employe->item(i, colSpecialite)->text().trimmed();
        if (spec.compare("voiture", Qt::CaseInsensitive) == 0)
            chauffeurs++;
        else if (spec.compare("assistant", Qt::CaseInsensitive) == 0)
            assistants++;
    }

    QString message = QString(
                          "📊 **Statistiques Employés**\n\n"
                          "👥 Total : %1\n"
                          "🚗 Spécialité voiture : %2\n"
                          "🧾 Spécialité assistant : %3"
                          ).arg(total).arg(chauffeurs).arg(assistants);

    QMessageBox::information(this, "Statistiques", message);
}





void MainWindow::on_pushButton_ajouter_emp_clicked()
{

        QString id = ui->lineEdit_ID->text().trimmed();
        QString nom = ui->lineEdit_Nom->text();
        QString prenom = ui->lineEdit_Prenom->text();
        QString role = ui->comboBox_Role->currentText();
        QString numTel = ui->lineEdit_NumTel->text();
        QString email = ui->lineEdit_Email->text();
        QString specialite = ui->comboBox_Specialite->currentText();
        float salaire = ui->lineEdit_Salaire->text().toFloat();
        QString motPasse = ui->lineEdit_Mdp->text();

        QSqlQuery query;
        query.prepare(R"(INSERT INTO EMPLOYE
                     (ID_EMPLOYE, NOM, PRENOM, ROLE, NUMTEL, EMAIL, SPECIALITE, SALAIRE, MDP)
                     VALUES (:id, :nom, :prenom, :role, :numTel, :email, :spec, :salaire, :mdp))");

        query.bindValue(":id", id);
        query.bindValue(":nom", nom);
        query.bindValue(":prenom", prenom);
        query.bindValue(":role", role);
        query.bindValue(":numTel", numTel);
        query.bindValue(":email", email);
        query.bindValue(":spec", specialite);
        query.bindValue(":salaire", salaire);
        query.bindValue(":mdp", motPasse);

        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Employé ajouté !");
            afficherEmployes(); // rafraîchit le tableau
        } else {
            QMessageBox::critical(this, "Erreur", query.lastError().text());
        }


}



 void MainWindow::on_pushButton_supp_emp_clicked()
{
        QString id = ui->lineEdit_ID->text().trimmed();

        QSqlQuery query;
        query.prepare("DELETE FROM EMPLOYE WHERE ID_EMPLOYE=:id");
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Employé supprimé !");
            afficherEmployes();
        } else {
            QMessageBox::critical(this, "Erreur", query.lastError().text());
        }
}




void MainWindow::on_pushButton_modifier_emp_clicked()
{

        QString id = ui->lineEdit_ID->text().trimmed();
        QString nom = ui->lineEdit_Nom->text();
        QString prenom = ui->lineEdit_Prenom->text();
        QString role = ui->comboBox_Role->currentText();
        QString numTel = ui->lineEdit_NumTel->text();
        QString email = ui->lineEdit_Email->text();
        QString specialite = ui->comboBox_Specialite->currentText();
        float salaire = ui->lineEdit_Salaire->text().toFloat();
        QString motPasse = ui->lineEdit_Mdp->text();

        QSqlQuery query;
        query.prepare(R"(UPDATE EMPLOYE SET
                     NOM=:nom, PRENOM=:prenom, ROLE=:role, NUMTEL=:numTel, EMAIL=:email,
                     SPECIALITE=:spec, SALAIRE=:salaire, MDP=:mdp
                     WHERE ID_EMPLOYE=:id)");

        query.bindValue(":id", id);
        query.bindValue(":nom", nom);
        query.bindValue(":prenom", prenom);
        query.bindValue(":role", role);
        query.bindValue(":numTel", numTel);
        query.bindValue(":email", email);
        query.bindValue(":spec", specialite);
        query.bindValue(":salaire", salaire);
        query.bindValue(":mdp", motPasse);

        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Employé modifié !");
            afficherEmployes();
        } else {
            QMessageBox::critical(this, "Erreur", query.lastError().text());
        }
    }

void MainWindow::on_pushButton_33_clicked()
{
    ui->lineEdit_ID->clear();
    ui->lineEdit_Nom->clear();
    ui->lineEdit_Prenom->clear();
    ui->comboBox_Role->setCurrentIndex(0);         // remet le premier rôle par défaut
    ui->lineEdit_NumTel->clear();
    ui->lineEdit_Email->clear();
    ui->comboBox_Specialite->setCurrentIndex(0);  // remet la première spécialité
    ui->lineEdit_Salaire->clear();
    ui->lineEdit_Mdp->clear();
}



//candidat

void MainWindow::on_pushButton_23_clicked()  // ➕ Ajouter un candidat
{
    QString id = ui->lineEdit_ID_Candidat->text().trimmed();
    QString nom = ui->lineEdit_Nom_Candidat->text().trimmed();
    QString prenom = ui->lineEdit_Prenom_Candidat->text().trimmed();
    QString num_tel = ui->lineEdit_NumTel_Candidat->text().trimmed();
    QString adresse = ui->lineEdit_Adresse_Candidat->text().trimmed();
    QString mot_de_passe = ui->lineEdit_Mdp_Candidat->text().trimmed();

    if (id.isEmpty() || nom.isEmpty() || prenom.isEmpty() || num_tel.isEmpty() ||
        adresse.isEmpty() || mot_de_passe.isEmpty()) {
        QMessageBox::warning(this, "Champs manquants", "Veuillez remplir tous les champs avant d'ajouter un candidat.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO CANDIDAT (ID_CANDIDAT, NOM, PRENOM, NUM_TEL, ADRESSE, MOT_DE_PASSE) "
                  "VALUES (:id, :nom, :prenom, :num_tel, :adresse, :mdp)");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":num_tel", num_tel);
    query.bindValue(":adresse", adresse);
    query.bindValue(":mdp", mot_de_passe);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Échec de l'ajout : " + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Succès", "Candidat ajouté avec succès !");
    chargerCandidats();
}


void MainWindow::on_pushButton_147_clicked()
{
    QString id = ui->lineEdit_ID_Candidat->text().trimmed();
    QString nom = ui->lineEdit_Nom_Candidat->text().trimmed();
    QString prenom = ui->lineEdit_Prenom_Candidat->text().trimmed();
    QString num_tel = ui->lineEdit_NumTel_Candidat->text().trimmed();
    QString adresse = ui->lineEdit_Adresse_Candidat->text().trimmed();
    QString mot_de_passe = ui->lineEdit_Mdp_Candidat->text().trimmed();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer l'ID du candidat à modifier.");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE CANDIDAT SET NOM = :nom, PRENOM = :prenom, NUM_TEL = :num_tel, "
                  "ADRESSE = :adresse, MOT_DE_PASSE = :mdp WHERE ID_CANDIDAT = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":num_tel", num_tel);
    query.bindValue(":adresse", adresse);
    query.bindValue(":mdp", mot_de_passe);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Échec de la modification : " + query.lastError().text());
        return;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(this, "Avertissement", "Aucun candidat trouvé avec cet ID.");
        return;
    }

    QMessageBox::information(this, "Succès", "Candidat modifié avec succès !");
    chargerCandidats();
}


void MainWindow::on_pushButton_20_clicked()
{
    QString id = ui->lineEdit_ID_Candidat->text().trimmed();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer l'ID du candidat à supprimer.");
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM CANDIDAT WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Échec de la suppression : " + query.lastError().text());
        return;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(this, "Avertissement", "Aucun candidat trouvé avec cet ID.");
        return;
    }

    QMessageBox::information(this, "Succès", "Candidat supprimé avec succès !");
    chargerCandidats();
}

void MainWindow::on_pushButton_77_clicked()
{
    ui->idS->clear();
    ui->lieuS->clear();
    ui->prixS->clear();
    ui->typeS->setCurrentIndex(0);
    ui->dateS_2->setDate(QDate::currentDate());
    ui->hDeb->setTime(QTime(8,0));
    ui->hArr->setTime(QTime(9,0));
}

void MainWindow::on_pushButton_On_clicked()
{
    qDebug() << "Sending ON...";
    A.write_to_arduino("ON\n");
}
void MainWindow::on_pushButton_Off_clicked()
{
    qDebug() << "Sending OFF...";
    A.write_to_arduino("OFF\n");  // use the same A object
}
void MainWindow::lireArduino()
{
    QByteArray data = arduino->readAll();
    qDebug() << "Arduino → Qt:" << data;
}

void MainWindow::on_pushButton_81_clicked()
{

}


void MainWindow::on_pdfseance_clicked()
{

        if (!ui->tabS) {
            QMessageBox::warning(this, "Erreur", "Table des séances introuvable.");
            return;
        }

        QString filename = QFileDialog::getSaveFileName(this, "Exporter Séances en PDF", "", "Fichiers PDF (*.pdf)");
        if (filename.isEmpty()) return;

        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(filename);
        printer.setPageSize(QPageSize(QPageSize::A4));

        QTextDocument document;
        QTextCursor cursor(&document);

        // Titre
        QTextCharFormat titleFormat;
        titleFormat.setFontPointSize(16);
        titleFormat.setFontWeight(QFont::Bold);
        cursor.setCharFormat(titleFormat);
        cursor.insertText("LISTE DES SÉANCES - DRIVING SCHOOL\n\n");

        QTextCharFormat normalFormat;
        normalFormat.setFontPointSize(10);
        cursor.setCharFormat(normalFormat);
        cursor.insertText("Date d'export : " + QDate::currentDate().toString("dd/MM/yyyy") + "\n\n");

        int rows = ui->tabS->rowCount();
        int cols = ui->tabS->columnCount();

        if (rows == 0) {
            cursor.insertText("Aucune séance à exporter.\n");
        } else {
            // Table
            QTextTableFormat tableFormat;
            tableFormat.setHeaderRowCount(1);
            tableFormat.setAlignment(Qt::AlignCenter);
            tableFormat.setCellPadding(6);
            tableFormat.setCellSpacing(2);
            tableFormat.setBorder(1);
            tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));

            QTextTable *table = cursor.insertTable(rows + 1, cols, tableFormat);

            // Header
            QTextCharFormat headerFormat;
            headerFormat.setFontWeight(QFont::Bold);
            headerFormat.setBackground(Qt::lightGray);

            for (int c = 0; c < cols; ++c) {
                QTextTableCell cell = table->cellAt(0, c);
                QTextCursor cellCursor = cell.firstCursorPosition();
                cellCursor.setCharFormat(headerFormat);
                QTableWidgetItem *h = ui->tabS->horizontalHeaderItem(c);
                QString head = h ? h->text() : QString("Col%1").arg(c);
                cellCursor.insertText(head);
            }

            // Data
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    QTextTableCell cell = table->cellAt(r + 1, c);
                    QTextCursor cellCursor = cell.firstCursorPosition();
                    QTableWidgetItem *it = ui->tabS->item(r, c);
                    QString txt = it ? it->text() : "";
                    cellCursor.insertText(txt);
                }
            }
        }

        cursor.movePosition(QTextCursor::End);
        cursor.insertText("\n\n--- Fin du rapport ---");

        document.print(&printer);

        QMessageBox::information(this, "Export PDF", "Export PDF des séances effectué :\n" + filename);


}
void MainWindow::on_triseance_clicked()
{
    if (!ui->tabS) {
        QMessageBox::warning(this, "Erreur", "Table des séances introuvable.");
        return;
    }

    QString critere;
    // utilise ui->comboBox_TrierSeance si présent sinon essaie de trouver le widget dans l'UI
    if (ui->comboBox_TrierSeance) critere = ui->comboBox_TrierSeance->currentText();
    else {
        QComboBox* cb = findChild<QComboBox*>("comboBox_TrierSeance");
        if (cb) critere = cb->currentText();
    }

    if (critere.isEmpty()) {
        QMessageBox::information(this, "Tri", "Veuillez sélectionner un critère de tri.");
        return;
    }

    // validation simple (on accepte les noms donnés plus tôt)
    QMap<QString, int> mapping;
    mapping["ID"] = 0;
    mapping["Type"] = 1;
    mapping["Date"] = 2;
    mapping["Heure_Debut"] = 3;
    mapping["Heure_Arriver"] = 4;
    mapping["Lieu"] = 5;
    mapping["Prix"] = 6;

    // normaliser critere (quelques alias courants)
    QString normalized = critere;
    if (normalized.compare("Heure Debut", Qt::CaseInsensitive) == 0) normalized = "Heure_Debut";
    if (normalized.compare("Heure Arriver", Qt::CaseInsensitive) == 0) normalized = "Heure_Arriver";
    if (normalized.compare("Date", Qt::CaseInsensitive) == 0) normalized = "Date";
    if (normalized.compare("Prix", Qt::CaseInsensitive) == 0) normalized = "Prix";
    if (normalized.compare("ID", Qt::CaseInsensitive) == 0) normalized = "ID";

    if (!mapping.contains(normalized)) {
        QMessageBox::warning(this, "Erreur", "Critère de tri non reconnu: " + critere);
        return;
    }

    int colonne = mapping[normalized];
    ui->tabS->setSortingEnabled(true);
    ui->tabS->sortByColumn(colonne, Qt::AscendingOrder);
    QMessageBox::information(this, "Tri", QString("Tri effectué avec succès sur : %1").arg(critere));
}

// Lorsque l'utilisateur clique sur une cellule du tableau ui->tabS (slot attendu: itemClicked)


// Afficher les statistiques des séances basées sur les données du tableau ui->tabS
void MainWindow::on_statseance_clicked()
{
    showStats();
}

void MainWindow::showStats()
{
    qDebug() << "📊 Méthode showStats() appelée!";

    if (!ui->tabS) {
        QMessageBox::information(this, "Statistiques", "Aucune table des séances disponible.");
        return;
    }

    int totalSeances = ui->tabS->rowCount();
    double revenuTotal = 0.0;
    QMap<QString, int> seancesParType;
    QMap<QString, int> seancesParMois;

    // Récupérer les données depuis ui->tabS
    for (int r = 0; r < ui->tabS->rowCount(); ++r) {
        QString type = ui->tabS->item(r, 1) ? ui->tabS->item(r, 1)->text() : "Inconnu";
        QString dateStr = ui->tabS->item(r, 2) ? ui->tabS->item(r, 2)->text() : "";
        QString prixStr = ui->tabS->item(r, 6) ? ui->tabS->item(r, 6)->text() : "0";

        // prix
        bool ok = false;
        double p = prixStr.toDouble(&ok);
        if (ok) revenuTotal += p;

        // type
        seancesParType[type] = seancesParType.value(type, 0) + 1;

        // mois (MM/YYYY)
        QDate d = QDate::fromString(dateStr, "dd/MM/yyyy");
        QString moisLabel = d.isValid() ? d.toString("MM/yyyy") : "Inconnu";
        seancesParMois[moisLabel] = seancesParMois.value(moisLabel, 0) + 1;
    }

    if (totalSeances == 0) {
        QMessageBox::information(this, "Statistiques", "📊 Aucune séance trouvée !");
        return;
    }

    // Dialog stats
    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("📊 Statistiques des Séances - Driving School");
    statsDialog->setMinimumSize(1000, 700);
    statsDialog->setStyleSheet("QDialog { background-color: #f5f5f5; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(statsDialog);

    QWidget *headerWidget = new QWidget();
    headerWidget->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2c3e50, stop:1 #3498db); color: white; padding: 15px; border-radius: 10px;");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);

    QLabel *totalLabel = new QLabel(QString("📈 %1\nSéances Total").arg(totalSeances));
    totalLabel->setStyleSheet("font-size: 16px; font-weight: bold; background: rgba(255,255,255,0.2); padding: 10px; border-radius: 5px;");
    totalLabel->setAlignment(Qt::AlignCenter);

    QLabel *revenuLabel = new QLabel(QString("💰 %1 €\nRevenu Total").arg(QString::number(revenuTotal, 'f', 2)));
    revenuLabel->setStyleSheet("font-size: 16px; font-weight: bold; background: rgba(255,255,255,0.2); padding: 10px; border-radius: 5px;");
    revenuLabel->setAlignment(Qt::AlignCenter);

    headerLayout->addWidget(totalLabel);
    headerLayout->addWidget(revenuLabel);
    headerLayout->addStretch();

    mainLayout->addWidget(headerWidget);

    QHBoxLayout *chartsLayout = new QHBoxLayout();

    if (!seancesParType.isEmpty()) {
        QWidget *pieChartContainer = new QWidget();
        pieChartContainer->setStyleSheet("background: white; padding: 15px; border-radius: 10px; border: 1px solid #ddd;");
        QVBoxLayout *pieLayout = new QVBoxLayout(pieChartContainer);

        createPieChart(seancesParType, "Répartition des Séances par Type", pieLayout);
        chartsLayout->addWidget(pieChartContainer);
    }

    if (!seancesParMois.isEmpty()) {
        QWidget *barChartContainer = new QWidget();
        barChartContainer->setStyleSheet("background: white; padding: 15px; border-radius: 10px; border: 1px solid #ddd;");
        QVBoxLayout *barLayout = new QVBoxLayout(barChartContainer);

        createBarChart(seancesParMois, "Séances par Mois", barLayout);
        chartsLayout->addWidget(barChartContainer);
    }

    mainLayout->addLayout(chartsLayout);

    QPushButton *closeButton = new QPushButton("Fermer");
    closeButton->setStyleSheet("QPushButton { background: #e74c3c; color: white; border: none; padding: 10px 20px; border-radius: 5px; font-weight: bold; }"
                               "QPushButton:hover { background: #c0392b; }");
    closeButton->setFixedWidth(100);
    mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);

    connect(closeButton, &QPushButton::clicked, statsDialog, &QDialog::close);

    statsDialog->exec();
}

// createPieChart
void MainWindow::createPieChart(const QMap<QString, int>& data, const QString& title, QLayout* layout)
{
    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #2c3e50; margin-bottom: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QPieSeries *series = new QPieSeries();

    QList<QColor> colors = {
        QColor("#3498db"), QColor("#e74c3c"), QColor("#2ecc71"),
        QColor("#f39c12"), QColor("#9b59b6"), QColor("#1abc9c"),
        QColor("#d35400"), QColor("#34495e")
    };

    int colorIndex = 0;
    int total = 0;
    for (int value : data) total += value;

    for (auto it = data.begin(); it != data.end(); ++it) {
        QString label = QString("%1\n%2 (%3%)")
        .arg(it.key())
            .arg(it.value())
            .arg(QString::number((it.value() * 100.0) / (total>0?total:1), 'f', 1));

        QPieSlice *slice = series->append(label, it.value());
        slice->setColor(colors[colorIndex % colors.size()]);
        slice->setLabelVisible(true);
        slice->setLabelPosition(QPieSlice::LabelOutside);
        slice->setBorderColor(Qt::white);
        slice->setBorderWidth(2);

        colorIndex++;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setBackgroundBrush(QBrush(Qt::transparent));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 300);

    layout->addWidget(chartView);
}

// createBarChart
void MainWindow::createBarChart(const QMap<QString, int>& data, const QString& title, QLayout* layout)
{
    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #2c3e50; margin-bottom: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QBarSeries *series = new QBarSeries();
    QBarSet *barSet = new QBarSet("Nombre de séances");

    QStringList categories;
    for (auto it = data.begin(); it != data.end(); ++it) {
        *barSet << it.value();
        categories << it.key();
    }

    series->append(barSet);
    series->setLabelsVisible(true);
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setBackgroundBrush(QBrush(Qt::transparent));

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%d");
    axisY->setTitleText("Nombre de séances");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 300);

    layout->addWidget(chartView);
}






// 2. Modifiez la fonction readArduino pour gérer l'enregistrement
void MainWindow::readArduino()
{
    static QString buffer;
    static QDateTime lastCardTime;
    static QString lastCardUID = "";

    QByteArray raw = A.read_from_arduino();
    if (raw.isEmpty())
        return;

    buffer += QString::fromUtf8(raw);

    while (buffer.contains("\n")) {
        QString line = buffer.section('\n', 0, 0).trimmed();
        buffer = buffer.section('\n', 1);

        qDebug() << "Arduino → Qt:" << line;

        if (line.startsWith("CARD_")) {
            QString cardUID = line.mid(5);

            // Anti-rebond: ignorer la même carte pendant 3 secondes
            QDateTime now = QDateTime::currentDateTime();
            if (cardUID == lastCardUID && lastCardTime.isValid()) {
                qint64 secondsSince = lastCardTime.secsTo(now);
                if (secondsSince < 3) {
                    qDebug() << "Carte ignorée (anti-rebond):" << cardUID;
                    continue; // Ignorer cette lecture
                }
            }

            // Mettre à jour le dernier scan
            lastCardUID = cardUID;
            lastCardTime = now;

            qDebug() << "Nouvelle carte détectée:" << cardUID;

            if (isRegisteringMode) {
                processRFIDRegistration(cardUID);
            } else {
                checkRFIDCard(cardUID);
            }
        }
        else if (line == "CMD_ON") {
            qDebug() << "Portail ouvert";
        }
        else if (line == "CMD_OFF") {
            qDebug() << "Portail fermé";
        }
    }
}
// 3. Fonction pour enregistrer une carte RFID
void MainWindow::on_pushButton_RegisterRFID_clicked()
{
    QString empID = ui->lineEdit_ID->text().trimmed();
    QString nom = ui->lineEdit_Nom->text().trimmed();
    QString prenom = ui->lineEdit_Prenom->text().trimmed();

    if (empID.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez d'abord sélectionner un employé.");
        return;
    }

    // Vérifier si l'employé existe dans la base
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT RFID_UID, NOM, PRENOM FROM EMPLOYE WHERE ID_EMPLOYE = :id");
    checkQuery.bindValue(":id", empID);

    if (!checkQuery.exec()) {
        QMessageBox::critical(this, "Erreur SQL", checkQuery.lastError().text());
        return;
    }

    if (!checkQuery.next()) {
        QMessageBox::warning(this, "Erreur", "Employé non trouvé dans la base de données.");
        return;
    }

    QString existingRFID = checkQuery.value(0).toString();
    QString existingNom = checkQuery.value(1).toString();
    QString existingPrenom = checkQuery.value(2).toString();

    // Vérifier si une carte est déjà enregistrée
    if (!existingRFID.isEmpty()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Carte existante",
                                      QString("Cet employé a déjà une carte RFID :\n\n"
                                              "UID : %1\n\n"
                                              "Voulez-vous la remplacer ?").arg(existingRFID),
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No) {
            return;
        }
    }

    // Activer le mode enregistrement
    isRegisteringMode = true;
    employeeToRegister = empID;

    // Créer la boîte de dialogue d'attente
    rfidWaitDialog = new QDialog(this);
    rfidWaitDialog->setWindowTitle("Enregistrement RFID");
    rfidWaitDialog->setModal(true);
    rfidWaitDialog->setFixedSize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(rfidWaitDialog);

    QLabel *label = new QLabel(
        QString("Veuillez approcher la carte RFID du lecteur...\n\n"
                "Employé : %1 %2\n"
                "ID : %3").arg(existingPrenom).arg(existingNom).arg(empID));
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    layout->addWidget(label);

    QProgressBar *progressBar = new QProgressBar();
    progressBar->setRange(0, 0); // Progress bar indéterminée
    layout->addWidget(progressBar);

    QPushButton *cancelButton = new QPushButton("Annuler");
    layout->addWidget(cancelButton);

    // Connexion du bouton annuler
    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        isRegisteringMode = false;
        employeeToRegister.clear();

        if (rfidWaitDialog) {
            rfidWaitDialog->reject();
            rfidWaitDialog = nullptr;
        }

        QMessageBox::information(this, "Annulé", "Enregistrement annulé.");
    });

    // Timer pour annuler automatiquement après 30 secondes
    QTimer::singleShot(30000, this, [this]() {
        if (isRegisteringMode) {
            isRegisteringMode = false;
            employeeToRegister.clear();

            if (rfidWaitDialog) {
                rfidWaitDialog->reject();
                rfidWaitDialog = nullptr;
            }

            QMessageBox::warning(this, "Timeout",
                                 "Temps d'attente dépassé. Aucune carte détectée.");
        }
    });

    // Nettoyer la boîte de dialogue quand elle se ferme
    connect(rfidWaitDialog, &QDialog::finished, this, [this]() {
        if (rfidWaitDialog) {
            rfidWaitDialog->deleteLater();
            rfidWaitDialog = nullptr;
        }
    });

    rfidWaitDialog->exec();
}

// 4. Fonction pour traiter l'enregistrement RFID
void MainWindow::processRFIDRegistration(const QString &cardUID)
{
    if (!isRegisteringMode || employeeToRegister.isEmpty()) {
        return;
    }

    // Fermer la boîte de dialogue d'attente
    if (rfidWaitDialog) {
        rfidWaitDialog->accept();
        rfidWaitDialog = nullptr;
    }

    // Vérifier si la carte est déjà attribuée à un autre employé
    QSqlQuery checkCardQuery;
    checkCardQuery.prepare("SELECT ID_EMPLOYE, NOM, PRENOM FROM EMPLOYE WHERE RFID_UID = :uid");
    checkCardQuery.bindValue(":uid", cardUID);

    bool cardAlreadyAssigned = false;
    QString existingEmpID, existingNom, existingPrenom;

    if (checkCardQuery.exec() && checkCardQuery.next()) {
        cardAlreadyAssigned = true;
        existingEmpID = checkCardQuery.value(0).toString();
        existingNom = checkCardQuery.value(1).toString();
        existingPrenom = checkCardQuery.value(2).toString();
    }

    // Si la carte est déjà attribuée et ce n'est pas le même employé
    if (cardAlreadyAssigned && existingEmpID != employeeToRegister) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Carte déjà attribuée",
                                      QString("Cette carte RFID est déjà attribuée à :\n"
                                              "%1 %2 (ID: %3)\n\n"
                                              "Voulez-vous quand même l'assigner à l'employé actuel ?")
                                          .arg(existingPrenom).arg(existingNom).arg(existingEmpID),
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No) {
            isRegisteringMode = false;
            employeeToRegister.clear();
            return;
        }
    }

    // Enregistrer la carte RFID dans la base de données
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE EMPLOYE SET RFID_UID = :uid WHERE ID_EMPLOYE = :id");
    updateQuery.bindValue(":uid", cardUID);
    updateQuery.bindValue(":id", employeeToRegister);

    if (updateQuery.exec()) {
        // Récupérer les informations de l'employé pour le message
        QSqlQuery empQuery;
        empQuery.prepare("SELECT NOM, PRENOM FROM EMPLOYE WHERE ID_EMPLOYE = :id");
        empQuery.bindValue(":id", employeeToRegister);

        QString nom, prenom;
        if (empQuery.exec() && empQuery.next()) {
            nom = empQuery.value(0).toString();
            prenom = empQuery.value(1).toString();
        }

        QMessageBox::information(this, "Succès",
                                 QString("✅ Carte RFID enregistrée avec succès !\n\n"
                                         "Employé : %1 %2\n"
                                         "ID : %3\n"
                                         "RFID UID : %4").arg(prenom).arg(nom).arg(employeeToRegister).arg(cardUID));

        // Ouvrir le portail en guise de confirmation
        A.write_to_arduino("ON\n");
        QTimer::singleShot(3000, this, [this]() {
            A.write_to_arduino("OFF\n");
        });

        // Mettre à jour l'affichage
        afficherEmployes();

    } else {
        QMessageBox::critical(this, "Erreur",
                              QString("❌ Erreur lors de l'enregistrement :\n%1")
                                  .arg(updateQuery.lastError().text()));
    }

    // Réinitialiser le mode
    isRegisteringMode = false;
    employeeToRegister.clear();
}

// 5. Fonction pour vérifier les cartes RFID (mode normal)
void MainWindow::checkRFIDCard(const QString &cardUID)
{
    QSqlQuery query;
    query.prepare("SELECT ID_EMPLOYE, NOM, PRENOM, ROLE, entree, sortie FROM EMPLOYE WHERE RFID_UID = :uid");
    query.bindValue(":uid", cardUID);

    if (query.exec() && query.next()) {
        // Carte autorisée
        QString id = query.value(0).toString();
        QString nom = query.value(1).toString();
        QString prenom = query.value(2).toString();
        QString role = query.value(3).toString();
        int entreeCount = query.value(4).toInt();
        int sortieCount = query.value(5).toInt();

        // DÉTERMINER SI C'EST UNE ENTRÉE OU SORTIE
        // Logique: Si entree > sortie, alors on considère que la personne est à l'intérieur
        bool estALInterieur = (entreeCount > sortieCount);

        if (estALInterieur) {
            // C'est une SORTIE
            sortieCount++;

            // Message de sortie
            QString message = QString("👋 Au revoir %1 %2 !\nSortie enregistrée.").arg(prenom).arg(nom);
            afficherToast(message);

            // Mettre à jour la base de données
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE EMPLOYE SET sortie = :sortie WHERE ID_EMPLOYE = :id");
            updateQuery.bindValue(":sortie", sortieCount);
            updateQuery.bindValue(":id", id);
            updateQuery.exec();

            // Fermer le portail
            A.write_to_arduino("ON\n"); // Ouvrir pour sortir
            QTimer::singleShot(3000, this, [this]() {
                A.write_to_arduino("OFF\n"); // Fermer après 3s
            });

        } else {
            // C'est une ENTRÉE
            entreeCount++;

            // Message d'entrée
            QString message = QString("👋 Bienvenue %1 %2 !\nEntrée enregistrée.").arg(prenom).arg(nom);
            afficherToast(message);

            // Mettre à jour la base de données
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE EMPLOYE SET entree = :entree WHERE ID_EMPLOYE = :id");
            updateQuery.bindValue(":entree", entreeCount);
            updateQuery.bindValue(":id", id);
            updateQuery.exec();

            // Ouvrir le portail
            A.write_to_arduino("ON\n"); // Ouvrir pour entrer
            QTimer::singleShot(3000, this, [this]() {
                A.write_to_arduino("OFF\n"); // Fermer après 3s
            });
        }

        // Mettre à jour l'affichage du tableau
        afficherEmployes();

        // Afficher le statut actuel
        QString statut = estALInterieur ? "à l'intérieur" : "à l'extérieur";
        qDebug() << QString("%1 %2 - Entrées: %3, Sorties: %4, Statut: %5")
                        .arg(prenom).arg(nom).arg(entreeCount).arg(sortieCount).arg(statut);

    } else {
        // Carte non autorisée
        QString errorMessage = QString("🚫 Accès non autorisé !");
        afficherToast(errorMessage);

        // Ne pas ouvrir le portail
    }
}
// 6. Ajoutez un bouton "Supprimer RFID" si nécessaire
void MainWindow::on_pushButton_RemoveRFID_clicked()
{
    QString empID = ui->lineEdit_ID->text().trimmed();
    if (empID.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez d'abord sélectionner un employé.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT RFID_UID, NOM, PRENOM FROM EMPLOYE WHERE ID_EMPLOYE = :id");
    query.bindValue(":id", empID);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Erreur", "Employé non trouvé.");
        return;
    }

    QString rfidUID = query.value(0).toString();
    QString nom = query.value(1).toString();
    QString prenom = query.value(2).toString();

    if (rfidUID.isEmpty()) {
        QMessageBox::information(this, "Info", "Cet employé n'a pas de carte RFID enregistrée.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Supprimer RFID",
                                  QString("Voulez-vous supprimer la carte RFID de cet employé ?\n\n"
                                          "Employé : %1 %2\n"
                                          "ID : %3\n"
                                          "RFID UID : %4").arg(prenom).arg(nom).arg(empID).arg(rfidUID),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery deleteQuery;
        deleteQuery.prepare("UPDATE EMPLOYE SET RFID_UID = NULL WHERE ID_EMPLOYE = :id");
        deleteQuery.bindValue(":id", empID);

        if (deleteQuery.exec()) {
            QMessageBox::information(this, "Succès", "✅ Carte RFID supprimée avec succès.");
            afficherEmployes();
        } else {
            QMessageBox::critical(this, "Erreur",
                                  QString("❌ Erreur lors de la suppression :\n%1")
                                      .arg(deleteQuery.lastError().text()));
        }
    }
}

void MainWindow::on_pushButton_TestRFID_clicked()
{
    // Test sans Arduino
    QString testUID = QInputDialog::getText(this, "Test RFID",
                                            "Entrez un UID RFID pour tester :",
                                            QLineEdit::Normal, "A1B2C3D4");

    if (!testUID.isEmpty()) {
        if (isRegisteringMode) {
            processRFIDRegistration(testUID);
        } else {
            checkRFIDCard(testUID);
        }
    }
}



void MainWindow::on_tableWidget_Employe_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    qDebug() << "Clic sur ligne:" << row;

    if (row < 0 || row >= ui->tableWidget_Employe->rowCount()) {
        qDebug() << "Ligne invalide";
        return;
    }

    // Récupérer les données de la ligne sélectionnée
    // IMPORTANT: Vérifiez que votre tableau a les bonnes colonnes
    QString id = getTableCellText(row, 0);  // Colonne 0: ID
    QString nom = getTableCellText(row, 1); // Colonne 1: Nom
    QString prenom = getTableCellText(row, 2); // Colonne 2: Prénom
    QString role = getTableCellText(row, 3); // Colonne 3: Rôle
    QString telephone = getTableCellText(row, 4); // Colonne 4: Téléphone
    QString email = getTableCellText(row, 5); // Colonne 5: Email
    QString specialite = getTableCellText(row, 6); // Colonne 6: Spécialité
    QString salaire = getTableCellText(row, 7); // Colonne 7: Salaire
    QString mdp = getTableCellText(row, 8); // Colonne 8: Mot de passe

    qDebug() << "Données récupérées - ID:" << id << "Nom:" << nom << "Prénom:" << prenom;

    // Mettre à jour les champs de formulaire
    ui->lineEdit_ID->setText(id);
    ui->lineEdit_Nom->setText(nom);
    ui->lineEdit_Prenom->setText(prenom);
    ui->lineEdit_NumTel->setText(telephone);
    ui->lineEdit_Email->setText(email);
    ui->lineEdit_Salaire->setText(salaire);
    ui->lineEdit_Mdp->setText(mdp);

    // Mettre à jour les ComboBox
    // Pour le rôle
    int roleIndex = ui->comboBox_Role->findText(role);
    if (roleIndex != -1) {
        ui->comboBox_Role->setCurrentIndex(roleIndex);
    } else {
        ui->comboBox_Role->setCurrentIndex(0);
    }

    // Pour la spécialité
    int specIndex = ui->comboBox_Specialite->findText(specialite);
    if (specIndex != -1) {
        ui->comboBox_Specialite->setCurrentIndex(specIndex);
    } else {
        ui->comboBox_Specialite->setCurrentIndex(0);
    }

    // Mettre en évidence la ligne sélectionnée
    highlightSelectedRow(row);
}

// Fonction helper pour obtenir le texte d'une cellule
QString MainWindow::getTableCellText(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget_Employe->item(row, column);
    if (item) {
        return item->text();
    }
    return "";
}

// Fonction pour mettre en évidence la ligne sélectionnée
void MainWindow::highlightSelectedRow(int row)
{
    // D'abord, réinitialiser toutes les lignes
    for (int i = 0; i < ui->tableWidget_Employe->rowCount(); i++) {
        for (int j = 0; j < ui->tableWidget_Employe->columnCount(); j++) {
            QTableWidgetItem *item = ui->tableWidget_Employe->item(i, j);
            if (item) {
                item->setBackground(Qt::white);
                item->setForeground(Qt::black);
            }
        }
    }

    // Ensuite, mettre en évidence la ligne sélectionnée
    for (int j = 0; j < ui->tableWidget_Employe->columnCount(); j++) {
        QTableWidgetItem *item = ui->tableWidget_Employe->item(row, j);
        if (item) {
            item->setBackground(QColor(173, 216, 230)); // Bleu clair
            item->setForeground(Qt::black);
        }
    }
}

void MainWindow::debugTableStructure()
{
    qDebug() << "=== Structure du tableau Employés ===";
    qDebug() << "Nombre de lignes:" << ui->tableWidget_Employe->rowCount();
    qDebug() << "Nombre de colonnes:" << ui->tableWidget_Employe->columnCount();

    // Afficher les en-têtes
    QStringList headers;
    for (int i = 0; i < ui->tableWidget_Employe->columnCount(); i++) {
        QTableWidgetItem *header = ui->tableWidget_Employe->horizontalHeaderItem(i);
        headers << (header ? header->text() : "Colonne " + QString::number(i));
    }
    qDebug() << "En-têtes:" << headers;

    // Afficher les premières données
    if (ui->tableWidget_Employe->rowCount() > 0) {
        qDebug() << "Première ligne:";
        for (int i = 0; i < ui->tableWidget_Employe->columnCount(); i++) {
            QTableWidgetItem *item = ui->tableWidget_Employe->item(0, i);
            qDebug() << "Colonne" << i << ":" << (item ? item->text() : "VIDE");
        }
    }
}

void MainWindow::diagnoseRFIDProblem()
{
    qDebug() << "=== DIAGNOSTIC RFID ===";

    // 1. Vérifier la connexion Arduino
    if (A.getserial()->isOpen()) {
        qDebug() << "✓ Arduino connecté sur:" << A.getarduino_port_name();
    } else {
        qDebug() << "✗ Arduino non connecté";
    }

    // 2. Vérifier les cartes enregistrées
    QSqlQuery query("SELECT COUNT(*) as total, SUM(CASE WHEN RFID_UID IS NOT NULL THEN 1 ELSE 0 END) as avec_rfid FROM EMPLOYE");
    if (query.next()) {
        qDebug() << "Employés total:" << query.value(0).toInt();
        qDebug() << "Employés avec RFID:" << query.value(1).toInt();
    }

    // 3. Envoyer une commande test à Arduino
    qDebug() << "Envoi commande TEST à Arduino...";
    A.write_to_arduino("TEST\n");

    // 4. Lire la réponse
    QTimer::singleShot(1000, this, [this]() {
        QByteArray response = A.read_from_arduino();
        if (!response.isEmpty()) {
            qDebug() << "Réponse Arduino:" << response;
        } else {
            qDebug() << "Pas de réponse d'Arduino";
        }
    });

    qDebug() << "=== FIN DIAGNOSTIC ===";
}
