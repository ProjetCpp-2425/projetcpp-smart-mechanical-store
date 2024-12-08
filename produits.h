#ifndef PRODUITS_H
#define PRODUITS_H

#include <QString>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>

class produits
{
public:
    int ID_PROD;
    QString QUANTITE;
    QString EMPLACEMENT;
    QString DATE_ENTREE;
    QString DATE_ESPIRATION;

public:
     produits();
produits(QString QUANTITE, QString EMPLACEMENT, QString DATE_ENTREE, QString DATE_ESPIRATION);

   produits(int ID_PROD, QString QUANTITE, QString EMPLACEMENT, QString DATE_ENTREE, QString DATE_ESPIRATION);


    void ajouter();
    QSqlQueryModel*afficher();
    bool supp(int ID);
    bool modifier();
    bool recherche(int ID);
    QSqlQueryModel* tri_par_id_decroissant();
    QSqlQueryModel* tri_par_id_croissant();
   QSqlQueryModel* tri_par_EMPLACEMENT_croissant();
   QSqlQueryModel* tri_par_EMPLACEMENT_decroissant();
   QSqlQueryModel* tri_par_QUANTITE_croissant();
   QSqlQueryModel* tri_par_QUANTITE_decroissant();

};

#endif // PATIENT_H
