/*
 *
 */

#ifndef CONFIGSAVETEST_H
#define CONFIGSAVETEST_H

#include <QObject>

class AbstractAppearance;
class ConfigSaveTest : public QObject
{
    Q_OBJECT
public:
    ConfigSaveTest();
private slots:
    void testGtk2();
    void testGtk3();
};

#endif // CONFIGSAVETEST_H
