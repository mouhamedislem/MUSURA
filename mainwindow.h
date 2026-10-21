#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ClientsPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void ouvrirModule(QWidget *page);
    void moduleIndisponible(const QString &nom);
    void restaurerBoutonActif();

    Ui::MainWindow *ui;
    ClientsPage *m_clients = nullptr;
};

#endif // MAINWINDOW_H
