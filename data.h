#ifndef DATA_H
#define DATA_H
#include <QString>

//класс, объект которого хранит данные, полученные из базы данных
class DataStorage
{
public:
    QString key;//ключ(дата-время)
    double value;// значение
    DataStorage (QString key_, double value_) { key = key_; value = value_; };//конструктор по умолчанию
};

// интерфейс IOC контейнера для чтения данных из файла
class IChartData
{
public:
    //функция для получения данных, сохраняемых в QVector <DataStorage>
    virtual QVector <DataStorage> getData (QString path_) = 0;
};
//реализация интерфейса: формат .sqlite
class ChartDataSqlite : public IChartData
{
public:
    //функция для получения данных из файла формата .sqlite, сохраняемых в QVector <DataStorage>
    QVector <DataStorage> getData (QString path_);
};
//реализация интерфейса: формат .json
class ChartDataJson : public IChartData
{
public:
    //функция для получения данных из файла формата .json, сохраняемых в QVector <DataStorage>
    QVector <DataStorage> getData (QString path_);
};

#endif // DATA_H
