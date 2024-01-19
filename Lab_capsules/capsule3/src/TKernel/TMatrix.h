#ifndef TK_MATRIX_H
#define TK_MATRIX_H

#include "TTuple.h"

#include <iostream>

using namespace std;

namespace T3D{
    
    template<class T, int L, int C>
    class TMatrix { /* want a matrix simillar to OpenGL in structure */
    private:
        T v[L*C];
        
    public:
        
        void setNull();
        void setIdentity();
        TMatrix<T, L, C>();
        
        // copy constructor + assignment operator
        TMatrix<T, L, C>(const TMatrix<T, L, C>&);
        TMatrix<T, L, C>& operator=(const TMatrix<T, L, C>&);
        
        TMatrix<T, L, C> transpose()const;
        
        T& operator() (int l, int c);
        
        const T& operator()(int l, int c) const;
        T& elem(int l, int c);
        const T elem(int l, int c)const;
        
        T& operator[] (int n);
        
        const T& operator[] (int n) const;
        
        TMatrix<T, L, C> operator+ (const TMatrix<T, L, C> & a)const;
        TMatrix<T, L, C> operator+= (const TMatrix<T, L, C> & a);
        TMatrix<T, L, C> operator- (const TMatrix<T, L, C> & a)const;
        TMatrix<T, L, C> operator-= (const TMatrix<T, L, C> & a);
        TMatrix<T, L, C> operator*(const T)const;
        
        // applying
        TTuple<T, L> operator*(const TTuple<T, C>&)const;
        TTuple<T, L-1> operator*(const TTuple<T, C-1>&)const;
        
        
        T* getBuffer();
        
        void print();
        
    };
    
    template<class T, int L, int C, int K>
    TMatrix<T, L, C> operator*(const TMatrix<T, L, K>& a, const TMatrix<T, K, C>& b){
        TMatrix<T, L, C> ret;
        
        for(int i=0;i<L;i++)
        for(int j=0;j<C;j++){
            ret(i, j) = 0;
            for(int k=0;k<K;k++)
            ret(i, j) += a(i,k)*b(k, j);
        }
        
        return ret;
        
    }
    
    
    
    template<class T>
    TMatrix<T, 4, 4> TRotate(T alfa, int axis){/* 0, 1, 2 = x, y, z */
        TMatrix<T, 4, 4> matrix;
        
        switch(axis)
        {
            case 0:
                matrix(1, 1) =   cos(alfa);
                matrix(1, 2) = - sin(alfa);
                matrix(2, 1) = 	sin(alfa);
                matrix(2, 2) = 	cos(alfa);
                break;
            case 1:
                matrix(0, 0) = 	cos(alfa);
                matrix(0, 2) = 	sin(alfa);
                matrix(2, 0) = -	sin(alfa);
                matrix(2, 2) = 	cos(alfa);
                break;
            case 2:
                matrix(0, 0) = 	cos(alfa);
                matrix(0, 1) = -	sin(alfa);
                matrix(1, 0) = 	sin(alfa);
                matrix(1, 1) = 	cos(alfa);
                break;
            default:
                
                break;
        }
        
        return matrix;
        
    }
    
    template<class T>
    TMatrix<T, 4, 4> TTranslate( const TTuple<T, 3>& v){
        TMatrix<T, 4, 4> ret;
        
        for(int i=0;i<3;i+=1)
        ret(i, 3) = v[i];
        
        return ret;
    }
    
    template<class T>
    TMatrix<T, 4, 4> TTranslate(T dx, T dy, T dz){
        
        TMatrix<T, 4, 4> matrix;
        
        matrix(0, 3) = dx;
        matrix(1, 3) = dy;
        matrix(2, 3) = dz;
        
        return matrix;
        
    }
    
    template<class T>
    TMatrix<T, 4, 4> TScale(T sx, T sy, T sz){
        TMatrix<T, 4, 4> matrix;
        
        matrix(0, 0) = sx;
        matrix(1, 1) = sy;
        matrix(2, 2) = sz;
        
        return matrix;
        
    }
    
