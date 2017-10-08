/*
Copyright (c) 2013-2017 Jinrong Xie (jrxie at ucdavis dot edu)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _SVECTOR_TEMPLATE_H
#define _SVECTOR_TEMPLATE_H

#pragma once

#ifdef USE_MPI
#include <mpi.h>
#ifdef _PROFILING_
#include "hpgv_utiltiming.h"
#include "parallel_test.h"
#endif
#endif

#include "mathtool.h"
#include "DError.h"
#include <iostream>
#include <assert.h>
#include <cstring>

namespace davinci{

template <typename T>
inline void swapxor(T &left, T &right)
{
    if (&left != &right){
        (int&)left  ^= (int&)right;
        (int&)right ^= (int&)left;
        (int&)left  ^= (int&)right;
    }

//     T tmp = left;
//     left = right;
//     right = tmp;
}

template<typename T>
class svector
{
private:
    T *m_data;
    size_t m_count;
    size_t m_size;
public:
    typedef T elm_type;
    svector(void);
    svector(size_t count);
    svector(size_t count,const T& val);
    ~svector();
    T* data_ptr(void);
    T* data_ptr(void) const;
    //const T* data_ptr(void);
    size_t size() const;
    size_t capacity() const;
    void resize(size_t newSize, bool is_profiling=false );
    void resize(size_t newSize,const T& val);
    void reserve(size_t count, bool is_profiling=false );
    void push_back(const T& val, bool is_profiling=false);
    void push_back(const svector<T>& vec, bool is_profiling=false);
    void remove( const T& val ); 
    void remove(const T& val, int (*comp)(const void *, const void *));
    void remove_all( const T& val ); 
    void remove_all(const T& val, int (*comp)(const void *, const void *));

    void clone(const svector<T>& vec, bool is_profiling=false);
    T&   back();
    T    back() const;
    void fill(const T& val);
    void clear();
    bool empty() const { return m_count==0;}
    bool contain(const T& val) const;
    void shrink_to_fit(bool is_profiling=false );
    void swap(svector<T>& right);
    void remove_duplicates(int (*comp)(const void *, const void *));
    void substract(const svector<T>& vec1, const svector<T>& vec2, int (*comp)(const void*, const void*) );//remove the elements appear in vec from *this

#ifndef MPI_INCLUDED
#define MPI_COMM_WORLD ((int)0x44000000)
    //************************************
    // Method:    mpi_allreduce
    // Description: Reduce the svector<T> from all the PEs that belongs to the communicator.
    // The svector<T> which invoke the method is both send and receive buffer.
    // Parameter: int root: Specify the PE id, which will obtain the reduced results.
    // Parameter: int mpi_datatype
    // Parameter: int communicator
    //************************************
    void mpi_reduce(int root, int mpi_datatype, int op, int communicator = MPI_COMM_WORLD);
    //************************************
    // Method:    mpi_reduce
    // Description: Reduce the svector<T> from all the PEs that belongs to the communicator.
    // The svector<T> which invoke the method is the receive buffer. Make sure the it is initialized
    // with the size at least a big as the one of send_vector, and initialized with appropriated initial value.
    // Parameter: int root
    // Parameter: svector<T> & send_vector
    // Parameter: int mpi_datatype
    // Parameter: int op
    // Parameter: int communicator
    //************************************
    void mpi_reduce(int root, svector<T>& send_vector, int mpi_datatype, int op, int communicator = MPI_COMM_WORLD);
    //************************************
    // Method:    mpi_allgatherv
    // Description: Gather svector<T> with different length from all PE within the communicator.
    // Parameter: const svector<T> & input_array
    // Parameter: svector<int> & count_array
    // Parameter: svector<int> & displace_array
    // Parameter: int mpi_datatype
    // Parameter: int communicator
    //************************************
    void mpi_allgatherv(const svector<T>& input_array, svector<int>& count_array,
                       svector<int>& displace_array, int mpi_datatype,
                       int communicator=MPI_COMM_WORLD);
#endif

    T&	operator [](size_t i);
    T	operator [](size_t i) const;
    template<typename T2>
    friend std::ostream& operator << ( std::ostream& os, const svector<T2> &t);
    template<typename T2>
    friend std::istream& operator >> ( std::istream& in, const svector<T2> &t);
    template<typename T2>
    friend std::ostream& operator << ( std::ostream& os, svector<T2> &t);
    template<typename T2>
    friend std::istream& operator >> ( std::istream& in, svector<T2> &t);
};

#ifdef USE_MPI
template<typename T>
void svector<T>::substract(const svector<T>& vec1, const svector<T>& vec2, int (*comp)(const void*, const void*) )
{
    int i=0, j=0;
    int size1 = vec1.size();
    int size2 = vec2.size();
    qsort(vec1.data_ptr(), size1, sizeof(T), comp);
    qsort(vec2.data_ptr(), size2, sizeof(T), comp);

    while ( i < size1 && j < size2){
        if (vec1[i] < vec2[j]) {
            push_back(vec1[i]);
            i++;
        }
        else if ( vec1[i] > vec2[j] ){
            j++;
        }else if ( vec1[i] == vec2[j]) {
            i++; j++;
        }
    }
    while (i < size1){
        push_back(vec1[i++]);
    }
}
#else
template<typename T>
void svector<T>::substract(const svector<T>& vec1, const svector<T>& vec2, int (*comp)(const void*, const void*) )
{
    D_ASSERT(0, "Method:svector<T>::mpi_allgatherv not implemented!\n",DERROR_ERR_MEM);
}
#endif

template<typename T>
void svector<T>::clone( const svector<T>& vec, bool is_profiling/*=false*/ )
{
    if (m_data){
        free(m_data);
        m_data = NULL;
    }
    if (!vec.data_ptr()){//vec is NULL
        m_count = 0;
        m_size = 0;
        return;
    }
    m_count = vec.size();
    m_size = m_count;
    m_data = (T*)malloc(m_size * sizeof(T));
    if (!m_data){
        char msg[256];
        sprintf(msg, "svector:clone: Out of memory: asking for %fMB.\n",float(m_count*sizeof(T))/1024.0f/1024.0f);
        D_ASSERT(m_data != 0, msg, DERROR_ERR_MEM);
    }
    memcpy(m_data, vec.data_ptr(), sizeof(T)*m_size);
}

