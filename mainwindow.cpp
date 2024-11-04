#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "vehicule.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(Vtmp.afficher()); // Assuming Vtmp is a global or class instance of Vehicule
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonAjouter_clicked() // Changed to match XML button name
{
    int id = ui->lineEditID->text().toInt();
    QString marque = ui->lineEditMarque->text();
    QString modele = ui->lineEditModele->text();
    QString annee = ui->lineEditAnnee->text();
    QString immatriculation = ui->lineEditImmatriculation->text();
    QString type = ui->lineEditType->text();
    QString etat = ui->lineEditEtat->text();
    QString historique_entretiens = ui->lineEditHE->text(); // Updated to match XML
    QString nom_proprietaire = ui->lineEditNomP->text(); // Updated to match XML

    Vehicule V(id, marque, modele, annee, immatriculation, type, etat, historique_entretiens, nom_proprietaire);
    bool test = V.ajouter();

    if (test) {
        QMessageBox::information(this, tr("OK"),
                                 tr("Ajout effectué\nCliquez sur Cancel pour quitter."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(this, tr("NOT OK"),
                                 tr("Ajout non effectué\nCliquez sur Cancel pour quitter."), QMessageBox::Cancel);
    }
}

void MainWindow::on_Supprimer_clicked()
{
    int id = ui->lineEditIDS->text().toInt();
    bool test = Vtmp.supprimer(id);

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Suppression effectuée\nCliquez sur Cancel pour quitter."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(this, tr("NOT OK"),
                                 tr("Suppression non effectuée\nCliquez sur Cancel pour quitter."), QMessageBox::Cancel);
    }
}

void MainWindow::on_RechercherID_clicked()
{
    int id = ui->lineEditIDR->text().toInt(); // Updated to match XML
    Vehicule V = Vtmp.findById(id); // Assuming Vtmp is a global or class instance of Vehicule

    if (V.getID() != -1) {
        ui->lineEditIDRM->setText(QString::number(V.getID())); // Assuming you need to set these fields
        ui->lineEditMarque->setText(V.getMarque());
        ui->lineEditModele->setText(V.getModele());
        ui->lineEditAnnee->setText(V.getAnnee());
        ui->lineEditImmatriculation->setText(V.getImmatriculation());
        ui->lineEditType->setText(V.getType());
        ui->lineEditEtat->setText(V.getEtat());
        ui->lineEditHE->setText(V.getHistoriqueEntretiens());
        ui->lineEditNomP->setText(V.getNomProprietaire());

        QMessageBox::information(this, tr("Véhicule trouvé"),
                                 tr("Les informations du véhicule ont été chargées."));
    } else {
        QMessageBox::critical(this, tr("Erreur"),
                              tr("Véhicule introuvable.\nVeuillez vérifier l'ID."),
                              QMessageBox::Cancel);
    }
}

void MainWindow::on_Rechercher_clicked()
{
    int id = ui->lineEditIDRM->text().toInt(); // Updated to match XML
    Vehicule V = Vtmp.findById(id); // Assuming Vtmp is a global or class instance of Vehicule

    if (V.getID() != -1) {
        // Populate fields with vehicle data for updating
        ui->lineEditIDRM->setText(QString::number(V.getID())); // Set the ID in the corresponding field
        ui->lineEditMarque->setText(V.getMarque());
        ui->lineEditModele->setText(V.getModele());
        ui->lineEditAnnee->setText(V.getAnnee());
        ui->lineEditImmatriculation->setText(V.getImmatriculation());
        ui->lineEditType->setText(V.getType());
        ui->lineEditEtat->setText(V.getEtat());
        ui->lineEditHE->setText(V.getHistoriqueEntretiens());
        ui->lineEditNomP->setText(V.getNomProprietaire());

        QMessageBox::information(this, tr("Véhicule trouvé"),
                                 tr("Les informations du véhicule ont été chargées pour mise à jour."));
    } else {
        QMessageBox::critical(this, tr("Erreur"),
                              tr("Véhicule introuvable.\nVeuillez vérifier l'ID."),
                              QMessageBox::Cancel);
    }
}

void MainWindow::on_sauvegarde_clicked()
{
    int id = ui->lineEditIDRM->text().toInt(); // Updated to match XML
    QString marque = ui->lineEditMarque->text();
    QString modele = ui->lineEditModele->text();
    QString annee = ui->lineEditAnnee->text();
    QString immatriculation = ui->lineEditImmatriculation->text();
    QString type = ui->lineEditType->text();
    QString etat = ui->lineEditEtat->text();
    QString historique_entretiens = ui->lineEditHE->text(); // Updated to match XML
    QString nom_proprietaire = ui->lineEditNomP->text(); // Updated to match XML

    Vehicule V(id, marque, modele, annee, immatriculation, type, etat, historique_entretiens, nom_proprietaire);
    bool test = V.mettreAJour(id); // Assuming mettreAJour is defined correctly

    if (test) {
        QMessageBox::information(this, tr("Sauvegarde réussie"),
                                 tr("Les informations du véhicule ont été mises à jour."));
    } else {
        QMessageBox::critical(this, tr("Erreur"),
                              tr("La mise à jour des informations du véhicule a échoué."),
                              QMessageBox::Cancel);
    }

    // Refresh the table view after saving
    ui->tableView->setModel(Vtmp.afficher()); // Assuming Vtmp is a global or class instance of Vehicule
}
