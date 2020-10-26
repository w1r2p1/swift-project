#ifndef ORDERSKEEPER_H
#define ORDERSKEEPER_H

#include <QObject>
#include <QSharedDataPointer>

#include "../swift-corelib/swiftcore.h"
#include "../swift-corelib/wampclient.h"


class OrdersKeeperData;

class OrdersKeeper : public QObject
{
    Q_OBJECT
public:
    explicit OrdersKeeper(QObject *parent = nullptr);
    OrdersKeeper(const OrdersKeeper &);
    OrdersKeeper &operator=(const OrdersKeeper &);
    ~OrdersKeeper();
signals:
    void activeEvent( const QJsonObject& j_data );
    void historyEvent( const QJsonObject& j_data );
    void orderEvent( const QString& event_name, const QJsonObject& j_data );
public slots:
    void onActiveEvent( const QJsonObject& j_data );
    void onHistoryEvent( const QJsonObject& j_data );
    void onOrderEvent( const QString& event_name, const QJsonObject& j_data );

    void requestHistory();
    void requestActive();

    void recheck_clients();

    void onWampSession( Wamp::Session * sess );
private:

    void pauseModule();


    bool is_pause;
    void getConnectedExchanges();

    QStringList _active_clients;

    QTimer * update_history_timer;
    QTimer * update_active_timer;

    QSharedDataPointer<OrdersKeeperData> data;
    Wamp::Session * session;
    QMutex m;
};

#endif // ORDERSKEEPER_H
