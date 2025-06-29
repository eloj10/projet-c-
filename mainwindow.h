#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "evennement.h"
#include <QMainWindow>
#include <QMainWindow>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QLineEdit>
#include <QValidator>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QImage>


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
    void on_pb_ajouter_clicked();

    void on_pb_supprimer_clicked();
    void afficher();


    void on_pb_modifier_clicked();
    void captureEcran();
    void on_pb_rechercher_clicked();
    void envoyerMessage();
    void on_pb_imprimer_clicked(); // Slot pour imprimer l'événement
    void afficherDetailsEvenement(const QString& nom); // Afficher les détails d'un événement


    //void on_DateEdit_userDateChanged(const QDate &date);
    //void on_pb_rechercheAvancee_clicked();  // Recherche avancée d'événements

    //void on_datee_userDateChanged(const QDate &date);

    void on_lineEdit_id_2_cursorPositionChanged(int oldPos, int newPos);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    evennement tmpevennement;
    QString obtenirReponseChatbot(const QString &message, const QString &nom, const QString &theme, const QString &lieu, const QString &sponsors, const QString &publiccible, const QString &type, const double &budget, const QString &programme);
    void printEventDetails(const QString& eventDetails); // Fonction d'impression
    };




#endif // MAINWINDOW_H
