#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSqlDatabase>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "arduino.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_candidat_5_clicked();

    void on_candidat_clicked();

    void on_vehicule_clicked();

    void on_employee_clicked();

    void on_seance_clicked();

    void on_examen_clicked();

    void on_candidat_2_clicked();

    void on_vehicule_2_clicked();

    void on_employee_2_clicked();

    void on_seance_2_clicked();

    void on_examen_2_clicked();

    void on_candidat_3_clicked();

    void on_vehicule_3_clicked();

    void on_employee_3_clicked();

    void on_seance_3_clicked();

    void on_examen_3_clicked();

    void on_candidat_4_clicked();

    void on_vehicule_4_clicked();

    void on_employee_6_clicked();

    void on_seance_6_clicked();

    void on_examen_4_clicked();

    void on_vehicule_5_clicked();

    void on_employee_5_clicked();

    void on_seance_5_clicked();

    void on_examen_5_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_Ajouter_clicked();

    void on_pushButton_supprimer_clicked();

    void on_pushButton_54_clicked();

    void on_btnStats_clicked();

    void on_pushButton_Rechercher_clicked();



    void chargerVehicules();

    void on_pushButton_55_clicked();

    void on_pushButton_56_clicked();

    void on_pushButton_57_clicked();

    void on_pushButton_58_clicked();

    void on_pushButton_105_clicked();

    void on_pushButton_Annuler_clicked();

    void on_pushButton_53_clicked();

    void on_tableWidget_Vehicule_cellClicked(int row, int column);



    void on_calendarWidget_clicked(const QDate &date);


    void afficherToast(QString message);
    void jouerSonNotification();
    void verifierDateAssuranceEtNotifier(const QString &matricule,const QDate &dateAssurance);
    void verifierAssurancesDepuisBase();

    void marquerDatesCalendrier();

    void on_pushButton_75_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_tableWidget_11_cellClicked(int row, int column);



    void on_pushButton_146_clicked();
    void rechargerTableSeancesDepuisDB();

    void on_pushButton_Rechercher_2_clicked();

    void on_stats_emp_clicked();

    void on_pushButton_ajouter_emp_clicked();



    void on_pushButton_modifier_emp_clicked();
    void on_pushButton_supp_emp_clicked();

    void afficherEmployes();

    void on_pushButton_33_clicked();



    void on_pushButton_23_clicked();

    void on_pushButton_147_clicked();

    void on_pushButton_20_clicked();

    void chargerCandidats();
    void on_pushButton_trier_clicked();

    void on_pushButton_77_clicked();

   void lireArduino();
    void on_pushButton_Off_clicked();
   void on_pushButton_On_clicked();


   void on_pushButton_81_clicked();

   void on_pdfseance_clicked();
   void createBarChart(const QMap<QString, int>& data, const QString& title, QLayout* layout);

   void createPieChart(const QMap<QString, int>& data, const QString& title, QLayout* layout);
   void showStats();

   void on_triseance_clicked();
   void on_statseance_clicked();

   void on_tabS_cellClicked(int row, int column);
   void readArduino();
   void checkRFIDCard(const QString &cardUID);
   void on_pushButton_RegisterRFID_clicked();
   void on_pushButton_RemoveRFID_clicked();
   void on_pushButton_TestRFID_clicked();
   void processRFIDRegistration(const QString &cardUID);

   void processRFIDCard(const QString &cardUID);
   void on_pushButton_CancelRegister_clicked();

   void highlightSelectedRow(int row);
   void debugTableStructure();
   void on_tableWidget_Employe_cellClicked(int row, int column);

   void on_pushButton_RegisterRFID__clicked();
   void diagnoseRFIDProblem();

   private:
    Ui::MainWindow *ui;
    QSerialPort *arduino;
    QString portName;

    bool isRegisteringMode;
    QString employeeToRegister;
    QDialog* rfidWaitDialog;
    QString getTableCellText(int row, int column);


    Arduino A;

};

#endif // MAINWINDOW_H
