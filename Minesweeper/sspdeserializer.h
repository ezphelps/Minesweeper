#ifndef SSPDESERIALIZER_H
#define SSPDESERIALIZER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QColor>
#include <QImage>

class sspDeserializer : public QObject
{
    Q_OBJECT
public:
    explicit sspDeserializer(QObject *parent = nullptr);
    QImage deserializeSSP(QString filename);

};

#endif // SSPDESERIALIZER_H
