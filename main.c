//
//  main.c
//  Fractal wave gen
//
//  Created by Carl Hudson on 20/01/2013.
//  Copyright (c) 2013 Carl Hudson. All rights reserved.
//

#include <stdio.h>


int outputWT[256];



void fractalWTGen(int potPos1, int potPos2, int potPos3, int WTtype) {
    
    
    // FRACTAL CODE STARTS HERE
    float maxVal=0;
    float maxVal2=0;
    
    float seed[3];
    
    float tempWT[256];
    
    float WTiteration2[256];
    float WTiteration3[256];
    
    
    
    
    //int WTtype=2;
    
    
    // WTtype is a switch:
    //
    // 0: use iteration 2 WT
    // 1: use iteration 3 TW
    // 2: use both (have to compress them by half and stitch together)
    
    
    
    
    
    int skipCounter=0;      // for selecting every second array entry
    
    
    float convPot1= (float) potPos1;
    float convPot2= (float) potPos2;
    float convPot3= (float) potPos3;
    
    for (int i=0; i<256; i++) {
        tempWT[i]=0;
        WTiteration2[i]=0;
        WTiteration3[i]=0;
    }
    
    // convert pot positions to [0.0 - 2.0]
    seed[0]=(convPot1/256)*2;
    seed[1]=(convPot2/256)*2;
    seed[2]=(convPot3/256)*2;
    
    
    // first iteration
    tempWT[0]=seed[0]*seed[0];
    tempWT[1]=seed[1]*seed[0];
    tempWT[2]=seed[2]*seed[0];
    
    
    // second iteration
    WTiteration2[0]=tempWT[0];
    WTiteration2[1]=tempWT[1];
    WTiteration2[2]=tempWT[2];
    for (int i=3; i<256; i++) {
        WTiteration2[i]=WTiteration2[i-3]*WTiteration2[1];
    }
    
    
    // third iteration
    WTiteration3[0]=WTiteration2[0];
    WTiteration3[1]=WTiteration2[1];
    WTiteration3[2]=WTiteration2[2];
    WTiteration3[3]=WTiteration2[3];
    WTiteration3[4]=WTiteration2[4];
    WTiteration3[5]=WTiteration2[5];
    WTiteration3[6]=WTiteration2[6];
    WTiteration3[7]=WTiteration2[7];
    WTiteration3[8]=WTiteration2[8];
    for (int i=9; i<256; i++) {
        WTiteration3[i]=WTiteration3[i-9]*WTiteration3[2];
    }
    
    ;
    
    
    // using WT type 1 or 2 (iteration 1 or 2)
    if (WTtype<2) {
        
        // generate output wavetable
        for (int i = 0; i<256; i++) {
            
            switch (WTtype) {
                    
                    // use iteration 2
                case(0):
                    tempWT[i]=WTiteration2[i];
                    break;
                    
                    // use iteration 3
                case(1):
                    tempWT[i]=WTiteration3[i];
                    break;
            }
            // find the maximum value
            if (tempWT[i]>maxVal) {
                maxVal=tempWT[i];
            }
        }
    } else {
        // using WT type 2 (both iterations A<>B)
        
        if (WTtype==2) {
            for (int j = 0; j<256; j++) {
                if (j<128) {
                    tempWT[j]=WTiteration3[skipCounter];
                    if (tempWT[j]>maxVal) {
                        maxVal=tempWT[j];
                    }
                } else {
                    tempWT[j]=WTiteration2[skipCounter-256];
                    if (tempWT[j]>maxVal2) {
                        maxVal2=tempWT[j];
                    }
                }
                skipCounter++;
                skipCounter++;
            }
            
        } else {
            // using WT type 3 (both iterations  B><A)
            for (int j = 0; j<256; j++) {
                
                if (j<128) {
                    tempWT[j]=WTiteration2[skipCounter];
                    if (tempWT[j]>maxVal) {
                        maxVal=tempWT[j];
                    }
                } else {
                    tempWT[j]=WTiteration3[skipCounter-256];
                    if (tempWT[j]>maxVal2) {
                        maxVal2=tempWT[j];
                    }
                }
                
                skipCounter++;
                skipCounter++;
            }
        }
        
    }
    
    
    // rescale output to maximum value and write to pot array
    
    // for WT mode 0/1
    if (WTtype<2) {
        for (int i=0; i<256; i++) {
            outputWT[i]= (float) (tempWT[i]/maxVal)*256;
        }
    }
    
    if (WTtype==2) {                                              // for WT mode 2 amplify A<>B
        for (int i=0; i<128; i++) {
            outputWT[i]= (float) (tempWT[i]/maxVal)*256;
        }
        for (int i=128; i<256; i++) {
            outputWT[i]= (float) (tempWT[i]/maxVal2)*256;
        }
    }
    
    if (WTtype==3)  {                                             // for WT mode 2 amplify B><A
        for (int i=0; i<128; i++) {
            outputWT[i]= (float) (tempWT[i]/maxVal)*256;
        }
        for (int i=128; i<256; i++) {
            outputWT[i]= (float) (tempWT[i]/maxVal2)*256;
        }
    }
    
    
}







void printResults() {
    
    for (int i=0; i<256; i++) {
        printf("%i\n",outputWT[i]);
    }
    
}



int main(int argc, const char * argv[])
{
    fractalWTGen(128, 256, 10, 3);  // seed 1, seed 2, seed 3, wavetable type (0-2)
    printResults();
    return 0;
}


