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

// интерфейс, строит новую диаграмму
class IChartDrawing
{
public:
    //функция, строящая новую диаграмму QChart* chart_ по данным QVector <DataStorage> data
    virtual void drawChart(QVector <DataStorage> data, bool isColored = true, QChart* chart_ = new QChart()) = 0;
};
//реализация интерфейса: столбчатая диаграмма
class barChartDrawing : public IChartDrawing
{
public:
    //функция, строящая новую столбчатую диаграмму QChart* chart_ по данным QVector <DataStorage> data
    virtual void drawChart(QVector <DataStorage> data, bool isColored = true, QChart* chart_= new QChart()); // по умолчанию - цветная
};
//реализация интерфейса: круговая диаграмма
class pieChartDrawing : public IChartDrawing//круговая
{
public:
    //функция, строящая новую круговую диаграмму QChart* chart_ по данным QVector <DataStorage> data
    virtual void drawChart(QVector <DataStorage> data, bool isColored = true, QChart* chart_= new QChart()); //  по умолчанию - цветная
};
class Charts//работа с диаграммами
{
    QChart *chart_; //содержит информацию о представлении диаграммы
    QVector <DataStorage> data_; //ключ-данные(дата-время) и значение данных из таблиц
    bool isColored_; // цвет диаграммы
public:
    Charts(): chart_( new QChart()), isColored_ (true){} // конструктор по умолчанию
    QChart* getChart(){return chart_;}; // геттер получения представления диаграммы
    void updateData(const QString& filePath); // обновление данных диаграммы
    void reDrawChart() const{
        IOCContainer::instance().GetObject<IChartDrawing>()->drawChart(data_,isColored_,chart_);
    }; // изменение представления диаграммы
    void changeColor(){
        if(isColored_ == true)
            isColored_ = false;
        else
            isColored_ = true;
    }; //смена цвета
};

#endif // CHARTS_H
