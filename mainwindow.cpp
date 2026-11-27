#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,ui(new Ui::MainWindow)
    ,startS("192.168.1.12",9993)
    ,lidar360("192.168.1.12",9991)// Client TCP pour envoyer les commandes "start" / "stop"
    ,voiture("192.168.1.12", 9992)// Client TCP dédié à la réception des données du Lidar
    ,batterie("192.168.1.12",9994)// Client TCP dédié à la réception des données de la voiture
{
    ui->setupUi(this);
    connect(ui->bpOn, &QPushButton::clicked, this, &MainWindow::start);// Connexion du clic du bouton "ON" pour déclencher la méthode start()
    connect(ui->BpOff, &QPushButton::clicked, this, &MainWindow::stop);// Connexion du clic du bouton "OFF" pour déclencher la méthode stop()
    // Vérifie si le conteneur du Lidar possède déjà un layout, sinon on en crée un dynamiquement
    if (!ui->Lidar->layout())
        ui->Lidar->setLayout(new QVBoxLayout());
    ui->Lidar->layout()->addWidget(&lidar360);// Ajout du widget Lidar dans le layout de l'interface

    // Vérifie si le conteneur de la voiture possède déjà un layout, sinon on en crée un dynamiquement
    if (!ui->voiture->layout())
        ui->voiture->setLayout(new QVBoxLayout());
    ui->voiture->layout()->addWidget(&voiture);// Ajout du widget Voiture dans le layout de l'interface

    if (!ui->batterie->layout())
        ui->batterie->setLayout(new QVBoxLayout());
    ui->batterie->layout()->addWidget(&batterie);
}
MainWindow::~MainWindow()
{
    delete ui;
}
// Méthode appelée lors du clic sur le bouton "ON"
void MainWindow::start()
{
    startS.sendDatas("start");// Envoie la commande "start" au serveur distant via le socket de commande
}
// Méthode appelée lors du clic sur le bouton "OFF"
void MainWindow::stop()
{
    startS.sendDatas("stop");// Envoie la commande "stop" au serveur distant via le socket de commande
}
