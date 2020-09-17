#ifndef PROTOTYPE_H
#define PROTOTYPE_H

class Traffic
{
public:
    virtual Traffic* clone() const = 0;
    virtual void initialize() {}
};

class Plane : public Traffic
{
public:
    Plane() {}
    Plane(const Plane&) {}
    Traffic* clone() const { return new Plane(*this); }
};

class TrafficClient
{
public:
    TrafficClient(Traffic *traffic) : m_pPrototypeTraffic(traffic) {}
    Traffic* makeTraffic() const
    {
        if (m_pPrototypeTraffic) {
            return m_pPrototypeTraffic->clone();
        }
        return 0;
    }

private:
    Traffic *m_pPrototypeTraffic;
};

#endif // PROTOTYPE_H
