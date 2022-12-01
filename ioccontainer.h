#ifndef IOCCONTAINER_H
#define IOCCONTAINER_H

#include <iostream>
#include <functional>
#include <cstdlib>
#include <memory>
#include <map>
using namespace std;
class IOCContainer
{    static int s_typeId;//переменная, определяющая Id следующего выделенного типа
/*Статический метод или член класс означает, что он один и тот же на
 все экземпляры (объекты) класса.
 Статический член класса - это свойство, относящееся к классу в
 целом, а не конкретному объекту.
 Статический член класс /метод класс можно использовать и без
 создания экземпляра класса*/
 public:
    template<typename T>
    static int GetTypeID(){
        static int typeId = s_typeId++;
        return typeId;
    }//метод, позволяющий обратиться к экземпляру локальной переменной для указанного типа
    class FactoryBase//абстрактная фабрика, дает возможность хранить объект, способ создания которого неизвестен
    {
    public:
        virtual ~FactoryBase() {}
    };
  /* Фабричный метод (также известен как Виртуальный конструктор, Factory
 Method) — это порождающий паттерн проектирования, который определяет
 общий интерфейс для создания объектов в суперклассе, позволяя
 подклассам изменять тип создаваемых объектов.
 Паттерн применяется когда
 1. Когда заранее неизвестно, объекты каких типов необходимо
 создавать;
 2. Когда система должна быть независимой от процесса создания
 новых объектов и расширяемой: в нее можно легко вводить новые
 классы, объекты которых система должна создавать;
 3. Когда создание новых объектов необходимо делегировать из
 базового класса классам наследникам;
 Достоинства паттерна Factory Method
 • Создает объекты разных типов, позволяя системе оставаться
 независимой как от самого процесса создания, так и от типов
 создаваемых объектов.
 Недостатки паттерна Factory Method
 • В случае классического варианта паттерна даже для
 порождения единственного объекта необходимо создавать
 соответствующую фабрику

   *  Абстрактная фабрика представляет собой некоторый полиморфный базовый класс(AbstractFactory),
 назначением которого является объявление интерфейсов фабричных методов(CreateProductA,
 CreateProductB), служащих для создания продуктов всех основных типов (один фабричный метод на
 каждый тип продукта).
   *   Достоинства паттерна Abstract Factory
     • Скрывает сам процесс порождения объектов, а также делает систему
     независимой от типов создаваемых объектов, специфичных для различных
     семейств или групп (пользователи оперируют этими объектами через
     соответствующие абстрактные интерфейсы).
     • Позволяет быстро настраивать систему на нужное семейство создаваемых
     объектов.

     Недостатки паттерна Abstract Factory
     • Трудно добавлять новые типы создаваемых продуктов или заменять
     существующие, так как интерфейс базового класса абстрактной фабрики
     фиксирован.*/
    map<int, shared_ptr<FactoryBase>> factories;//используем map для хранения фабрик
    template<typename T>
    class CFactory : public FactoryBase//шаблонный класс, конкретная фабрика
    {
        std::function<std::shared_ptr<T>()> functor;//поле хранит объект, у которого есть известный способ создания
    public:
        ~CFactory() {}
        CFactory(std::function<std::shared_ptr<T>()> functor)
            :functor(functor){}//конструктор по умолчанию. соответствует конструктору функтора
        std::shared_ptr<T> GetObject(){return functor();}//функция для получения функтора
    };

    template<typename T>
    std::shared_ptr<T> GetObject()//функция для получения функтора
    {
        auto typeId = GetTypeID<T>();//переменная, определяющая Id выделенного типа
        auto factoryBase = factories[typeId];//добавление в хранилище фабрик
        auto factory = std::static_pointer_cast<CFactory<T>>(factoryBase);//создание копии умного указателя типа Т
        return factory->GetObject();//получение объекта по указателю
    }
//Регистрация экземпляров
  //регистрация функтора
    template<typename TInterface, typename ...TS>
    void RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS> ...ts)> functor)
    {
        factories[GetTypeID<TInterface>()] = std::make_shared<CFactory<TInterface>>([=] {return functor(GetObject<TS>()...); });
    }
  //регистрация экземпляра объекта
    template<typename TInterface>
    void RegisterInstance(std::shared_ptr<TInterface> t)
    {
        factories[GetTypeID<TInterface>()] = std::make_shared<CFactory<TInterface>>([=] {return t; });
    }
  //регистрация функтора с указателем на functor
    template<typename TInterface, typename ...TS>
    void RegisterFunctor(std::shared_ptr<TInterface>(*functor)(std::shared_ptr<TS> ...ts))
    {
        RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS> ...ts)>(functor));
    }
    //Фабрика, вызывает необходимый конструктор для каждого экземпляра
    template<typename TInterface, typename TConcrete, typename ...TArguments>
    void RegisterFactory()
    {
        RegisterFunctor(
            std::function<std::shared_ptr<TInterface>(std::shared_ptr<TArguments> ...ts)>(
                [](std::shared_ptr<TArguments>...arguments) -> std::shared_ptr<TInterface>
        {
            return std::make_shared<TConcrete>(std::forward<std::shared_ptr<TArguments>>(arguments)...);
        }));

    }
    //фабрика, возвращает один экземпляр объекта для одного вызова
    template<typename TInterface, typename TConcrete, typename ...TArguments>
    void RegisterInstance()
    {
        RegisterInstance<TInterface>(std::make_shared<TConcrete>(GetObject<TArguments>()...));
    }

    static IOCContainer& instance()//метод, возвращающий ссылку на созданный контейнер
    {
        static IOCContainer gContainer;
        return gContainer;
    }

};

#endif // IOCCONTAINER_H
