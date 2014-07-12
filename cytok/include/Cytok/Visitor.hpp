//
//  Visitor.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 12/07/2014.
//
//

#ifndef CYTOK_Visitor_hpp
#define CYTOK_Visitor_hpp


#define DEFINE_VISITABLE() \
virtual ReturnType Accept(BaseVisitor& visitor)\
{\
    return ck::BaseVisitable<ReturnType>::Accept_impl(*this, visitor);\
}\


namespace ck
{
    ////////////////////////////////////////////
    ///
    ////////////////////////////////////////////
    
	class BaseVisitor
	{
    public:
		
        virtual ~BaseVisitor(){}
	};
    
    ////////////////////////////////////////////
    ///
    ////////////////////////////////////////////
    
    template
    <
        typename Type,
        typename ReturnType = void
    >
    class Visitor : public virtual BaseVisitor
    {
    public:
        virtual void visit(Type& visitable) = 0;
    };
    
    
    ////////////////////////////////////////////
    ///
    ////////////////////////////////////////////
    
    template<typename R = void>
    class BaseVisitable
    {
    public:
    
        typedef R ReturnType;
        
        virtual ~BaseVisitable(){}
        virtual ReturnType Accept(BaseVisitor& ) = 0;
    
    protected:
        
        template<typename Type>
        static ReturnType Accept_impl(Type& visited, BaseVisitor& visitor)
        {
            if(Visitor<Type,ReturnType>* vp =
               dynamic_cast<Visitor<Type,ReturnType>*>(&visitor))
            {
                return vp->visit(visited);
            }
            
            return ReturnType();
        }
    };
}

#endif
