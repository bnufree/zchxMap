#include "profiles.h"
#include <QSettings>
#include <QTextCodec>

namespace zchxEcdis {

Profiles* Profiles::minstance = 0;
Profiles::MGarbage Profiles::Garbage;

Profiles::Profiles(QObject *parent) :
    QObject(parent)
{
    configSettings = new QSettings("mapdata/settings.ini", QSettings::IniFormat);
    configSettings->setIniCodec(QTextCodec::codecForName("GB18030"));

    iniSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, QLatin1String("Frameworks"), QLatin1String("profiles"));
    iniSettings->setIniCodec(QTextCodec::codecForName("GB18030"));
}

Profiles::~Profiles()
{
}
/*-------------------------------------------
 * 实例化
---------------------------------------------*/
Profiles *Profiles::instance()
{
    if ( minstance == 0)
    {
        minstance = new Profiles();
    }
    return minstance;
}

/*-------------------------------------------
 * 设置默认值
---------------------------------------------*/
void Profiles::setDefault(const QString & prefix,const QString &key, const QVariant &value)
{
    configSettings->beginGroup(prefix);
    if(configSettings->value(key).toString().isEmpty())
    {
        configSettings->setValue(key, value);
    }
    configSettings->endGroup();
}
/*-------------------------------------------
 * 设置配置文件值
---------------------------------------------*/
void Profiles::setValue(const QString & prefix,const QString & key, const QVariant & value)
{
    configSettings->beginGroup(prefix);
    {
        configSettings->setValue(key, value);
    }
    configSettings->endGroup();
}
/*-------------------------------------------
 * 返回值
---------------------------------------------*/
QVariant Profiles::value(const QString & prefix,const QString &keys, const QVariant &defaultValue)
{
//    QVariant values;
//    configSettings->beginGroup(prefix);
//    {
//        values =  configSettings->value(keys, defaultValue);
//    }
//    configSettings->endGroup();
    return configSettings->value(prefix+"/"+keys,defaultValue);;
}

/*-------------------------------------------
 * 设置Ini默认值
---------------------------------------------*/
void Profiles::setUserDefault(const QString & prefix,const QString &key, const QVariant &value)
{
    iniSettings->beginGroup(prefix);
    if(iniSettings->value(key).toString().isEmpty())
    {
        iniSettings->setValue(key, value);
    }
    iniSettings->endGroup();
}

/*-------------------------------------------
 * 设置Ini配置文件值
---------------------------------------------*/
void Profiles::setUserValue(const QString & prefix,const QString & key, const QVariant & value)
{
    iniSettings->beginGroup(prefix);
    {
        iniSettings->setValue(key, value);
    }
    iniSettings->endGroup();
}
/*-------------------------------------------
 * 返回值Ini
---------------------------------------------*/
QVariant Profiles::userValue(const QString & prefix,const QString &keys, const QVariant &defaultValue)
{
    QVariant values;
    iniSettings->beginGroup(prefix);
    {
        values =  iniSettings->value( keys,defaultValue);
    }
    iniSettings->endGroup();
    return values;
}
}