    template<class T>
    TMatrix<T, 4, 4> TScale( const TTuple<T, 3>& v){
        TMatrix<T, 4, 4> ret;
        
        for(int i=0;i<3;i+=1)
        ret(i, i) = v[i];
        
        return ret;
        
    }
    
    template<class T>
    T GrRad(T alfa){
        T ret;
        
        ret = (T) (((T) (alfa * (T) M_PI)) /(T)180);
        
        return ret;
    }
    
    // implementation stars here
    
    template<class T, int L, int C>
    TMatrix<T, L, C>::TMatrix(const TMatrix<T, L, C>& M){
        for(int i=0;i<L;i+=1)
        for(int j=0;j<C;j+=1)
        elem(i, j) = M(i, j);
    }
    
    template<class T, int L, int C>
    TMatrix<T, L, C>& TMatrix<T, L, C>::operator=(const TMatrix<T, L, C>& M){
        for(int i=0;i<L;i+=1)
        for(int j=0;j<C;j+=1)
        elem(i, j) = M(i, j);
        
        return *this;
    }
    
    
    template<class T, int L, int C>
    void TMatrix<T, L, C>::setNull(){
        for(int i=0;i<L;i+=1)
        for(int j=0;j<C;j+=1)
        elem(i, j) = 0;
    }
    
    template<class T, int L, int C>
    void TMatrix<T, L, C>::setIdentity(){
        int min = (L<C) ? L : C;
        
        setNull();
        
        for(int i=0;i<min;i+=1)
        elem(i, i) = 1;
    }
    
    
    template<class T, int L, int C>
    TMatrix<T, L, C>::TMatrix(){
        setIdentity();
    }
    
    template<class T, int L, int C>
    T& TMatrix<T, L, C>::operator() (int l, int c){
        return elem(l, c);
    }
    
    template<class T, int L, int C>
    const T&TMatrix<T, L, C>:: operator()(int l, int c) const{
        return  v[c*L+l];
    }
    
    template<class T, int L, int C>
    T& TMatrix<T, L, C>::elem(int l, int c){
        return v[c*L+l];
    }
    
    template<class T, int L, int C>
    const T TMatrix<T, L, C>::elem(int l, int c)const{
        return v[c*L+l];
    }
    
    
    template<class T, int L, int C>
    T& TMatrix<T, L, C>::operator[] (int n){
        return v[n];
    }
    
    template<class T, int L, int C>
    TMatrix<T, L, C> TMatrix<T, L, C>::transpose()const{
        TMatrix<T, L, C> ret;
        for(int i=0;i<L;++i)
        for(int j=0;j<C;++j)
        ret(i, j) = elem(j, i);
        return ret;
    }
    
    template<class T, int L, int C>
    const T& TMatrix<T, L, C>::operator[] (int n) const{
        return v[n];
    }
    
    template<class T, int L, int C>
    TMatrix<T, L, C> TMatrix<T, L, C>::operator+ (const TMatrix<T, L, C> & a)const{
        TMatrix<T, L, C> ret;
        for (int i=0; i<L*C; i+=1) {
            ret[i] = v[i] + a[i];
        }
        return ret;
    }
    
    template<class T, int L, int C>
    TMatrix<T, L, C> TMatrix<T, L, C>::operator+= (const TMatrix<T, L, C> & a){
        for (int i=0; i<L*C; i+=1) {
            v[i] += a[i];
        }
        return (*this);
    }
    
    template<class T, int L, int C>
    TMatrix<T, L, C> TMatrix<T, L, C>::operator- (const TMatrix<T, L, C> & a)const{
        TMatrix<T, L, C> ret;
        for (int i=0; i<L*C; i+=1) {
            ret[i] = v[i] - a[i];
        }
        return ret;
    }
    
