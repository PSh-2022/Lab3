#ifndef WIDGET_H
#define WIDGET_H
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QPair>
#include <QPoint>
#include <QString>


class QComboBox;
class QCheckBox;
class QChartView;
class QChart;
typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE


//QT_CHARTS_BEGIN_NAMESPACE;
class QChartView;
class QChart;
//QT_CHARTS_END_NAMESPACE;


typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

//QT_CHARTS_USE_NAMESPACE;

class WidgetChart:public QWidget
{
    Q_OBJECT
public:
    explicit WidgetChart(QWidget *parent = nullptr){}
    ~WidgetChart(){}


};

//виды диаграмм
enum TypeThemeWidget
{
    Area,
    Bar,
    Pie,
    Line,
    Spline,
    Scatter,
};

class ThemeWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ThemeWidget(QWidget *parent = nullptr);
    ~ThemeWidget();

private Q_SLOTS://слоты
    void updateUI();

private:
    QComboBox *createTheme() const;//тип диаграммы
    QComboBox *createAnimation() const;//анимация
    QComboBox *createLegend() const;//легенда диаграммы
    void connectSignals();//связь сигналов
public:
    QChart *createAreaChart() const;//диаграммы с областями
    QChart *createBarChart(int valueCount) const;//столбиковая диаграмма
    QChart *createPieChart() const;//круговая диаграмма
    QChart *createLineChart() const;//линейная диаграмма
    QChart *createSplineChart() const;//гладкая диаграмма
    QChart *createScatterChart() const;//точечная диаграмма
private:
    int m_listCount;
    int m_valueMax;
    int m_valueCount;

    QList<QChartView *> m_charts;
    DataTable m_dataTable; //табличное представление
    QComboBox *m_theme;//тип графика
    QCheckBox *m_antialiasCheck;//склаживание
    QComboBox *m_animated;//анимация
    QComboBox *m_legend;//легенда
};
#endif // WIDGET_H
