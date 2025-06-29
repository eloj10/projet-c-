#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "evennement.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QSqlQueryModel>
#include <QLineEdit>
#include <QStringList>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPixmap>
#include <QImage>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);


    // Validator pour l'ID de l'évènement (doit être un entier)
    ui->lineEdit3->setValidator( nullptr);


    // Affiche les évènements dans le tableau
    ui->tabevennement->setModel(tmpevennement.afficher());  // Remplacement de tmpetudiant par tmpevennement
    connect(ui->pb_button, &QPushButton::clicked, this, &MainWindow::captureEcran);
    connect(ui->pb_rechercher, &QPushButton::clicked, this, &MainWindow::on_pb_rechercher_clicked);
    connect(ui->pb_send, &QPushButton::clicked, this, &MainWindow::envoyerMessage);
    connect(ui->pb_imprimer, &QPushButton::clicked, this, &MainWindow::on_pb_imprimer_clicked);

}



MainWindow::~MainWindow()
{
    delete ui;

}



void MainWindow::on_pb_ajouter_clicked()
{
    // Récupération des valeurs des champs
    QString nom = ui->lineEdit_nom->text();
    QString theme = ui->lineEdit_theme->text();
    QString lieu = ui->lineEdit_lieu->text();
    QString sponsors = ui->lineEdit_sponsors->text();
    QString publicCible = ui->lineEdit_public->text();
    QString type = ui->lineEdit_type->text();
    double budget = ui->lineEdit_budget->text().toDouble();
    QString programme = ui->textEdit->toPlainText();

    // Contrôles de saisie
    if (nom.isEmpty() || theme.isEmpty() || lieu.isEmpty() || sponsors.isEmpty() || publicCible.isEmpty() || type.isEmpty() || programme.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }

    // Vérifier que le budget est un nombre valide et positif
    if (budget <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un budget valide (supérieur à 0).");
        return;
    }

    // Création de l'objet Evennement
    evennement e(nom, theme, lieu, type, publicCible, sponsors, budget, programme);

    // Appel de la méthode d'ajout
    bool test = e.ajouter();
    if(test)
    {
        // Mise à jour de la table pour afficher le nouvel évènement
        ui->tabevennement->setModel(tmpevennement.afficher());  // Remplacement de tableevennement et rafraîchissement

        QMessageBox::information(nullptr, QObject::tr("Ajouter un évènement"),
                  QObject::tr("Évènement ajouté.\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Ajouter un évènement"),
                  QObject::tr("Erreur lors de l'ajout !.\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);
    }
}


void MainWindow::afficher()
{
    // Appel de la méthode 'afficher' de la classe tmpevennement pour récupérer la liste des événements
    QSqlQueryModel *model = tmpevennement.afficher();

    // Vérifiez que le modèle contient des données
    if (model) {
        // Affecter ce modèle à la QTableView pour afficher les événements
        ui->tabevennement->setModel(model);
        ui->tabevennement->resizeColumnsToContents();  // Ajuste les colonnes à la taille du contenu
    } else {
        QMessageBox::warning(this, tr("Erreur d'affichage"), tr("Erreur lors du chargement des événements."));
    }
}


void MainWindow::on_pb_supprimer_clicked()
{
    // Récupération du nom de l'événement pour la suppression depuis le QLineEdit nommé lineEdit3
    QString nom = ui->lineEdit3->text();

    // Contrôle de saisie : vérifier que le champ nom n'est pas vide
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer le nom de l'événement à supprimer.");
        return; // Arrêter l'exécution si le nom est vide
    }

    // Suppression de l'événement
    bool test = tmpevennement.supprimer(nom);  // Utilisation de la méthode supprimer de la classe Evennement

    if (test)
    {
        // Mise à jour de la table pour afficher les événements après suppression
        ui->tabevennement->setModel(tmpevennement.afficher());  // Rafraîchissement de la table

        QMessageBox::information(nullptr, QObject::tr("Supprimer un évènement"),
                QObject::tr("Événement supprimé avec succès.\n"
                            "Click Cancel to exit."), QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Supprimer un évènement"),
                QObject::tr("Erreur lors de la suppression de l'événement.\n"
                            "Click Cancel to exit."), QMessageBox::Cancel);
    }
}



void MainWindow::on_pb_modifier_clicked()
{
    // استرجاع المعلومات من واجهة المستخدم
    QString nom = ui->lineEdit_nom->text();
    QString theme = ui->lineEdit_theme->text();
    QString lieu = ui->lineEdit_lieu->text();
    QString sponsors = ui->lineEdit_sponsors->text();
    QString publicCible = ui->lineEdit_public->text();
    QString type = ui->lineEdit_type->text();
    double budget = ui->lineEdit_budget->text().toDouble();
    QString programme = ui->textEdit->toPlainText();

    // التحقق من صحة الإدخال
    if (nom.isEmpty() || theme.isEmpty() || lieu.isEmpty() || sponsors.isEmpty() ||
        publicCible.isEmpty() || type.isEmpty() || programme.isEmpty()) {
        QMessageBox::warning(this, tr("Modifier un événement"), tr("Veuillez remplir tous les champs."));
        return;
    }

    if (budget <= 0) {
        QMessageBox::warning(this, tr("Modifier un événement"), tr("Veuillez entrer un budget valide."));
        return;
    }

    // إنشاء كائن الحدث الجديد
    evennement e(nom, theme, lieu, type, publicCible, sponsors, budget, programme);

    // استدعاء دالة التعديل وتمرير الكائن "e"
    bool test = tmpevennement.modifier(e);

    if (test) {
        // تحديث عرض الجدول بعد التعديل
        ui->tabevennement->setModel(tmpevennement.afficher());
        QMessageBox::information(this, tr("Modifier un événement"),
                                 tr("Événement modifié avec succès."));
    } else {
        QMessageBox::critical(this, tr("Modifier un événement"),
                              tr("Erreur lors de la modification !"));
    }
}
void MainWindow::captureEcran()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Screenshot"), "", tr("Images (*.png *.jpg *.bmp)"));

    if (filePath.isEmpty()) {
        return;
    }

    QPixmap screenshot = ui->tabevennement->grab();


    if (screenshot.save(filePath)) {
        QMessageBox::information(this, tr("Capture d'écran"), tr("Screenshot saved successfully at:\n%1").arg(filePath));
    } else {
        QMessageBox::critical(this, tr("Capture d'écran"), tr("Failed to save the screenshot."));
    }
}

