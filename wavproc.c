/*  ERGASIA 2 - wavproc.c  */
/*  Vekris Alexandros - AM 1115202000021  */
/* Implemented MODES: 1 */
#include <stdio.h>
#include <stdlib.h>
// #define MODE 1
int main(void){
    int flag, c, i, x, sum, j, k, p, SoF, MS, SR, Bs, Bal, BpS, SoD, c2, c3;
    //each variable is explained in its part below
    flag=1; c=82; i=1;
    //read "RIFF" part
    //'c' stores the value of letters R-I-F-F serially
    while (flag==1 && i<5){
        x=getchar();                //'x' stores the decimal value of each 0x-byte
        if (x==c && i==1)
            c=73;
        else if (x==c && (i==2 || i==3 || i==4))
            c=70;
        else
            flag=0;
        i++;       
    }
    if (flag==0){
        fprintf(stderr, "'RIFF' not found\n");
    }
    while (flag==1){
        //read "SizeOfFile" part
        SoF=0;
        for (i=0;i<7;i+=2){
            p=1;
            for (k=1;k<=i;k++)      //for eaxmple: 0xD4F563A2--> reading 0x-A2-63-F5-D4, so it is like we slided
                p*=16;              //each byte. Real value of D4 is D4*16*16*16*16. Same for all other bytes.   
            c=getchar();            //'c' stores the decimal value of each 0x-byte
            SoF=SoF+c*p;
        }
        fprintf(stderr, "Size of file: %d\n", SoF);
        //read "WAVE" part
        //'c' stores the value of letters W-A-V-E serially
        i=1; c=87;
        while (flag==1 && i<5){
            x=getchar();                    //'x' stores the decimal value of each 0x-byte
            if (x==c && i==1)
                c=65;
            else if (x==c && i==2)
                c=86;
            else if (x==c && i==3)
                c=69;
            else if (x==c && i==4){} //no action needed in this case...it is just to avoid the 'else' temporarily
            else
                flag=0;
            i++;       
        }
        if (flag==0){
            fprintf(stderr, "'WAVE' not found\n");
            continue;                                   //no need to search more in the .wav file
        }
        //read "fmt " part
        //'c' stores the value of letters f-m-t-'space' serially
        flag=1; i=1; c=102;
        while (flag==1 && i<5){
            x=getchar();                    //'x' stores the decimal value of each 0x-byte
            if (x==c && i==1)
                c=109;
            else if (x==c && i==2)
                c=116;
            else if (x==c && i==3)
                c=32;
            else if (x==c && i==4){} //no action needed in this case...it is just to avoid the 'else' temporarily
            else
                flag=0;
            i++;       
        }
        if (flag==0){
            fprintf(stderr, "'fmt ' not found\n");
            continue;                                       //no need to search more in the .wav file
        }
        //read format chunck (=16)
        sum=0;
        for (i=0;i<7;i+=2){
            p=1;
            for (k=1;k<=i;k++)   //calculation of each power of 16 
                p*=16;           //same explanation as line 29...SLIDING
            c=getchar();         //'c' stores the decimal valueof each 0x-byte
            sum=sum+p*c;         //'sum' is the size of format chunck
        }
        if (sum==16)
            fprintf(stderr, "Size of format chunk: %d\n", sum);
        else{
            fprintf(stderr, "Error! Size of format chunck should be 16.\n");
            flag==0;
            continue;
        }
        //read WAVE type format
        sum=0;
        for (i=0;i<3;i+=2){            
            p=1;
            for (k=1;k<=i;k++)     //calculation of each power of 16 
                p*=16;             //SLIDING
            c=getchar();           //'c' stores the decimal value of each 0x-byte
            sum=sum+p*c;           //'sum' is the WAVE type format
        }
        if (sum==1)
            fprintf(stderr, "WAVE type format: %d\n", sum);
        else{
            fprintf(stderr, "Error! WAVE type format should be 1.\n");
            flag==0;
            continue;
        }
        //read Mono/Stereo
        MS=0;
        for (i=0;i<3;i+=2){
            p=1;
            for (k=1;k<=i;k++)    //calculation of each power of 16 
                p*=16;            //SLIDIND
            c=getchar();          //'c' stores the decimal value of each 0x-byte
            MS=MS+p*c;            //'MS' is the Mono/Stereo type
        }
        if (MS==1 || MS==2)
            fprintf(stderr, "Mono/Stereo: %d\n", MS);
        else{
            fprintf(stderr, "Error! Mono/Stereo should be 1 or 2.\n");
            flag==0;
            continue;
        }
        //read Sample Rate
        SR=0;
        for (i=0;i<7;i+=2){
            p=1;
            for (k=1;k<=i;k++)    //calculation of each power of 16 
                p*=16;            //SLIDING
            c=getchar();          //'c' stores the decimal value of each 0x-byte
            SR=SR+p*c;            //'SR' is the Sample Rate
        }
        fprintf(stderr, "Sample Rate: %d\n", SR);
        //read Bytes/sec
        Bs=0;
        for (i=0;i<7;i+=2){       
            p=1;
            for (k=1;k<=i;k++)    //calculation of each power of 16 
                p*=16;            //SLIDING
            c=getchar();          //'c' stores the decimal value of each 0x-byte 
            Bs=Bs+p*c;            //'Bs' is the Bytes/sec
        }
        fprintf(stderr, "Bytes/sec: %d\n", Bs);
        //read Block Alignment
        Bal=0;
        for (i=0;i<3;i+=2){
            p=1;
            for (k=1;k<=i;k++)     //calculation of each power of 16 
                p*=16;             //SLIDING
            c=getchar();           //'c' stores the decimal value of each 0x-byte
            Bal=Bal+p*c;           //'Bal' is the Block Alignment
        }
        if (Bs==SR*Bal)
            fprintf(stderr, "Block Alignment: %d\n", Bal);
        else{
            fprintf(stderr, "Error! Bytes/sec should be equal to 'Sample Rate x Block Alignment'.\n");
            flag==0;
            continue;
        }
        //read Bits/sample
        BpS=0;
        for (i=0;i<3;i+=2){
            p=1;
            for (k=1;k<=i;k++)     //calculation of each power of 16 
                p*=16;             //SLIDING
            c=getchar();           //'c' stores the decimal value of each 0x-byte
            BpS=BpS+p*c;           //'BpS' is the Bits/sample
        }
        if (BpS==8 || BpS==16){
            fprintf(stderr, "Bits/sample: %d\n", BpS);
            if (Bal!=(BpS/8*MS)){
                fprintf(stderr, "Error! Block Alignment should be 'Bits per sample / 8 x Mono/Stereo'.\n");
                flag=0;
                continue;
            }
        }
        else{
            fprintf(stderr, "Error! Bits per sample should be 8 or 16.\n");
            flag=0;
            continue;
        }
        flag=1; c=100; i=1;
        //read "data" part
        //'c' stores the value of letters d-a-t-a serially
        while (flag==1 && i<5){
            x=getchar();                //'x' stores the decimal value of each 0x-byte
            if (x==c && i==1)
                c=97;
            else if (x==c && i==2) 
                c=116;
            else if (x==c && i==3)
                c=97;
            else if (x==c && i==4){}    //no action needed in this case...it is just to avoid the 'else' temporarily
            else
                flag=0;
            i++;       
        }
        if (flag==0){
            fprintf(stderr, "'data' not found\n");
            continue;
        }
        //read SizeOfData
        SoD=0;
        for (i=0;i<7;i+=2){
            p=1;
            for (k=1;k<=i;k++)     //calculation of each power of 16 
                p*=16;             //SLIDING
            c=getchar();           //'c' stores the decimal value of each 0x-byte
            SoD=SoD+p*c;           //'SoD' is the Size of data chunck
        }
        fprintf(stderr, "Size of data chunck: %d\n", SoD);
        i=0; c=69; k=0;
        while (flag==1){
            x=getchar();
            i++;
            if (x==EOF){
                i--;           //don't have to count the EOF character
                k=-1;          //reached EOF
                j=i;           //remember value of 'i' at EOF
                break;
            }
            else if (c==x && k==0){         //searching for the 'Extra Data' phrase in the 0x-bytes
                c==120; 
                k++;                        //counting the letters of 'Extra Data' found
            }
            else if (c==x && k==1){
                c=116;
                k++;
            }
            else if (c==x && k==2){
                c=114;
                k++;
            }
            else if (c==x && k==3){
                c=97;
                k++;
            }
            else if (c==x && k==4){
                c=68;
                k++;
            }
            else if (c==x && k==5){
                c2=97;
                k++;
            }
            else if (c2==x && k==6){
                c2=116;
                k++;
            }
            else if (c2==x && k==7){
                c3=97;
                k++;
            }
            else if (c3==x && k==8){
                j=i-9;
                k=-1;                  //found 'Extra Data' phrase
            }
            else{
                c=69;
                k=0;
            }
        }
        if (j<SoD && k==-1){         //reached EOF or found 'Extra Data' phrase
            fprintf(stderr, "Error! Insufficient data.\n");
            flag=0;                  //no need to search more in the .wav file
            continue;
        }
        else if (i+36>SoF){          //if we reached EOF
            fprintf(stderr, "Error! Bad file size.\n");
            flag=0;                  //no need to search more in the .wav file
            continue;               
        }
        flag=0;                      //process successfully finished
    }
    return 0;
}