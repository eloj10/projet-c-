#ifndef EVENNEMENT_H
#define EVENNEMENT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include <QSqlError>

class evennement
{
public:
    // Constructeurs
    evennement();
    evennement(QString nom, QString theme, QString lieu, QString type, QString publiccible, QString sponsors, double budget, QString programme);

    // Getters
    QString get_nom() const { return nom; }
    QString get_theme() const { return theme; }
    QString get_lieu() const { return lieu; }
    QString get_sponsors() const { return sponsors; }
    QString get_publiccible() const { return publiccible; }
    QString get_type() const { return type; }
    double get_budget() const { return budget; }
    QString get_programme() const { return programme; }
    // QDate get_datee() const { return datee; }

    // Setters
    void setNom(const QString &nom);
    void setTheme(const QString &theme);
    void setLieu(const QString &lieu);
    void setType(const QString &type);
    void setPubliccible(const QString &publiccible);
    void setSponsors(const QString &sponsors);
    void setBudget(double budget);
    void setProgramme(const QString &programme);
    // void setDate(const QDate &datee);

    // Méthodes CRUD
    bool ajouter();
    bool modifier(evennement &e);
    QSqlQueryModel *afficher();
    bool supprimer(QString nom);
    QSqlQueryModel* rechercherAvancee(QString nom, QString theme, QString lieu, QString type, QString publiccible, QString sponsors, double budgetMin, double budgetMax, QString programme);

private:
    QString nom;
    QString theme;
    QString lieu;
    QString sponsors;
    QString publiccible;
    QString type;
    QString programme;
    double budget;
    // QDate datee;
};

#endif // EVENNEMENT_H
