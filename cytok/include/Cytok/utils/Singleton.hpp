//
//  Singleton.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 04/07/2014.
//
//

#ifndef CYTOK_Singleton_hpp
#define CYTOK_Singleton_hpp

#include <utility>

namespace ck
{
    namespace utils
    {
        ///
        /// \class Generic Singleton
        /// \brief
        /// Should be use as follow:
        /// \code
        /// class MySingleton : public ck::utils::Singleton<MySingleton>
        /// {
        ///     ...
        /// }
        /// \endcode
        template <class Type>
        class Singleton
        {
        public:
            
            template<typename... Args>
            static Type* instance(Args... args)
            {
                if (!myInstance)
                {
                    myInstance = new Type(std::forward<Args>(args)...);
                }
                
                return myInstance;
            }
            
            static
            void destroy_instance()
            {
                delete myInstance;
                myInstance = nullptr;
            }
            
        private:
            static Type* myInstance;
        };
        
        template<class Type>
        Type* Singleton<Type>::myInstance = nullptr;
    }
}

#endif
