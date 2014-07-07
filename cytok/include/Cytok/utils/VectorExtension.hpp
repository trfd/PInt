//
//  Vector.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 06/07/2014.
//
//

#ifndef CYTOK_Vector_hpp
#define CYTOK_Vector_hpp

#include <vector>

namespace ck
{
    namespace utils
    {
        /*
         int find(Type obj);
         void add(Type obj);
         void insert(Type obj , int atIdx);
         void remove(Type obj);
         void remove(int atIdx);
         void change(int atIdx, Type obj);
         void change(Type oldObj , Type newObj);
         void swap(int idx1, int idx2);
         void swap(Type obj1, Type obj2);
         */
        
        /// Wrap convenient usage of std::vector
        class VectorExtension
        {
    
        public:
            
            template<typename Type>
            static int find(std::vector<Type>& vect , Type obj);

            template<typename Type>
            static void add(std::vector<Type>& vect , Type obj);
            
            template<typename Type>
            static void insert(std::vector<Type>& vect , Type obj , int atIdx);
            
            template<typename Type>
            static void remove(std::vector<Type>& vect , Type obj);
            
            template<typename Type>
            static void remove(std::vector<Type>& vect , int atIdx);
            
            template<typename Type>
            static void change(std::vector<Type>& vect,int atIdx, Type obj);
            
            template<typename Type>
            static void change(std::vector<Type>& vect,Type oldObj , Type newObj);
            
            template<typename Type>
            static void swap(std::vector<Type>& vect,int idx1, int idx2);
            
            template<typename Type>
            static void swap(std::vector<Type>& vect,Type obj1, Type obj2);
            
        };
    }
}

#include "Cytok/utils/VectorExtension_impl.hpp"

#endif