void MainWindow::on_pb_rechercher_clicked()
{
    // Récupérer la chaîne saisie dans le QLineEdit pour la recherche avancée
    QString searchText = ui->lineEdit_search->text();

    // Initialiser les variables pour chaque critère
    QString nom, theme, lieu, type,sponsors, publiccible,  programme;
    double budgetMin = -1, budgetMax = -1;

    // Analyser la chaîne pour extraire les critères
    QStringList filters = searchText.split(' ');  // Diviser par espace

    foreach (QString filter, filters) {
        QStringList keyValue = filter.split('=');  // Diviser en clé et valeur
        if (keyValue.size() == 2) {
            QString key = keyValue[0].trimmed();
            QString value = keyValue[1].trimmed();

            // Assigner la valeur en fonction de la clé
            if (key == "nom") {
                nom = value;
            } else if (key == "theme") {
                theme = value;
            } else if (key == "lieu") {
                lieu = value;
            } else if (key == "type") {
                type = value;
            } else if (key == "sponsors") {
                sponsors = value;
            } else if (key == "publiccible") {
                publiccible = value;
            } else if (key == "programme") {
                programme = value;
            } else if (key == "budgetMin") {
                budgetMin = value.toDouble();
            } else if (key == "budgetMax") {
                budgetMax = value.toDouble();
            }
        }
    }

    // Appeler la méthode de recherche avancée avec les critères extraits
    QSqlQueryModel* model = tmpevennement.rechercherAvancee(nom, theme, lieu, type, publiccible, sponsors, budgetMin, budgetMax, programme);

    if (model) {
        // Mettre à jour la table avec les résultats de la recherche
        ui->tabevennement->setModel(model);
    } else {
        QMessageBox::critical(this, QObject::tr("Recherche avancée"),
                              QObject::tr("Erreur lors de la recherche."),
                              QMessageBox::Ok);
    }
}


void MainWindow::envoyerMessage()
{
    // Récupérer le texte saisi par l'utilisateur
    QString utilisateurMessage = ui->lineEdit_input->text();

    // Ajouter le message de l'utilisateur dans la zone de texte
    ui->textBrowser->append("Vous: " + utilisateurMessage);
    QString nom = ui->lineEdit_nom->text();
    QString theme = ui->lineEdit_theme->text();
    QString lieu = ui->lineEdit_lieu->text();
    QString sponsors = ui->lineEdit_sponsors->text();
    QString publiccible = ui->lineEdit_public->text();
    QString type = ui->lineEdit_type->text();
    double budget = ui->lineEdit_budget->text().toDouble();
    QString programme = ui->textEdit->toPlainText();

    // Générer la réponse du chatbot
    QString reponseChatbot = obtenirReponseChatbot(utilisateurMessage, nom, theme, lieu, sponsors, publiccible, type, budget, programme);

    // Ajouter la réponse du chatbot dans la zone de texte
    ui->textBrowser->append("Chatbot: " + reponseChatbot);

    // Vider le champ de texte après l'envoi
    ui->lineEdit_input->clear();
}


