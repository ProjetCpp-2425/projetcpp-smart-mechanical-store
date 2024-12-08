#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QComboBox>
#include <QLineEdit>
#include <QTableView>
#include <QSerialPort> // Pour gérer le port série
#include "produits.h"  // Inclure la classe produits pour les interactions avec la base de données

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void showEvent(QShowEvent *event);
    void on_enregistrerButton_8_clicked();








    void on_ajouter_clicked();

    void on_recherche_clicked();

    void on_modifier_clicked();

    void on_supprimer_clicked();

    void on_statistique_clicked();

    void on_pdf_clicked();



    void on_pushButton_clicked();

    //void on_enregistrerButton_10_clicked();

private:
    Ui::MainWindow *ui;

       QComboBox *comboBox;  // ComboBox pour choisir le critère de recherche
       QLineEdit *lineEdit;  // Champ pour saisir le terme de recherche
       QTableView *tableView; // Table pour afficher les résultats
       QSqlQueryModel *model;
       QSerialPort serial; // Objet pour gérer la communication série

       // Méthode privée pour obtenir la quantité générale
       void sendToArduino(int totalQuantity);
       void sendTotalQuantityToArduino(); // Méthode pour envoyer la quantité à l'Arduino
       void setupSerialPort();


};

#endif // MAINWINDOW_H
