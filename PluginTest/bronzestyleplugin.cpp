#include "bronzestyleplugin.h"


QStringList BronzeStylePlugin::keys() const
{
    return QStringList() << "Bronze";
}

QStyle *BronzeStylePlugin::create(const QString &key)
{
    if(key.toLower() == "bronze")
        return new BronzeStylePlugin
}
