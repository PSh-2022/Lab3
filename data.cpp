#include "data.h"
#include <QMessageBox>
#include <QtSql>

//формат .sqlite
QVector <DataStorage> ChartDataSqlite::getData (QString path_)
{
    QVector <DataStorage> data;//вектор данных

    static QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");// соединяемся с драйвером "QSQLITE"
    dbase.setDatabaseName(path_);

    if (!dbase.open())//если не открыт
    {
        QMessageBox messageBox;
        messageBox.setText("Impossible to open a database" + path_);
        messageBox.exec();
    }

    else//если возможно
    {
        QSqlQuery query("SELECT * FROM " + dbase.tables().takeFirst()); // формируем запрос для SQL таблицы
        int i = 0;
        while (query.next() && i < 10) // берем первые 10 строк(в исходной таблице данных слишком много)
        {
            QString key = query.value(0).toString(); // конвертируем ключ в строку
            double value = query.value(1).toDouble(); // конвертируем значение в Double
            data.push_back(DataStorage(key, value)); // помещаем в data
            i++;
        }
    }

    return data;
}

//формат .json
QVector <DataStorage> ChartDataJson::getData (QString path_)
{
    QVector <DataStorage> data;//вектор данных

    // Для начала прочитаем весь файл в QString
    QString fileInString;
    QFile file;
    file.setFileName(path_);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))//если не открыта
    {
        QMessageBox messageBox;
        messageBox.setText("Impossible to open a database" + path_);
        messageBox.exec();
    }

    fileInString = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileInString.toUtf8());//конвертируем в JSON

    if (!doc.isArray())//для Json, использующих [ ] - Array, для использующих {} - Object
    {
        QMessageBox messageBox;
        messageBox.setText("Enter Json as a massive" + path_);
        messageBox.exec();
    }

    QJsonArray jsonArr = doc.array();
    int i = 0;
    foreach (const QJsonValue & value, jsonArr) // для всех
    {
        if (value.isObject() && i < 10) // первые 10 строк
        {
            QJsonObject obj = value.toObject();
            QString key = obj["Time"].toString(); // конвертируем ключ в строку
            double value = obj["Value"].toDouble(); // конвертируем значение в double

            data.push_back(DataStorage(key, value)); // добавляем в data
            i++;
        }
    }
    return data;
}
