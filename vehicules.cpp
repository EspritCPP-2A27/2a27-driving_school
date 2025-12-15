#include "vehicules.h"

vehicules::vehicules() {}
#include "vehiculemanager.h"
#include <QMessageBox>
#include <QTimer>
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
#include <QRegularExpression>
#include <QLabel>
#include <QBrush>
#include <QColor>

VehiculeManager::VehiculeManager(QObject *parent) : QObject(parent) {}

void VehiculeManager::chargerVehicules(QTableWidget *tableWidget)
{
    if (!tableWidget) return;

    tableWidget->setRowCount(0);
    QSqlQuery query("SELECT MATRICULE, TYPE, ETAT_VOITURE, KILOMETRAGE, "
                    "TO_CHAR(DATE_MAINTENANCE, 'DD/MM/YYYY'), ASSURANCE, DISPONIBILITE, "
                    "TO_CHAR(DATE_ASSURANCE, 'DD/MM/YYYY') FROM VEHICULE");

    int row = 0;
    while (query.next()) {
        tableWidget->insertRow(row);
        for (int col = 0; col < 8; ++col) {
            tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
}

bool VehiculeManager::verifierMatriculeValide(const QString &matricule)
{
    QRegularExpression regex("^[0-9]+ nt [0-9]+$");
    return regex.match(matricule).hasMatch();
}

bool VehiculeManager::verifierVehiculeExiste(const QString &matricule)
{
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM VEHICULE WHERE MATRICULE = :mat");
    checkQuery.bindValue(":mat", matricule);
    if (!checkQuery.exec()) {
        return false;
    }
    checkQuery.next();
    return checkQuery.value(0).toInt() > 0;
}

void VehiculeManager::ajouterVehicule(const QString &matricule, const QString &type, const QString &etat,
                                      const QString &dispo, const QString &km, const QString &assurance,
                                      const QDate &dateMaintenance, const QDate &dateAssurance, QTableWidget *tableWidget)
{
    if (matricule.isEmpty()) {
        emit notification("Veuillez entrer un matricule.");
        return;
    }

    if (!verifierMatriculeValide(matricule)) {
        emit notification("Le matricule doit être au format : '123 nt 6450'.");
        return;
    }

    QDate today = QDate::currentDate();
    if (dateMaintenance < today) {
        emit notification("La date de maintenance ne peut pas être avant aujourd'hui.");
        return;
    }

    if (dateAssurance < today) {
        emit notification("La date d'assurance ne peut pas être avant aujourd'hui.");
        return;
    }

    bool kmOk;
    int kmNum = km.toInt(&kmOk);
    if (!kmOk) {
        emit notification("Le kilométrage doit être un nombre.");
        return;
    }

    if (verifierVehiculeExiste(matricule)) {
        emit notification("Un véhicule avec ce matricule existe déjà !");
        return;
    }

    QSqlQuery query;
    query.prepare(R"(INSERT INTO VEHICULE (MATRICULE, TYPE, ETAT_VOITURE, KILOMETRAGE, DATE_MAINTENANCE, ASSURANCE, DISPONIBILITE, DATE_ASSURANCE)
                    VALUES (:mat, :type, :etat, :km, TO_DATE(:dateM, 'DD/MM/YYYY'), :ass, :dispo, TO_DATE(:dateA, 'DD/MM/YYYY')))");

    query.bindValue(":mat", matricule);
    query.bindValue(":type", type);
    query.bindValue(":etat", etat);
    query.bindValue(":km", kmNum);
    query.bindValue(":dateM", dateMaintenance.toString("dd/MM/yyyy"));
    query.bindValue(":ass", assurance);
    query.bindValue(":dispo", dispo);
    query.bindValue(":dateA", dateAssurance.toString("dd/MM/yyyy"));

    if (query.exec()) {
        emit notification("Véhicule ajouté avec succès !");
        chargerVehicules(tableWidget);
        verifierDateAssuranceEtNotifier(matricule, dateAssurance);
        emit vehiculeAjoute();
    } else {
        emit notification("Insertion échouée : " + query.lastError().text());
    }
}

void VehiculeManager::supprimerVehicule(const QString &matricule, QTableWidget *tableWidget)
{
    if (matricule.isEmpty()) {
        emit notification("Veuillez entrer un matricule a supprimer");
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM VEHICULE WHERE MATRICULE = :mat");
    query.bindValue(":mat", matricule);

    if (!query.exec()) {
        emit notification("Erreur SQL : " + query.lastError().text());
        return;
    }

    if (query.numRowsAffected() == 0) {
        emit notification("Aucun vehicule trouve avec ce matricule");
        return;
    }

    emit notification("Vehicule supprime avec succes");
    chargerVehicules(tableWidget);
    emit vehiculeSupprime();
}

void VehiculeManager::rechercherVehicule(const QString &matricule, QTableWidget *tableWidget)
{
    if (!tableWidget) return;

    if (matricule.isEmpty()) {
        emit notification("Veuillez entrer un matricule a rechercher");
        return;
    }

    chargerVehicules(tableWidget);
    bool found = false;
    for (int i = 0; i < tableWidget->rowCount(); ++i) {
        QTableWidgetItem *item = tableWidget->item(i, 0);
        if (item && item->text().compare(matricule, Qt::CaseInsensitive) == 0) {
            found = true;
            for (int j = 0; j < tableWidget->columnCount(); ++j) {
                tableWidget->item(i, j)->setBackground(QBrush(QColor("lightgreen")));
            }

            tableWidget->scrollToItem(item);

            QTimer::singleShot(20000, [tableWidget, i]() {
                if (i < tableWidget->rowCount()) {
                    for (int j = 0; j < tableWidget->columnCount(); ++j) {
                        tableWidget->item(i, j)->setBackground(Qt::white);
                    }
                }
            });
            break;
        }
    }

    if (!found) {
        emit notification("Aucun véhicule trouvé avec ce matricule.");
    }
}

void VehiculeManager::trierVehicules(const QString &critere, QTableWidget *tableWidget)
{
    if (!tableWidget) return;

    if (critere != "Etat" && critere != "Disponibilite") {
        emit notification("Veuillez choisir un critère de tri.");
        return;
    }

    QString requete;
    if (critere == "Etat") {
        requete = "SELECT MATRICULE, TYPE, ETAT_VOITURE, KILOMETRAGE, "
                  "TO_CHAR(DATE_MAINTENANCE,'DD/MM/YYYY'), ASSURANCE, DISPONIBILITE, "
                  "TO_CHAR(DATE_ASSURANCE,'DD/MM/YYYY') "
                  "FROM VEHICULE WHERE ETAT_VOITURE = 'Fonctionnelle'";
    } else if (critere == "Disponibilite") {
        requete = "SELECT MATRICULE, TYPE, ETAT_VOITURE, KILOMETRAGE, "
                  "TO_CHAR(DATE_MAINTENANCE,'DD/MM/YYYY'), ASSURANCE, DISPONIBILITE, "
                  "TO_CHAR(DATE_ASSURANCE,'DD/MM/YYYY') "
                  "FROM VEHICULE WHERE DISPONIBILITE = 'OUI'";
    }

    QSqlQuery query(QSqlDatabase::database());
    if (!query.exec(requete)) {
        emit notification("Erreur SQL : " + query.lastError().text());
        return;
    }

    tableWidget->setRowCount(0);
    int row = 0;
    while (query.next()) {
        tableWidget->insertRow(row);
        for (int col = 0; col < 8; col++) {
            tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }

    emit notification(QString("Tri appliqué selon : %1").arg(critere));

    QTimer::singleShot(20000, [this, tableWidget]() {
        chargerVehicules(tableWidget);
    });
}

void VehiculeManager::modifierVehicule(const QString &matricule, const QString &type, const QString &etat,
                                       const QString &dispo, const QString &km, const QString &assurance,
                                       const QDate &dateMaintenance, const QDate &dateAssurance, QTableWidget *tableWidget)
{
    if (matricule.isEmpty()) {
        emit notification("veuillez entrer le matricule du vehicule a modifier");
        return;
    }

    if (!verifierVehiculeExiste(matricule)) {
        emit notification("Aucun véhicule trouvé avec ce matricule !");
        return;
    }

    bool kmOk;
    int kmNum = km.toInt(&kmOk);
    if (!kmOk) {
        emit notification("Le kilométrage doit être un nombre.");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE VEHICULE SET TYPE=:type, ETAT_VOITURE=:etat, "
                  "KILOMETRAGE=:km, DATE_MAINTENANCE=TO_DATE(:dm, 'DD/MM/YYYY'), "
                  "ASSURANCE=:ass, DISPONIBILITE=:dispo, DATE_ASSURANCE=TO_DATE(:da, 'DD/MM/YYYY') "
                  "WHERE MATRICULE=:mat");

    query.bindValue(":mat", matricule);
    query.bindValue(":type", type);
    query.bindValue(":etat", etat);
    query.bindValue(":km", kmNum);
    query.bindValue(":dm", dateMaintenance.toString("dd/MM/yyyy"));
    query.bindValue(":ass", assurance);
    query.bindValue(":dispo", dispo);
    query.bindValue(":da", dateAssurance.toString("dd/MM/yyyy"));

    if (query.exec()) {
        emit notification("Véhicule modifié avec succès !");
        chargerVehicules(tableWidget);
        emit vehiculeModifie();
    } else {
        emit notification("Échec de la modification : " + query.lastError().text());
    }
}

void VehiculeManager::exporterPDF(QTableWidget *tableWidget, QWidget *parent)
{
    if (!tableWidget || !parent) return;

    QString filePath = QFileDialog::getSaveFileName(parent, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
    if (filePath.isEmpty()) return;

    QPdfWriter pdf(filePath);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);

    QPainter painter(&pdf);
    painter.setRenderHint(QPainter::Antialiasing);

    int marginLeft = 60;
    int marginTop = 100;
    int rowHeight = 70;
    int headerHeight = 40;
    int tableWidth = pdf.width() - marginLeft * 3;
    int colCount = tableWidget->columnCount();
    int equalWidth = tableWidth / colCount;

    painter.setFont(QFont("Arial", 28, QFont::Bold));
    painter.drawText(QRect(0, marginTop, pdf.width(), 120), Qt::AlignCenter, "Liste des véhicules");

    marginTop += 120;
    painter.setFont(QFont("Arial", 12));
    painter.drawText(marginLeft, marginTop, "Date d'export : " + QDate::currentDate().toString("dd/MM/yyyy"));

    marginTop += 40;
    painter.setBrush(QColor("#E8E8E8"));
    painter.setPen(Qt::NoPen);
    painter.drawRect(marginLeft, marginTop, tableWidth, headerHeight);

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 11, QFont::Bold));

    int x = marginLeft;
    for (int j = 0; j < colCount; j++) {
        QString header = tableWidget->horizontalHeaderItem(j) ?
                             tableWidget->horizontalHeaderItem(j)->text() : QString("Col %1").arg(j);
        painter.drawText(QRect(x + 3, marginTop + 5, equalWidth, headerHeight),
                         Qt::AlignLeft | Qt::AlignVCenter, header);
        x += equalWidth;
    }

    painter.setPen(QPen(Qt::black, 1));
    painter.drawLine(marginLeft, marginTop + headerHeight, marginLeft + tableWidth, marginTop + headerHeight);

    marginTop += headerHeight;
    painter.setFont(QFont("Arial", 10));

    for (int i = 0; i < tableWidget->rowCount(); i++) {
        if (i % 2 == 0) {
            painter.setBrush(QColor("#F7F7F7"));
            painter.setPen(Qt::NoPen);
            painter.drawRect(marginLeft, marginTop, tableWidth, rowHeight);
        }

        painter.setPen(Qt::black);
        int xPos = marginLeft;
        for (int j = 0; j < colCount; j++) {
            QString text = tableWidget->item(i, j) ? tableWidget->item(i, j)->text() : "";
            painter.drawText(QRect(xPos + 3, marginTop + 5, equalWidth, rowHeight),
                             Qt::AlignLeft | Qt::AlignVCenter, text);
            xPos += equalWidth;
        }

        painter.setPen(QPen(Qt::gray, 1));
        painter.drawLine(marginLeft, marginTop + rowHeight, marginLeft + tableWidth, marginTop + rowHeight);

        marginTop += rowHeight;

        if (marginTop > pdf.height() - 100) {
            pdf.newPage();
            marginTop = 100;
        }
    }

    painter.end();
    emit notification("Le fichier a été exporté avec succès !");
}

void VehiculeManager::marquerDatesCalendrier(QCalendarWidget *calendarWidget)
{
    if (!calendarWidget) return;

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
            calendarWidget->setDateTextFormat(d1, format);

        if (d2.isValid())
            calendarWidget->setDateTextFormat(d2, format);
    }
}

void VehiculeManager::verifierAssurancesDepuisBase()
{
    QSqlQuery query("SELECT MATRICULE, DATE_ASSURANCE FROM VEHICULE");

    while (query.next()) {
        QString matricule = query.value(0).toString();
        QDate dateAssurance = query.value(1).toDate();
        verifierDateAssuranceEtNotifier(matricule, dateAssurance);
    }
}

void VehiculeManager::verifierDateAssuranceEtNotifier(const QString &matricule, const QDate &dateAssurance)
{
    QDate today = QDate::currentDate();
    int joursRestants = today.daysTo(dateAssurance);

    if (joursRestants <= 7 && joursRestants >= 0) {
        QString msg = QString("⚠️ L'assurance du véhicule %1 doit être renouvelée dans %2 jour(s) !")
                          .arg(matricule).arg(joursRestants);
        emit notification(msg);
    }
}

void VehiculeManager::afficherToast(const QString &message, QWidget *parent)
{
    if (!parent) return;

    QWidget *toast = new QWidget(parent);
    toast->setStyleSheet("background-color: white; color: black; padding: 20px; "
                         "border-color: rgb(0, 0, 0); border-radius: 15px; font-size: 14px;");

    QLabel *label = new QLabel(message, toast);
    QVBoxLayout *layout = new QVBoxLayout(toast);
    layout->addWidget(label);
    toast->setLayout(layout);
    toast->adjustSize();
    toast->move(parent->width()/2 - toast->width()/2, parent->height() - 120);
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

void VehiculeManager::jouerSonNotification()
{
    QSoundEffect *son = new QSoundEffect(this);
    son->setSource(QUrl("qrc:/sons/notif.wav"));
    son->setVolume(0.50);
    son->play();
}

QString VehiculeManager::getVehiculeInfoAtDate(const QDate &date)
{
    QString dateStr = date.toString("dd/MM/yyyy");
    QStringList infoTrouvee;

    QSqlQuery query;
    query.prepare(R"(SELECT MATRICULE, TO_CHAR(DATE_MAINTENANCE, 'DD/MM/YYYY'),
                    TO_CHAR(DATE_ASSURANCE, 'DD/MM/YYYY') FROM VEHICULE)");

    if (!query.exec()) {
        return "Erreur lors de la lecture de la base.";
    }

    while (query.next()) {
        QString matricule = query.value(0).toString();
        QString dateMaintenance = query.value(1).toString();
        QString dateAssurance = query.value(2).toString();

        if (dateMaintenance == dateStr)
            infoTrouvee << QString("🚗 %1 → Date de maintenance").arg(matricule);

        if (dateAssurance == dateStr)
            infoTrouvee << QString("🚙 %1 → Date d'assurance").arg(matricule);
    }

    return infoTrouvee.isEmpty() ? QString("Aucun véhicule trouvé le %1").arg(dateStr) : infoTrouvee.join("\n");
}

void VehiculeManager::remplirChampsDepuisTableau(int row, QTableWidget *tableWidget,
                                                 QLineEdit *matriculeEdit, QComboBox *typeCombo,
                                                 QRadioButton *radioPanne, QRadioButton *radioFonctionnelle,
                                                 QLineEdit *kmEdit, QDateEdit *dateMaintenanceEdit,
                                                 QRadioButton *radioTousRisque, QRadioButton *radioIntermediaire,
                                                 QRadioButton *radioRC, QComboBox *dispoCombo, QDateEdit *dateAssuranceEdit)
{
    if (!tableWidget || row < 0 || row >= tableWidget->rowCount()) return;

    QString matricule = tableWidget->item(row, 0)->text();
    QString type = tableWidget->item(row, 1)->text();
    QString etat = tableWidget->item(row, 2)->text();
    QString kilometrage = tableWidget->item(row, 3)->text();
    QString dateMaintenance = tableWidget->item(row, 4)->text();
    QString assurance = tableWidget->item(row, 5)->text();
    QString dispo = tableWidget->item(row, 6)->text();
    QString dateAssurance = tableWidget->item(row, 7)->text();

    if (matriculeEdit) {
        matriculeEdit->setText(matricule);
        matriculeEdit->setReadOnly(true);
    }

    if (typeCombo) {
        int typeIndex = typeCombo->findText(type);
        if (typeIndex != -1) typeCombo->setCurrentIndex(typeIndex);
    }

    if (radioPanne && radioFonctionnelle) {
        if (etat == "En panne")
            radioPanne->setChecked(true);
        else
            radioFonctionnelle->setChecked(true);
    }

    if (kmEdit) kmEdit->setText(kilometrage);

    if (dateMaintenanceEdit) {
        QDate dateM = QDate::fromString(dateMaintenance, "dd/MM/yyyy");
        dateMaintenanceEdit->setDate(dateM);
    }

    if (radioTousRisque && radioIntermediaire && radioRC) {
        if (assurance == "Tous Risque")
            radioTousRisque->setChecked(true);
        else if (assurance == "Intermediaire")
            radioIntermediaire->setChecked(true);
        else
            radioRC->setChecked(true);
    }

    if (dispoCombo) {
        int dispoIndex = dispoCombo->findText(dispo);
        if (dispoIndex != -1) dispoCombo->setCurrentIndex(dispoIndex);
    }

    if (dateAssuranceEdit) {
        QDate dateA = QDate::fromString(dateAssurance, "dd/MM/yyyy");
        dateAssuranceEdit->setDate(dateA);
    }
}

void VehiculeManager::afficherStatistiques(QTableWidget *tableWidget, QWidget *parent)
{
    if (!tableWidget || !parent) return;

    int total = tableWidget->rowCount();
    if (total == 0) {
        emit notification("🚗 Aucun véhicule trouvé !");
        return;
    }

    int fonctionnelles = 0;
    int enPanne = 0;
    int colEtat = 2;

    for (int i = 0; i < total; ++i) {
        QString etat = tableWidget->item(i, colEtat)->text().trimmed();
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

    QWidget *window = new QWidget(parent);
    window->setAttribute(Qt::WA_DeleteOnClose);
    QVBoxLayout *layout = new QVBoxLayout(window);
    layout->addWidget(chartView);
    window->setLayout(layout);
    window->setWindowTitle("Statistiques véhicules");
    window->resize(700, 500);
    window->show();
}
