#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

static long modpow(long x, long y, long m){
	long res = 1;
	while(y){
		if(y&1)res = res * x % m;
		x = x * x % m;
		y >>= 1;
	}
	return res;
}

const long mod = 998244353;
const long pr = 3;

static void butterfly(long a[], int h){
	static bool first = true;
	static long sum_e[30];
	if(first){
		first = false;
		long es[30], ies[30];
		int cnt2 = __builtin_ctz(mod - 1);
		long e = modpow(pr, (mod-1)>>cnt2, mod);
		long ie = modpow(e, mod-2, mod);
		for(int i=cnt2; i>=2; i--){
			es[i-2] = e;
			ies[i-2] = ie;
			e = e * e % mod;
			ie = ie * ie % mod;
		}
		long now = 1;
		for(int i=0; i<=cnt2-2; i++){
			sum_e[i] = es[i] * now % mod;
			now = now * ies[i] % mod;
		}
	}
	for(int ph=1; ph<=h; ph++){
		int w = 1<<(ph-1), p=1<<(h-ph);
		long now = 1;
		for(int s=0; s<w; s++){
			int offset = s<<(h-ph+1);
			for(int i=0; i<p; i++){
				long l = a[i+offset],
					 r = a[i+offset+p] * now % mod;
				a[i+offset] = (l + r) % mod;
				a[i+offset+p] = (l - r + mod) % mod;
			}
			now = now * sum_e[__builtin_ctz(~s)] % mod;
		}
	}
}
static void butterfly_inv(long a[], int h){
	static bool first = true;
	static long sum_ie[30];
	if(first){
		first = false;
		long es[30], ies[30];
		int cnt2 = __builtin_ctz(mod - 1);
		long e = modpow(pr, (mod-1)>>cnt2, mod);
		long ie = modpow(e, mod-2, mod);
		for(int i=cnt2; i>=2; i--){
			es[i-2] = e;
			ies[i-2] = ie;
			e = e * e % mod;
			ie = ie * ie % mod;
		}
		long now = 1;
		for(int i=0; i<=cnt2-2; i++){
			sum_ie[i] = ies[i] * now % mod;
			now = now * es[i] % mod;
		}
	}
	for(int ph=h; ph>=1; ph--){
		int w = 1<<(ph-1), p=1<<(h-ph);
		long inow = 1;
		for(int s=0; s<w; s++){
			int offset = s<<(h-ph+1);
			for(int i=0; i<p; i++){
				long l = a[i+offset],
					 r = a[i+offset+p];
				a[i+offset] = (l + r) % mod;
				a[i+offset+p] = (l - r + mod) * inow % mod;
			}
			inow = inow * sum_ie[__builtin_ctz(~s)] % mod;
		}
	}
}

long *convolution(long a[], int n, long b[], int m){
	int t = 0;
	while(n+m-1 > (1<<t))t++;
	if(t > 26)exit(1);
	long *c = (long*)calloc(1<<t, sizeof(long));
	memcpy(c, a, sizeof(long) * n);
	long *d = (long*)calloc(1<<t, sizeof(long));
	memcpy(d, b, sizeof(long) * m);
	butterfly(c, t); butterfly(d, t);
	for(int i=0; i<(1<<t); i++)c[i] = c[i] * d[i] % mod;
	free(d);
	butterfly_inv(c, t);
	long inv = modpow(1<<t, mod-2, mod);
	for(int i=0; i<(1<<t); i++)c[i] = c[i] * inv % mod;
	return c;
}