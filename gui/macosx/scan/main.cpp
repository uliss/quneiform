#include <iostream>
#include <QApplication>
#include <QPushButton>
#include <QDialog>
#include <QHBoxLayout>
#include <QSystemLocale>
#include <QDebug>
#include "../MacScan/MacScan.h"

#include "cocoahelpbutton.h"
using namespace std;

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    qDebug() << QLocale::system().name();
    qDebug() << QLocale().uiLanguages();

    showScanDialog();
    
    return app.exec();
}

