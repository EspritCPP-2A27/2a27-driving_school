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





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

   chargerVehicules();
    rechargerTableSeancesDepuisDB();
   afficherEmployes();
    marquerDatesCalendrier();


    QGraphicsOpacityEffect *effet = new QGraphicsOpacityEffect(this);

    ui->labelWelcome->setGraphicsEffect(effet);

    QPropertyAnimation *anim = new QPropertyAnimation(effet, "opacity");

    anim->setDuration(3000);

    anim->setStartValue(1.0);

    anim->setEndValue(0.0);

    anim->setEasingCurve(QEasingCurve::InOutQuad);

    QTimer::singleShot(1000, anim, SLOT(start()));

    connect(anim, &QPropertyAnimation::finished, [=]() {

        ui->labelWelcome->hide();
    });
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
    if (!ui->tabS) return; // Vérifie que le tableau existe
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
    QSqlQuery query("SELECT * FROM EMPLOYE");

    ui->tableWidget_Employe->setRowCount(0); // clear table
    int row = 0;
    while (query.next()) {
        ui->tableWidget_Employe->insertRow(row);
        ui->tableWidget_Employe->setItem(row, 0, new QTableWidgetItem(query.value("ID_EMPLOYE").toString()));
        ui->tableWidget_Employe->setItem(row, 1, new QTableWidgetItem(query.value("NOM").toString()));
        ui->tableWidget_Employe->setItem(row, 2, new QTableWidgetItem(query.value("PRENOM").toString()));
        ui->tableWidget_Employe->setItem(row, 3, new QTableWidgetItem(query.value("ROLE").toString()));
        ui->tableWidget_Employe->setItem(row, 4, new QTableWidgetItem(query.value("NUMTEL").toString()));
        ui->tableWidget_Employe->setItem(row, 5, new QTableWidgetItem(query.value("EMAIL").toString()));
        ui->tableWidget_Employe->setItem(row, 6, new QTableWidgetItem(query.value("SPECIALITE").toString()));
        ui->tableWidget_Employe->setItem(row, 7, new QTableWidgetItem(query.value("SALAIRE").toString()));
        ui->tableWidget_Employe->setItem(row, 8, new QTableWidgetItem(query.value("MDP").toString()));
        row++;
    }
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

            marquerDatesCalendrier();

            ui->lineEdit_Kilometrage->clear();
            ui->matricule->clear();
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
        int colEtat = 2; // colonne "Etat"

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
        requete =
            "SELECT MATRICULE, TYPE, ETAT_VOITURE, KILOMETRAGE, "
            "TO_CHAR(DATE_MAINTENANCE,'DD/MM/YYYY'), ASSURANCE, DISPONIBILITE, "
            "TO_CHAR(DATE_ASSURANCE,'DD/MM/YYYY') "
            "FROM VEHICULE WHERE ETAT_VOITURE = 'Fonctionnelle'";
    }
    else if (critere == "Disponibilite") {
        requete =
            "SELECT MATRICULE, TYPE, ETAT_VOITURE, KILOMETRAGE, "
            "TO_CHAR(DATE_MAINTENANCE,'DD/MM/YYYY'), ASSURANCE, DISPONIBILITE, "
            "TO_CHAR(DATE_ASSURANCE,'DD/MM/YYYY') "
            "FROM VEHICULE WHERE DISPONIBILITE = 'OUI'";
    }

    // ✅ utiliser la connexion active
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

    QMessageBox::information(this, "Tri appliqué",
                             QString("Affichage filtré selon : %1").arg(critere));

    // recharge après 20 secondes
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

        painter.setFont(QFont("Arial", 10));

        int marginLeft = 100;

        int marginTop = 150;

        int lineHeight = 25;


        painter.setFont(QFont("Arial", 14, QFont::Bold));

        painter.drawText(marginLeft, marginTop, "Liste des vehicules");

        marginTop += 40;

        painter.setFont(QFont("Arial", 10, QFont::Bold));

        QString header;

        for (int j = 0; j < ui->tableWidget_Vehicule->columnCount(); ++j)

            header += ui->tableWidget_Vehicule->horizontalHeaderItem(j)->text() + " | ";

        painter.drawText(marginLeft, marginTop, header);

        marginTop += 20;

        painter.setFont(QFont("Arial", 10));

        painter.setPen(Qt::black);

        for (int i = 0; i < ui->tableWidget_Vehicule->rowCount(); ++i) {

            QString line;

            for (int j = 0; j < ui->tableWidget_Vehicule->columnCount(); ++j) {

                QTableWidgetItem *item = ui->tableWidget_Vehicule->item(i, j);

                line += (item ? item->text() : "") + " | ";
            }
            painter.drawText(marginLeft, marginTop, line);

            marginTop += lineHeight;

            if (marginTop > pdf.height() - 100) {

                pdf.newPage();

                marginTop = 150;
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
    query.prepare(R"(
        SELECT MATRICULE, TO_CHAR(DATE_MAINTENANCE, 'DD/MM/YYYY'), TO_CHAR(DATE_ASSURANCE, 'DD/MM/YYYY')
        FROM VEHICULE
    )");

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


//ajouter seance
// Ajouter séance
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

        // Vider et remplir le tableau avec les résultats
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

