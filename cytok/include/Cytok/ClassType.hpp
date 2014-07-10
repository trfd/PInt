//
//  ClassType.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 10/07/2014.
//
//

#ifndef CYTOK_ClassType_hpp
#define CYTOK_ClassType_hpp

#include "Hash.hpp"
#include "utils/MacrosUtils.hpp"

#define DEFINE_CLASS(_class_) \
static ck::ClassType type;\
virtual ck::ClassType* classType() \
{ return &_class_::type; }

#define REGISTER_ROOT_CLASS(_class_) \
ck::ClassType _class_::type = ck::ClassType(#_class_)

#define REGISTER_CLASS(_class_,_parent_) \
ck::ClassType _class_::type = ck::ClassType(#_class_ ,&_parent_::type)

namespace ck
{
	class ClassType 
	{
    public:
        
		ClassType(const std::string& className , ClassType* rootClass = NULL)
        :  myRootClass(rootClass) ,myName(className)
        {
            if(myRootClass)
                myFullname = myRootClass->myFullname+"-"+myName;
            else
                myFullname = myName;
         
            myID = utils::hash<32>::get(myFullname);
        }
        
        ClassType* root()
        {
            return myRootClass;
        }
        
        std::string& fullname()
        {
            return myFullname;
        }
        
        std::string& name()
        {
            return myName;
        }
        
        uint32_t id()
        {
            return myID;
        }
        
    private:
        
        ClassType* myRootClass;
        
        uint32_t myID = UINT32_MAX;
        
        std::string myFullname;
        std::string myName;
	};
}

#endif
