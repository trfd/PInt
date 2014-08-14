///  
///  CKPair.hpp
///  part of Cytok Library.
///
///  Cytok Library the core and tools library for the CytokEngine.
///  CytokEngine is a research game-engine project.
///  
///  Created by Baptiste Dupy on 12/08/2014.
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

#ifndef CYTOK_Pair_hpp
#define CYTOK_Pair_hpp

namespace ck
{ 
    struct Compare
    {
        enum PairItem
        {
            FIRST,
            SECOND
        };
    };

    struct Equal
    {
        enum PairItem
        {
            FIRST,
            SECOND
        };
    };

    #pragma region Pair
   
    /// <summary>
    /// Pair that wraps equality and strict weak comparison
    /// operators using underlying based on std::pair.
    /// </summary>
    /// This class is used to avoid writing new comparators
    /// or new subclass of std::pair when dealing with specific
    /// pairs: std::<int,ObjectType*> is a class example where
    /// the integer is used to sort out object (comparator) 
    /// but two pairs can be equal even if they have different
    /// integer value (the ObjectType pointers really matter)
    /// Thus the type Pair<int,ObjectType*,Compare::FIRST,Equal::SECOND>
    /// wraps correct equality, difference, and comparators for you.
    template
    <
        typename _T1, 
        typename _T2,
        Compare::PairItem __comp,
        Equal::PairItem __equal
    >
    struct Pair 
    : public std::pair < _T1, _T2 >
    {
        typedef std::pair<_T1, _T2> STDPair;

        typedef std::conditional
        < 
            (__comp == Compare::FIRST),
            typename STDPair::first_type,
            typename STDPair::second_type
        > CompareType;

        typedef std::conditional
        <
            (__equal == Equal::FIRST),
            typename STDPair::first_type,
            typename STDPair::second_type
        > EqualType;

        #pragma region Constructors
        
        template<class _U, class _V>
        Pair(const std::pair<_U, _V>& pr_)
        : STDPair(pr_)
        {}

        template<class _U, class _V> 
        Pair(std::pair<_U, _V>&& pr_)
        : STDPair(std::forward<std::pair<_U, _V>> pr_)
        {}

        Pair(const _T1& t1_, const _T2& t2_)
        : STDPair(t1_, t2_)
        {}

        template<typename _U,typename _V>
        Pair(_U&& u_ , _V&& v_)
        : STDPair(std::forward<_U>(u_), std::forward<_V>(v_))
        {}

        template <class... _Args1, class... _Args2>
        Pair(std::piecewise_construct_t pwc_,
             std::tuple<_Args1...> first_args_,
             std::tuple<_Args2...> second_args_)
        : STDPair(pwc_,first_args_,second_args_)
        {}

        #pragma endregion
        
        template<typename _U>
        friend bool operator==(const Pair& lhs_, _U&& rhs_);

        template<typename _U>
        friend bool operator==(_U&& lhs_, const Pair& rhs_);


        template<typename _U>
        friend bool operator<(const Pair& lhs_, _U&& rhs_);

        template<typename _U>
        friend bool operator<(_U&& lhs_, const Pair& rhs_);

      
        inline bool operator< (const Pair& rhs_)
        {
            return PairCompare<Pair<_T1, _T2, __comp, __equal>, __comp>::value(*this, rhs_);
        }

        
        inline bool operator==(const Pair& rhs_)
        {
            return PairEqual<Pair<_T1, _T2, __comp, __equal>, __equal>::value(*this, rhs_);
        }

    };

    #pragma endregion

    #pragma region Pair Operators

    #pragma region Equality

    template< typename _T1,typename _T2,Compare::PairItem __comp,Equal::PairItem __equal>
    inline bool operator==(const Pair<_T1, _T2, __comp, __equal>& lhs_,
                           const Pair<_T1, _T2, __comp, __equal>& rhs_)
    {
        return PairEqual<Pair<_T1, _T2, __comp, __equal>, __equal>::value(lhs_, rhs_);
    }

    template<typename _T1, typename _T2, Compare::PairItem __comp, Equal::PairItem __equal,typename _U>
    inline bool operator==(const Pair<_T1, _T2, __comp, __equal>& lhs_,
                           _U&& rhs_)
    {
        return PairEqual<Pair<_T1, _T2, __comp, __equal>, __equal>::value(lhs_, std::forward<_U>(rhs_));
    }

    template< typename _T1, typename _T2, Compare::PairItem __comp, Equal::PairItem __equal, typename _U>
    inline bool operator==(_U&& lhs_,
                           const Pair<_T1, _T2, __comp, __equal>& rhs_)
    {
        return PairEqual<Pair<_T1, _T2, __comp, __equal>, __equal>::value(std::forward<_U>(lhs_), rhs_);
    }

