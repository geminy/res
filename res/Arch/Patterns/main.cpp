#include "singleton.h"
#include "abstractfactory.h"
#include "builder.h"
#include "factorymethod.h"
#include "prototype.h"

#include <iostream>

using namespace std;

void singleton_test()
{
    cout << __func__ << endl;

    Singleton::Instance();
    Singleton::Instance();

    putenv("SINGLETON=SingletonA");
    SingletonBase::Instance();

    putenv("SINGLETON=SingletonB");
    SingletonRegistry::Instance();
}

void abstract_factory_test()
{
    cout << __func__ << endl;

    IPhoneFactory iphone;
    Client client;
    client.makePhone(&iphone);

    NexusFactory nexus;
    Client client2;
    client2.makePhone(&nexus);
}

void factory_method_test()
{
    MyApplication<MyDocument> app;
    app.newDocument();
    app.docType();
}

void prototype_test()
{
    TrafficClient tc(new Plane);
    tc.makeTraffic();
}

void builder_test()
{
    cout << __func__ << endl;

    CarClient client;
    NormalCarBuilder builder;
    client.createCar(&builder);
}

int main(int argc, char *argv[])
{
    singleton_test();
    abstract_factory_test();
    builder_test();
    factory_method_test();
    prototype_test();

    return 0;
}
