#include "clientspage.h"
#include "ui_gestion_utilisateurs_MESURA.h"

#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QRegularExpression>
#include <QFileDialog>
#include <QTextDocument>
#include <QPdfWriter>
#include <QPageSize>
#include <QPageLayout>

ClientsPage::ClientsPage(Ui::MainWindow *ui, QWidget *fenetre, QObject *parent)
    : QObject(parent)
    , ui(ui)
    , m_fenetre(fenetre)
{
    // Tableau
    ui->tableClients->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableClients->verticalHeader()->setVisible(false);
    ui->tableClients->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableClients->setSelectionMode(QAbstractItemView::SingleSelection);

    // L'identifiant est attribué automatiquement
    ui->lineEditIdClient->setReadOnly(true);

    // Boutons de la liste
    connect(ui->btnAjouterClient,       &QPushButton::clicked, this, &ClientsPage::onNouveauFormulaire);
    connect(ui->btnModifierClient,      &QPushButton::clicked, this, &ClientsPage::onModifier);
    connect(ui->btnSupprimerClient,     &QPushButton::clicked, this, &ClientsPage::onSupprimer);
    connect(ui->btnConsulterClient,     &QPushButton::clicked, this, &ClientsPage::onConsulter);
    connect(ui->btnExporterClient,      &QPushButton::clicked, this, &ClientsPage::onExporterPdf);
    connect(ui->btnRechercherClient,    &QPushButton::clicked, this, &ClientsPage::onRechercher);
    connect(ui->btnReinitialiserClient, &QPushButton::clicked, this, &ClientsPage::onReinitialiser);
    connect(ui->lineEditRechercheClient, &QLineEdit::returnPressed, this, &ClientsPage::onRechercher);
    connect(ui->comboTriClient, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ClientsPage::onTrier);

    // Boutons de la fiche client
    connect(ui->btnAjouterFicheClient,       &QPushButton::clicked, this, &ClientsPage::onAjouter);
    connect(ui->btnModifierFicheClient,      &QPushButton::clicked, this, &ClientsPage::onModifier);
    connect(ui->btnSupprimerFicheClient,     &QPushButton::clicked, this, &ClientsPage::onSupprimer);
    connect(ui->btnNouveauFormulaireClient,  &QPushButton::clicked, this, &ClientsPage::onNouveauFormulaire);

    // Un clic sur une ligne remplit la fiche
    connect(ui->tableClients, &QTableWidget::itemSelectionChanged,
            this, &ClientsPage::onLigneSelectionnee);
}

// ---------- Utilitaires ----------

int ClientsPage::ligneSelectionnee() const
{
    return ui->tableClients->currentRow();
}

void ClientsPage::viderFormulaire()
{
    ui->lineEditIdClient->clear();
    ui->lineEditNomClient->clear();
    ui->lineEditPrenomClient->clear();
    ui->lineEditEmailClient->clear();
    ui->lineEditTelClient->clear();
    ui->lineEditAdresseClient->clear();
    ui->comboTypeClient->setCurrentIndex(0);
}

void ClientsPage::remplirFormulaire(int row)
{
    auto txt = [&](int col) {
        auto *it = ui->tableClients->item(row, col);
        return it ? it->text() : QString();
    };
    ui->lineEditIdClient->setText(txt(0));
    ui->lineEditNomClient->setText(txt(1));
    ui->lineEditPrenomClient->setText(txt(2));
    ui->lineEditEmailClient->setText(txt(3));
    ui->lineEditTelClient->setText(txt(4));
    ui->lineEditAdresseClient->setText(txt(5));
    ui->comboTypeClient->setCurrentText(txt(6));
}

void ClientsPage::setLigne(int row, const QString &id)
{
    ui->tableClients->setItem(row, 0, new QTableWidgetItem(id));
    ui->tableClients->setItem(row, 1, new QTableWidgetItem(ui->lineEditNomClient->text().trimmed()));
    ui->tableClients->setItem(row, 2, new QTableWidgetItem(ui->lineEditPrenomClient->text().trimmed()));
    ui->tableClients->setItem(row, 3, new QTableWidgetItem(ui->lineEditEmailClient->text().trimmed()));
    ui->tableClients->setItem(row, 4, new QTableWidgetItem(ui->lineEditTelClient->text().trimmed()));
    ui->tableClients->setItem(row, 5, new QTableWidgetItem(ui->lineEditAdresseClient->text().trimmed()));
    ui->tableClients->setItem(row, 6, new QTableWidgetItem(ui->comboTypeClient->currentText()));
}

bool ClientsPage::validerFormulaire()
{
    if (ui->lineEditNomClient->text().trimmed().isEmpty()) {
        QMessageBox::warning(m_fenetre, "Erreur", "Le nom est obligatoire.");
        return false;
    }
    if (ui->lineEditPrenomClient->text().trimmed().isEmpty()) {
        QMessageBox::warning(m_fenetre, "Erreur", "Le prénom est obligatoire.");
        return false;
    }
    static const QRegularExpression emailRx(R"(^[\w.\-+]+@[\w\-]+(\.[\w\-]+)+$)");
    if (!emailRx.match(ui->lineEditEmailClient->text().trimmed()).hasMatch()) {
        QMessageBox::warning(m_fenetre, "Erreur", "Adresse email invalide.");
        return false;
    }
    static const QRegularExpression telRx(R"(^\d{8}$)");
    if (!telRx.match(ui->lineEditTelClient->text().trimmed()).hasMatch()) {
        QMessageBox::warning(m_fenetre, "Erreur", "Le téléphone doit contenir exactement 8 chiffres.");
        return false;
    }
    if (ui->lineEditAdresseClient->text().trimmed().isEmpty()) {
        QMessageBox::warning(m_fenetre, "Erreur", "L'adresse est obligatoire.");
        return false;
    }
    return true;
}

