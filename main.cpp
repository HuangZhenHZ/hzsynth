#include <bits/stdc++.h>
#include <fcntl.h>

template <class T>
struct mat {
	int n,m;
	T *p;
	
	mat();
	mat(int n, int m);
	mat(const mat &);
	mat& operator = (const mat &o);
	~mat();
	
	void init(int _n, int _m);
	T* operator[] (int x);
};

template <class T>
mat<T>::mat(){
	n=m=0;
	p=NULL;
}

template <class T>
mat<T>::mat(int n, int m):n(n),m(m){
	p = new T[n*m]();
}

template <class T>
mat<T>::mat(const mat &o){
	n=o.n; m=o.m;
	p = new T[n*m];
	memcpy(p, o.p, sizeof(T)*n*m);
}

template <class T>
mat<T>& mat<T>::operator = (const mat<T> &o){
	if (n*m != o.n*o.m){
		delete[] p;
		p = new T[n*m];
	}
	memcpy(p, o.p, sizeof(T)*n*m);
	return *this;
}

template <class T>
mat<T>::~mat(){
	delete[] p;
}

template <class T>
void mat<T>::init(int _n, int _m){
	if (n*m == _n*_m){
		memset(p,0,sizeof(T)*n*m);
		return;
	}
	n=_n; m=_m;
	delete[] p;
	p = new T[n*m]();
}	

template <class T>
T* mat<T>::operator[] (int x){
	return p + x*m;
}

// --- --- --- ---

const int BITS_USE = 20;


bool read_plane(mat<int> &v, int bits=8){
	static unsigned char *p = NULL;
	static int p_sz = 0;
	
	int s = 0;
	if (bits==8) s = v.n * v.m;
	else if (bits==10) s = v.n * v.m * 2;
	else assert(false);
	
	if (s > p_sz){
		delete[] p;
		p = new unsigned char [s];
		p_sz = s;
	}
	
	if ((int)fread(p,1,s,stdin)!=s) return true;
	
	if (bits==8){
		for(int i=0; i<s; ++i){
			v.p[i] = int(p[i]) << (BITS_USE - 8) | 1<<(BITS_USE-9);
		}
	}
	
	if (bits==10){
		int t = v.n * v.m;
		for(int i=0; i<t; ++i){
			v.p[i] = ( int(p[i<<1|1])<<8 | p[i<<1] ) << (BITS_USE-10) | 1<<(BITS_USE-11);
		}
	}
	
	return false;
}


bool write_plane(mat<int> &v){
	static unsigned char *p = NULL;
	static int p_sz = 0;
	
	int s = v.n * v.m * 2;
	if (s > p_sz){
		delete[] p;
		p = new unsigned char [s];
		p_sz = s;
	}
	
	int t = v.n * v.m;
	for(int i=0; i<t; ++i){
		if (v.p[i]<=0){
			p[i<<1] = p[i<<1|1] = 0;
			continue;
		}
		if (v.p[i] >= 1<<BITS_USE){
			p[i<<1|0] = 0xff;
			p[i<<1|1] = 0x03;
			continue;
		}
		int c = v.p[i] >> (BITS_USE - 10);
		p[i<<1|0] = c & 0xff;
		p[i<<1|1] = (c>>8) & 0x03;
	}
	
	if ((int)fwrite(p,1,s,stdout)!=s) return true;
	return false;
}
			

struct frame {
	mat<int> y,u,v;
	
	void init(int w, int h, int type=1){
		if (type==1){ // yuv420
			y.init(h,w);
			u.init(h/2,w/2);
			v.init(h/2,w/2);
		}
		if (type==2){ // yuv444
			y.init(h,w);
			u.init(h,w);
			v.init(h,w);
		}
		//assert(false);
	}
	
	bool read(){
		if (read_plane(y)) return true;
		if (read_plane(u)) return true;
		if (read_plane(v)) return true;
		return false;
	}
	
	bool write(){
		if (write_plane(y)) return true;
		if (write_plane(u)) return true;
		if (write_plane(v)) return true;
		return false;
	}
};


mat<int> simp_sharp(mat<int> &v){
	mat<int> o(v.n,v.m);
	for(int i=1; i<v.n-1; ++i)
	for(int j=1; j<v.m-1; ++j){
		o[i][j] = v[i][j] * 5 - (v[i-1][j] + v[i+1][j] + v[i][j-1] + v[i][j+1]);
	}
	return o;
}


int main(){
	_setmode(_fileno(stdin), _O_BINARY);
	_setmode(_fileno(stdout), _O_BINARY);
	
	frame f;
	f.init(720,576);
	
	fprintf(stderr,"init ok\n");
	
	while (true){
		if (f.read()){
			fprintf(stderr,"read error\n");
			return 0;
		}
		
		f.y = simp_sharp(f.y);
		//for(int i=0; i<f.y.n; ++i)
		//for(int j=0; j<f.y.m/2; ++j){
		//	f.y[i][j]=0;
		//}
		
		if (f.write()){
			fprintf(stderr,"write error\n");
			return 0;
		}
	}
	return 0;
}

		
		

 
	
	
	
	
		
		
	

