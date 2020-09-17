#ifndef SINGLETON_H
#define SINGLETON_H

#include <string>
#include <map>

#define ENV_SINGLETON "SINGLETON"

/*
 * @class Singleton
 */
class Singleton
{
public:
    static Singleton* Instance();

private:
    Singleton();
    Singleton(const Singleton&);
    Singleton& operator =(const Singleton&);

private:
    static Singleton *s_self;
};

/*
 * @class SingletonBase
 */
class SingletonBase
{
public:
    static SingletonBase* Instance();

protected:
    SingletonBase();

private:
    static SingletonBase *s_self;
};

/*
 * @class SingletonA
 */
class SingletonA : public SingletonBase
{
private:
    friend class SingletonBase;

    SingletonA();
};

/*
 * @class SingletonRegistry
 */
class SingletonRegistry
{
public:
    static SingletonRegistry* Instance();

protected:
    static void Register(const std::string &name, SingletonRegistry *singleton);

    SingletonRegistry();

private:
    static SingletonRegistry* Lookup(const std::string &name);

private:
    static SingletonRegistry* s_self;
    static std::map<std::string, SingletonRegistry*> s_registry;
};

/*
 * @class SingletonB
 */
class SingletonB : public SingletonRegistry
{
public:
    SingletonB();

private:
    static SingletonB s_self;
};

#endif // SINGLETON_H
