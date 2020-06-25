#include <stdio.h>
#include <stdlib.h>

static int product[150];
static int product2[150];
static int result[150];
static int ti_2[100];
static int random_keys[318];
volatile int resultkon[1]={0XBEBEBEBE};

////////////////CUSTOM INSTRUCTION SET///////////////

void instr_add(unsigned int *a1, unsigned int *a2){

    asm volatile(
    ".insn r CUSTOM_0, 0x7, 3, %0, %1, %2  \n"   
    : "=r"(&a1[0])				 
    : "r"(&a1[0]), "r"(&a2[0])			

    );
    return;
}

void array_add(int *a1, int *a2, int length) {
int i =0;
    switch(length%3) {

        case 0:
            for (i = 0; i < (length / 3); i++) {
                instr_add((unsigned int*)&a1[3 * i], (unsigned int*)&a2[3*i]);
            }
	    break;
        case 1:
            for (i = 0; i < ((length-1) / 3); i++) {
                instr_add((unsigned int*)&a1[3 * i], (unsigned int*)&a2[3*i]);
            }
            a1[length-1] = a1[length-1] + a2[length-1];
	    break;      
	case 2:
            for (i = 0; i < ((length-2) / 3); i++) {
                instr_add((unsigned int*)&a1[3 * i], (unsigned int*)&a2[3*i]);
            }
            a1[length-1] = a1[length-1] + a2[length-1];
            a1[length-2] = a1[length-2] + a2[length-2];
	    break;
    } //end of switch case
} //end of function


void instr_equ(unsigned int *a1, unsigned int *a2){

    asm volatile(
    ".insn r CUSTOM_0, 0x7, 5, %0, %1, %2  \n"  
    : "=r"(&a1[0])				 
    : "r"(&a1[0]), "r"(&a2[0])			

    );
    return;
}


void array_equ(int *a1,int *a2,int length) {
int i = 0;
    switch(length%3) {

        case 0:
            for (i = 0; i < (length / 3); i++) {
                instr_equ((unsigned int*)&a1[3 * i],(unsigned int*) &a2[3*i]);
            }
	    break;
        case 1:
            for (i = 0; i < ((length-1) / 3); i++) {
                instr_equ((unsigned int*)&a1[3 * i],(unsigned int*) &a2[3*i]);
            }
            a1[length-1] = a2[length-1];
	    break;
        case 2:
            for (i = 0; i < ((length-2) / 3); i++) {
                instr_equ((unsigned int*)&a1[3 * i],(unsigned int*) &a2[3*i]);
            }
            a1[length-1] = a2[length-1];
            a1[length-2] = a2[length-2];
	    break;
    } //end of switch case
} //end of function



void instr_mod(unsigned int *a1, unsigned int mod) {

    asm volatile(
    ".insn r CUSTOM_0, 0x7, 6, %0, %1, %2  \n"   
    : "=r"(&a1[0])				 
    : "r"(&a1[0]), "r"(mod)			

    );
    return;
}
void array_mod(int *a1, int mod,int length){
	int i = 0;

	switch(length%3) {

	case 0:
	    for (i = 0; i < (length / 3); i++) {
		instr_mod((unsigned int *) &a1[3 * i], (unsigned int) mod);
	    }
	    break;
	case 1:
	    for (i = 0; i < ((length-1) / 3); i++) {
		instr_mod((unsigned int *) &a1[3 * i], (unsigned int) mod);
	    }
	    a1[length-1] = a1[length-1] % mod;
	    break;
	case 2:
	    for (i = 0; i < ((length-2) / 3); i++) {
		instr_mod((unsigned int *) &a1[3 * i], (unsigned int) mod);
	    }
	    a1[length-1] = a1[length-1] % mod;
	    a1[length-2] = a1[length-2] % mod;
	    break;
	} //end of switch case
} //end of function


/////////////////////////////////////////////////



int *polymult(int *a, int size_a, int *b, int size_b, int mod, int star_mult){

    int line[size_b][size_a + size_b];
    int i,j,k;
    int *return_address1;
    int pola[size_a];
    int polb[size_b];

    //make all line vectors zero
    for (i = 0; i < size_b; ++i){
        for(j = 0; j < size_a + size_b -1; ++j){
            line[i][j] = 0;
            product[j] = 0;
        }
    }

    // make mod calculations
    array_equ(pola,a,size_a);
    array_mod(pola,mod,size_a);

    array_equ(polb,b,size_b);
    array_mod(polb,mod,size_b);


    // make calculations for partial products, if need add mod calculations
    if(star_mult == 1){
        for (j = size_b -1; j >= 0; j = j-1){
            for (k = size_a-1; k >= 0; k = k-1 ){
                line[size_b-1-j][k] = pola[k]*polb[j];
            }
        }
    }
    else{
        for (j = size_b -1; j >= 0; j = j-1){
            for (k = size_a-1; k >= 0; k = k-1 ){
                line[size_b-1-j][k + j] = pola[k]*polb[j];
            }
        }
    }

    //mod calculations
    for (i = 0; i < size_b; ++i){
	array_mod(line[i],mod,size_a+size_b-1);
    }


    // construct product
    for(i = 0; i < size_b; ++i){
	array_add(product,line[i],size_a+size_b-1);
    }

    //mod calculations
    array_mod(product,mod,(size_a+size_b-1));

    return_address1 = &product[0];

    return return_address1;
}


