#include "mainwindow.h"
#include "ui_gestion_utilisateurs_MESURA.h"
#include "clientspage.h"

#include <QMessageBox>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Écran de démarrage : connexion, puis module "Utilisateurs"
    ui->stackPrincipal->setCurrentWidget(ui->pageLogin);
    ui->stackModules->setCurrentWidget(ui->pageUtilisateurs);


    // Module Clients (logique dans clientspage.cpp)
    m_clients = new ClientsPage(ui, this, this);

    // --- Navigation connexion / mot de passe oublié ---
    connect(ui->btnConnexion, &QPushButton::clicked, this, [this]() {
        ui->stackPrincipal->setCurrentWidget(ui->appPage);
    });
    connect(ui->btnMotDePasseOublie, &QPushButton::clicked, this, [this]() {
        ui->stackPrincipal->setCurrentWidget(ui->pageMotDePasseOublie);
    });
    connect(ui->btnRetourConnexion, &QPushButton::clicked, this, [this]() {
        ui->stackPrincipal->setCurrentWidget(ui->pageLogin);
    });
    connect(ui->btnDeconnexion, &QPushButton::clicked, this, [this]() {
        ui->stackPrincipal->setCurrentWidget(ui->pageLogin);
    });

    // --- Navigation entre modules (menu latéral) ---
    connect(ui->btnUtilisateurs, &QPushButton::clicked, this, [this]() {
        ouvrirModule(ui->pageUtilisateurs);
    });
    connect(ui->btnMachines, &QPushButton::clicked, this, [this]() {
        ouvrirModule(ui->pageMachines);
    });
    connect(ui->btnClients, &QPushButton::clicked, this, [this]() {
        ouvrirModule(ui->pageClients);
    });

    connect(ui->btnCommandes, &QPushButton::clicked, this, [this]() {
        ouvrirModule(ui->pagecommandes);
    });

    // Modules pas encore développés par l'équipe
    connect(ui->btnCreations, &QPushButton::clicked, this, [this]() {
        moduleIndisponible("Gestion des créations");
    });
    connect(ui->btnFournitures, &QPushButton::clicked, this, [this]() {
        moduleIndisponible("Gestion des fournitures");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ouvrirModule(QWidget *page)
{
    ui->stackModules->setCurrentWidget(page);
}

void MainWindow::moduleIndisponible(const QString &nom)
{
    QMessageBox::information(this, nom, "Ce module n'est pas encore intégré.");
    restaurerBoutonActif();   // le bouton cliqué reste coché sinon
}

void MainWindow::restaurerBoutonActif()
{
    QWidget *page = ui->stackModules->currentWidget();
    if (page == ui->pageMachines)
        ui->btnMachines->setChecked(true);
    else if (page == ui->pageClients)
        ui->btnClients->setChecked(true);
    else if (page == ui->pagecommandes)
        ui->btnCommandes->setChecked(true);
    else
        ui->btnUtilisateurs->setChecked(true);
}
