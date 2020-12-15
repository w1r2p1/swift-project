#include "swiftcore.h"
#include <QDebug>
#include <QCoreApplication>

SwiftCore::SwiftCore()
{
}

QSqlQuery SwiftCore::createQuery(const QString &dbname)
{
    static QHash<QString, QSqlDatabase> _dbs;
    if ( _dbs.contains( dbname ) ) {
         if ( _dbs[dbname].isOpen()  ) {
              return QSqlQuery( _dbs[dbname] );
         }
    } else {
        _dbs[dbname] = QSqlDatabase::addDatabase("QMYSQL", dbname );
    }

    QSqlDatabase db = _dbs[dbname];
    if( QSqlDatabase::contains( dbname ) )
    {
        db = QSqlDatabase::database( dbname );
        if ( !db.isOpen() ) {
            db.setHostName( SwiftCore::getSettings()->value(SETTINGS_NAME_MYSQL_HOST,"localhost" ).toString() );
            db.setPort( SwiftCore::getSettings()->value(SETTINGS_NAME_MYSQL_PORT, 3306 ).toInt() );
            db.setUserName( SwiftCore::getSettings()->value(SETTINGS_NAME_MYSQL_USER,"swiftbot" ).toString() );
            db.setPassword( SwiftCore::getSettings()->value(SETTINGS_NAME_MYSQL_PASSWORD,"swiftbot" ).toString() );
            db.setDatabaseName( SwiftCore::getSettings()->value("mysql_db","swiftbot" ).toString() );
            if ( !db.open() ) {
                qWarning() << db.lastError().text();
                qApp->exit( 1 );
            }
        }
    }
    else
    {
        db.setHostName( SwiftCore::getSettings()->value(SETTINGS_NAME_MYSQL_HOST,"localhost" ).toString() );
        db.setPort( SwiftCore::getSettings()->value(SETTINGS_NAME_MYSQL_PORT, 3306 ).toInt() );
        db.setUserName( SwiftCore::getSettings()->value(SETTINGS_NAME_MYSQL_USER,"swiftbot" ).toString() );
        db.setPassword( SwiftCore::getSettings()->value(SETTINGS_NAME_MYSQL_PASSWORD,"swiftbot" ).toString() );
        db.setDatabaseName( SwiftCore::getSettings()->value("mysql_db","swiftbot" ).toString() );
        if ( !db.open() ) {
            qWarning() << db.lastError().text();
            qApp->exit( 1 );
        }
    }
    _dbs[dbname] = db;
    return QSqlQuery( db );
}

double SwiftBot::Market::rate() {
    double r = 0;
    if ( wamp_client->isConnected() ) {
        QJsonObject j_ret = QJsonDocument::fromJson( wamp_client->getSession()->call( RPC_CURRENT_RATES ).toString().toUtf8() ).object();
        if ( j_ret.contains( QString::number( id ) ) ) {
            r = j_ret.value( QString::number( id ) ).toString().toDouble();
        }
    }
    return r;
}

QPair<Asks, Bids> SwiftBot::Market::orderBooks() {
    QMap<double,double> asks;
    QMap<double,double> bids;
    if ( wamp_client->isConnected() ) {
        QJsonObject j_ret = QJsonDocument::fromJson( wamp_client->getSession()->call( RPC_ORDERBOOKS_CACHE ).toString().toUtf8() ).object();
        const QJsonArray j_asks( j_ret.value("asks").toArray() );
        for( auto it = j_asks.begin(); it != j_asks.end(); it++ ) {
            asks[ it->toArray().at(1).toString().toDouble() ] = it->toArray().at(2).toString().toDouble();
        }
        const QJsonArray j_bids( j_ret.value("bids").toArray() );
        for( auto it = j_bids.begin(); it != j_bids.end(); it++ ) {
            bids[ it->toArray().at(1).toString().toDouble() ] = it->toArray().at(2).toString().toDouble();
        }
    }
    return QPair<Asks,Bids>({Asks( asks ),Bids( bids )});
}

SwiftBot::Exchange SwiftBot::Market::exchange() {
    return Exchange( exchange_id );
}

SwiftBot::Currency SwiftBot::Market::baseCurrency() {
    return Currency( base_currency_id );
}

SwiftBot::Currency SwiftBot::Market::quoteCurrency() {
    return Currency( quote_currency_id );
}

