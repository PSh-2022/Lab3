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


MainWindow::MainWindow(QWidget *parent): QWidget(parent)
{
    themeWidget = new ThemeWidget();
   this->setGeometry(200, 200, 2000, 800);//размер главного окна
   setWindowTitle("Chart's by DB"); // заголовок окна
   QString homePath = QDir::homePath();
   dirModel =  new QFileSystemModel(this);
   dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
   dirModel->setRootPath(homePath);

   fileModel = new QFileSystemModel(this);
   fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);

   fileModel->setRootPath(homePath);
   treeView = new QTreeView();//покажем как дерево, создаем новый вид
   treeView->setModel(dirModel);

   treeView->expandAll();

   QSplitter *splitter = new QSplitter(parent);
   tableView = new QTableView;
   tableView->setModel(fileModel);
   splitter->addWidget(treeView);
   splitter->addWidget(tableView);

//добавление диаграммы
    QChartView *chartView;
    QChart *chartBar =  themeWidget->createSplineChart();
    chartView = new QChartView(chartBar);
   splitter->addWidget(themeWidget);
   QItemSelectionModel *selectionModel = treeView->selectionModel();
   QModelIndex rootIx = dirModel->index(0, 0, QModelIndex());//корневой элемент

   QModelIndex indexHomePath = dirModel->index(homePath);
   QFileInfo fileInfo = dirModel->fileInfo(indexHomePath);

   treeView->header()->resizeSection(0, 200);//соединение слота и сигнала, который вызывается при осуществлении выбора элемента в TreeView
   connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
           this, SLOT(on_selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
   //установка курсора в TreeView относительно модельного индекса
   QItemSelection toggleSelection;
   QModelIndex topLeft;
   topLeft = dirModel->index(homePath);
   dirModel->setRootPath(homePath);

   toggleSelection.select(topLeft, topLeft);
   selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
}
//выбор элемента в TreeView с помощью курсора
void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
   Q_UNUSED(deselected);
   QModelIndex index = treeView->selectionModel()->currentIndex();
   QModelIndexList indexs =  selected.indexes();
   QString filePath = "";
 // заполняем statusbar относительно выделенного модельного индекса
   if (indexs.count() >= 1) {
       QModelIndex ix =  indexs.constFirst();
       filePath = dirModel->filePath(ix);
   }

   int length = 200;
   int dx = 30;

   if (dirModel->fileName(index).length() * dx > length) {
       length = length + dirModel->fileName(index).length() * dx;
       qDebug() << "r = " << index.row() << "c = " << index.column() << dirModel->fileName(index) << dirModel->fileInfo(
                    index).size();
   }

   treeView->header()->resizeSection(index.column(), length + dirModel->fileName(index).length());
   tableView->setRootIndex(fileModel->setRootPath(filePath));
}

MainWindow::~MainWindow()
{

}
