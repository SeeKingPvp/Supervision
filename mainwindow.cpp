#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QStatusBar>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , startS("192.168.1.12", 9993)      // Client TCP pour envoyer les commandes "start" / "stop"
    , lidar360("192.168.1.12", 9991)    // Client TCP dédié à la réception des données du Lidar
    , voiture("192.168.1.12", 9992)     // Client TCP dédié à la réception des données de la voiture
    , batterie("192.168.1.12", 9994)    // Client TCP dédié à la réception des données de la batterie
{
    ui->setupUi(this);
    setWindowTitle("Supervision");

    connect(ui->bpOn, &QPushButton::clicked, this, &MainWindow::start);   // Clic sur "ON" -> start()
    connect(ui->BpOff, &QPushButton::clicked, this, &MainWindow::stop);   // Clic sur "OFF" -> stop()

    // Vérifie si le conteneur du Lidar possède déjà un layout, sinon on en crée un dynamiquement
    if (!ui->Lidar->layout())
        ui->Lidar->setLayout(new QVBoxLayout());
    ui->Lidar->layout()->addWidget(&lidar360);   // Ajout du widget Lidar dans le layout de l'interface
    // Idem pour la voiture
    if (!ui->voiture->layout())
        ui->voiture->setLayout(new QVBoxLayout());
    ui->voiture->layout()->addWidget(&voiture);  // Ajout du widget Voiture dans le layout de l'interface
    // Idem pour la batterie
    if (!ui->batterie->layout())
        ui->batterie->setLayout(new QVBoxLayout());
    ui->batterie->layout()->addWidget(&batterie);

    // ---- Labels d'état (connecté / déconnecté) ----
    // État initial : tout est déconnecté tant qu'aucune connexion n'est établie
    setStatus(ui->labelLidar, "Lidar 360", false);
    setStatus(ui->labelBatterie, "Batterie", false, true);
    setStatus(ui->labelVoiture, "Voiture", false, true);
    statusBar()->showMessage("Commandes : déconnecté");

    // Chaque widget prévient quand sa connexion TCP change
    connect(&lidar360, &WidgetLidar::connectionChanged, this, [this](bool c) {
        setStatus(ui->labelLidar, "Lidar 360", c);
    });
    connect(&batterie, &WidgetBatterie::connectionChanged, this, [this](bool c) {
        setStatus(ui->labelBatterie, "Batterie", c, true);
    });
    connect(&voiture, &WidgetVoiture::connectionChanged, this, [this](bool c) {
        setStatus(ui->labelVoiture, "Voiture", c, true);
    });
    connect(&startS, &TCPclient::connectionChanged, this, [this](bool c) {
        statusBar()->showMessage(c ? "Commandes : connecté" : "Commandes : déconnecté");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setStatus(QLabel *label, const QString &nom, bool connecte, bool feminin)
{
    QString etat;
    if (connecte)
        etat = feminin ? QString("connectée") : QString("connecté");
    else
        etat = feminin ? QString("déconnectée") : QString("déconnecté");

    label->setText(nom + " : " + etat);
    label->setStyleSheet(connecte ? "color: #1a9c1a; font-weight: bold;"
                                  : "color: #d32020; font-weight: bold;");
}

// Méthode appelée lors du clic sur le bouton "ON"
void MainWindow::start()
{
    // Envoie la commande "start" au serveur distant via le socket de commande
    if (!startS.sendDatas("start"))
        statusBar()->showMessage("Commande \"start\" non envoyée : serveur déconnecté", 3000);
}

// Méthode appelée lors du clic sur le bouton "OFF"
void MainWindow::stop()
{
    // Envoie la commande "stop" au serveur distant via le socket de commande
    if (!startS.sendDatas("stop"))
        statusBar()->showMessage("Commande \"stop\" non envoyée : serveur déconnecté", 3000);
}

