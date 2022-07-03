#ifndef IOCCONTAINER_H
#define IOCCONTAINER_H

#include <iostream>
#include <functional>
#include <cstdlib>
#include <memory>
#include <map>

using namespace std;


class IOCContainer

{
    static int s_typeId;
public:

    template<typename T>
    static int GetTypeID(){
        static int typeId = s_typeId++;
        return typeId;
    }
    class FactoryBase//фабрика
    {
    public:
        virtual ~FactoryBase() {}
    };

    map<int, shared_ptr<FactoryBase>> factories;

    template<typename T>
    class CFactory : public FactoryBase
    {
        std::function<std::shared_ptr<T>()> functor;
    public:
        ~CFactory() {}
        CFactory(std::function<std::shared_ptr<T>()> functor)
            :functor(functor){}

        std::shared_ptr<T> GetObject(){
            return functor();}
    };

    template<typename T>
    std::shared_ptr<T> GetObject()
    {
        auto typeId = GetTypeID<T>();
        auto factoryBase = factories[typeId];
        auto factory = std::static_pointer_cast<CFactory<T>>(factoryBase);
        return factory->GetObject();
    }
//экземпляры
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

    //с указателем на functor
    template<typename TInterface, typename ...TS>
    void RegisterFunctor(std::shared_ptr<TInterface>(*functor)(std::shared_ptr<TS> ...ts))
    {
        RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS> ...ts)>(functor));
    }


    //Фабрика, которая будет вызывать необходимый конструктор для каждого экземпляра
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
    //фабрика, возвращает один экземпляр ообъекта для одного вызова
    template<typename TInterface, typename TConcrete, typename ...TArguments>
    void RegisterInstance()
    {
        RegisterInstance<TInterface>(std::make_shared<TConcrete>(GetObject<TArguments>()...));
    }

public:
    static IOCContainer& instance()
    {
        static IOCContainer gContainer;
        return gContainer;
    }

};

#endif // IOCCONTAINER_H
