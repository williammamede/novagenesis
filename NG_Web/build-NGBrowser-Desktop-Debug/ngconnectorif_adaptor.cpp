/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -i ngconnectorif_adaptor.h -a :ngconnectorif_adaptor.cpp ../NGBrowser/ngconnectorif.xml
 *
 * qdbusxml2cpp is Copyright (C) 2016 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "ngconnectorif_adaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class ConnectionAdaptor
 */

ConnectionAdaptor::ConnectionAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

ConnectionAdaptor::~ConnectionAdaptor()
{
    // destructor
}

void ConnectionAdaptor::complete(const QString &str)
{
    // handle method call NGConnector.Connection.complete
    QMetaObject::invokeMethod(parent(), "complete", Q_ARG(QString, str));
}