int *polymult2(int *a, int size_a, int *b, int size_b, int star_mult){

    int line[size_b][size_a + size_b];
    int i,j,k;
    int *return_address2;
    int pola[size_a];
    int polb[size_b];

    //make all line vectors zero
    for (i = 0; i < size_b; ++i){
        for(j = 0; j < size_a + size_b -1; ++j){
            line[i][j] = 0;
            product2[j] = 0;
        }
    }

    array_equ(pola,a,size_a);

    array_equ(polb,b,size_b);

    // make calculations for partial products, if need add mod calculations
    if(star_mult == 1){
        for (j = size_b -1; j >= 0; j = j-1){
            for (k = size_a-1; k >= 0; k = k-1 ){
                line[size_b-1-j][k] = pola[k]*polb[j];
            }
        }
    }
    else{
        for (j = size_b -1; j >= 0; j = j-1){
            for (k = size_a-1; k >= 0; k = k-1 ){
                line[size_b-1-j][k + j] = pola[k]*polb[j];
            }
        }
    }

    // construct product
    for(i=0;i<size_b;++i){
	   array_add(product2,line[i],size_a+size_b-1);
    }

    return_address2 = &product2[0];

    return return_address2;
}

int *polydiv(int *num, int size_N, int*denum, int size_D, int mod){

    int u,d,d2,i,b_N,r_d;
    int *return_address3;

    int v[size_N];
    int q[size_N];
    int *product;
    int num_temp[size_N];
    int denum_temp[size_D];

    array_equ(num_temp,num,size_N);



    array_mod(num_temp, mod, size_N);
 


    // make mod calculation for coefficents
    array_equ(denum_temp,denum,size_D);
    array_mod(denum_temp,mod,size_D);

    for (i = 0; i < size_N; ++i){
        q[i] = 0;
        v[i] = 0;
    }

    //find b_N (denum) and degree denum
    for (i = size_D-1; i >= 0; i = i-1){
        if( denum_temp[i] != 0 )
            break;
    }
    d2 = i; //degree of f
    b_N = denum_temp[i];

    // Set u := (b_N)^–1 mod p (denum) //
    for (u = 0; u < mod; ++u){
        if ( (b_N*u)%mod == 1 )
            break;
    }

    // find degree num and r_d
    for (i = size_N-1; i >= 0; i = i-1){
        if( num_temp[i] != 0 )
            break;
    }
    d = i;

    r_d = num_temp[d];

    // While-1 deg num >= deg denum do
    while (d >= d2){

        // Set v := u × r_d × X^(d–N)
        v[(d-d2)] = u*r_d;

        array_mod(v, mod, size_N);
        
        // v*b
        product = polymult(denum_temp,size_D,v,size_D,mod,0);

        // make mod calculation for coefficents
        array_mod(product, mod, size_N);
        

        //r = r- v*b
        for (i = 0; i < size_N; ++i){
            num_temp[i] = num_temp[i] - product[i];
        }


        // make mod calculation for coefficents
        array_mod(num_temp, mod, size_N);
	

        // q = q + v;
        array_add(q, v, size_N);
        
        array_mod(q, mod, size_N);
        

        // Set d := deg r(X) (num)
        for (i = size_N-1; i >= 0; i = i-1){
            if( num_temp[i] != 0 )
                break;
        }
        d = i;
        r_d = num_temp[d];

        // make zero for next calculations
        for (i = 0; i < size_N; ++i){
            v[i] = 0;
        }


    } //End While-1

    array_equ(result,q,size_N);
    


    for(i = size_N; i < (2*size_N); ++i){
        result[i] = num_temp[i-size_N];
    }

    return_address3 = &result[0];

    return return_address3;
}


