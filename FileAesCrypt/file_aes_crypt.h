#pragma once

#include <QtWidgets/QWidget>
#include "ui_file_aes_crypt.h"

class FileAesCrypt : public QWidget
{
    Q_OBJECT

public:
    FileAesCrypt(QWidget *parent = nullptr);
    ~FileAesCrypt();

    void InitializeEvents();

private slots:
    void SlotFileNocryptSe();
    void SlotFileCryptSe();
    void SlotFileEncrypt();
    void SlotFileDecrypt();

private:
    Ui::FileAesCryptClass ui;
};
