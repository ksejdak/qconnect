#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>

namespace Ui
{
    class TCPClient;
}

class TCPClient : public QWidget
{
    Q_OBJECT

public:
    explicit TCPClient(QWidget* parent = 0);
    ~TCPClient();

public slots:
    void connectToServer();
    void disconnectFromServer();
    void send();

private:
    void enableEdit(bool enable);
    void showError(QString message);

private:
    Ui::TCPClient* m_ui;
    QTcpSocket m_socket;
};

#endif
