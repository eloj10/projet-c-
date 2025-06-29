#include "evennement.h"
#include <QDebug>
#include "connexion.h"
#include <QSqlQuery>
#include <QSqlError>


evennement::evennement()
{
    nom="";
    theme="";
    lieu="";
    sponsors="";
    publiccible="";
    type="";
    budget=0.0;
    programme="";
   // datee=QDate::currentDate();
}


void evennement::setNom(const QString &nom) { this->nom = nom; }
void evennement::setTheme(const QString &theme) { this->theme = theme; }
void evennement::setLieu(const QString &lieu) { this->lieu = lieu; }
void evennement::setType(const QString &type) { this->type = type; }
void evennement::setPubliccible(const QString &publiccible) { this->publiccible = publiccible; }
void evennement::setSponsors(const QString &sponsors) { this->sponsors = sponsors; }
void evennement::setBudget(double budget) { this->budget = budget; }
void evennement::setProgramme(const QString &programme) { this->programme = programme; }
//void evennement::setDate(const QDate &datee) { this->datee = datee; }



evennement::evennement(QString nom, QString theme, QString lieu, QString type, QString publiccible, QString sponsors, double budget, QString programme)
{
    this->nom = nom;
    this->theme = theme;
    this->lieu = lieu;
    this->type = type;
    this->publiccible = publiccible;
    this->sponsors = sponsors;
    this->budget = budget;
    this->programme = programme;

    //this->datee = datee;
}

bool evennement::ajouter()
{
    QSqlQuery query;

    // Prepare the SQL query using placeholders
    query.prepare("INSERT INTO EVENNEMENT (nom, theme, lieu, sponsors, publiccible, type, budget, programme) "
                  "VALUES (:nom, :theme, :lieu, :publiccible, :sponsors, :type, :budget, :programme)");

    // Bind the values for the placeholders
    query.bindValue(":nom", nom);
    query.bindValue(":theme", theme);
    query.bindValue(":lieu", lieu);
    query.bindValue(":sponsors", sponsors);
    query.bindValue(":publiccible", publiccible);
    query.bindValue(":type", type);
    query.bindValue(":budget", budget);
    query.bindValue(":programme", programme);
    //query.bindValue(":datee", datee.toString("yyyy-MM-dd "));  // Convert QDateTime to string

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Error adding event: " << query.lastError().text();
        return false;
    }
}

//////////////////////

QSqlQueryModel* evennement::afficher()
{
    QSqlQueryModel * model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM EVENNEMENT");
    return model;
}




////////////



bool evennement::supprimer(QString nom)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EVENNEMENT WHERE nom = :nom");
    query.bindValue(":nom", nom);

    return query.exec();
}

////////////////


bool evennement::modifier(evennement &e)
{
    QSqlQuery query;

    query.prepare("UPDATE EVENNEMENT SET theme = :theme, lieu = :lieu, "
                  "sponsors = :sponsors, publicCible = :publicCible, type = :type, "
                  "budget = :budget, programme = :programme "
                  "WHERE nom = :nom");

    query.bindValue(":nom", e.get_nom());
    query.bindValue(":theme", e.get_theme());
    query.bindValue(":lieu", e.get_lieu());
    query.bindValue(":sponsors", e.get_sponsors());
    query.bindValue(":publicCible", e.get_publiccible());
    query.bindValue(":type", e.get_type());
    query.bindValue(":budget", e.get_budget());
    query.bindValue(":programme", e.get_programme());

    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification: " << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* evennement::rechercherAvancee(QString nom, QString theme, QString lieu, QString type, QString publicCible, QString sponsors, double budgetMin, double budgetMax, QString programme)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    QString queryStr = "SELECT * FROM EVENNEMENT WHERE 1 = 1 ";

    if (!nom.isEmpty()) queryStr += "AND nom LIKE :nom ";
    if (!theme.isEmpty()) queryStr += "AND theme LIKE :theme ";
    if (!lieu.isEmpty()) queryStr += "AND lieu LIKE :lieu ";
    if (!type.isEmpty()) queryStr += "AND type LIKE :type ";
    if (!publicCible.isEmpty()) queryStr += "AND publiccible LIKE :publicCible ";
    if (!sponsors.isEmpty()) queryStr += "AND sponsors LIKE :sponsors ";
    if (budgetMin != -1) queryStr += "AND budget >= :budgetMin ";
    if (budgetMax != -1) queryStr += "AND budget <= :budgetMax ";
    if (!programme.isEmpty()) queryStr += "AND programme LIKE :programme ";

    query.prepare(queryStr);

    if (!nom.isEmpty()) query.bindValue(":nom", "%" + nom + "%");
    if (!theme.isEmpty()) query.bindValue(":theme", "%" + theme + "%");
    if (!lieu.isEmpty()) query.bindValue(":lieu", "%" + lieu + "%");
    if (!type.isEmpty()) query.bindValue(":type", "%" + type + "%");
    if (!publicCible.isEmpty()) query.bindValue(":publicCible", "%" + publicCible + "%");
    if (!sponsors.isEmpty()) query.bindValue(":sponsors", "%" + sponsors + "%");
    if (budgetMin != -1) query.bindValue(":budgetMin", budgetMin);
    if (budgetMax != -1) query.bindValue(":budgetMax", budgetMax);
    if (!programme.isEmpty()) query.bindValue(":programme", "%" + programme + "%");

    query.exec();
    model->setQuery(query);
    return model;
}
/*
QSqlQueryModel* Evennement::rechercherAvance(const QString &nom, const QString &theme, const QString &lieu, const QString &publicCible)
{
    QSqlQuery query;
    QString queryString = "SELECT * FROM evennement WHERE 1=1";  // 1=1 permet de toujours avoir une condition vraie

    // Ajouter les conditions selon les critères
    if (!nom.isEmpty()) {
        queryString += " AND nom LIKE :nom";
    }
    if (date.isValid()) {
        queryString += " AND date_evenement = :date";
    }
    if (!theme.isEmpty()) {
        queryString += " AND theme LIKE :theme";
    }
    if (!lieu.isEmpty()) {
        queryString += " AND lieu LIKE :lieu";
    }
    if (!publicCible.isEmpty()) {
        queryString += " AND public_cible LIKE :publicCible";
    }

    query.prepare(queryString);

    // Associer les valeurs aux critères, si présents
    if (!nom.isEmpty()) {
        query.bindValue(":nom", "%" + nom + "%");  // Recherche partielle avec wildcard
    }
    if (date.isValid()) {
        query.bindValue(":date", date);
    }
    if (!theme.isEmpty()) {
        query.bindValue(":theme", "%" + theme + "%");
    }
    if (!lieu.isEmpty()) {
        query.bindValue(":lieu", "%" + lieu + "%");
    }
    if (!publicCible.isEmpty()) {
        query.bindValue(":publicCible", "%" + publicCible + "%");
    }

    query.exec();

    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery(query);
    return model;
}
*/
