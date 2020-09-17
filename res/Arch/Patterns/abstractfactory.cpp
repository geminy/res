#include "abstractfactory.h"

#include <iostream>

/* Glass */
Glass::Glass()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

std::string Glass::glassType() const
{
    return std::string("Unknown Glass");
}

/* IPhoneGlass */
IPhoneGlass::IPhoneGlass()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

std::string IPhoneGlass::glassType() const
{
    return std::string("IPhone Glass");
}

/* NexusGlass */
NexusGlass::NexusGlass()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

std::string NexusGlass::glassType() const
{
    return std::string("Nexus Glass");
}

/* Camera */
Camera::Camera()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

std::string Camera::cameraType() const
{
    return std::string("Unknown Camera");
}

/* IPhoneCamera */
IPhoneCamera::IPhoneCamera()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

std::string IPhoneCamera::cameraType() const
{
    return std::string("IPhone Camera");
}

/* NexusCamera */
NexusCamera::NexusCamera()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

std::string NexusCamera::cameraType() const
{
    return std::string("Nexus Camera");
}

/* Phone */
Phone::Phone()
    : m_glass(0)
    , m_camera(0)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void Phone::addGlass(Glass *glass)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    m_glass = glass;
}

Glass* Phone::getGlass()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return m_glass;
}

void Phone::addCamera(Camera *camera)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    m_camera = camera;
}

Camera* Phone::getCamera()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return m_camera;
}

/* PhoneFactory */
PhoneFactory::PhoneFactory()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

Phone* PhoneFactory::createPhone()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return 0;
}

Glass* PhoneFactory::createGlass()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return 0;
}

Camera* PhoneFactory::createCamera()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return 0;
}

/* IPhoneFactory */
IPhoneFactory::IPhoneFactory()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

Phone* IPhoneFactory::createPhone()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return new Phone;
}

Glass* IPhoneFactory::createGlass()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return new IPhoneGlass;
}

Camera* IPhoneFactory::createCamera()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return new IPhoneCamera;
}

/* NexusFactory */
NexusFactory::NexusFactory()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

Phone* NexusFactory::createPhone()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return new Phone;
}

Glass* NexusFactory::createGlass()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return new NexusGlass;
}

Camera* NexusFactory::createCamera()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return new NexusCamera;
}

/* Client */
Client::Client()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

Phone* Client::makePhone(PhoneFactory *phone)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    if (phone) {
        Phone *myPhone = phone->createPhone();
        Glass *glass = phone->createGlass();
        Camera *camera = phone->createCamera();
        myPhone->addGlass(glass);
        myPhone->addCamera(camera);
        std::cout << myPhone->getGlass()->glassType() << std::endl;
        std::cout << myPhone->getCamera()->cameraType() << std::endl;

        return myPhone;
    }
    return 0;
}
