# PRESENT-Cipher

We as team Walkie_Talkie of 3 members (Ajay Tarole[11840090], Ashish Kumar Suraj[11840230] and Rudraksh Kashyap[11840970]) have studied the PRESENT Cipher and implemented several aspects of it using different types of codes related to the same. We have also made a report on the Cipher as well as you can see a nice presentation for the same. Thanks:)

We have implemented the PRESENT Cipher, 80 bit in C.

  - Generate Difference Distribution Table
  - Generate Linear Approximation Table
  - Differential attack on three round of Present Cipher
  - Varified 5 rounds integral property of PRESENT

## Encrytion

Run present.c file in root folder.

```sh
gcc present.c
./a.out
```


#### Demo
```sh
gcc present.c
./a.out
Enter Key(only 20-digit hex): 00000000000000000000
Enter Plain Text: 0000000000000000
--------------------------------------------------------------------------
Key:          00000000000000000000
plain text:   0000000000000000
Encrypt Text: 5579c1387b228445
```

### For DDT
```sh
python ./differential/DDT.py
```

### For LAT
```sh
python ./linear/LAT.py
```

## Running Differential Attack on 3 Round Present Cipher
```sh
gcc ./differential/differentialAttack.c
./a.out
```

### Demo
```sh
gcc ./differential/differentialAttack.c
./a.out
filtered pairs: = 16384
Possible Keys
00000000 0.007812
00100000 0.007812
10100000 0.015625
10110000 0.003906
00001000 0.062500
10001000 0.007812
01001000 0.007812
00101000 0.015625
10101000 0.007812
00011000 0.015625
01011000 0.007812
00111000 0.003906
11110100 0.003906
00001100 0.007812
00011100 0.007812
01011100 0.015625
01111100 0.003906
Actual 8-bit subkey of k2
00001000
```


## 5 round Integral property of PRESENT
```sh
gcc ./Integral/integral.c
./a.out
```

### Demo
```sh
gcc ./Integral/integral.c
./a.out
4096
```

Note: We are taking 4096 different messages and doing this experiment and output 4096 tell us that for 4 right most bit are balanced for all 4096 messages.


## Video Presentation Link
https://drive.google.com/drive/folders/12--ClgdB1mslO-HQLPnEm6TLUsSmjBeo?usp=sharing