#ifdef USE_MPI
template<typename T>
void svector<T>::mpi_allgatherv( const svector<T>& input_array, svector<int>& count_array,
                                svector<int>& displace_array, int mpi_datatype,
                                int communicator/*=MPI_COMM_WORLD*/ )
{
    //svector<int> count_array;//send size array
    //svector<int> displace_array;//displacement array.
    //svector<int> send_gather;//array for all gathered leaf node indices.
    int  local_count = input_array.size();
    int groupsize = 0, myid = -1;
    MPI_Comm_rank(communicator, &myid);
    MPI_Comm_size(communicator, &groupsize);

    count_array.resize(groupsize, 0);
    //1. gather how many ints each processor is going to send in step 2.
    MPI_Allgather(&local_count, 1, mpi_datatype, count_array.data_ptr(), 1, mpi_datatype, communicator);

    displace_array.resize(groupsize, 0);
    displace_array[0] = 0;
    for (int iPE = 1; iPE < groupsize ; iPE++){
        displace_array[iPE] = displace_array[iPE-1] + count_array[iPE-1];
    }
    this->resize(displace_array[groupsize-1]+count_array[groupsize-1], -1);//
    //2.Gather leaf node assignment from all processor, so that every processor
    //has a copy of this gathered information.
    MPI_Allgatherv( input_array.data_ptr(),//assigned local leaf node array of current PE
                    local_count,//send element count for current PE
                    mpi_datatype,//send element type
                    this->data_ptr(),//receive buffer address
                    count_array.data_ptr(),//receive element counts from each PE.
                    displace_array.data_ptr(),//displacement in receive buffer for each PE.
                    mpi_datatype,
                    communicator);
}

template<typename T>
void svector<T>::mpi_reduce( int root, int mpi_datatype, int op, int communicator /*= MPI_COMM_WORLD*/ )
{
    int rank=-1;
    MPI_Comm_rank(communicator, &rank);
    int err;
    if (rank == root)
    {
        err = MPI_Reduce(MPI_IN_PLACE, data_ptr(), size(), mpi_datatype, op, root, communicator);
    }else
    {
        err = MPI_Reduce(data_ptr(), NULL, size(), mpi_datatype, op, root, communicator);
    }

    if (err != MPI_SUCCESS)
    {
        char msg[128];
        sprintf(msg, "svector<T>::mpi_reduce failed!");
        D_P_MSG(rank, rank, msg);
    }
}

