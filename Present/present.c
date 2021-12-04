//Ajay Tarole 
#include <stdio.h>  

//Sbox 
int sbox[16]={12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2};
//PLayer
int player[64]={
   0,16,32,48,1,17,33,49,2,18,34,50,3,19,35,51,
   4,20,36,52,5,21,37,53,6,22,38,54,7,23,39,55,
   8,24,40,56,9,25,41,57,10,26,42,58,11,27,43,59,
   12,28,44,60,13,29,45,61,14,30,46,62,15,31,47,63
   };
//generate rounds key
int* RoundKey(int *b2,int *k1)
{     
    int temp,round_counter=1;
    //cout<<temp;
    for(int k=0;k<32;k++)
    {
       for(int i=63;i>=0;i--)
       {
          b2[k*64+i]=k1[i+16];
       }
        int k19[61]={0};
       for(int i=79;i>=19;i--)
       {
          k19[i-19]=k1[i];
       }
       for(int i=79;i>=0;i--)
       {
        if(i>60)
        k1[i]=k1[i-61];
        else
        k1[i]=k19[i];
       }
       int sb_in[4]={0};
       int sum=0,j=8,sum1;
       for(int i=3;i>=0;i--)
       {
         sum+=j*k1[76+i];
         j=j/2;
       }
     
       sum1=sbox[sum];
       for(int i=3;i>=0;i--)
       {
         k1[79-i]=sum1%2;
         sum1/=2;
       }
        temp=round_counter;
        for(int i=15;i<=19;i++)
        {
          if(temp%2)
          {
            k1[i] = k1[i]^(temp%2);
          }
          temp/=2;    
       }
      round_counter++;
     }
     return b2;
}
//PRESENT Encryption function
int* Present_encrypt(int* plain, int* r_key)
{
   
   for(int i=1;i<=31;i++)
   {
      //add round key
      for(int j=63;j>=0;j--)
      {
         plain[j] = plain[j] ^ r_key[(i-1)*64+j];
      }
      //s-box
      for(int j=15;j>=0;j--)
      {
         int t1=0,t2=8,t3;
         for(int k=4*(j+1)-1;k>=4*j;k--)
         {
           t1+=t2*(plain[k]%2);
           t2/=2;
         }
         t3=sbox[t1];
         for(int k=4*j;k<4*(j+1);k++)
         {
           plain[k]=t3%2;
           t3/=2;
         }
      }
      //player
      int a[64]={0};
      for(int i=63;i>=0;i--)
      {   
          a[player[i]]=plain[i];
      }
      for(int i=0;i<64;i++)
         plain[i]=a[i];
   }
   //last round(32) add round key
    for(int j=0;j<64;j++)
    {
         plain[j] = plain[j] ^ r_key[31*64+j];
    }
     return plain;
}
//print Hexadecimal function
void PrintHex(unsigned char x)
{

 if(x%16<10) printf("%c",((x%16)+'0'));
 if(x%16>=10) printf("%c",((x%16-10)+'a'));
}   