int* ext_euclid(int* polyR, int* polyf, int size, int mod) {

    int *return_address4;
    int N, i, j;
    int ri_2[size]; // MX-ring poly, a
    int ri_1[size]; // f-random poly, f[N] = 0, b
    int ri[size];

    int ti_1[size]; // ti_1[0] = 1
    int ti[size];
    int qi_1[size];
    int temp[size];
    int *res;
    int *res1;
    int *res2;
    int controlR = 1;

    N = size - 1;
    for (i = 0; i < size; ++i) {
        ti_2[i] = 0;
        ti_1[i] = 0;
    }
    array_equ(ri_2,polyR,size);
    array_equ(ri_1,polyf,size);

    ti_1[0] = 1;

    while (controlR != 0) {

        controlR = 0;
        // make mod for ri_2
   	array_mod(ri_2,mod,size);

        // make mod for ri_1
   	array_mod(ri_1,mod,size);

        res = polydiv(ri_2, N + 1, ri_1, N + 1, mod);
        for (i = N + 1; i < (2 * N + 2); ++i) {
            ri[i - (N + 1)] = res[i];
        }

/// qi_1 = (ri_2 - ri)/ri_1; ///

        for (i = 0; i < N + 1; ++i) {
            temp[i] = ri_2[i] - ri[i];
        }


        //make mod calc
    	array_mod(temp,mod,size);

        res1 = polydiv(temp, N + 1, ri_1, N + 1, mod);
   	array_equ(qi_1,res1,size);

        //make mod calc
   	array_mod(qi_1,mod,size);

/// ti = ti_2 - qi_1*ti_1; ///

        res2 = polymult(qi_1, N + 1, ti_1, N + 1, mod, 0);
   	array_equ(temp,res2,size);

        // make mod calculations
   	array_mod(temp,mod,size);

        for (i = 0; i < N + 1; ++i) {
            ti[i] = ti_2[i] - temp[i];
        }

  	array_mod(ti,mod,size);

/// ri_2 = ri_1; ri_1 = ri; ti_2 = ti_1; ti_1 = ti; ///

array_equ(ri_2,ri_1,size);
array_equ(ri_1,ri,size);
array_equ(ti_2,ti_1,size);
array_equ(ti_1,ti,size);

        for (i = 0; i < N + 1; ++i) {
              qi_1[i] = 0;
            controlR += ri[i];
        }
    }

        for (j = 0; j < mod; ++j) {
            if (((ri_2[0] * j) % mod) == 1)
                break;
        }


        for (i = 1; i < N; ++i) {
            if (ri_2[i] != 0)
                break;
        }

        for (i = 0; i < N + 1; ++i) {
            ti_2[i] = (ti_2[i] * j) % mod;
        }


        return_address4 = &ti_2[0];
        return return_address4;
    }



int* generate_keys(int N, int p, int q){
    resultkon[0] = 0xaaaaaaa0;
    int f[55] = {-1, 1, 1, 0, -1, 0, 1, 0, 0, 1, -1, -1, 0, 1, 0, -1, 0, 1, 1, 0, 0,  -1, 0, 1, 0, 0, 1, 1, 1,  -1, 0, 1, 0, 0, 1, 1, 1,    -1, 0, 1, 0, 0, 1, 1, 1};
    int g[55] = {-1, 0, 1, 1, 0, 1, 0, 0, -1, 0, -1, -1, 0, 1, 0, -1, 0, 1, 0, 0, 1, -1, 0, 1, 0, 0, 1, -1, 0, 1, 0, 0, 1, -1, 0, 1, 0, 1,   1, 0, 0, 1, -1, 0, 1, 0, 1};

    int *fp;
    int *fq;
    int *fg;
    int *pk;
    int polyR[N+1];
    int i;
    int* return_address5;


    polyR[N] = 1;
    polyR[0] = -1;

    for(i = 1; i < N; ++i){
        polyR[i] = 0;
    }



    for (i = N; i < 55; ++i ){
        f[i] = 0;
        g[i] = 0;
    }

    for(i = 0; i < N; ++i){
        random_keys[i] = f[i];
    }

    for(i = N; i < (2*N); ++i){
        random_keys[i] = g[i-N];
    }

    fp = ext_euclid(polyR, f, (N+1), p);

    for(i = (2*N); i < (3*N); ++i){
        random_keys[i] = fp[i-(2*N)];
    }

    fq = ext_euclid(polyR, f, (N+1), q);

    for(i = (3*N); i < (4*N); ++i){
        random_keys[i] = fq[i-(3*N)];
    }

    fg = polymult2(fq, N, g, N, 0);

    for(i = 0; i < (2*N-1); ++i){
        fg[i] = fg[i]*p;
    }

    pk = polydiv(fg, (2*N-1), polyR, (N+1), q);

    for(i = (4*N); i < (5*N); ++i){
        random_keys[i] = pk[i-(4*N)+(2*N-1)];
    }

    for(i = (5*N); i < (6*N+1); ++i){
        random_keys[i] = polyR[i-(5*N)];
    }

    return_address5 = &random_keys[0];
    resultkon[0] = 0xaaaaaaa1;
    return return_address5;
}


