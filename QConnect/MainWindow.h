#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "TCPServer.h"
#include "TCPClient.h"

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

public slots:
    void serverModeClicked();
    void clientModeClicked();
    void showAbout();

private:
    Ui::MainWindow* m_ui;
    TCPServer* m_server;
    TCPClient* m_client;
};

#endif
