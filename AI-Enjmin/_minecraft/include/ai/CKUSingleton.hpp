///  
///  Singleton.hpp
///  part of Cytok Library.
///
///  Cytok Library the core and tools library for the CytokEngine.
///  CytokEngine is a research game-engine project.
///  
///  Created by Baptiste Dupy on 04/07/2014.
///  Contact:
///          Mail:       <baptiste.dupy@gmail.com>
///          GitHub:     trfd <https://github.com/trfd>
///          Website:    <http://www.trf-d.com>
///
///  Copyright (C) 2014 Baptiste (trfd) Dupy
///
///  This program is free software: you can redistribute it and/or modify
///  it under the terms of the GNU General Public License as published by
///  the Free Software Foundation, either version 3 of the License, or
///  (at your option) any later version.
///
///  This program is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///  GNU General Public License for more details.
///
///  You should have received a copy of the GNU General Public License
///  along with this program.  If not, see <http://www.gnu.org/licenses/>.
/// 

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
        /// class m_singleton : public ck::utils::Singleton<m_singleton>
        /// {
        ///     ...
        /// }
        /// \endcode
        template <class _T>
        class Singleton
        {
        public:
            
            inline static _T* instance()
            {
                if (!m_instance)
                {
                    m_instance = new _T();
                    m_instance->init();
                }
                
                return m_instance;
            }
            
            static
            void destroy_instance()
            {
                delete m_instance;
                m_instance = nullptr;
            }

            virtual void init(){}
            
        private:
            static _T* m_instance;
        };
        
        template<class _T>
        _T* Singleton<_T>::m_instance = nullptr;
    }
}

#endif
