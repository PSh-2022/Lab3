#include "chart.h"


// столбчатые
void barChartDrawing::drawChart(QVector<DataStorage> data, bool isColored, QChart* chart_)
{
    chart_->setTitle("Bar chart"); // заголовок

    QBarSeries *series = new QBarSeries{chart_}; // серии данных для столбчатой диаграммы

    int i = 0; // счетчик кол-ва считываемых данных из базы
    int j = 0; // переключение чб/цвет
    foreach (DataStorage elem, data) // для каждого элемента
    {
        QString legendHeader (elem.key); // задаем ключ
        QBarSet *set = new QBarSet(legendHeader); // задаём набор данных для столбчатой диаграммы
        if (!isColored) // цвет
        {
            set->setColor(QColor(j,j,j));
            j+=25;

        }
        *set << elem.value; // добавляем значение в набор данных для столбчатой диаграммы
        series->append(set); // добавлеяем набор в серию данных для столбчатой диаграммы
        i++;
    }

    chart_->removeAllSeries(); // очищаем предыдущие серии
    chart_->addSeries(series); // добавляем серию в Диаграмму
    chart_->setAnimationOptions(QChart::SeriesAnimations); // анимация
    chart_->createDefaultAxes(); // Устанавливаем оси
}

// круговые
void pieChartDrawing::drawChart(QVector<DataStorage> data, bool isColored, QChart* chart_)
{
    chart_->setTitle("Pie chart"); // заголовок

    QPieSeries *series = new QPieSeries{chart_}; //серии данных для круговой диаграммы

    int i = 0; //счетчик кол-ва считываемых данных из базы
    int j = 0; //чб/цвет
    foreach (DataStorage elem, data) // для каждого элемента
    {
        QString legendHeader (elem.key); // задаем ключ
        series->append(legendHeader, elem.value); // добавлеяем ключ и значение в серию
        if (!isColored) // цвет
        {
            series->slices().at(i)->setColor(QColor(j,j,j));
            j+=25;
        }
        i++;
    }

    chart_->removeAllSeries(); // очищаем предыдущие серии
    chart_->addSeries(series); // добавляем серию в диаграмму
    chart_->setAnimationOptions(QChart::SeriesAnimations); //анимация
    chart_->createDefaultAxes(); // устанавливаем оси
}

//перерисовка диаграммы
void Charts::reDrawChart() const
{
    IOCContainer::instance().GetObject<IChartDrawing>()->drawChart(data_,isColored_,chart_);
}

// обновление данных
void Charts::updateData(const QString& filePath)
{
    data_ = IOCContainer::instance().GetObject<IChartData>()->getData(filePath);//дата

    if (data_.isEmpty())//если пуст
    {
        QMessageBox messageBox;
        messageBox.setText("File is empty\n");//сообщение: файл пуст
        messageBox.exec();
        return;
    }
    reDrawChart();
}

// uеттер для получения QT класса QChart
QChart* Charts::getChart()
{
    return chart_;
}

//смена цвета
void Charts::changeColor()
{
    if(isColored_ == true)
        isColored_ = false;
    else
        isColored_ = true;
}