int* ntru_encyrpt(int N, int q, int* message, int* public_key, int* polyR){
    resultkon[0] = 0xbbbbbbb0;
    int *return_address6;
    int *CT;

    int random_val[48] = { 1, -1, 0, 1, -1, 1, -1, 0,1, 1, 0, -1, 1, -1, 1, 0,1, 1, 0, -1, 1, -1, 1, 0,1, 1, 0, 1, -1, 1, -1, 0, 1, -1, 0, 1, -1, 1, -1, 0,1, 1, 0, -1, 1, -1, 1, 0};
    int *temp;
    int i;


    temp = polymult(public_key,N,random_val,48,q,0);

    /////// PUBLIC_KEY*RANDOM_VALUE ///////

    array_add(temp,message,48);
    ////// PUBLIC_KEY*RANDOM_VALUE + MESSAGE ///////

    CT = polydiv(temp,(2*N),polyR,(N+1),q);

    for(i = (2*N); i < (3*N); ++i){
        CT[i-(2*N)] = CT[i];
    }

    return_address6 = &CT[0];
    resultkon[0] = 0xbbbbbbb1;
    return return_address6;
}


int* ntru_deceypt(int N, int p, int q, int* secret_key_f, int* secret_key_fp, int* Enc_Message, int* polyR){
    resultkon[0] = 0xccccccc0;
    int* return_address7;
    int* a;
    int* a2;
    int* c;
    int* c2;
    int i;

    // a = f.e mod q
    a = polymult2(secret_key_f,N,Enc_Message,N,0);

    a2 = polydiv(a,(2*N),polyR,(N+1),q);
/////////// Vector a2 (f.e modq) /////////

    for(i = (2*N); i < ((2*N)+N); ++i){
        a2[i-(2*N)] = a2[i];
    }

    //centerlifting a2
    for (i = 0; i < N; ++i){
        if(a2[i] <= q/2)
            a2[i] = a2[i];
        else
            a2[i] = (-1)*(q-a2[i]);
    }

    c = polymult2(secret_key_fp,N,a2,N,0);
/////////"Vector c(fp*a2)//////////

    c2 = polydiv(c,(2*N),polyR,(N+1),p);
    for(i = (2*N); i < ((2*N)+N); ++i){
        c2[i-(2*N)] = c2[i];
    }
    ////////"Vector c2 (decrypted message)/////////

    return_address7 = &c2[0];
    resultkon[0] = 0xccccccc1;	
    return return_address7;
}


int main(){
    int* Enc_Message;
    int* Dec_Message;
    int* keys;
    int N = 53;
    int p = 3;
    int q = 101;

    int message[48] = {1, 1, 2, 1, 1, 1, 1, 2,1, 1, 0, 1, 1, 2, 1, 0,1, 2, 0, 1, 1, 1, 1, 0,1, 1, 0, 2, 1, 2, 1, 0, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2};
    int public_key[N];
    int secret_key_f[N];
    int secret_key_g[N];	
    int secret_key_fp[N];
    int secret_key_fq[N];	
    int ring_poly[N+1];
    int i;

    // Alice generates public key from her randomly created secret keys.

    keys = generate_keys(N, p, q);

    for(i = 0; i < N; ++i){
        secret_key_f[i] = keys[i];
    }

    for(i = N; i < (2*N); ++i){
        secret_key_g[i-N] = keys[i];
    }

    for(i = (2*N); i < (3*N); ++i){
        secret_key_fp[i-(2*N)] = keys[i];
    }

    for(i = (3*N); i < (4*N); ++i){
        secret_key_fq[i-(3*N)] = keys[i];
    }

    for(i = (4*N); i < (5*N); ++i){
        public_key[i-(4*N)] = keys[i];
    }

    for(i = (5*N); i < (6*N+1); ++i){
        ring_poly[i-(5*N)] = keys[i];
    }

    // Bob encrypts message using Alice's public key and sends it to Alice.

    Enc_Message = ntru_encyrpt(N, q, message, public_key, ring_poly);

    // Alice decrypts the message using her secret keys.

    Dec_Message = ntru_deceypt(N, p, q, secret_key_f, secret_key_fp, Enc_Message, ring_poly);


    // Comparison and Proof
    for(i = 0; i < 32; ++i){
        if(!(message[i] == Dec_Message[i])){
            resultkon[0] = 0xBABABABA;
 	   
            break;	
	}
        else{
            resultkon[0] = 0xDEDEDEDE;
	}
    }
			

    return 0;
}
