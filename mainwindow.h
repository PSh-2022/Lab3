#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QPushButton>
#include <QTableView>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include "data.h"
#include "chart.h"

class MainWindow : public QWidget
{
    Q_OBJECT
private slots:
// Слот обработки
    void slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected); // выбор файла
    void slotChooseDirectory(); //папка для отображения всех записей
    void slotSelectionComboboxChanged(); // выбор типа данных с помощью Combobox (выпадающего списка)
    void slotSelectionColorChanged(); // выбора цвета: Цветной/ЧБ с помощью Checkbox (Выбор: Да/Нет)
    void slotSaveToPdf(); // Слот обработки сохранения Диаграммы в PDF

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QFileSystemModel *fileModel; // модель файловой системы
    QTableView *tableView; // модель табличного представления
    QString homePath;
    QComboBox* boxType; // выпадающий список с типами диаграмм
    QCheckBox *checkboxColor; // чекбокс для смены цвета диаграммы

    // Анонимная структура для взаимодействия с Диаграммой
    struct
    {
        Charts* chart;
        QChartView* chartView;
    } chartSettings;
};

#endif // MAINWINDOW_H
