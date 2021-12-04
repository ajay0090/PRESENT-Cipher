//Ajay Tarole 
#include <stdio.h>  

//Sbox 
int sbox[16]=     {12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2};

//Sbox inverse
int sbox_invr[16]={5,14,15,8,12,1,2,13,11,4,6,3,0,7,9,10};

//PLayer
int player[64]={0,16,32,48, 1,17,33,49, 2,18,34,50, 3,19,35,51,
                4,20,36,52, 5,21,37,53, 6,22,38,54, 7,23,39,55,
                8,24,40,56, 9,25,41,57,10,26,42,58,11,27,43,59,
               12,28,44,60,13,29,45,61,14,30,46,62,15,31,47,63
};

//Initial 80 bit key
        int key[80]={   0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                        0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                        0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                        0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                        0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};

int  expendKey[384];
          
//generate rounds key
void RoundKey(int *b2,int *k1)
{     
    int temp,round_counter=1;
    for(int k=0;k<6;k++)
    {
       for(int i=63;i>=0;i--)
       {b2[k*64+i]=k1[i];} //copy update key to b2

       //aplying left shift by 61
       int k19[61]={0};
       for(int i=79;i>=19;i--)
       {k19[i-19]=k1[i];} 

       for(int i=79;i>=0;i--)
       {
        if(i>60){k1[i]=k1[i-61];}
        else    {k1[i]=k19[i];}
       }

       int sum=0,j=8;
       for(int i=3;i>=0;i--)
       {sum+=j*k1[76+i]; j=j/2;}
       
       //applying Sbox
       int sum1=sbox[sum];         
       for(int i=3;i>=0;i--)
       { k1[79-i]=sum1%2; sum1/=2;}
       
       //applying Add Round counter
       temp=round_counter;
       for(int i=15;i<=19;i++)
       {
          if(temp%2){k1[i] = k1[i]^(temp%2);}
          temp/=2;    
       }
       round_counter++; //round counter increament
     }
}

//PRESENT Two Round Encryption function
void Present_encrypt(int* plain, int* r_key)
{
   for(int i=1;i<=5;i++)
   {
      //add round key
      for(int j=63;j>=0;j--)
      {
         plain[j] = plain[j] ^ r_key[(i-1)*64+j];
      }
      //s-box
      for(int j=0;j<16;j++)
      {
         int t1=0,t2=8,t3;
         for(int k=4*(j);k<4*(j+1);k++)
         {t1+=t2*(plain[k]%2); t2/=2;}

         t3=sbox[t1];

         for(int k=4*(j+1)-1;k>=4*j;k--)
         { plain[k]=t3%2; t3/=2;}
      }

      //player
      int a[64]={0};
      for(int i=63;i>=0;i--)
      {  a[player[i]]=plain[i];}
      for(int i=63;i>=0;i--)
      {  plain[i]=a[i];  }

   }
   //last add round key
    /*for(int j=0;j<64;j++)
    {
         plain[j] = plain[j] ^ r_key[5*64+j];
    }*/
}

//Main function
int main() 
{
    RoundKey(expendKey,key);   //call RoundKey function
        
    int count=0;
    //run the distinguisher for 2^12 times for different massages
    for(int l=0;l<4096;l++)
    {
        int cipher[64]={0}; 
        
        int a = l>>0 & 1;
    	int b = l>>1 & 1;
    	int c = l>>2 & 1;
    	int d = l>>3 & 1;
        int e = l>>4 & 1;
        int f = l>>5 & 1;
        int g = l>>6 & 1;
        int h = l>>7 & 1;
        int p = l>>8 & 1;
        int q = l>>9 & 1;
        int r = l>>10 & 1;
        int s = l>>11 & 1;

        
        for(int i=0;i<16;i++)
        {
            //update plaintext
            int plaintext[64]={0};
            plaintext[59]=a,plaintext[58]=b,plaintext[57]=c,plaintext[56]=d;
            plaintext[55]=e,plaintext[54]=f,plaintext[53]=g,plaintext[52]=h;
            plaintext[51]=p,plaintext[50]=q,plaintext[49]=r,plaintext[48]=s;
            int n=i,j=63;
            //update last 4 bits plaintext
            while(n!=0)
            {
               plaintext[j--]=n%2;
               n=n/2;
            }

            Present_encrypt(plaintext,expendKey); //calling 5 round encryption function
            for(int k=0;k<64;k++)
            {
               cipher[k]^=plaintext[k]; //xoring
            }
        }
        c=0;
        for(int i=60;i<64;i++)//condition to check balanced property
        {
            if(cipher[i]==0)c++;
        }
        if(c==4)count++;
    }
    //printf("%d\n",count);
   return 0;
}


