//
//  VectorExtension_impl.hpp
//  CYTOK
//
//  Created by Baptiste Dupy on 06/07/2014.
//
//

#ifndef CYTOK_VectorExtension_impl_hpp
#define CYTOK_VectorExtension_impl_hpp

namespace ck
{
	namespace utils
    {
        template<typename Type>
        int VectorExtension::find(std::vector<Type>& vect , Type obj)
        {
            return std::distance( vect.begin(), std::find( vect.begin(), vect.end(), obj ) );
        }
        
        template<typename Type>
        void VectorExtension::add(std::vector<Type>& vect , Type obj)
        {
            vect.push_back(obj);
        }
        
        template<typename Type>
        void VectorExtension::insert(std::vector<Type>& vect , Type obj , int atIdx)
        {
            if(atIdx > vect.size() || atIdx < 0)
                return;
            
            vect.insert(vect.begin()+atIdx , obj);
        }
        
        template<typename Type>
        void VectorExtension::remove(std::vector<Type>& vect , Type obj)
        {
            int idx = find(vect,obj);
            
            if(idx >= vect.size() || idx < 0)
                return;
            
            remove(vect,idx);
        }
        
        template<typename Type>
        void VectorExtension::remove(std::vector<Type>& vect , int atIdx)
        {
            if(atIdx >= vect.size() || atIdx < 0)
                return;
            
            vect.erase(vect.begin()+atIdx);
        }
        
        template<typename Type>
        void VectorExtension::change(std::vector<Type>& vect,int atIdx, Type obj)
        {
            if(atIdx >= vect.size() || atIdx < 0)
                return;
            
            vect[atIdx] = obj;
        }
        
        template<typename Type>
        void VectorExtension::change(std::vector<Type>& vect,Type oldObj , Type newObj)
        {
            int idx = find(vect,oldObj);
            
            if(idx >= vect.size() || idx < 0)
                return;
            
            change(vect,idx,newObj);
        }
        
        template<typename Type>
        void VectorExtension::swap(std::vector<Type>& vect,int idx1, int idx2)
        {
            if(idx1 >= vect.size() || idx1 < 0 ||
               idx2 >= vect.size() || idx2 < 0)
                return;
            
            
            Type tmp = vect[idx1];
            vect[idx1] = vect[idx2];
            vect[idx2] = tmp;
        }
        
        template<typename Type>
        void VectorExtension::swap(std::vector<Type>& vect,Type obj1, Type obj2)
        {
            int idx1 = find(vect,obj1);
            
            if(idx1 >= vect.size() || idx1 < 0)
                return;
            
            int idx2 = find(vect,obj2);
            
            if(idx2 >= vect.size() || idx2 < 0)
                return;
            
            swap(vect,idx1,idx2);
        }
    }
}

#endif
