#include "mainwindow.h"
#include <QSplitter>
#include <QListView>
#include <QTreeView>
#include <QTextEdit>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStatusBar>
#include <QDebug>
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QTime>
#include <QtCharts/QBarCategoryAxis>
#include <QFileDialog>

// определение s_typeId IOC контейнера
int IOCContainer::s_typeId = 121;

namespace
{
bool isCharCanPrintPDF = false;//допустимость сохранения в pdf

}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    QString homePath = QDir::homePath();//путь
    setGeometry(200, 200, 800, 800);//размер главного окна
    setWindowTitle("PrintChart");

    // определяем файловую систему:
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileModel->setRootPath(homePath);

    tableView = new QTableView;
    tableView->setModel(fileModel);//поиск файлов
    chartSettings.chart = new Charts{};//добавляем диаграмму
    chartSettings.chartView = new QChartView{};
    IOCContainer::instance().RegisterInstance<IChartDrawing, barChartDrawing>();//по умолчанию строим столбчатую диаграмму

    // кнопки
    auto directoryButton = new QPushButton ("open folder");
    auto printChartButton = new QPushButton ("print chart");
    checkboxColor = new QCheckBox("black-and-white");
    auto boxLabel = new QLabel("choose type of the diagram");
    auto directoryLabel = new QLabel("choose DB file");

    boxType = new QComboBox(); // выбор типа графика
    boxType->insertItem(1, QString("BarChart"));
    boxType->insertItem(2, QString("PieChart"));

    // расположение
    auto horizontalLayout=new QHBoxLayout(this);
    auto verticalLeftLayout = new QVBoxLayout();
    auto verticalRightLayout = new QVBoxLayout();
    auto chartLayout = new QHBoxLayout(); // положение для кнопок над графиком

    horizontalLayout->addLayout(verticalLeftLayout);
    horizontalLayout->addLayout(verticalRightLayout);
    verticalRightLayout->addLayout(chartLayout);
    verticalLeftLayout->addWidget(tableView);
    verticalRightLayout->addWidget(chartSettings.chartView);
    verticalLeftLayout->addWidget(directoryLabel);
    verticalLeftLayout->addWidget(directoryButton);
    // кнопки над графиком
    chartLayout->addWidget(boxLabel); //метка над графиком
    chartLayout->addWidget(boxType); // тип графика
    chartLayout->addWidget(checkboxColor); // ч/б
    chartLayout->addWidget(printChartButton); // сохранение в PDF
    QItemSelectionModel *selectionModel = tableView->selectionModel();

    // слоты
    connect(directoryButton,&QPushButton::clicked,this,&MainWindow::slotChooseDirectory); // кнопка выбора папки
    connect(boxType,SIGNAL(currentTextChanged(const QString&)),this,SLOT(slotSelectionComboboxChanged())); // выбор типа графика
    connect(checkboxColor, SIGNAL(toggled(bool)), this, SLOT(slotSelectionColorChanged())); // выбор ч/б
    // выбор файла
    connect(selectionModel,SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,SLOT(slotSelectionChanged(const QItemSelection &, const QItemSelection &)));

    connect(printChartButton,SIGNAL(clicked()), this, SLOT(slotSaveToPdf())); // сохранение в PDF
}

// Слот обработки выбора конкретного файла
void MainWindow::slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    QModelIndexList indexs =  selected.indexes(); // берем индекс файла
    if (indexs.count() < 1)
        return;

    QString filePath{""};
    filePath = fileModel->filePath(indexs.constFirst()); // получаем путь к файлу

    if (filePath.endsWith(".sqlite")) // если путь файла оканчивается на .sqlite
    {
        IOCContainer::instance().RegisterInstance<IChartData, ChartDataSqlite>(); // задаём интерфейсу чтение SQLITE файлов
        chartSettings.chart->updateData(filePath); // обновляем данные
        chartSettings.chartView->setChart(chartSettings.chart->getChart()); //задаем диаграмму
        isCharCanPrintPDF = true; // можно сохранить в pdf
    }
    else if (filePath.endsWith(".json")) // если путь файла оканчивается на .json
    {
        IOCContainer::instance().RegisterInstance<IChartData, ChartDataJson>();// задаём интерфейсу чтение Json файлов
        chartSettings.chart->updateData(filePath); // обновляем данные
        chartSettings.chartView->setChart(chartSettings.chart->getChart());//задаем диаграмму
        isCharCanPrintPDF = true;// можно сохранить в pdf
    }
    else
    {
        isCharCanPrintPDF = false; // нельзя сохранить в pdf
        QMessageBox messageBox;
        messageBox.setText("Choose a file with .sqlite or .json format.");
        messageBox.exec();
    }
}

// выбор папки для отображения файлов
void MainWindow::slotChooseDirectory()
{
    QFileDialog dialog{this};
    dialog.setFileMode(QFileDialog::Directory); // открываем диалог
    if ( dialog.exec() )
        homePath = dialog.selectedFiles().first(); // получаем выбранную директорию из диалога
    tableView->setRootIndex(fileModel->setRootPath(homePath)); // устанавливаем в отображение файлов этот путь
}

// выбор типа диаграмм с помощью Combobox (выпадающего списка)
void MainWindow::slotSelectionComboboxChanged()
{
    QString chartType = boxType->currentText(); // получаем текст из Combobox

    if(chartType == "PieChart")
    {
        IOCContainer::instance().RegisterInstance<IChartDrawing, pieChartDrawing>(); // задаём интерфейсу круговую диаграмму
        chartSettings.chart->reDrawChart(); // перерисовываем
        return;
    }
    else if (chartType == "BarChart")
    {
        IOCContainer::instance().RegisterInstance<IChartDrawing, barChartDrawing>(); // задаём интерфейсу столбчатую диаграмму
        chartSettings.chart->reDrawChart(); // перерисовываем
        return;
    }
}

// слот обработки выбора цвета: Цветной/ЧБ с помощью Checkbox (Выбор: Да/Нет)
void MainWindow::slotSelectionColorChanged()
{
    if (checkboxColor->checkState()) // если Чекбокс активен
    {
        chartSettings.chart->changeColor(); // меняем цвет
        chartSettings.chart->reDrawChart(); // перерисовываем
    }
    else
    {
        chartSettings.chart->changeColor(); // меняем цвет
        chartSettings.chart->reDrawChart(); // перерисовываем
    }
}

// сохраниение программы в PDF
void MainWindow::slotSaveToPdf()
{
    if (isCharCanPrintPDF) // если печатают
    {
        QString savingPath("");

        QFileDialog dialog(this); // создаём диалог

        dialog.setViewMode(QFileDialog::Detail); // устанавливаем детальный вид с выбором расширения

        if (dialog.exec())
        {
            savingPath = dialog.selectedFiles().first(); // Получаем из диалога директорию для сохранения
        }

        QPdfWriter writer(savingPath+".pdf"); // добавляем расширение .pdf

        writer.setCreator("Someone"); // Указываем создателя документа

        QPainter painter(&writer);

        chartSettings.chartView->render(&painter); // сохраняем диаграмму в pdf
        painter.end();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText("Have no chart to print");
        messageBox.exec();
    }
}

MainWindow::~MainWindow()
{

}

