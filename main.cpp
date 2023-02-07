#include "FilesEditor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FilesEditor w;
    w.show();
    return a.exec();
}