SwiftBot::Market::Market(const quint32 &market_id):
    id( market_id ),
    _storage( SwiftCore::getAssets() )
{
    name = _storage->getMarketName( id );
    exchange_id = _storage->getMarketExchangeId( id );
    arbitrage_pair_id = _storage->getMarketArbitragePairId( id );
    base_currency_id = _storage->getMarketBaseCurrency( id );
    quote_currency_id = _storage->getMarketPriceCurrency( id );
    is_enabled = _storage->isMarketActive( id );
    amount_precision = _storage->getMarketAmountPrecision( id );
    price_precision = _storage->getMarketPricePrecision( id );
}

SwiftBot::Coin::Coin(const quint32 &coin_id) : id( coin_id ), _storage( SwiftCore::getAssets() ) {
    is_enabled = true;
    name = _storage->getCoinName( id );
}

SwiftBot::Coin SwiftBot::Currency::coin() {
    return Coin( coin_id );
}

double SwiftBot::Currency::totalUsd() {
    static double _r = 0;
    static quint64 last_update = 0;
    if ( QDateTime::currentSecsSinceEpoch() - last_update > 10 ) {
        if ( wamp_client != nullptr && wamp_client && wamp_client->isConnected() ) {
            _r = SwiftBot::method( RPC_BALANCE_GET, {id} ).toDouble();
            last_update = QDateTime::currentSecsSinceEpoch();
        }
    }
    return _r;
}

double SwiftBot::Currency::balance() {
    static double _r = 0;
    static quint64 last_update = 0;
    if ( QDateTime::currentSecsSinceEpoch() - last_update > 10 ) {
        if ( wamp_client != nullptr && wamp_client && wamp_client->isConnected() ) {
            _r = SwiftBot::method( RPC_BALANCE_GET, {id} ).toDouble();
            last_update = QDateTime::currentSecsSinceEpoch();
        }
    }
    return _r;
}

SwiftBot::Currency::Currency(const quint32 &currency_id) : id( currency_id ), _storage( SwiftCore::getAssets() ) {
    coin_id = _storage->getCurrencyCoin( id );
    exchange_id = _storage->getCurrencyExchangeId( id );
    is_enabled = _storage->isCurrencyActive( id );
    name = _storage->getCurrencyName( id );
}

SwiftBot::Markets SwiftBot::Exchange::markets() {
    Markets r;
    QList<quint32> _pairs( _storage->getExchangePairs( id ) );
    for( auto it = _pairs.begin(); it != _pairs.end(); it++ ) {
        r.insert( *it, Market( *it ) );
    }
    return r;
}

void SwiftBot::Exchange::forEachMarket(SwiftBot::MarketsEach func) {
    Markets all( markets() );
    for( auto it = all.begin(); it != all.end(); it++ ) {
        func( it.value() );
    }
}

SwiftBot::Exchange::Exchange(const QString &name_) : name( name_ ), _storage( SwiftCore::getAssets() ) {
    id = _storage->getExchangeId( name );
    is_enabled = true;
}

SwiftBot::Exchange::Exchange(const quint32 &exchange_id) : id( exchange_id ), _storage( SwiftCore::getAssets() ) {
    name = _storage->getExchangeName( id );
    is_enabled = true;
}

bool SwiftBot::Exchange::isRequstsSeparated() {
    return _storage->isSeparatedApi( id );
}

void SwiftBot::ArbitragePair::eachPair(SwiftBot::ArbitragePairsEach pair_each) {
    static ArbitragePairs _a;
    if ( _a.isEmpty() ) {
        _a = ArbitragePair::all();
    }
    for( auto it = _a.begin(); it != _a.end(); it++ ) {
        pair_each( it.value() );
    }
}

SwiftBot::ArbitragePairs SwiftBot::ArbitragePair::all() {
    static ArbitragePairs _r;
    if ( _r.isEmpty() ) {
        QList<quint32> _apids( SwiftCore::getAssets()->getArbitragePairs().keys() );
        for( auto it = _apids.begin(); it != _apids.end(); it++ ) {
            _r.insert( *it, ArbitragePair( *it ) );
        }
    }
    return _r;
}

SwiftBot::Markets SwiftBot::ArbitragePair::markets() {
    Markets _r;
    for( auto it = _markets.begin(); it != _markets.end(); it++ ) {
        _r.insert( *it, Market( *it ) );
    }
    return _r;
}

void SwiftBot::ArbitragePair::eachMarkets(const SwiftBot::MarketsEach &markets_each) {
    for( auto it = _markets.begin(); it != _markets.end(); it++ ) {
        markets_each( Market( *it ) );
    }
}

SwiftBot::ArbitragePair::ArbitragePair(const quint32 &arbitrage_pair_id) : id(arbitrage_pair_id), _storage( SwiftCore::getAssets() ) {
    _markets = _storage->getArbitragePairs().value( id );
    name = _storage->getArbitragePairName( id );
    base_coin_id = _storage->getArbitragePairBaseCoinId( id );
    quote_coin_id = _storage->getArbitragePairMarketCoinId( id );

}

