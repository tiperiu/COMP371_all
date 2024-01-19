#ifndef TK_TUPLE__H_
#define TK_TUPLE__H_

/* This is a templated version of tuples and matrices */

#include <cmath>

using namespace std;

namespace T3D{
    
    template<class C, int N>
    class TTuple {
        
    private:
        C v[N];// the actual vector
        
    public:
        
        TTuple<C,N>(){
            for(int i=0;i<N;i++)
                v[i] = 0;
        }
        
        TTuple<C,N>(const C a){
            v[0] = a;
            for(int i=1;i<N;i++)
                v[i] = 0;
        }
        
        TTuple<C,N>(const C a, const C b){
            v[0] = a;
            v[1] = b;
            for(int i=2;i<N;i++)
                v[i] = 0;
        }
        
        TTuple<C,N>(const C a, const C b, const C c){
            v[0] = a;
            v[1] = b;
            v[2] = c;
            for(int i=3;i<N;i++)
                v[i] = 0;
        }
        
        TTuple<C,N>(const C* a){
            for(int i=0;i<N;i++)
                v[i] = a[i];
        }
        
        C& operator[] (int i);
        // better write comments here ....
        const C& operator[] (int i) const;
        
        TTuple<C,N>& operator= (const TTuple<C,N>& t);
        TTuple<C,N> operator+ (const TTuple<C,N>& a)const;
        TTuple<C,N> operator+= (const TTuple<C,N>& a);
        TTuple<C,N> operator- (const TTuple<C,N>& a)const;
        TTuple<C,N> operator- ()const;
        TTuple<C,N> operator-= (const TTuple<C,N>& a);
        TTuple<C,N> operator* (const C& a)const;
        TTuple<C,N> operator*= (const C& a);
        C operator*(const TTuple<C,N>& a)const;
        
        
        C size()const{
            C ret = 0;
            for(int i=0;i<N;i++)
                ret+=v[i]*v[i];
            return sqrt(ret);
        }
        
        C length()const{
            return size();
        }
        
        C norm()const{
            return size();
        }
        
    };
    
    
    template<class C, int N>
    C length(const TTuple<C,N>& v){
        return v.size();
    }
    
    template<class C, int N>
    TTuple<C,N> normalize(const TTuple<C,N>& v){
        TTuple<C,N> ret = v;
        C s = v.size();
        
        if(s<1e-10)
            return ret;
        
        s = ((C)1) / s;
        for(int i=0;i<N;i++)
            ret[i] = v[i] * s;
        
        return ret;
    }
    
    template<class T>
    TTuple<T, 3> operator^(const TTuple<T, 3> & v, const TTuple<T, 3> & a){
        TTuple<T, 3> ret;
        
        ret[0] = v[1] * a[2] - v[2] * a[1];
        ret[1] = v[2] * a[0] - v[0] * a[2];
        ret[2] = v[0] * a[1] - v[1] * a[0];
        
        return ret;
    }
    
    
    template<class T>
    TTuple<T, 3> cross(const TTuple<T, 3> & v, const TTuple<T, 3> & a){
        TTuple<T, 3> ret;
        ret = v^a;
        return ret;
    }
    
    template<class C, int N>
    C dot (const TTuple<C,N>& a, const TTuple<C,N>& t){
        C ret = a * t;
        return ret;
    }
    
    
    template<class C, int N>
    TTuple<C,N> operator* (const C a, const TTuple<C,N>& t);
    
    
    template<class C>
    TTuple<C, 3> getNormal(TTuple<C, 3> P1, TTuple<C, 3> P2, TTuple<C, 3> P3){
        
        TTuple<C, 3>  V1 = P2 - P1;
        TTuple<C, 3>  V2 = P3 - P1;
        return V1^V2;
    }
    
    
    // impementation start here
    
    
    template<class C, int N>
    C& TTuple<C, N>::operator[] (int i){
        i = i % N;
        return v[i];
    }
    
    // better write comments here ....
    template<class C, int N>
    const C& TTuple<C, N>::operator[] (int i) const {
        i = i % N;
        return v[i];
    }
    
    template<class C, int N>
    TTuple<C,N>& TTuple<C, N>::operator= (const TTuple<C,N>& t) {
        for (int i=0; i<N; i+=1) {
            v[i] = t[i];
        }
        return (*this);
    }
    
    
    template<class C, int N>
    TTuple<C,N> TTuple<C, N>::operator+ (const TTuple<C,N>& a)const{
        TTuple<C,N> ret;
        for (int i=0; i<N; i+=1) {
            ret[i] = v[i] + a[i];
        }
        return ret;
    }
    
    template<class C, int N>
    TTuple<C,N> TTuple<C, N>::operator+= (const TTuple<C,N>& a){
        for (int i=0; i<N; i+=1) {
            v[i] += a[i];
        }
        return (*this);
    }
    
    template<class C, int N>
    TTuple<C,N> TTuple<C, N>::operator- (const TTuple<C,N>& a)const{
        TTuple<C,N> ret;
        for (int i=0; i<N; i+=1) {
            ret[i] = v[i] - a[i];
        }
        return ret;
    }
    
    template<class C, int N>
    TTuple<C,N> TTuple<C, N>::operator- ()const{
        TTuple<C,N> ret;
        for (int i=0; i<N; i+=1) {
            ret[i] = -v[i];
        }
        return ret;
    }
    
    template<class C, int N>
    TTuple<C,N> TTuple<C, N>::operator-= (const TTuple<C,N>& a){
        for (int i=0; i<N; i+=1) {
            v[i] -= v[i];
        }
        return (*this);
    }

    
    template<class C, int N>
    TTuple<C,N> TTuple<C, N>::operator* (const C& a)const{
        TTuple<C,N> ret;
        for (int i=0; i<N; i+=1) {
            ret[i] = v[i] * a;
        }
        return ret;
    }
    
    template<class C, int N>
    TTuple<C,N> TTuple<C, N>::operator*= (const C& a){
        for (int i=0; i<N; i+=1) {
            v[i] *= a;
        }
        return (*this);
    }
    
    
    template<class C, int N>
    C TTuple<C, N>::operator*(const TTuple<C,N>& a)const{
        C ret = 0;
        for (int i=0; i<N; i+=1) {
            ret+= v[i] * a[i];
        }
        return ret;
    }
    
    
    template<class C, int N>
    TTuple<C,N> operator* (const C a, const TTuple<C,N>& t){
        TTuple<C,N> ret;
        for (int i=0; i<N; i+=1) {
            ret[i] = t[i] * a;
        }
        return ret;
    }
    
    
    
    typedef TTuple<double, 3> TPoint3d;
    typedef TTuple<double, 3> TVector3d;

typedef TTuple<double, 3> TPoint;
typedef TTuple<double, 3> TVector;
    
    typedef TTuple<double, 2> TPoint2d;
    typedef TTuple<double, 2> TVector2d;
    
}

#endif // T_TUPLE__H_