    template<class T, int L, int C>
    TMatrix<T, L, C> TMatrix<T, L, C>::operator-= (const TMatrix<T, L, C> & a){
        for (int i=0; i<L*C; i+=1) {
            v[i] -= a[i];
        }
        return (*this);
    }
    
    template<class T, int L, int C>
    T* TMatrix<T, L, C>::getBuffer(){
        return v;
    }
    
    template<class T, int L, int C>
    void TMatrix<T, L, C>::print(){
        for(int i=0;i<L;i++){
            for(int j=0;j<C;j++)
            std::cout<<elem(i, j)<<" ";
            std::cout<<std::endl;
        }
    }
    
    template<class T, int L, int C>
    TMatrix<T, L, C> TMatrix<T, L, C>::operator*(const T f)const{
        
        TMatrix<T, L, C> ret ;
        for(int i=0;i<L;i++)
        for(int j=0;j<C;j++)
        ret(i, j) = elem(i, j) * f;
        return ret;
        
    }
    
    // applying
    template<class T, int L, int C>
    TTuple<T, L> TMatrix<T, L, C>::operator*(const TTuple<T, C>& v)const{
        TTuple<T, L> ret;
        
        for(int i = 0;i<L;i++){
            ret[i] = 0;
            for(int j=0;j<C;j++)
            ret[i]+=elem(i, j)*v[j];
        }
        return ret;
    }
    
    template<class T, int L, int C>
    TTuple<T, L-1> TMatrix<T, L, C>::operator*(const TTuple<T, C-1>& v)const{
        TTuple<T, L-1> ret;
        
        for(int i = 0;i<L-1;i++){
            ret[i] = 0;
            for(int j=0;j<C-1;j++)
            ret[i]+=elem(i, j)*v[j];
            ret[i]+=elem(i, C-1);
        }	
        return ret;
    }
    
    
    // Some low dimension matrix op
    
    template<class C>
    C det(TMatrix<C, 2, 2> m){
        return m(0, 0)*m(1, 1)- m(0, 1)*m(1, 0);
    }
    
    template<class C>
    C det(TMatrix<C, 3, 3> m){
        
        C ret = 
        m(0, 0) * m(1, 1) * m(2, 2) + 
        m(1, 0) * m(2, 1) * m(0, 2) +
        m(0, 1) * m(1, 2) * m(2, 0) - (
                                       m(2, 0) * m(1, 1) * m(0, 2) +
                                       m(0, 0) * m(2, 1) * m(1, 2) +
                                       m(0, 1) * m(1, 0) * m(2, 2));
        
        return ret;
        
    }
    
    
    
    template<class C>
    TMatrix<C, 2, 2>  extract(int l, int c, TMatrix<C, 3, 3>& m){
        TMatrix<C, 2, 2> ret;
        
        int cl = 0;
        int cc = 0;
        for(int i=0;i<3;i++){
            if(i==l)
            continue;
            cc = 0;
            for(int j=0;j<3;j++){
                if(j==c)
                continue;
                ret(cl, cc) = m(i, j);
                cc++;
            }
            cl++;
        }
        
        return ret;
        
    }
    
    template<class C>
    TMatrix<C, 3, 3> invert(TMatrix<C, 3, 3>& m){
        
        TMatrix<C, 3, 3> temp;
        
        for(int i=0;i<3;i++)
        for(int j=0;j<3;j++){
            C sign;
            if((i+j)%2==0)
            sign = 1;
            else 
            sign = -1;
            temp(i, j) =sign* det<C>(extract<C>(j, i, m));
        }
        
        C dt = det(m);
        
        // what happens if the determinant is small ?
        
        TMatrix<C, 3, 3> inv = temp * (1.0 / dt);
        return inv;
    }
    
}

typedef TMatrix<double, 4, 4> TMatrix4x4;
typedef TMatrix<double, 3, 3> TMatrixd3x3;

#endif // T_MATRIX_H
