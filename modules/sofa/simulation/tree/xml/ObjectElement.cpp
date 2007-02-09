#include <sofa/simulation/tree/xml/ObjectElement.h>
#include <sofa/simulation/tree/xml/Element.inl>
#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace simulation
{

namespace tree
{

namespace xml
{

using namespace sofa::defaulttype;
using helper::Creator;

//template class Factory< std::string, objectmodel::BaseObject, Node<objectmodel::BaseObject*>* >;

ObjectElement::ObjectElement(const std::string& name, const std::string& type, BaseElement* parent)
    : Element<core::objectmodel::BaseObject>(name, type, parent)
{
}

ObjectElement::~ObjectElement()
{
}

bool ObjectElement::initNode()
{
    //if (!Element<core::objectmodel::BaseObject>::initNode()) return false;
    core::objectmodel::BaseContext* ctx = dynamic_cast<core::objectmodel::BaseContext*>(getParent()->getBaseObject());

    std::cout << "ObjectElement: creating "<<getAttribute( "type", "" )<<std::endl;

    core::objectmodel::BaseObject *obj = core::ObjectFactory::CreateObject(ctx, this);

    if (obj == NULL)
        obj = Factory::CreateObject(this->getType(), this);
    if (obj == NULL)
        return false;
    setObject(obj);
    obj->setName(getName());

    return true;
}

SOFA_DECL_CLASS(Object)

Creator<BaseElement::NodeFactory, ObjectElement> ObjectNodeClass("Object");

const char* ObjectElement::getClass() const
{
    return ObjectNodeClass.c_str();
}

} // namespace xml

} // namespace tree

} // namespace simulation

} // namespace sofa

