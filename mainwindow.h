#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QTableWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // ===== Séance CRUD + UI =====
    void on_pushButton_Ajouter_2_clicked();
    void on_pushButton_Modifier2_clicked();
    void on_pushButton_Supprimer2_clicked();
    void on_pushButton_Rechercher2_clicked();
    void on_pushButton_TrierSeance_clicked();
    void on_tabS_itemClicked(QTableWidgetItem *item);
    void on_pb_annuler_ajout_materiel_clicked();

    // ===== (Stubs) — gardés vides si ton .ui les connecte =====
    void on_candidat_clicked();
    void on_candidat_2_clicked();
    void on_candidat_3_clicked();
    void on_candidat_4_clicked();
    void on_candidat_5_clicked();
    void on_vehicule_clicked();
    void on_vehicule_2_clicked();
    void on_vehicule_3_clicked();
    void on_vehicule_4_clicked();
    void on_vehicule_5_clicked();
    void on_employee_clicked();
    void on_employee_2_clicked();
    void on_employee_3_clicked();
    void on_employee_5_clicked();
    void on_employee_6_clicked();
    void on_seance_clicked();
    void on_seance_2_clicked();
    void on_seance_3_clicked();
    void on_seance_5_clicked();
    void on_seance_6_clicked();
    void on_examen_clicked();
    void on_examen_2_clicked();
    void on_examen_3_clicked();
    void on_examen_4_clicked();
    void on_examen_5_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_Ajouter_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_54_clicked();
    void on_btnStats_clicked();
    void on_pushButton_Rechercher_clicked();

private:
    Ui::MainWindow *ui;

    void initSeanceTable();
    void rechargerTableSeancesDepuisDB();
};

#endif // MAINWINDOW_H
