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
int key[80]={   0,0,0,0, 0,0,1,0, 0,0,0,0, 0,0,0,0,
                0,0,1,0, 0,0,1,0, 0,1,0,0, 0,0,0,0,
                0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                0,0,0,0, 0,0,1,1, 0,0,0,0, 0,1,0,1
};


int  PossibleCipher[36][64];
int  filterPairs[16384];
int  expendKey[192];
          
//generate rounds key
void RoundKey(int *b2,int *k1)
{     
    int temp,round_counter=1;
    for(int k=0;k<3;k++)
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
   for(int i=1;i<=2;i++)
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
    for(int j=0;j<64;j++)
    {
         plain[j] = plain[j] ^ r_key[2*64+j];
    }
}

//filter pairs
void filter()
{
     
   int AfterSbox[6]={2,4,6,8,12,14}; //posible output differences for input difference 9
   int count=0; //counter for filtering pairs

   for(int i=0;i<6;i++)
   {
       for(int j=0;j<6;j++)
       {
           int s1=AfterSbox[i];
           int s2=AfterSbox[j];
           for(int k=63;k>=0;k=k-16)
           {
                PossibleCipher[i*6+j][k]=s1%2;
                PossibleCipher[i*6+j][k-8]=s2%2;
                s1/=2;s2/=2;
           }   
       }
   }
   for(int j=0;j<262144;j++)
   {
          int plaintext1[64]={0};
          int plaintext2[64]={0};
          int m1=j;
          int m2=j^16388; //difference

          //plaintext 1
          int j1=63;
          while(m1!=0)
          {
              int t=m1%2;
              plaintext1[j1]=t;
              m1=m1/2; j1--;
          }
         
          //plaintext 2
          int j2=63;
          while(m2!=0)
          {
              int t=m2%2;
              plaintext2[j2]=t;
              m2=m2/2; j2--;
          }
         
          Present_encrypt(plaintext1,expendKey);//two round Present_encrypt function
          Present_encrypt(plaintext2,expendKey);//two round Present_encrypt function
            
          for(int u=0;u<36;u++)
          {
               int count1=0;
               for(int k=63;k>=0;k--)
               {
                    if(PossibleCipher[u][k]==(plaintext1[k]^plaintext2[k]))count1++;
               }
               if(count1==64)
               {filterPairs[count]=j;count++;break;}
    
          }
    }
    printf("filtered pairs: = %d\n",count);
}

//Attack
void attack()
{
    for(int i=0;i<64;i++)
    {    
        int counter=0;
    	int a = i>>0 & 1;
    	int b = i>>1 & 1;
    	int c = i>>2 & 1;
    	int d = i>>3 & 1;
    	int p = i>>4 & 1;
    	int q = i>>5 & 1;
    	int r = i>>6 & 1;
    	int s = i>>7 & 1;

        int key_guess[64]={0};

        key_guess[7]=a;
        key_guess[15]=b;
        key_guess[23]=c;
        key_guess[31]=d;
        key_guess[39]=p;
        key_guess[47]=q;
        key_guess[55]=r;
        key_guess[63]=s;
     
        for(int j=0;j<16384;j++)
        {
          int plaintext1[64]={0};
          int plaintext2[64]={0};

          int m1=filterPairs[j];
          int m2=m1^16388;     //difference
          
          //plaintext 1
          int j1=63;
          while(m1!=0)
          {
              int t=m1%2;
              plaintext1[j1]=t;
              m1=m1/2; j1--;
          }

          //plaintext 2
          int j2=63;
          while(m2!=0)
          {
              int t=m2%2;
              plaintext2[j2]=t;
              m2=m2/2; j2--;
          }
          
          Present_encrypt(plaintext1,expendKey);//two round Present_encrypt function
          Present_encrypt(plaintext2,expendKey);//two round Present_encrypt function
          
          //decryption
          for(int k=7;k<64;k=k+8)
          {
           plaintext1[k]=plaintext1[k]^key_guess[k]; //key add in C1
           plaintext2[k]=plaintext2[k]^key_guess[k]; //key add in C2
          }

          int s0_c1=plaintext1[63]*1+plaintext1[47]*2+plaintext1[31]*4+plaintext1[15]*8; //inverse pLayer 
          int s0_c2=plaintext2[63]*1+plaintext2[47]*2+plaintext2[31]*4+plaintext2[15]*8; //inverse pLayer

          int s8_c1=plaintext1[55]*1+plaintext1[39]*2+plaintext1[23]*4+plaintext1[7]*8;  //inverse pLayer
          int s8_c2=plaintext2[55]*1+plaintext2[39]*2+plaintext2[23]*4+plaintext2[7]*8;  //inverse pLayer
          
          int s0_c1_invr=sbox_invr[s0_c1]; //inverse Sbox
          int s0_c2_invr=sbox_invr[s0_c2]; //inverse Sbox
          int s8_c1_invr=sbox_invr[s8_c1]; //inverse Sbox
          int s8_c2_invr=sbox_invr[s8_c2]; //inverse Sbox
          
          if(((s0_c1_invr^s0_c2_invr) == 9) && ((s8_c1_invr^s8_c2_invr) == 9))
          {
             counter+=1;
          }
       }   
       if(counter>=1024)
       {printf("%d%d%d%d%d%d%d%d %f\n",a,b,c,d,p,q,r,s,(float)counter/262144);}
    }
}

//Main function
int main() 
{
        RoundKey(expendKey,key);   //call RoundKey function
        filter();                  //filtering plaintext pairs
        printf("Possible Keys\n");
        attack();                  //two round attack
        printf("Actual 8-bit subkey of k2\n"); //print actual key
        for(int i=7;i<64;i+=8)
        {  
            printf("%d",expendKey[2*64+i]);
        }
        printf("\n");
        
   return 0;
}


