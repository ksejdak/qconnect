#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QMutex>

namespace Ui
{
    class TCPServer;
}

class TCPServer : public QWidget
{
    Q_OBJECT

public:
    explicit TCPServer(QWidget* parent = 0);
    ~TCPServer();

public slots:
    void start();
    void stop();
    void accept();
    void readSocket();

    void chooseSaveFile();

private:
    void enableEdit(bool enable);
    void showError(QString message);

private:
    Ui::TCPServer* m_ui;
    QTcpServer m_tcpServer;
    QMutex m_mutex;
};

#endif
