#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSqlDatabase>

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
    void marquerDatesCalendrier();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
