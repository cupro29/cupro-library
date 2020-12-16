#include<math.h>

const double pi = 3.141592653589793;
typedef struct{
	double re, im;
} comp;
comp csum(comp a, comp b){
	comp res;
	res.re = a.re + b.re;
	res.im = a.im + b.im;
	return res;
}
comp cprod(comp a, comp b){
	comp res;
	res.re = a.re * b.re - a.im * b.im;
	res.im = a.re * b.im + a.im * b.re;
	return res;
}
void dft(comp f[], int n){
	if(n == 1)return;
	comp f0[n/2], f1[n/2], zeta, pow_zeta;
	for(int i=0; i<n/2; i++){
		f0[i] = f[2*i + 0];
		f1[i] = f[2*i + 1];
	}
	dft(f0, n/2);
	dft(f1, n/2);
	zeta.re = cos(2.0 * pi / (double)n);
	zeta.im = sin(2.0 * pi / (double)n);
	pow_zeta.re = 1.0;
	pow_zeta.im = 0.0;
	for(int i=0; i<n; i++){
		f[i] = csum(f0[i % (n/2)], cprod(pow_zeta, f1[i % (n/2)]));
		pow_zeta = cprod(zeta, pow_zeta);
	}
}
void idft(comp f[], int n){
	if(n == 1)return;
	comp f0[n/2], f1[n/2], zeta, pow_zeta;
	for(int i=0; i<n/2; i++){
		f0[i] = f[2*i + 0];
		f1[i] = f[2*i + 1];
	}
	idft(f0, n/2);
	idft(f1, n/2);
	zeta.re = cos(2.0 * pi / (double)n);
	zeta.im = -sin(2.0 * pi / (double)n);
	pow_zeta.re = 1.0;
	pow_zeta.im = 0.0;
	for(int i=0; i<n; i++){
		f[i] = csum(f0[i % (n/2)], cprod(pow_zeta, f1[i % (n/2)]));
		pow_zeta = cprod(zeta, pow_zeta);
	}
}