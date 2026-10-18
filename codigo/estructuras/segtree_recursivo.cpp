// Segment Tree recursivo con arreglos globales (version del equipo).
// Parte 1: update puntual + query de suma en rango.
// Parte 2 (namespace lazy): update de suma en rango + query.
// Indices 0..n-1, rangos cerrados [l, r]. O(log n) por operacion.
// IMPORTANTE: dimensionar los vectores DESPUES de leer n (si se
// hace al declararlos, n todavia vale 0 y quedan vacios):
//   s.resize(n); /* leer s */ st.assign(4 * n, 0);
//   build(1, 0, n - 1);
//   query(1, 0, n - 1, l, r);    update(1, 0, n - 1, val, pos);
//   lazy::n = n; lazy::s = s;
//   lazy::st.assign(4 * n, 0); lazy::lazy.assign(4 * n, 0);
//   lazy::build(1, 0, n - 1);
//   lazy::update(1, 0, n - 1, l, r, val);
//   lazy::query(1, 0, n - 1, l, r);
// Las sumas son int: si pueden pasar de ~2e9, usar long long.

int n; //tamano del arreglo original
vector<int>s;//arreglo original
vector<int>st;//segment tree (4*n nodos)

void build(int v,int l,int r){
    if(l==r){
        st[v]=s[l];
    }
    else{
        int m=(l+r)/2;
        build(2*v,l,m);
        build(2*v+1,m+1,r);
        // en este caso es una suma pero puede ser cualquier
        // operacion asociativa (a o b) o c = a o (b o c)
        st[v]=st[2*v]+st[2*v+1];
    }
}

// v=nodo, tl=posicion izquierda en el st, tr=posicion derecha
// en el st, l y r rango de la query
int query(int v,int tl,int tr,int l,int r){
 if(tl>r || tr<l){
    // cero porque es el neutro de la suma; si fuera otra
    // operacion hay que poner su neutro
    return 0;
 }
 if(tl>=l && tr<=r){
    return st[v];
 }
 int tm=(tl+tr)/2;
 return query(2*v,tl,tm,l,r)+query(2*v+1,tm+1,tr,l,r);

}

void update(int v,int tl,int tr,int val,int pos){
    if(tl==tr){
        st[v]=val;
        return;
    }
    int tm=(tl+tr)/2;
    if(pos<=tm)update(2*v,tl,tm,val,pos);
    else update(2*v+1,tm+1,tr,val,pos);
    st[v]=st[2*v]+st[2*v+1];
}


//************************ LAZY PROPAGATION ************************/
namespace lazy{
int n;
vector<int>s;
vector<int> st;
vector<int>lazy;

void build(int v,int l,int r){
    if(l==r){
        st[v]=s[l];
    }
    else{
        int m=(l+r)/2;
        build(2*v,l,m);
        build(2*v+1,m+1,r);
        // en este caso es una suma pero puede ser cualquier
        // operacion asociativa (a o b) o c = a o (b o c)
        st[v]=st[2*v]+st[2*v+1];
    }
}

// aplica el valor al nodo y apunta lo que se aumento para sus hijos
void apply(int v, int l, int r, int val){
    st[v]+=(r-l+1)*val;
    lazy[v]+=val;
}

void push(int v, int l, int r){//baja el valor pendiente a sus hijos
    if (lazy[v]==0){
        return;
    }
    int tm =(l+r)/2;
    apply(2*v,l,tm,lazy[v]);
    apply(2*v+1,tm+1,r,lazy[v]);
    lazy[v]=0;

}
void update(int v,int tl,int tr,int l,int r,int val){
    if(tl>r||tr<l){
        return;
    }
    if(tl>=l&&tr<=r){
        apply(v,tl,tr,val);
        return;
    }
    push(v,tl,tr);
    int tm =(tl+tr)/2;
    update(2*v,tl,tm,l,r,val);
    update(2*v+1,tm+1,tr,l,r,val);
    st[v]=st[2*v]+st[2*v+1];
}
int query(int v, int tl , int tr,int l, int r){
    if(tl>r||tr<l){
        return 0;
    }
    if(tl>=l && tr<=r){
        return st[v];
    }
    push(v,tl,tr);
    int tm= (tl+tr)/2;
    return query(2*v,tl,tm,l,r)+query(2*v+1,tm+1,tr,l,r);
}
}
