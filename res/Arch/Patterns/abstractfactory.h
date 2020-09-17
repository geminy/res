#ifndef ABSTRACTFACTORY_H
#define ABSTRACTFACTORY_H

#include <string>

/*
 * @class Glass
 */
class Glass
{
public:
    Glass();
    virtual std::string glassType() const;
};

/*
 * @class IPhoneGlass
 */
class IPhoneGlass : public Glass
{
public:
    IPhoneGlass();
    std::string glassType() const;
};

/*
 * @class NexusGlass
 */
class NexusGlass : public Glass
{
public:
    NexusGlass();
    std::string glassType() const;
};

/*
 * @class Camera
 */
class Camera
{
public:
    Camera();
    virtual std::string cameraType() const;
};

/*
 * @class IPhoneCamera
 */
class IPhoneCamera : public Camera
{
public:
    IPhoneCamera();
    std::string cameraType() const;
};

/*
 * @class NexusCamera
 */
class NexusCamera : public Camera
{
public:
    NexusCamera();
    std::string cameraType() const;
};

/*
 * @class Phone
 */
class Phone
{
public:
    Phone();
    void addGlass(Glass *glass);
    Glass* getGlass();
    void addCamera(Camera *camera);
    Camera* getCamera();

private:
    Glass *m_glass;
    Camera *m_camera;
};

/*
 * @class PhoneFactory
 */
class PhoneFactory
{
public:
    PhoneFactory();
    virtual Phone* createPhone();
    virtual Glass* createGlass();
    virtual Camera* createCamera();
};

/*
 * @class IPhoneFactory
 */
class IPhoneFactory : public PhoneFactory
{
public:
    IPhoneFactory();
    Phone* createPhone();
    Glass* createGlass();
    Camera* createCamera();
};

/*
 * @class NexusFactory
 */
class NexusFactory : public PhoneFactory
{
public:
    NexusFactory();
    Phone* createPhone();
    Glass* createGlass();
    Camera* createCamera();
};

/*
 * @class Client
 */
class Client
{
public:
    Client();
    Phone* makePhone(PhoneFactory *phone);
};

#endif // ABSTRACTFACTORY_H
