#include "commande.h"

Commande::Commande()
{
    id_commande = 0;
    date_c = QDate::currentDate();
    date_l = QDate::currentDate();
    montant = 0.0;
    id_f = 0;
}

Commande::Commande(QString id, QDate dateC, QDate dateL, float mont, QString idF)
{
    id_commande = id;
    date_c = dateC;
    date_l = dateL;
    montant = mont;
    id_f = idF;
}

bool Commande::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO COMMANDE (ID_COMMANDE, DATE_C, DATE_L, MONTANT, ID_F) "
                  "VALUES (:id, :dateC, :dateL, :montant, :idF)");

    query.bindValue(":id", id_commande);
    query.bindValue(":dateC", date_c);
    query.bindValue(":dateL", date_l);
    query.bindValue(":montant", montant);
    query.bindValue(":idF", id_f);

    return query.exec();
}

QSqlQueryModel* Commande::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT C.*, F.NOM_F, F.PRENOM_F FROM COMMANDE C "
                    "JOIN FOURNISSEUR F ON C.ID_F = F.ID_F");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date Commande"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Livraison"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Montant"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ID Fournisseur"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Nom Fournisseur"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Prénom Fournisseur"));

    return model;
}

bool Commande::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM COMMANDE WHERE ID_COMMANDE = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Commande::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE COMMANDE SET DATE_C=:dateC, DATE_L=:dateL, "
                  "MONTANT=:montant, ID_F=:idF WHERE ID_COMMANDE=:id");

    query.bindValue(":id", id_commande);
    query.bindValue(":dateC", date_c);
    query.bindValue(":dateL", date_l);
    query.bindValue(":montant", montant);
    query.bindValue(":idF", id_f);

    return query.exec();
}

QSqlQueryModel* Commande::rechercher(const QString &critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT C.*, F.NOM_F, F.PRENOM_F FROM COMMANDE C "
                    "JOIN FOURNISSEUR F ON C.ID_F = F.ID_F "
                    "WHERE C.ID_COMMANDE LIKE '%" + critere + "%' OR "
                    "C.MONTANT LIKE '%" + critere + "%'");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date Commande"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Livraison"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Montant"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ID Fournisseur"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Nom Fournisseur"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Prénom Fournisseur"));

    return model;
}

QSqlQueryModel* Commande::trier(const QString &critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT C.*, F.NOM_F, F.PRENOM_F FROM COMMANDE C "
                    "JOIN FOURNISSEUR F ON C.ID_F = F.ID_F "
                    "ORDER BY " + critere);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date Commande"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Livraison"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Montant"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ID Fournisseur"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Nom Fournisseur"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Prénom Fournisseur"));

    return model;
}
