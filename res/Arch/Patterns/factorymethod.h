#ifndef FACTORYMETHOD_H
#define FACTORYMETHOD_H

#include <string>

class Document
{
public:
    virtual std::string type() = 0;
};

class MyDocument : public Document
{
public:
    std::string type() { return std::string("MyDocument"); }
};

class Applicaion
{
public:
    Applicaion() : m_pDoc(0) {}
    ~Applicaion()
    {
        if (m_pDoc) {
            delete m_pDoc;
            m_pDoc = 0;
        }
    }

    Document* newDocument()
    {
        if (!m_pDoc) {
            m_pDoc = createDocument();
        }
        return m_pDoc;
    }
    std::string docType() { return m_pDoc? m_pDoc->type() : std::string("Unknown"); }

protected:
    virtual Document* createDocument() = 0;

private:
    Document *m_pDoc;
};

template<class TheProduct>
class MyApplication : public Applicaion
{
protected:
    Document* createDocument() { return new TheProduct; }
};

#endif // FACTORYMETHOD_H
