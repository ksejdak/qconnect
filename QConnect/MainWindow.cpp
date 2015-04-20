#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Version.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_server(new TCPServer())
    , m_client(new TCPClient())
{
    m_ui->setupUi(this);

    setWindowTitle(windowTitle() + " " + VERSION);

    m_ui->widget->setLayout(new QHBoxLayout());
    m_ui->widget->layout()->addWidget(m_server);
    m_ui->widget->layout()->addWidget(m_client);

    connect(m_ui->radioButtonServer, SIGNAL(clicked()), this, SLOT(serverModeClicked()));
    connect(m_ui->radioButtonClient, SIGNAL(clicked()), this, SLOT(clientModeClicked()));
    connect(m_ui->buttonAbout, SIGNAL(clicked()), this, SLOT(showAbout()));
    connect(m_ui->buttonClose, SIGNAL(clicked()), this, SLOT(close()));

    serverModeClicked();
}

MainWindow::~MainWindow()
{
    delete m_client;
    delete m_server;
    delete m_ui;
}

void MainWindow::serverModeClicked()
{
    m_client->hide();
    m_server->show();
}

void MainWindow::clientModeClicked()
{
    m_server->hide();
    m_client->show();
}

void MainWindow::showAbout()
{
    QString info;
    info += QString("QConnect %1\n\n").arg(VERSION);
    info += "Simple TCP/IP server & client.\n";
    info += "Author: Kuba Sejdak\n";
    info += "Contact: kuba.sejdak@gmail.com\n\n";
    info += QString("Compiled with: Qt %1").arg(qVersion());

    QMessageBox::information(this, "About QConnect", info, QMessageBox::Ok);
}
