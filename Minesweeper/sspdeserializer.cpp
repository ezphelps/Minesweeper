#include "sspdeserializer.h"

sspDeserializer::sspDeserializer(QObject *parent)
    : QObject{parent}
{

}

/// \brief sspDeserializer::deserializeSSP
/// Deserialize an ssp file.
/// \param filename
QImage sspDeserializer::deserializeSSP(QString filename)
{
    QFile file(filename);

    // Read file in if it is readable.
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString json = in.readAll(); // Read text from file.
        file.close();

        //Convert document to obect.
        QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
        QJsonObject genericObject = doc.object();

        QJsonObject framesObject = genericObject.value("frames").toObject();
        for(QString frame : framesObject.keys())
        {
            QJsonArray frameArray = framesObject.value(frame).toArray();
            QImage image(genericObject.value("height").toInt(), genericObject.value("width").toInt(), QImage::Format_RGBA8888);

            //Loop through each pixel of fame and set rgba values.
            for (int i = 0; i < genericObject.value("height").toInt(); i++)
            {
                for(int j = 0; j < genericObject.value("width").toInt(); j++)
                {
                    QJsonArray pixel = frameArray[i].toArray()[j].toArray();
                    QColor pixelColor(pixel[0].toInt(), pixel[1].toInt(),pixel[2].toInt(),pixel[3].toInt());
                    if (pixelColor == QColor(0,0,0,255))
                    {
                        pixelColor = QColor("transparent");
                        image.setPixelColor(j,i,pixelColor);
                    }
                    image.setPixelColor(j,i,pixelColor);
                }
            }

            return image;
        }
    }
}
