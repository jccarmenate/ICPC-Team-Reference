// Merge Sort Tree (version del equipo). Cada nodo guarda ORDENADOS
// los valores de su rango. Memoria y build O(n log n).
// lesskquery(1, 0, n - 1, l, r, value): cuantos s[i] <= value con
//   i en [l, r]. O(log^2 n).
// k_th_smoller(l, r, k): k-esimo menor de s[l..r], con k desde 1 y
//   1 <= k <= r - l + 1. O(log^3 n).
// Uso: mergesort::n = n; mergesort::s = arreglo;
//      mergesort::build(1, 0, n - 1);
// Requiere n <= MAXN. Para otro caso de prueba basta volver a
// llamar build.
namespace mergesort{

    int n;
    const int  MAXN = 100005;
    vector<int>s;
    vector<int>st[4*MAXN];

    void build(int node,int l,int r){
        if (l==r){
            st[node]={s[l]};
        }
        else{
            int m=(l+r)/2;
            build(2*node,l,m);
            build(2*node+1,m+1,r);

            st[node].resize(r-l+1);
            merge(st[2*node].begin(),st[2*node].end(),
                  st[2*node+1].begin(),st[2*node+1].end(),
                  st[node].begin());

        }
    }

    int lesskquery(int v, int tl,int tr,int l,int r,int value){
        if(tl>r||tr<l){
            return 0;
        }
        if(tl>=l && tr<=r){
            // aqui busco los menores iguales; si quisiera los
            // menores estrictos uso lower_bound
            return upper_bound(st[v].begin(),st[v].end(),value)-st[v].begin();

        }
        int tm=(tl+tr)/2;
        return lesskquery(2*v,tl,tm,l,r,value)+lesskquery(2*v+1,tm+1,tr,l,r,value);
    }


    int k_th_smoller(int ql,int qr,int k){
        int lo=0;
        int hi=n-1;
        while(lo<hi){
            int mid=(lo+hi)/2;
            if(lesskquery(1,0,n-1,ql,qr,st[1][mid])>=k){
                hi=mid;
            }
            else{

            lo=mid+1;
        }
        }
        return st[1][lo];
    }

}
