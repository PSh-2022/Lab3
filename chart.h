#ifndef CHARTS_H
#define CHARTS_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

#include <QJsonDocument>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts>
#include "data.h"
#include "ioccontainer.h"

// интерфейс IOC контейнера для отрисовки диаграмм
class IChartDrawing
{
public:
    virtual void drawChart(QVector <DataStorage> data, bool isColored = true, QChart* chart_ = new QChart()) = 0;//отрисовка диаграмм
};

class barChartDrawing : public IChartDrawing//столбчатая
{
public:
    virtual void drawChart(QVector <DataStorage> data, bool isColored = true, QChart* chart_= new QChart()); // по умолчанию - цветная
};

class pieChartDrawing : public IChartDrawing//круговая
{
public:
    virtual void drawChart(QVector <DataStorage> data, bool isColored = true, QChart* chart_= new QChart()); //  по умолчанию - цветная
};

class Charts//работа с диаграммами
{
private:

    QChart *chart_; //диаграмма
    QVector <DataStorage> data_; // ключ данных( дата-время) и значение данных из таблиц
    bool isColored_; // цвет диаграммы
public:
    Charts(): chart_( new QChart()), isColored_ (true){} // конструктор по умолчанию

    QChart* getChart(); // геттер для получения диаграммы класса QChart

    void updateData(const QString& filePath); // обновление данных
    void reDrawChart() const; // перерисовка диаграмм
    void changeColor(); //смена цвета

};

#endif // CHARTS_H
