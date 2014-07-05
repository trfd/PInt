//
//  IDException.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 05/07/2014.
//
//

#ifndef CYTOK_IDException_hpp
#define CYTOK_IDException_hpp

namespace ck
{
    class InvalidIDException: public std::exception
    {
        virtual const char* what() const throw()
        {
            return "Invalid ID was generated or found";
        }
    };

}

#endif
