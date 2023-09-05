#include "file_aes_crypt.h"
#include "qaesencryption.h"
#include <QCryptographicHash>
#include <QCoreApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

#define FILE_PATH QCoreApplication::applicationDirPath() + "/../../data/"

FileAesCrypt::FileAesCrypt(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.tabWidget->setCurrentIndex(0);
    QDir dir(FILE_PATH);
    dir.mkpath(FILE_PATH);
    InitializeEvents();
}

FileAesCrypt::~FileAesCrypt()
{}

void FileAesCrypt::InitializeEvents()
{
    connect(ui.nocrypt_file_se, &QPushButton::clicked, this, &FileAesCrypt::SlotFileNocryptSe);
    connect(ui.crypt_file_se, &QPushButton::clicked, this, &FileAesCrypt::SlotFileCryptSe);
    connect(ui.file_encrypt, &QPushButton::clicked, this, &FileAesCrypt::SlotFileEncrypt);
    connect(ui.file_decrypt, &QPushButton::clicked, this, &FileAesCrypt::SlotFileDecrypt);
}

void FileAesCrypt::SlotFileNocryptSe()
{
    QString file_path = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("普通文件选择"), FILE_PATH, QString("*.xml"));
    ui.nocrypt_file->clear();
    QFileInfo file_info(file_path);
    if (file_info.exists())
        ui.nocrypt_file->setText(file_path);
}

void FileAesCrypt::SlotFileCryptSe()
{
    QString file_path = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("加密文件选择"), FILE_PATH, QString("*.crypt"));
    ui.nocrypt_file->clear();
    QFileInfo file_info(file_path);
    if (file_info.exists())
        ui.crypt_file->setText(file_path);
}

void FileAesCrypt::SlotFileEncrypt()
{
    QFileInfo file_info(ui.nocrypt_file->text());
    if (!file_info.exists())
        return;
	QAESEncryption encryption(QAESEncryption::Aes::AES_128, QAESEncryption::ECB);
    QByteArray key = QCryptographicHash::hash(ui.crypt_password->text().toLocal8Bit(), QCryptographicHash::Algorithm::Sha512);
    QFile file(file_info.absoluteFilePath());
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QByteArray encode_bytearray = encryption.encode(file.readAll(), key);
        file.close();
        QFile encode_file(file_info.absoluteFilePath() + ".crypt");
        if (encode_file.open(QIODevice::WriteOnly))
        {
            encode_file.write(encode_bytearray);
            encode_file.close();
        }
    }
    QMessageBox::information(this, QString::fromLocal8Bit("文件加密"), QString::fromLocal8Bit("加密完成"));
}

void FileAesCrypt::SlotFileDecrypt()
{
    QFileInfo file_info(ui.crypt_file->text());
    if (!file_info.exists())
        return;
    QAESEncryption decryption(QAESEncryption::Aes::AES_128, QAESEncryption::ECB);
    QByteArray key = QCryptographicHash::hash(ui.decrypt_password->text().toLocal8Bit(), QCryptographicHash::Algorithm::Sha512);
    QFile file(file_info.absoluteFilePath());
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray decode_bytearray = decryption.decode(file.readAll(), key);
        file.close();
        QString decode_path = file_info.absoluteFilePath().remove(".crypt");
        QFile decode_file(decode_path);
        if (decode_file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            decode_bytearray = decryption.RemovePadding(decode_bytearray);
            decode_file.write(decode_bytearray);
            decode_file.close();
        }
    }
    QMessageBox::information(this, QString::fromLocal8Bit("文件解密"), QString::fromLocal8Bit("解密完成"));
}
