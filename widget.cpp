#include <QtWidgets>
#include <QtCharts>
#include <QtCore/QTime>
#include <QtEvents>
#include <QtGlobal>
#include "widget.h"
ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_listCount(3), //кол-во графиков
    m_valueMax(10), //максимальное значение
    m_valueCount(7),
    m_dataTable(generateRandomData(m_listCount, m_valueMax, m_valueCount)),
    typeChart(AddTypeCharts()),//выпадающий список, отображающий возможные типы графиков
    BlackWhiteCheck(new QCheckBox("Черно-белый график")),
    printChart(new QPushButton("Печать графика")) //кнопка печати
{
    connectSignals();
    QVBoxLayout *baseLayout = new QVBoxLayout();//вертикальная раскладка
    QHBoxLayout *settingsLayout = new QHBoxLayout(); //горизонтальная
    settingsLayout->addWidget(new QLabel("Выберите тип диаграммы:"));//добавление виджетов на раскладку
    settingsLayout->addWidget(typeChart);
    settingsLayout->addWidget(BlackWhiteCheck);
    settingsLayout->addWidget(printChart);
    settingsLayout->addStretch();
    baseLayout->addLayout(settingsLayout);//создаем график
    chartView = new QChartView(createAreaChart());//добавляем горизонтальную раскладку
    baseLayout->addWidget(chartView);
    setLayout(baseLayout);
    //изначально графики цветные, чекбокс выключен
    BlackWhiteCheck->setChecked(true);
    updateUI();
}

ThemeWidget::~ThemeWidget()
{
}

void ThemeWidget::connectSignals()
{
    connect(typeChart,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ThemeWidget::updateUI);
    connect(BlackWhiteCheck, &QCheckBox::toggled, this, &ThemeWidget::updateUI);
    connect(printChart, SIGNAL(clicked()), this, SLOT(openFileDialogWindow()));
}

void ThemeWidget::openFileDialogWindow()
{
    QFileDialog *fileDialog = new QFileDialog(this);//заголовок файла
    fileDialog-> setWindowTitle (tr ("Открыть изображение"));//путь к файлу по умолчанию
    fileDialog->setDirectory(".");//фильтр файлов, только файлы sqlite и json
    fileDialog->setNameFilter(tr("Images(*.sqlite *.json)"));//вобор нескольких файлов, по умолчанию - один файл QFileDialog :: ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);//режим просмотра
    fileDialog->setViewMode(QFileDialog::Detail);//путь к выбранным файлам
    QStringList fileNames;
    if(fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
    }
}


DataTable ThemeWidget::generateRandomData(int listCount, int valueMax, int valueCount) const
{
    DataTable dataTable;

    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    for (int i(0); i < listCount; i++) {
        DataList dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++) {
            yValue = yValue + (qreal)(rand() % valueMax) / (qreal) valueCount;
            QPointF value((j + (qreal) rand() / (qreal) RAND_MAX) * ((qreal) m_valueMax / (qreal) valueCount),
                          yValue);
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Data(value, label);
        }
        dataTable << dataList;
    }

    DataTable dataTable;

    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("BLOOD_SUGAR.sqlite");
    if (sdb.open())
    {
        qDebug() << "BD open\n";
        QSqlQuery query("SELECT VALUE FROM BLOOD_SUGAR", sdb);
        int index = 0;
        DataList dataList;
        while (query.next() && (index < 10))
        {
            QString temp = query.value(0).toString();
            QPointF value(temp, index);

            dataList << Data(value, "BLOOD_SUGAR");

            index++;
        }
        dataTable << dataList;
    }
    else
    {
        qDebug() << "BD not open\n";
    }

    for (int i(0); i < listCount; i++) {
        DataList dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++) {
            yValue = yValue + (qreal)(qrand() % valueMax) / (qreal) valueCount;
            QPointF value((j + (qreal) rand() / (qreal) RAND_MAX) * ((qreal) m_valueMax / (qreal) valueCount),
                          yValue);
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Data(value, label);
        }
        dataTable << dataList;
    }
    return dataTable;
}

