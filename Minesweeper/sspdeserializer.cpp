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

    //read file in if it is readable
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString json = in.readAll(); // read text from file
        file.close();

        //convert json to obect
        QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
        QJsonObject genericObject = doc.object();
        //spriteModel->frameHeight = genericObject.value("height").toInt();
        //spriteModel->frameWidth = genericObject.value("width").toInt();
        //spriteModel->frameCount = genericObject.value("numberOfFrames").toInt();
        //frames now is object that needs to be set to actual frames
        QJsonObject framesObject = genericObject.value("frames").toObject();
        for(QString frame : framesObject.keys())
        {
            QJsonArray frameArray = framesObject.value(frame).toArray();
            QImage image(genericObject.value("height").toInt(), genericObject.value("width").toInt(), QImage::Format_RGBA8888);

            //loop through each pixel of fame and set rgba values
            for (int i = 0; i<genericObject.value("height").toInt(); i++)
            {
                for(int j = 0; j<genericObject.value("width").toInt(); j++)
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
