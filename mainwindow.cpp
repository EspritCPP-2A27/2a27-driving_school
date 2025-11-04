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


void MainWindow::on_pushButton_Ajouter_clicked()
{

        QString matricule = ui->matricule->text().trimmed();
        QString type = ui->comboBox_TypeVehicule->currentText();
        QString etat = ui->radioButton_EnPanne->isChecked() ? "En panne" : "Fonctionnelle";
        QString dispo = ui->comboBox_dipo->currentText();
        QString km = ui->lineEdit_Kilometrage->text().trimmed();
        QString assurance;
        QDate dateMaintenance = ui->dateEdit_Maintenance->date();
        QDate dateAssurance = ui->date_assurance->date();
        QDate today = QDate::currentDate();


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


