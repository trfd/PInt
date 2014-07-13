//
//  Delegate.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 13/07/2014.
//
//

#ifndef CYTOK_Delegate_hpp
#define CYTOK_Delegate_hpp

namespace ck
{
    /// C# delegate
    template
    <
        typename... Args
    >
	class Delegate
	{
    public:
        
        typedef Functor<void,Args...> FunctorType;
        typedef std::vector<FunctorType*> FunctorList;
        typedef std::vector<FunctorType*>::iterator FunctorList_it;
        
        Delegate& operator +=(FunctorType* b)
        {
            this->myFunctors.push_back(b);
            
            return *this;
        }
    
        Delegate& operator -=(FunctorType* b)
        {
            FunctorList_it it = std::find(myFunctors.begin(),
                                          myFunctors.end(),
                                          b);
            
            if(it != myFunctors.end())
                this->myFunctors.erase(it);
            
            return *this;
        }
        
        void operator()(Args&&... args)
        {
            for(FunctorList_it it = myFunctors.begin() ;
                it != myFunctors.end() ; ++it)
            {
                (*it)(std::forward<Args>(args)...);
            }
        }
        
    private:
        
        FunctorList myFunctors;
		
	};
}

#endif
