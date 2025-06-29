#ifndef CONNEXION_H
#define CONNEXION_H
#include <QSqlDatabase>
#include <QSqlDatabase>
#include <QSqlError>  // Ajoute cette ligne pour inclure QSqlError
#include <QMessageBox>

class Connexion
{
private:
    QSqlDatabase db;
public:
    Connexion();
    bool ouvrirConnexion();
    void fermerConnexion();

};

#endif // CONNEXION_H
