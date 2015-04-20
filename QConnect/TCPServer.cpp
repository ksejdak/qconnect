#include "TCPServer.h"
#include "ui_TCPServer.h"

#include <QMutexLocker>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>

TCPServer::TCPServer(QWidget* parent)
    : QWidget(parent)
    , m_ui(new Ui::TCPServer)
{
    m_ui->setupUi(this);

    connect(m_ui->buttonStart, SIGNAL(clicked()), this, SLOT(start()));
    connect(&m_tcpServer, SIGNAL(newConnection()), this, SLOT(accept()));
    connect(m_ui->buttonChooseFile, SIGNAL(clicked()), this, SLOT(chooseSaveFile()));
}

TCPServer::~TCPServer()
{
    stop();
    delete m_ui;
}

void TCPServer::start()
{
    if(m_ui->checkBoxSave->isChecked())
    {
        QString saveFilename = m_ui->lineEditSave->text();
        if(saveFilename.isEmpty())
        {
            showError("Save file name is empty.");
            return;
        }

        QFile saveFile(saveFilename);
        saveFile.open(QFile::Text | QFile::Append);
        if(!saveFile.isOpen())
        {
            showError("Cannot create file to save received data.");
            return;
        }
    }

    bool wasOk = false;
    unsigned int portNumber = m_ui->lineEditPort->text().toUInt(&wasOk);
    if(!wasOk)
    {
        showError("Empty or invalid port number.");
        return;
    }

    // This is non-blocking call.
    if(!m_tcpServer.listen(QHostAddress::Any, portNumber))
    {
        showError("Failed to start TCP/IP server.");
        return;
    }

    disconnect(m_ui->buttonStart, SIGNAL(clicked()), this, SLOT(start()));
    connect(m_ui->buttonStart, SIGNAL(clicked()), this, SLOT(stop()));
    m_ui->buttonStart->setText("Stop");
    enableEdit(false);
}

void TCPServer::stop()
{
    if(!m_tcpServer.isListening())
        return;

    m_tcpServer.close();

    disconnect(m_ui->buttonStart, SIGNAL(clicked()), this, SLOT(stop()));
    connect(m_ui->buttonStart, SIGNAL(clicked()), this, SLOT(start()));
    m_ui->buttonStart->setText("Start");
    enableEdit(true);
}

void TCPServer::accept()
{
    QTcpSocket* socket = m_tcpServer.nextPendingConnection();
    if(!socket)
        return;

    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
}

void TCPServer::readSocket()
{
    QTcpSocket* socket = dynamic_cast<QTcpSocket*>(sender());
    QString message = socket->readAll();
    message += "\n";

    if(m_ui->checkBoxTime->isChecked())
    {
        QString currentTimestamp = QDateTime::currentDateTime().toString();
        message = "[" + currentTimestamp + "] " + message;
    }

    QMutexLocker locker(&m_mutex);
    m_ui->plainTextEdit->insertPlainText(message);
    if(!m_ui->checkBoxSave->isChecked())
        return;

    QString saveFilename = m_ui->lineEditSave->text();
    QFile saveFile(saveFilename);
    saveFile.open(QFile::Text | QFile::Append);
    if(!saveFile.isOpen())
        return;

    saveFile.write(message.toStdString().c_str(), message.size());
    saveFile.flush();
}

void TCPServer::chooseSaveFile()
{
    QString saveFilename = QFileDialog::getSaveFileName();
    if(saveFilename.isEmpty())
        return;

    m_ui->lineEditSave->setText(saveFilename);
}

void TCPServer::enableEdit(bool enable)
{
    m_ui->lineEditPort->setEnabled(enable);
    m_ui->lineEditSave->setEnabled(enable);
    m_ui->buttonChooseFile->setEnabled(enable);
    m_ui->checkBoxTime->setEnabled(enable);
    m_ui->checkBoxSave->setEnabled(enable);
}

void TCPServer::showError(QString message)
{
    QMessageBox messageBox;
    messageBox.setIcon(QMessageBox::Critical);
    messageBox.setWindowTitle("Error");
    messageBox.setText(message);
    messageBox.setStandardButtons(QMessageBox::Ok);

    messageBox.exec();
}
