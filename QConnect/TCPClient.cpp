#include "TCPClient.h"
#include "ui_TCPClient.h"

#include <QMessageBox>

TCPClient::TCPClient(QWidget* parent)
    : QWidget(parent)
    , m_ui(new Ui::TCPClient)
{
    m_ui->setupUi(this);

    connect(m_ui->buttonConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));
    connect(m_ui->buttonSend, SIGNAL(clicked()), this, SLOT(send()));
}

TCPClient::~TCPClient()
{
    if(m_socket.isOpen())
        m_socket.close();

    delete m_ui;
}

void TCPClient::connectToServer()
{
    QString serverAddress = m_ui->lineEditAddress->text();
    bool wasOk = false;
    int serverPort = m_ui->lineEditPort->text().toInt(&wasOk);

    if(serverAddress.isEmpty() || !wasOk)
    {
        showError("Server IP address or port is invalid.");
        return;
    }

    m_socket.connectToHost(serverAddress, serverPort, QTcpSocket::WriteOnly);
    bool isConnected = m_socket.waitForConnected(5000);
    if(!isConnected)
    {
        showError("Cannot connect to server. Check if server is running or if cable is properly plugged.");
        return;
    }

    disconnect(m_ui->buttonConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));
    connect(m_ui->buttonConnect, SIGNAL(clicked()), this, SLOT(disconnectFromServer()));
    m_ui->buttonConnect->setText("Disconnect");
    enableEdit(false);
}

void TCPClient::disconnectFromServer()
{
    if(m_socket.error() == QTcpSocket::ConnectionRefusedError)
        showError("Server was closed unexpectedly.");

    m_socket.disconnectFromHost();

    disconnect(m_ui->buttonConnect, SIGNAL(clicked()), this, SLOT(disconnectFromServer()));
    connect(m_ui->buttonConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));
    m_ui->buttonConnect->setText("Connect");
    enableEdit(true);
}

void TCPClient::send()
{
    QString message = m_ui->lineEditSend->text();
    m_socket.write(message.toStdString().c_str(), message.size());

    m_ui->lineEditSend->clear();
    m_ui->plainTextEdit->appendPlainText(message);
}

void TCPClient::enableEdit(bool enable)
{
    m_ui->lineEditAddress->setEnabled(enable);
    m_ui->lineEditPort->setEnabled(enable);

    m_ui->buttonSend->setEnabled(!enable);
}

void TCPClient::showError(QString message)
{
    QMessageBox messageBox;
    messageBox.setIcon(QMessageBox::Critical);
    messageBox.setWindowTitle("Error");
    messageBox.setText(message);
    messageBox.setStandardButtons(QMessageBox::Ok);

    messageBox.exec();
}
