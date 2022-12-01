#include "chart.h"


// столбчатая диаграмма
void barChartDrawing::drawChart(QVector<DataStorage> data, bool isColored, QChart* chart_)
{
    chart_->setTitle("Bar chart"); // заголовок
    QBarSeries *series = new QBarSeries{chart_}; // серии данных для столбчатой диаграммы
    int i = 0; // счетчик количества считываемых данных из базы
    int j = 0; // переключение чб/цвет
    foreach (DataStorage elem, data) // для каждого элемента, полученного из базы данных
    {
        QString legendHeader (elem.key); // задаем легенду карты по времени добавления данных, полученному из базы данных
        QBarSet *set = new QBarSet(legendHeader); // передаем легенду
        if (!isColored) // цвет
        {//чб
            set->setColor(QColor(j,j,j));
            j+=15;}
        *set << elem.value; // добавляем значение в набор данных для столбчатой диаграммы]
        series->append(set); // добавлеяем набор в серию данных для столбчатой диаграммы
        i++;//переходим к следующему набору
    }
    chart_->removeAllSeries(); // очищаем предыдущие серии
    chart_->addSeries(series); // добавляем серию в Диаграмму
    chart_->createDefaultAxes(); // Устанавливаем оси
}

// круговая диаграмма
void pieChartDrawing::drawChart(QVector<DataStorage> data, bool isColored, QChart* chart_)
{
    chart_->setTitle("Pie chart"); // задаем заголовок
    QPieSeries *series = new QPieSeries(chart_); //серии данных для круговой диаграммы
    int i = 0; //счетчик количества считываемых данных из базы
    int j = 0; //чб/цвет
    foreach (DataStorage elem, data) // для каждого элемента
    {
        QString legendHeader (elem.key); // задаем легенду карты по времени добавления данных, полученному из базы данных
        QPieSlice *slice = new QPieSlice();
        slice->setLabel(elem.key);
        if (!isColored) // цвет
        {//чб
            slice->setBrush(QColor(j,j,j));
            j+=15;}
        slice->setValue(elem.value);
        series->append(slice);
        i++;//переходим к следующему набору
    }
    chart_->removeAllSeries(); // очищаем предыдущие серии
    chart_->addSeries(series); // добавляем серию в диаграмму
    chart_->createDefaultAxes(); // устанавливаем оси
}


// обновление данных диаграммы
void Charts::updateData(const QString& filePath)
{
    data_ = IOCContainer::instance().GetObject<IChartData>()->getData(filePath);//получаем новые данные из ридера контейнера и сразу их записываем
    if (data_.isEmpty())//если данные не получены, вызываем сообщение об ошибке
    {
        QMessageBox messageBox;
        messageBox.setText("File is empty\n");//сообщение: файл пуст
        messageBox.exec();
        return;
    }
    reDrawChart();//измеяем представление диаграммы
}
