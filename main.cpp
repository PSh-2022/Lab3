#include "mainwindow.h"
#include <QApplication>
//файл для создания окна приложения и вывода в нем результатов работы программы
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
/*При создании сложных приложений существенно увеличивается трудоёмкость разработки кода.
Основная концепция идеологии MVC – разделить части программы, отвечающие за хранение и
доступ к данным, их отображение и взаимодействие с пользователем на три отдельных модуля,
разработка которых относительно независима.
Такой подход:
• увеличивает степень повторного использования кода (например, одна модель может быть
применена к нескольким различным между собой представлениям данных);
• облегчает модификацию программы (в идеале можно менять модель, вид или контроллер
отдельно);
• становится выгоден при разработке приложений в архитектуре "клиент-сервер",
приложений СУБД, использующих хранилища данных, или для перенаправления
однопоточного GUI в многопоточные обработчики.
Итак, MVC включает в себя 3 типа объектов:
• Модель – осуществляет соединение с источником данных, служит их логической моделью,
предоставляет интерфейс другим компонентам архитектуры. Модель представляет знания
о моделируемом объекте, но не должна содержать информации о том, как эти знания
визуализировать;
• Вид (представление) – обеспечивает конечное (например, экранное) представление
данных для пользователя. Вид получает из модели модельные индексы, являющиеся
ссылками на элементы данных. Сообщая модельные индексы модели, вид может получить
элементы из источника данных. Существенно то, что к модели можно применить
несколько видов, не изменяя её. Например, одни и те же данные можно показать в
виде таблицы, графика или круговой диаграммы;
• Контроллер - отвечает за пользовательский интерфейс. Он обеспечивает связь между
пользователем и системой: контролирует ввод данных пользователем и использует
модель и представление для реализации необходимой реакции.
Существуют различные реализации MVC, во многих из них базовый шаблон MVC модифицируется.
*/
/*Приложение реализовано с учетом принципа DI - Dependency Inversion Principle
Правило:
Классы верхних уровней не должны зависеть от классов нижних
уровней. Оба должны зависеть от абстракций. Абстракции не должны
зависеть от деталей. Детали должны зависеть от абстракций.

Обычно при проектировании программ можно выделить
два уровня классов.

Классы нижнего уровня реализуют базовые операции вроде работы с
диском, передачи данных по сети, подключения к базе данных и прочее.

Классы высокого уровня содержат сложную бизнес-логику программы,
которая опирается на классы низкого уровня для осуществления более
простых операций.*/
