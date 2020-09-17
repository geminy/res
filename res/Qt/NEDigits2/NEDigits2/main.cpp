#include <QApplication>
#include <QQuickView>
#include <QtQml>
#include <QDesktopWidget>
#include "NE2048.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    int screenWidth = 0;
    int screenHeight = 0;
    bool isWideScreen = false;
    QDesktopWidget *deskWgt = app.desktop();
    if (deskWgt) {
        screenWidth = deskWgt->screenGeometry().width();
        screenHeight = deskWgt->screenGeometry().height();
        if (screenWidth > screenHeight) {
            isWideScreen = true; // gub
        }
    }
    else {
        // else none
    }

    qmlRegisterType<NE2048>("naturEarth", 1, 0, "NE2048");

    QQuickView view;
    if (view.rootContext()) {
        view.rootContext()->setContextProperty("screenWidth", screenWidth);
        view.rootContext()->setContextProperty("screenHeight", screenHeight);
        view.rootContext()->setContextProperty("isWideScreen", isWideScreen);
    }
    view.setSource(QUrl(QStringLiteral("qrc:///main.qml")));
    view.show();

    return app.exec();
}
