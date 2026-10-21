#ifndef CLIENTSPAGE_H
#define CLIENTSPAGE_H

#include <QObject>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QWidget;

// Logique du module "Gestion des clients".
// Elle agit sur les widgets de la page pageClients de l'interface principale.
class ClientsPage : public QObject
{
    Q_OBJECT

public:
    ClientsPage(Ui::MainWindow *ui, QWidget *fenetre, QObject *parent = nullptr);

private slots:
    void onAjouter();
    void onModifier();
    void onSupprimer();
    void onConsulter();
    void onNouveauFormulaire();
    void onRechercher();
    void onReinitialiser();
    void onTrier();
    void onExporterPdf();
    void onLigneSelectionnee();

private:
    bool validerFormulaire();
    void viderFormulaire();
    void remplirFormulaire(int row);
    void setLigne(int row, const QString &id);
    int  ligneSelectionnee() const;

    Ui::MainWindow *ui;
    QWidget *m_fenetre;
    int m_prochainId = 1;
};

#endif // CLIENTSPAGE_H