// ---------- Slots ----------

void ClientsPage::onAjouter()
{
    if (!validerFormulaire())
        return;

    ui->tableClients->blockSignals(true);
    int row = ui->tableClients->rowCount();
    ui->tableClients->insertRow(row);
    setLigne(row, QString::number(m_prochainId++));
    ui->tableClients->blockSignals(false);

    QMessageBox::information(m_fenetre, "Succès", "Client ajouté avec succès.");
    onNouveauFormulaire();
}

void ClientsPage::onModifier()
{
    int row = ligneSelectionnee();
    if (row < 0) {
        QMessageBox::warning(m_fenetre, "Modifier", "Sélectionnez d'abord un client dans la liste.");
        return;
    }
    if (!validerFormulaire())
        return;

    const QString id = ui->tableClients->item(row, 0)->text();
    ui->tableClients->blockSignals(true);
    setLigne(row, id);
    ui->tableClients->blockSignals(false);
    QMessageBox::information(m_fenetre, "Succès", "Client modifié avec succès.");
}

void ClientsPage::onSupprimer()
{
    int row = ligneSelectionnee();
    if (row < 0) {
        QMessageBox::warning(m_fenetre, "Supprimer", "Sélectionnez d'abord un client dans la liste.");
        return;
    }
    auto rep = QMessageBox::question(m_fenetre, "Confirmation",
                                     "Voulez-vous vraiment supprimer ce client ?");
    if (rep == QMessageBox::Yes) {
        ui->tableClients->removeRow(row);
        viderFormulaire();
    }
}

void ClientsPage::onConsulter()
{
    int row = ligneSelectionnee();
    if (row < 0) {
        QMessageBox::warning(m_fenetre, "Consulter", "Sélectionnez d'abord un client dans la liste.");
        return;
    }
    remplirFormulaire(row);
}

void ClientsPage::onNouveauFormulaire()
{
    ui->tableClients->clearSelection();
    ui->tableClients->setCurrentCell(-1, -1);
    viderFormulaire();
    ui->lineEditNomClient->setFocus();
}

void ClientsPage::onLigneSelectionnee()
{
    int row = ligneSelectionnee();
    if (row >= 0 && ui->tableClients->item(row, 0))
        remplirFormulaire(row);
}

void ClientsPage::onRechercher()
{
    const QString texte = ui->lineEditRechercheClient->text().trimmed();

    int col = 0;
    switch (ui->comboTypeRechercheClient->currentIndex()) {
    case 1: col = 1; break;   // Nom
    case 2: col = 4; break;   // Téléphone
    default: col = 0; break;  // ID client
    }

    for (int r = 0; r < ui->tableClients->rowCount(); ++r) {
        auto *it = ui->tableClients->item(r, col);
        bool visible = texte.isEmpty()
                       || (it && it->text().contains(texte, Qt::CaseInsensitive));
        ui->tableClients->setRowHidden(r, !visible);
    }
}

void ClientsPage::onReinitialiser()
{
    ui->lineEditRechercheClient->clear();
    for (int r = 0; r < ui->tableClients->rowCount(); ++r)
        ui->tableClients->setRowHidden(r, false);
}

void ClientsPage::onTrier()
{
    Qt::SortOrder ordre = (ui->comboTriClient->currentIndex() == 0)
                              ? Qt::AscendingOrder
                              : Qt::DescendingOrder;
    ui->tableClients->sortItems(1, ordre);   // tri sur la colonne Nom
}

void ClientsPage::onExporterPdf()
{
    if (ui->tableClients->rowCount() == 0) {
        QMessageBox::warning(m_fenetre, "Exporter", "La liste des clients est vide.");
        return;
    }

    QString chemin = QFileDialog::getSaveFileName(m_fenetre, "Exporter en PDF",
                                                  "clients.pdf", "Fichiers PDF (*.pdf)");
    if (chemin.isEmpty())
        return;
    if (!chemin.endsWith(".pdf", Qt::CaseInsensitive))
        chemin += ".pdf";

    QString html = "<h1 style='text-align:center;'>Liste des clients</h1>"
                   "<table border='1' cellspacing='0' cellpadding='4' width='100%'>"
                   "<tr style='background-color:#171717; color:#F5F0E6;'>";
    for (int c = 0; c < ui->tableClients->columnCount(); ++c)
        html += "<th>" + ui->tableClients->horizontalHeaderItem(c)->text().toHtmlEscaped() + "</th>";
    html += "</tr>";

    for (int r = 0; r < ui->tableClients->rowCount(); ++r) {
        if (ui->tableClients->isRowHidden(r))
            continue;
        html += "<tr>";
        for (int c = 0; c < ui->tableClients->columnCount(); ++c) {
            auto *it = ui->tableClients->item(r, c);
            html += "<td>" + (it ? it->text().toHtmlEscaped() : QString()) + "</td>";
        }
        html += "</tr>";
    }
    html += "</table>";

    QPdfWriter writer(chemin);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageOrientation(QPageLayout::Landscape);

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&writer);

    QMessageBox::information(m_fenetre, "Export", "PDF exporté avec succès.");
}
