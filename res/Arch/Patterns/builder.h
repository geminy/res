#ifndef BUILDER_H
#define BUILDER_H

class Engine;
class Classis;
class Body;
class ElectricalEquipment;

class Car
{
public:
    Car() : m_pEngine(0), m_pClassis(0), m_pBody(0), m_pEquipment(0) {}

    void addEngine(Engine *engine) { m_pEngine = engine; }
    void addClassis(Classis *classis) { m_pClassis = classis; }
    void addBody(Body *body) { m_pBody = body; }
    void addElectricalEquipment(ElectricalEquipment *equipment) { m_pEquipment = equipment; }

private:
    Engine *m_pEngine;
    Classis *m_pClassis;
    Body *m_pBody;
    ElectricalEquipment *m_pEquipment;
};

class Engine {};
class Classis {};
class Body {};
class ElectricalEquipment {};

class CarBuilder
{
public:
    virtual void buildCar() {}
    virtual void buildEngine() {}
    virtual void buildClassis() {}
    virtual void buildBody() {}
    virtual void buildElectricalEquipment() {}

    virtual Car* getCar() { return 0; }

protected:
    CarBuilder() {}
};

class NormalCarBuilder : public CarBuilder
{
public:
    NormalCarBuilder() : m_pCar(0) {}

    void buildCar() { m_pCar = new Car; }
    void buildEngine() { m_pCar ? m_pCar->addEngine(new Engine) : (void)0; }
    void buildClassis() { m_pCar ? m_pCar->addClassis(new Classis) : (void)0; }
    void buildBody() { m_pCar ? m_pCar->addBody(new Body) : (void)0; }
    void buildElectricalEquipment() { m_pCar ? m_pCar->addElectricalEquipment(new ElectricalEquipment) : (void)0; }

    Car* getCar() { return m_pCar; }

private:
    Car* m_pCar;
};

class CarClient
{
public:
    Car* createCar(CarBuilder *builder)
    {
        if (builder) {
            builder->buildCar();
            builder->buildEngine();
            builder->buildClassis();
            builder->buildBody();
            builder->buildElectricalEquipment();

            return builder->getCar();
        }

        return 0;
    }
};

#endif // BUILDER_H
