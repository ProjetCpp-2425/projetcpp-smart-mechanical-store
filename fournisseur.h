#ifndef FOURNISSEUR_H
#define FOURNISSEUR_H

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

class fournisseur
{
public:
    fournisseur();
    fournisseur(QString, QString, QString, QString, QString, QString);

    // Setters

    void setnom(QString n);
    void setprenom(QString n);
    void setadresse(QString n);
    void settel(QString tel);
    void setemail(QString n);

    // Getters
    QString get_id();
    QString get_nom();
    QString get_prenom();
    QString get_adresse();
    QString get_tel();
    QString get_email();

    // Fonctions de base
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(QString);
    bool modifier(QString);
    bool recherchee(QString nom, QSqlQueryModel*& model);
    QSqlQueryModel * recherche(QString recherche);
    bool exporterPDF(const QString& fileName, QSqlQueryModel* model);
    void genererStatistiquesTYPEF(QSqlQueryModel* model);
    QSqlQueryModel* trierParNom();
    QMap<QString, int> getCountByAdresse();
    bool searchById(const QString &id) const;
    bool searchByName(const QString &name) const;







private:
    QString  nom, prenom, adresse, email,tel,id;
};

#endif // FOURNISSEUR_H
