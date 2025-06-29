#include "connexion.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>  // Ajoute cette ligne pour inclure QSqlError
#include <QMessageBox>
Connexion::Connexion()
{}

bool Connexion::ouvrirConnexion()
{


    db=QSqlDatabase::addDatabase("QODBC");

db.setDatabaseName("source_projet2A");
db.setUserName("loj");//inserer nom de l'utilisateur
db.setPassword("loujayna");//inserer mot de passe de cet utilisateur


if (db.open()){
      return true;
    qDebug() << "Connexion réussie à la base MySQL";}
else{
    qDebug() << "Erreur de connexion MySQL : " << db.lastError().text();
    QMessageBox::critical(nullptr, QObject::tr("Erreur de connexion"),
                              QObject::tr("La connexion à la base de données MySQL a échoué.\n") +
                              db.lastError().text(), QMessageBox::Cancel);
    return false;
}

}

void Connexion::fermerConnexion()
{db.close();}