SwiftBot::Order::Order() {

}

SwiftBot::Order SwiftBot::Order::create(const quint32 &market_id, const double &amount, const double &rate, const quint32 &type)
{
    Order o( market_id, amount, rate, type );
    o.save();
    return o;
}

void SwiftBot::Order::update() {
    if ( local_id == 0 ) {
        save();
    } else {
        QSqlQuery q("");
        if ( !q.exec() ) {
            qWarning() << q.lastError().text();
        }
    }
}

SwiftBot::Market SwiftBot::Order::market() {
    static Market _cache( market_id );
    return _cache;
}

void SwiftBot::Order::save() {
    QSqlQuery q("");
    if ( !q.exec() ) {
        qWarning() << q.lastError().text();
    } else {
        local_id = q.lastInsertId().toUInt();
    }
}

SwiftBot::Order::Order(const quint32 &market_id_, const double &amount_, const double &rate_, const quint32 &type_)
    : local_id(0),
      amount( amount_ ),
      rate( rate_ ),
      market_id( market_id_ ),
      type( type_ )
{
    exchange_id = SwiftBot::Market( market_id ).exchange_id;
    fee = SwiftCore::getExchangeFee( exchange_id );
    amount_left = amount;
    created_at = QDateTime::currentDateTime();
}

SwiftBot::Order::Order(const QSqlRecord &q) {
    local_id = q.value("id").toUInt();
    remote_id = q.value("remote_id").toUInt();
    amount = q.value("amount").toDouble();
    amount_left = q.value("amount_left").toDouble();
    rate = q.value("rate").toDouble();
    price = q.value("price").toDouble();
    fee = q.value("fee").toDouble();
    created_at = q.value("created_at").toDateTime();
    completed_at = q.value("completed_at").toDateTime();
    market_id = q.value("market_id").toUInt();
    exchange_id = q.value("exchange_id").toUInt();
    type = q.value("type").toUInt();
    status = q.value("status").toUInt();
}

SwiftBot::Order::Order(const quint32 &id) {
    QSqlQuery q("SELECT * FROM orders WHERE id="+QString::number( id ) );
    if ( q.exec() && q.next() ) {
        local_id = q.value("id").toUInt();
        remote_id = q.value("remote_id").toUInt();
        amount = q.value("amount").toDouble();
        amount_left = q.value("amount_left").toDouble();
        rate = q.value("rate").toDouble();
        price = q.value("price").toDouble();
        fee = q.value("fee").toDouble();
        created_at = q.value("created_at").toDateTime();
        completed_at = q.value("completed_at").toDateTime();
        market_id = q.value("market_id").toUInt();
        exchange_id = q.value("exchange_id").toUInt();
        type = q.value("type").toUInt();
        status = q.value("status").toUInt();
    }
}

QJsonObject SwiftBot::Order::toJson() {
    QJsonObject j_obj;
    j_obj["market_id"] = QString::number( market_id );
    j_obj["local_id"] = QString::number( local_id );
    j_obj["remote_id"] = remote_id;
    j_obj["type"] = type == 0 ? "sell" : "buy";
    j_obj["created_at"] = QString::number( created_at.toSecsSinceEpoch() );
    j_obj["completed_at"] = QString::number( completed_at.toSecsSinceEpoch() );
    j_obj["amount"] = QString::number( amount, 'f', market().amount_precision );
    j_obj["amount_left"] = QString::number( amount_left, 'f', market().amount_precision );
    j_obj["rate"] = QString::number( rate, 'f', market().price_precision );
    j_obj["price"] = QString::number( price, 'f', market().price_precision );
    j_obj["status"] = QString::number( status );
    return j_obj;
}

void SwiftBot::Order::update(const QJsonObject &j_data) {
    remote_id = j_data.value("remote_id").toString();
    type = j_data.value("type").toString() == "sell" ? 0 : 1;
    status = j_data.value("status").toString().toUInt();
    created_at = QDateTime::fromTime_t( j_data.value("created_at").toString().toUInt() );
    completed_at = QDateTime::fromTime_t( j_data.value("completed_at").toString().toUInt() );
    amount = j_data.value("amount").toString().toDouble();
    amount_left = j_data.value("amount_left").toString().toDouble();
    rate = j_data.value("rate").toString().toDouble();
    price = j_data.value("price").toString().toDouble();
    exchange_id = market().exchange_id;
    update();
}