template<typename T>
void svector<T>::mpi_reduce( int root, svector<T>& send_vector, int mpi_datatype, int op, int communicator /*= MPI_COMM_WORLD*/ )
{
    int err = MPI_Reduce(send_vector.data_ptr(), data_ptr(), size(), mpi_datatype, op, root, communicator);
    int rank = -1;
    MPI_Comm_rank(communicator, &rank);
    if (err != MPI_SUCCESS)
    {
        char msg[128];
        sprintf(msg, "svector<T>::mpi_reduce failed!");
        D_P_MSG(rank, rank, msg);
    }
}
#else
template<typename T>
void svector<T>::mpi_allgatherv( const svector<T>& input_array, svector<int>& count_array,
    svector<int>& displace_array, int mpi_datatype,
    int communicator/*=MPI_COMM_WORLD*/ )
{
    fprintf(stderr, "svector<T>::mpi_allgatherv not implemented in none MPI program, use #define USE_MPI to enable its implementation!\n");
    fflush(stderr);
}
template<typename T>
void svector<T>::mpi_reduce( int root, svector<T>& send_vector, int mpi_datatype, int op, int communicator /*= MPI_COMM_WORLD*/ )
{
    fprintf(stderr, "svector<T>::mpi_reduce not implemented in none MPI program, use #define USE_MPI to enable its implementation!\n");
    fflush(stderr);
}
template<typename T>
void svector<T>::mpi_reduce( int root, int mpi_datatype, int op, int communicator /*= MPI_COMM_WORLD*/ )
{
    fprintf(stderr, "svector<T>::mpi_reduce not implemented in none MPI program, use #define USE_MPI to enable its implementation!\n");
    fflush(stderr);
}
#endif

template<typename T>
void svector<T>::remove_duplicates(int (*comp)(const void *, const void *))
{
    if (empty()){
        return;
    }
    qsort(data_ptr(), size(), sizeof(T), comp);
    int unique_count=0;
    for (int j = 0 ; j < m_count ; j++){
        if (comp(&(m_data[unique_count]), &(m_data[j]))!=0 ){
            unique_count++;
            m_data[unique_count] = m_data[j];
        }
    }
    unique_count++;
    resize(unique_count);
}

template<typename T>
bool svector<T>::contain( const T& val ) const
{
    for (int i = 0; i < m_count ; i++){
        if (val == m_data[i]){
            return true;
        }
    }
    return false;
}

template <typename T>
T* svector<T>::data_ptr(void)
{
    return empty() ? 0 : &(m_data[0]);
}

template<typename T>
T* svector<T>::data_ptr( void ) const
{
    return empty() ? 0 : &(m_data[0]);
}
// template <typename T>
// const T* svector<T>::data_ptr(void)
// {
//     return empty() ? 0 : &(m_data[0]);
// }

template<typename T>
void svector<T>::swap( svector<T>& right )
{
    if (this == &right)
        ;
    else {
        //swap control information
        swapxor(m_data,right.m_data);
        swapxor(m_count, right.m_count);
        swapxor(m_size, right.m_size);
    }
}

template<typename T>
void svector<T>::clear()
{
    if (m_data){
        free(m_data);
        m_data = NULL;
        m_count = 0;
        m_size = 0;
    }
}

template<typename T>
size_t svector<T>::capacity() const
{
    return m_size;
}

template<typename T>
void svector<T>::shrink_to_fit(bool is_profiling/*=false */)
{
    if (m_count == 0 && m_data)
    {
        free(m_data);
    }

    #ifdef _PROFILING_
    if (!is_profiling){
    #endif
       
        if (m_count < m_size && m_count > 0){
            m_data = (T*)realloc(m_data, sizeof(T) * m_count);
            if (!m_data){
                char *p=NULL;
                *p ='a';
                D_ASSERT(m_data != 0, "svector:shrink_to_fit(): Out of memory", DERROR_ERR_MEM);
            }

            m_size = m_count;
        }
    #ifdef _PROFILING_
    }else{
        HPGV_TIMING_BEGIN(parallel_test::T_g_m);
        if (m_count < m_size && m_count > 0){
            m_data = (T*)realloc(m_data, sizeof(T) * m_count);
            if (!m_data){
                char *p=NULL;
                *p ='a';
                D_ASSERT(m_data != 0, "svector:shrink_to_fit(): Out of memory", DERROR_ERR_MEM);
            }

            m_size = m_count;
        }
        HPGV_TIMING_END(parallel_test::T_g_m);
    }
    #endif
}

