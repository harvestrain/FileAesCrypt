#include "file_aes_crypt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FileAesCrypt w;
    w.show();
    return a.exec();
}
