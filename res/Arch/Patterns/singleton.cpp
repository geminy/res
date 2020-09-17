#include "singleton.h"

#include <iostream>
#include <cstring>

/* Singleton - lazy*/
Singleton* Singleton::s_self = 0;

Singleton* Singleton::Instance()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    if (0 == s_self) {
        s_self = new Singleton;
    }
    return s_self;
}

Singleton::Singleton()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

/* SingletonBase */
SingletonBase* SingletonBase::s_self = 0;

/* uses env var */
SingletonBase* SingletonBase::Instance()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    if (0 == s_self) {
        const char *singletonName = getenv(ENV_SINGLETON);
        if (0 == singletonName) {
            s_self = new SingletonBase;
        }
        else {
            if (strcmp(singletonName, "SingletonA") == 0) {
                s_self = new SingletonA;
            }
            else {
                s_self = new SingletonBase;
            }
        }
    }
    return s_self;
}

SingletonBase::SingletonBase()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

/* SingletonA */
SingletonA::SingletonA()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

/* SingletonRegistry */
SingletonRegistry* SingletonRegistry::s_self = 0;
std::map<std::string, SingletonRegistry*> SingletonRegistry::s_registry;

SingletonRegistry* SingletonRegistry::Instance()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    if (0 == s_self) {
        const char *singletonName = getenv(ENV_SINGLETON);
        if (0 != singletonName) {
            s_self = Lookup(std::string(singletonName));
        }
    }
    return s_self;
}

void SingletonRegistry::Register(const std::string &name, SingletonRegistry *singleton)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    if (s_registry.find(name) != s_registry.end()) {
        std::cout << name << " has been instantiated!" << std::endl;
        abort();
    }
    s_registry[name] = singleton;
}

SingletonRegistry::SingletonRegistry()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

SingletonRegistry* SingletonRegistry::Lookup(const std::string &name)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    std::map<std::string, SingletonRegistry*>::iterator it = s_registry.find(name);
    if (s_registry.end() != it) {
        return it->second;
    }
    return 0;
}

/* SingletonB - hungry */
SingletonB SingletonB::s_self;

SingletonB::SingletonB()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    SingletonRegistry::Register(std::string("SingletonB"), this);
}