template<typename T>
inline T& svector<T>::back()
{
    return (*this)[m_count-1];
}

template<typename T>
inline T svector<T>::back() const
{
    return (*this)[m_count-1];
}

template<typename T>
inline T svector<T>::operator[]( size_t i ) const
{
    if (i < 0 || i >= m_count){
        char msg[128];
        sprintf(msg,"svector: index Out of bound:[%d/%d]",i,m_count);
        char* p=NULL;
        *p = 'a';//Intensionally raise an alarm
//         assert(i>=0 && i < m_count);
//         D_ASSERT(i>=0 && i < m_count, msg, DERROR_ERR_MEM);
    }
    return m_data[i];
}

template<typename T>
inline T& svector<T>::operator[]( size_t i )
{
//     if (!(i>=0 && i < m_count)){
//         char msg[128];
//         sprintf(msg,"svector: index Out of bound:[%d/%d]",i,m_count);
//         char* p=NULL;
//         *p = 'a';
//         assert(i>=0 && i < m_count);
//         D_ASSERT(i>=0 && i < m_count, msg, DERROR_ERR_MEM);
//     }
    return m_data[i];
}

template<typename T>
size_t svector<T>::size() const
{
    return m_count;
}

template<typename T>
inline void svector<T>::push_back( const T& val, bool is_profiling/*=false*/ )
{
    if (m_count>=m_size){
        size_t final_size = m_size*2;
        final_size = final_size < 1 ? 1 : final_size;
#ifdef _PROFILING_
        if (!is_profiling){
#endif
            this->reserve(final_size);

#ifdef _PROFILING_
        }else{
            HPGV_TIMING_BEGIN(parallel_test::T_g_m);
            this->reserve(final_size);
            HPGV_TIMING_END(parallel_test::T_g_m);
        }
#endif
    }
    m_data[m_count] = val;
    m_count++;
}

template<typename T>
void svector<T>::push_back( const svector<T>& vec, bool is_profiling/*=false*/ )
{
    int count = vec.size();
    for (int j = 0; j < count ; j++){
        push_back(vec[j], is_profiling);
    }
}

template <typename T>
void svector<T>::remove( const T& val ) 
{
    int unique_count=0;
    for (int iprobe = 0 ; iprobe < m_count; iprobe++ )
    {
        if (!(m_data[iprobe]==val))
        {
            m_data[unique_count] = m_data[iprobe];
            unique_count++;
            break;
        }
    }
    m_count = unique_count;
    shrink_to_fit();
}

template<typename T>
void svector<T>::remove(const T& val, 
                        int (*comp)(const void *, const void *))
{
    int unique_count=0;
    for (int iprobe = 0 ; iprobe < m_count; iprobe++ )
    {
        if (!comp(&(m_data[iprobe]), &val))
        {
            m_data[unique_count] = m_data[iprobe];
            unique_count++;
            break;
        }
    }
    m_count = unique_count;
    shrink_to_fit();
}

template <typename T>
void svector<T>::remove_all( const T& val ) 
{
    int unique_count=0;
    for (int iprobe = 0 ; iprobe < m_count; iprobe++ )
    {
        if (!(m_data[iprobe]==val))
        {
            m_data[unique_count] = m_data[iprobe];
            unique_count++;
        }
    }
    m_count = unique_count;
    shrink_to_fit();
}

