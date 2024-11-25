#ifndef COMMANDE_H
#define COMMANDE_H

#include <QObject>
#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

class Commande : public QObject
{
    Q_OBJECT
private:
    QString id_commande;
    QDate date_c;
    QDate date_l;
    float montant;
    QString id_f;

public:
    // Constructors
    Commande();
    Commande(QString id, QDate dateC, QDate dateL, float mont, QString idF);

    // Getters
    QString getId() const { return id_commande; }
    QDate getDateC() const { return date_c; }
    QDate getDateL() const { return date_l; }
    float getMontant() const { return montant; }
    QString getIdF() const { return id_f; }

    // Setters
    void setId(QString id) { id_commande = id; }
    void setDateC(QDate date) { date_c = date; }
    void setDateL(QDate date) { date_l = date; }
    void setMontant(float mont) { montant = mont; }
    void setIdF(QString id) { id_f = id; }

    // SQL Functions
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
    bool modifier();
    QSqlQueryModel* rechercher(const QString &critere);
    QSqlQueryModel* trier(const QString &critere);
};

#endif // COMMANDE_H
