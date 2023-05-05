/* AUtils 18/01/2021

 $$$$$$$$$$$$$$$
 $   Utils.h   $
 $$$$$$$$$$$$$$$

 by W.B. Yates
 Copyright (c) W.B. Yates. All rights reserved.
 History:
 
 Template utilities for streaming std::pair, std::map and std::vector, and 
 converting between std::map and std::vector

*/


#ifndef __UTILS_H__
#define __UTILS_H__



#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>


//
// stream  ooperators for std::vector, std::map, and std::pair
//

template <class T, class U>
inline std::ostream&
operator<<(std::ostream& ostr, const std::pair<T,U>& v)
{
    ostr << v.first << ' ' << v.second << ' ';
    return ostr;
}

template <class T, class U>
inline std::istream&
operator>>(std::istream& istr, std::pair<T,U>&v)
{
    istr >> v.first >> v.second;
    return istr;
}


template <typename T>
std::ostream&
operator<<( std::ostream& ostr, const std::vector<T>& v )
{
    ostr << v.size() << '\n';
    int count = 1;
    
    for (typename std::vector<T>::const_iterator  i = v.begin(); i != v.end(); ++i, ++count)
    {
        ostr << *i << ' ';
        if ( (count % 20) == 0 )
            ostr << '\n';
    }

    return ostr;
}

template <typename T>
std::istream& 
operator>>( std::istream& istr, std::vector<T>& v )
{
    int len = 0;    
    istr >> len;
    v.resize(len);

    for (typename std::vector<T>::iterator  i = v.begin(); i != v.end(); ++i)
        istr >> *i;

    return istr;
}



template <typename K, typename T>
std::ostream& 
operator<<(std::ostream& ostr, const std::map<K,T>& m)
{
    ostr << m.size() << '\n';

    for (typename std::map<K,T>::const_iterator idx = m.begin(); idx != m.end(); ++idx)
    {
        ostr << idx->first << ' ' << idx->second << '\n';
    }
    
    return ostr;
}

template <typename K, typename T>
std::istream&
operator>>(std::istream& istr, std::map<K,T>& m)
{
    m.clear();

    K key = K();
    T item = T();
    
    int len = 0;
    istr >> len;
    
    auto lastPos = m.begin();
    for (int i = 0; i < len; ++i)
    {
        istr >> key >> item;
        lastPos = m.insert( lastPos, typename std::map<K,T>::value_type( key, item ));
    }
    return istr;
}




//
// useful templates
//


template <typename K, typename T> 
std::vector<K> 
inline toKeyVector(const std::map<K,T>& m)
// return a sorted vector of the unique keys in map
{
    typename std::vector<K> v(m.size());
    std::transform( m.begin(), m.end(), v.begin(), [](const std::pair<K,T>& c) -> K { return c.first; } );
    return v;
}

template <typename K, typename T> 
inline std::vector<T> 
toValueVector(const std::map<K,T>& m)
// return a sorted vector of the values in map
{
    typename std::vector<T> v(m.size());
    std::transform( m.begin(), m.end(), v.begin(), [](const std::pair<K,T>& c) -> T { return c.second; } );
    return v;
}

//

template <typename K, typename T = int> 
std::map<K,T> 
toKeyMap(const std::vector<K>& v)
// use the vector index as a value
{
    typename std::map<K,T> m;
    
    T c = T();
    auto lastPos = m.begin();
    
    for (auto i = v.begin(); i != v.end(); ++i, ++c)
    {
        // this works more effieciently when the vector is sorted least value first 
        lastPos = m.insert( lastPos, typename std::map<K,T>::value_type(*i, c) );    
    } 
    return m;
}

template <typename K, typename T = int> 
std::map<K,T> 
toValueMap(const std::vector<T>& v)
// use the vector index as a key
{
    typename std::map<K,T> m;
    
    K c = K();
    auto lastPos = m.begin();
    
    for (auto i = v.begin(); i != v.end(); ++i, ++c)
    {
        // this works more effieciently when the vector is sorted least value first 
        lastPos = m.insert( lastPos, typename std::map<K,T>::value_type(c, *i) );    
    } 
    return m;
}

#endif