template<typename T>
void svector<T>::remove_all(const T& val, 
                            int (*comp)(const void *, const void *))
{
    int unique_count=0;
    for (int iprobe = 0 ; iprobe < m_count; iprobe++ )
    {
        if (!comp(&(m_data[iprobe]), &val))
        {
            m_data[unique_count] = m_data[iprobe];
            unique_count++;
        }
    }
    m_count = unique_count;
    shrink_to_fit();
}
template<typename T>
void svector<T>::reserve( size_t count, bool is_profiling/*=false*/  )
{
    if (count <= 0){
        clear();
    }
    else if (count != m_size){
#ifdef _PROFILING_
        if (!is_profiling){
#endif
            m_data = (T*)realloc(m_data, sizeof(T) * count);

#ifdef _PROFILING_
        }else{
            HPGV_TIMING_BEGIN(parallel_test::T_g_m);
            m_data = (T*)realloc(m_data, sizeof(T) * count);
            HPGV_TIMING_END(parallel_test::T_g_m);
        }
#endif
        char msg[256];
        sprintf(msg, "svector:reserve: Out of memory: asking for %fMB.\n",float(count*sizeof(T))/1024.0f/1024.0f);
//         float nMB = float(count*sizeof(T))/1024.0f/1024.0f;
//         if (nMB > 400.0f)
//         {
//             sprintf(msg, "svector:resize: you're asking %fMB too much!\n",nMB);
//         }

        D_ASSERT(m_data != 0, msg, DERROR_ERR_MEM);
        m_size = count;
        if (count < m_count){
            m_count = count;
        }
    }
}

template<typename T>
void svector<T>::resize( size_t newSize, bool is_profiling/*=false*/  )
{
#ifdef _PROFILING_
    if (!is_profiling){
#endif
        resize(newSize, T());

#ifdef _PROFILING_
    }else{
        HPGV_TIMING_BEGIN(parallel_test::T_g_m);
        resize(newSize, T());
        HPGV_TIMING_END(parallel_test::T_g_m);
    }
#endif
}

template<typename T>
void svector<T>::resize( size_t newSize,const T& val )
{
    //if newSize == m_size, do nothing.
    if (newSize < m_size){
        reserve(newSize);
    }else if (newSize > m_size){
        reserve(newSize);
        T* tmp = &m_data[m_count];
        for (int i = 0; i < newSize-m_count ; i++){
            tmp[i] = val;
        }
        m_count = newSize;
    }
}

template<typename T>
svector<T>::svector( void )
    :m_data(NULL),m_size(0),m_count(0)
{
}

template<typename T>
svector<T>::svector( size_t count )
    :m_size(count),m_count(count)
{
    m_data = (T*)calloc(m_size, sizeof(T));
    char msg[256];
    sprintf(msg, "svector:2: Out of memory: asking for %fMB.\n",float(count*sizeof(T))/1024.0f/1024.0f);
    D_ASSERT(m_data != 0, msg, DERROR_ERR_MEM);
}

template<typename T>
svector<T>::svector( size_t count,const T& val )
    :m_size(count),m_count(count)
{
    m_data = (T*)malloc(m_size * sizeof(T));
    char msg[256];
    sprintf(msg, "svector:3: Out of memory: asking for %fMB.\n",float(count*sizeof(T))/1024.0f/1024.0f);
    D_ASSERT(m_data != 0, msg, DERROR_ERR_MEM);

    fill(val);
}

template<typename T>
svector<T>::~svector()
{
    clear();
}

template<typename T>
void svector<T>::fill(const T& val )
{
    for (size_t i = 0; i < m_count; i++){
        m_data[i] = val;
    }
}
template<typename T>
std::istream& operator >>( std::istream& in, svector<T> &t )
{
    for (int i = 0; i < t.m_count ; i++){
        in >> t.m_data[i];
    }
    return in;
}

template<typename T>
std::ostream& operator <<( std::ostream& os, svector<T> &t )
{
    for (int i = 0; i < t.m_count ; i++){
        os <<i<<":"<< t.m_data[i] << std::endl;
    }
    return os;
}

template<typename T>
std::istream& operator >>( std::istream& in, const svector<T> &t )
{
    for (int i = 0; i < t.m_count ; i++){
        in >> t.m_data[i];
    }
    return in;
}

template<typename T>
std::ostream& operator << ( std::ostream& os, const svector<T> &t )
{
    for (int i = 0; i < t.m_count ; i++){
        os <<i<<":"<< t.m_data[i] << std::endl;
    }
    return os;
}

template <typename T>
T* data_ptr(svector<T> &vec )
{
    return vec.empty() ? 0 : &(vec[0]);
}

template <typename T>
const T* data_ptr(const svector<T> &vec )
{
    return vec.empty() ? 0 : &(vec[0]);
}

}//end of namespace

#endif


