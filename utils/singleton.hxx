#ifndef SINGLETON_H
#define SINGLETON_H

template<class Initializer>
class Singleton
{
public:
    typedef decltype(Initializer()()) ValueType;

    Singleton():
        value(Initializer()())
    {
        assert(!singleton);
        singleton = this;
    }

    static const ValueType& get()
    {
        if (!singleton)
            Singleton<Initializer>();
        return singleton->value;
    }

private:
    static Singleton<Initializer>* singleton;

    const ValueType value;
};

template<class Initializer>
Singleton<Initializer>*
Singleton<Initializer>::singleton = nullptr;

#endif