QString MainWindow::obtenirReponseChatbot(const QString &message ,const QString &nom, const QString &theme, const QString &lieu, const QString &sponsors, const QString &publiccible, const QString &type, const double &budget, const QString &programme)
{
    // Dictionnaire de réponses simples basées sur des mots-clés
    if (message.contains("bonjour", Qt::CaseInsensitive)) {
        return "Bonjour ! Comment puis-je vous aider ?";
    } else if (message.contains("comment ça va", Qt::CaseInsensitive)) {
        return "Je vais bien, merci ! Et vous ?";
    } else if (message.contains("quel est ton nom", Qt::CaseInsensitive)) {
        return "Je suis un chatbot, je n'ai pas de nom propre.";
    } else if (message.contains("heure", Qt::CaseInsensitive)) {
        return "Je ne peux pas dire l'heure, mais vous pouvez vérifier sur votre appareil !";
    } else if (message.contains("merci", Qt::CaseInsensitive)) {
        return "De rien ! N'hésitez pas à me poser d'autres questions.";
    }
    // Réponses concernant les attributs de l'événement
    else if (message.contains("nom", Qt::CaseInsensitive)) {
        return "L'événement s'appelle : " + nom;
    } else if (message.contains("thème", Qt::CaseInsensitive)) {
        return "Le thème de l'événement est : " + theme;
    } else if (message.contains("lieu", Qt::CaseInsensitive)) {
        return "L'événement aura lieu à : " + lieu;
    } else if (message.contains("sponsors", Qt::CaseInsensitive)) {
        return "Les sponsors de l'événement sont : " + sponsors;
    } else if (message.contains("public cible", Qt::CaseInsensitive)) {
        return "Le public cible de cet événement est : " + publiccible;
    } else if (message.contains("type", Qt::CaseInsensitive)) {
        return "Le type de budget de l'événement est : " + type;
    } else if (message.contains("budget", Qt::CaseInsensitive)) {
        return "Le budget de l'événement est de : " + QString::number(budget) + " dinars.";
    } else if (message.contains("programme", Qt::CaseInsensitive)) {
        return "Le programme de l'événement comprend : " + programme;
    }
    // Si aucune question ne correspond
    else {
        return "Désolé, je n'ai pas compris votre question. Pouvez-vous reformuler ?";
    }
}




void MainWindow::afficherDetailsEvenement(const QString &nom)
{
    // Imaginons que vous avez une fonction qui récupère les détails de l'événement
    QString evenementDetails = "Nom: " + nom + "\n";
    evenementDetails += "Thème: Technologie\n";
    evenementDetails += "Lieu: Paris\n";
    evenementDetails += "Type: Conférence\n";
    evenementDetails += "Public cible: Professionnels\n";
    evenementDetails += "Sponsors: XYZ\n";
    evenementDetails += "Budget: 5000 EUR\n";
    evenementDetails += "Programme: Introduction à Qt";

    // Afficher les détails dans la zone texte
    ui->textEdit_eventDetails->setText(evenementDetails);
}

void MainWindow::on_pb_imprimer_clicked()
{
    // Récupérer le contenu à imprimer
    QString eventDetails = ui->textEdit_eventDetails->toPlainText();

    if (eventDetails.isEmpty()) {
        QMessageBox::warning(this, "Aucun événement", "Aucun événement sélectionné ou affiché.");
        return;
    }

    // Créer un objet QPrinter pour l'impression
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        // Si l'utilisateur accepte l'impression, on utilise QPainter pour imprimer
        QPainter painter(&printer);

        // Commencer l'impression
        if (painter.begin(&printer)) {
            // Imprimer les détails de l'événement
            painter.drawText(100, 100, eventDetails);  // Placer le texte à la position (100,100)
            painter.end();  // Terminer l'impression
        } else {
            QMessageBox::warning(this, "Erreur d'impression", "Impossible de démarrer l'impression.");
        }
    }
}