//Main function
int main() {
         char c1[100],c2[100];
         printf("Enter Key(only 20-digit hex): ");
         scanf("%s",c1);
         printf("Enter Plain Text(only 16-digit hex): ");
         scanf("%s",c2);
         printf("--------------------------------------------------------------------------");
         int plain[64]={0};
         int key[80]={0};

         int l=79;
         for(int j=0;j<20;j++)
         {
          if(c1[j]=='0')
          {key[l]=0;key[l-1]=0;key[l-2]=0;key[l-3]=0;}
          else if(c1[j]=='1')
          {key[l]=0;key[l-1]=0;key[l-2]=0;key[l-3]=1;}
          else if(c1[j]=='2')
          {key[l]=0;key[l-1]=0;key[l-2]=1;key[l-3]=0;}
          else if(c1[j]=='3')
          {key[l]=0;key[l-1]=0;key[l-2]=1;key[l-3]=1;}
          else if(c1[j]=='4')
          {key[l]=0;key[l-1]=1;key[l-2]=0;key[l-3]=0;}
          else if(c1[j]=='5')
          {key[l]=0;key[l-1]=1;key[l-2]=0;key[l-3]=1;}
          else if(c1[j]=='6')
          {key[l]=0;key[l-1]=1;key[l-2]=1;key[l-3]=0;}
          else if(c1[j]=='7')
          {key[l]=0;key[l-1]=1;key[l-2]=1;key[l-3]=1;}
          else if(c1[j]=='8')
          {key[l]=1;key[l-1]=0;key[l-2]=0;key[l-3]=0;}
          else if(c1[j]=='9')
          {key[l]=1;key[l-1]=0;key[l-2]=0;key[l-3]=1;}
          else if(c1[j]=='a')
          {key[l]=1;key[l-1]=0;key[l-2]=1;key[l-3]=0;}
          else if(c1[j]=='b')
          {key[l]=1;key[l-1]=0;key[l-2]=1;key[l-3]=1;}
          else if(c1[j]=='c')
          {key[l]=1;key[l-1]=1;key[l-2]=0;key[l-3]=0;}
          else if(c1[j]=='d')
          {key[l]=1;key[l-1]=1;key[l-2]=0;key[l-3]=1;}
          else if(c1[j]=='e')
          {key[l]=1;key[l-1]=1;key[l-2]=1;key[l-3]=0;}
          else if(c1[j]=='f')
          {key[l]=1;key[l-1]=1;key[l-2]=1;key[l-3]=1;}    
           l=l-4;
          }  
         l=63;
         for(int j=0;j<16;j++)   
         {
          if(c2[j]=='0')
          {plain[l]=0;plain[l-1]=0;plain[l-2]=0;plain[l-3]=0;}
          else if(c2[j]=='1')
          {plain[l]=0;plain[l-1]=0;plain[l-2]=0;plain[l-3]=1;}
          else if(c2[j]=='2')
          {plain[l]=0;plain[l-1]=0;plain[l-2]=1;plain[l-3]=0;}
          else if(c2[j]=='3')
          {plain[l]=0;plain[l-1]=0;plain[l-2]=1;plain[l-3]=1;}
          else if(c2[j]=='4')
          {plain[l]=0;plain[l-1]=1;plain[l-2]=0;plain[l-3]=0;}
          else if(c2[j]=='5')
          {plain[l]=0;plain[l-1]=1;plain[l-2]=0;plain[l-3]=1;}
          else if(c2[j]=='6')
          {plain[l]=0;plain[l-1]=1;plain[l-2]=1;plain[l-3]=0;}
          else if(c2[j]=='7')
          {plain[l]=0;plain[l-1]=1;plain[l-2]=1;plain[l-3]=1;}
          else if(c2[j]=='8')
          {plain[l]=1;plain[l-1]=0;plain[l-2]=0;plain[l-3]=0;}
          else if(c2[j]=='9')
          {plain[l]=1;plain[l-1]=0;plain[l-2]=0;plain[l-3]=1;}
          else if(c2[j]=='a')
          {plain[l]=1;plain[l-1]=0;plain[l-2]=1;plain[l-3]=0;}
          else if(c2[j]=='b')
          {plain[l]=1;plain[l-1]=0;plain[l-2]=1;plain[l-3]=1;}
          else if(c2[j]=='c')
          {plain[l]=1;plain[l-1]=1;plain[l-2]=0;plain[l-3]=0;}
          else if(c2[j]=='d')
          {plain[l]=1;plain[l-1]=1;plain[l-2]=0;plain[l-3]=1;}
          else if(c2[j]=='e')
          {plain[l]=1;plain[l-1]=1;plain[l-2]=1;plain[l-3]=0;}
          else if(c2[j]=='f')
          {plain[l]=1;plain[l-1]=1;plain[l-2]=1;plain[l-3]=1;}
          
           l=l-4;
        }

     printf("\nKey:          ");
     for(int i=0;i<20;i++)
        printf("%c",c1[i]);
     printf("\n");

     int b2[2048]={0};
     int* b1=RoundKey(b2,key); //call RoundKey function
     
     printf("plain text:   ");
     for(int i=0;i<16;i++)
         printf("%c",c2[i]);
     printf("\n");

     int* c=Present_encrypt(plain,b1);//call Present_encrypt function

     printf("Encrypt Text: ");
     for(int j=15;j>=0;j--)
     {
         int t1=0,t2=8,t3;
         for(int k=4*(j+1)-1;k>=4*j;k--)
         {
           t1+=t2*(c[k]%2);
           t2/=2;
         }
         PrintHex(t1);
     }
      printf("\n");

return 0;
}