QComboBox *ThemeWidget::AddTypeCharts() const
{
    QComboBox *themeComboBox = new QComboBox();
    themeComboBox->addItem("Area", TypeThemeWidget::Area);
    themeComboBox->addItem("Bar", TypeThemeWidget::Bar);
    themeComboBox->addItem("Line", TypeThemeWidget::Line);
    themeComboBox->addItem("Pie", TypeThemeWidget::Pie);
    themeComboBox->addItem("Spline", TypeThemeWidget::Spline);
    themeComboBox->addItem("Scatter", TypeThemeWidget::Scatter);
    return themeComboBox;
}

QChart *ThemeWidget::createAreaChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Area chart");

    QLineSeries *lowerSeries = nullptr;
    QString name("Series ");
    int nameIndex = 0;
    for (int i(0); i < m_dataTable.count(); i++) {
        QLineSeries *upperSeries = new QLineSeries(chart);
        for (int j(0); j < m_dataTable[i].count(); j++) {
            Data data = m_dataTable[i].at(j);
            if (lowerSeries) {
                const QVector<QPointF>& points = lowerSeries->pointsVector();
                upperSeries->append(QPointF(j, points[i].y() + data.first.y()));
            } else {
                upperSeries->append(QPointF(j, data.first.y()));
            }
        }
        QAreaSeries *area = new QAreaSeries(upperSeries, lowerSeries);
        area->setName(name + QString::number(nameIndex));

        nameIndex++;
        chart->addSeries(area);
        chart->createDefaultAxes();
        lowerSeries = upperSeries;
    }

    return chart;
}

QChart *ThemeWidget::createBarChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Bar chart");

    QStackedBarSeries *series = new QStackedBarSeries(chart);
    for (int i(0); i < m_dataTable.count(); i++) {
        QBarSet *set = new QBarSet("Bar set " + QString::number(i));
        for (const Data &data : m_dataTable[i])
            *set << data.first.y();

        series->append(set);
    }
    chart->addSeries(series);
    chart->createDefaultAxes();

    return chart;
}

QChart *ThemeWidget::createLineChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Line chart");

    QString name("Series ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable) {
        QLineSeries *series = new QLineSeries(chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));

        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();

    return chart;
}

QChart *ThemeWidget::createPieChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Pie chart");

    qreal pieSize = 1.0 / m_dataTable.count();
    for (int i = 0; i < m_dataTable.count(); i++) {
        QPieSeries *series = new QPieSeries(chart);
        for (const Data &data : m_dataTable[i]) {
            QPieSlice *slice = series->append(data.second, data.first.y());
            if (data == m_dataTable[i].first()) {
                slice->setLabelVisible();
                slice->setExploded();
            }
        }
        qreal hPos = (pieSize / 2) + (i / (qreal) m_dataTable.count());
        series->setPieSize(pieSize);
        series->setHorizontalPosition(hPos);
        series->setVerticalPosition(0.5);
        chart->addSeries(series);
    }

    return chart;
}

QChart *ThemeWidget::createSplineChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Spline chart");
    QString name("Series ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable) {
        QSplineSeries *series = new QSplineSeries(chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));

        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    return chart;
}

QChart *ThemeWidget::createScatterChart() const
{
    // scatter chart
    QChart *chart = new QChart();
    chart->setTitle("Scatter chart");
    QString name("Series ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable) {
        QScatterSeries *series = new QScatterSeries(chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));

        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    return chart;
}

void ThemeWidget::updateUI()
{
    TypeThemeWidget typeChart_ = static_cast<TypeThemeWidget>(
                typeChart->itemData( typeChart->currentIndex()).toInt());

    switch (typeChart_)
    {
    case TypeThemeWidget::Area :
        chartView->setChart(createAreaChart());
        break;
    case TypeThemeWidget::Bar :
        chartView->setChart(createBarChart());
        break;
    case TypeThemeWidget::Line :
        chartView->setChart(createLineChart());
        break;
    case TypeThemeWidget::Pie :
        chartView->setChart(createPieChart());
        break;
    case TypeThemeWidget::Spline :
        chartView->setChart(createSplineChart());
        break;
    case TypeThemeWidget::Scatter :
        chartView->setChart(createScatterChart());
        break;
    }
