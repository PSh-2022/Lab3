#ifndef DATA_H
#define DATA_H
#include <QString>

class DataStorage
{
public:
    QString key;//ключ(дата-время)
    double value;// значение
    DataStorage (QString key_, double value_) { key = key_; value = value_; };//конструктор по умолчанию
};

// интерфейс IOC контейнера для чтения данных для отрисовки диаграмм
class IChartData
{
public:
    virtual QVector <DataStorage> getData (QString path_) = 0;//возвращаем данные из таблиц
};
//используем IOC контейнер, менять IChartData при смене типа баз данных не нужно
// формат.sqlite
class ChartDataSqlite : public IChartData
{
public:
    QVector <DataStorage> getData (QString path_);
};

//формат .json
class ChartDataJson : public IChartData
{
public:
    QVector <DataStorage> getData (QString path_);
};

#endif // DATA_H