    #pragma endregion


    template< typename _T1, typename _T2, Compare::PairItem __comp, Equal::PairItem __equal,typename _U>
    inline bool operator< (const Pair<_T1, _T2, __comp, __equal>& lhs_,
                           const _U&& rhs_)
    {
        return PairCompare<Pair<_T1, _T2, __comp, __equal>, __comp>::value(lhs_, std::forward<_U>(rhs_));
    }

    template< typename _T1, typename _T2, Compare::PairItem __comp, Equal::PairItem __equal, typename _U>
    inline bool operator< (const typename Pair<_T1, _T2, __comp, __equal>::CompareType& lhs_,
                           const Pair<_T1, _T2, __comp, __equal>& rhs_)
    {
        return PairCompare<Pair<_T1, _T2, __comp, __equal>, __comp>::value(std::forward<_U>(lhs_), rhs_);
    }
   
    #pragma endregion

    #pragma region PairEqual

    template<typename _PairType, Equal::PairItem __item>
    struct PairEqual;

    template<typename _PairType>
    struct PairEqual<_PairType, Equal::FIRST>
    {
        template<typename _U,typename _V>
        inline bool operator() (_U&& lhs_, _V&& rhs_) const
        {
            return value(std::forward<_U>(lhs_),
                         std::forward<_V>(rhs_));
        }

        inline static bool value(const _PairType& lhs_,
                                 const _PairType& rhs_)
        {
            return lhs_.first == rhs_.first;
        }

        template<typename _Raw>
        inline static bool value(const _Raw& lhs_,
                                 const _PairType& rhs_)
        {
            return lhs_ == rhs_.first;
        }

        template<typename _Raw>
        inline static bool value(const _PairType& lhs_,
                                 const _Raw& rhs_)
        {
            return lhs_.first == rhs_;
        }
    };

    template<typename _PairType>
    struct PairEqual<_PairType, Equal::SECOND>
    {
        template<typename _U, typename _V>
        inline bool operator() (_U&& lhs_, _V&& rhs_) const
        {
            return value(std::forward<_U>(lhs_),
                         std::forward<_V>(rhs_));
        }

        inline static bool value(const _PairType& lhs_,
                                 const _PairType& rhs_)
        {
            return lhs_.second == rhs_.second;
        }

        template<typename _Raw>
        inline static bool value(const _Raw& lhs_,
                                 const _PairType& rhs_)
        {
            return lhs_ == rhs_.second;
        }

        template<typename _Raw>
        inline static bool value(const _PairType& lhs_,
                                 const _Raw& rhs_)
        {
            return lhs_.second == rhs_;
        }
    };

    #pragma endregion

    #pragma region PairCompare

    template<typename _PairType, Compare::PairItem __item>
    struct PairCompare;

    template<typename _PairType>
    struct PairCompare<_PairType, Compare::FIRST>
    {
        template<typename _U, typename _V>
        inline bool operator() (_U&& lhs_, _V&& rhs_) const
        {
            return value(std::forward<_U>(lhs_),
                         std::forward<_V>(rhs_));
        }

        inline static bool value(const _PairType& lhs_,
                                 const _PairType& rhs_)
        {
            return lhs_.first<rhs_.first;
        }

        template<typename _Raw>
        inline static bool value(const _Raw& lhs_,
                                 const _PairType& rhs_)
        {
            return lhs_<rhs_.first;
        }

        template<typename _Raw>
        inline static bool value(const _PairType& lhs_,
                                 const _Raw& rhs_)
        {
            return lhs_.first<rhs_;
        }
    };

    template<typename _PairType>
    struct PairCompare<_PairType, Compare::SECOND>
    {
        template<typename _U, typename _V>
        inline bool operator() (_U&& lhs_, _V&& rhs_) const
        {
            return value(std::forward<_U>(lhs_),
                         std::forward<_V>(rhs_));
        }

       inline static bool value(const _PairType& lhs_,
                                const _PairType& rhs_)
       {
           return lhs_.second<rhs_.second;
       }

       template<typename _Raw>
       inline static bool value(const _Raw& lhs_,
                                const _PairType& rhs_)
       {
           return lhs_<rhs_.second;
       }

       template<typename _Raw>
       inline static bool value(const _PairType& lhs_,
                                const _Raw& rhs_)
       {
           return lhs_.second<rhs_;
       }
    };

    #pragma endregion

}

#endif //CYTOK_Pair_hpp