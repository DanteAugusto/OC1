#include "systemc.h"
#include <bitset>
//quebra a instrução em várias partes

//[0000][00000][00000][00000]
//[0000]
//[00000]
SC_MODULE(pc){
    sc_in_clk clk;
    sc_in<sc_int<1>> isjump;
    sc_in<sc_uint<5>> jump;
    sc_in<sc_int<25>> inst1;
    sc_in<sc_uint<25>> inst2;
    sc_in<sc_int<25>> inst3;
    sc_in<sc_uint<25>> inst4;
    sc_int<2> regbubble;
    sc_uint<5> regpointer;
    sc_out<sc_int<1>> bubbleOut;
    sc_out<sc_int<5>> pointerOut;

    void lowbubble(){
        if(clk == 0){
            if(regbubble > 0){
                regbubble = regbubble - 1;
                bubbleOut.write(1);
            }
        }
    }
    void jumps(){
        if(clk == 1){
            if(isjump.read() == 1){
                std::bitset<25> instruc = std::bitset<25>(inst1);
                //0001001000001100010            
                // ld pointerOut memAddr
                int x = 0
                for(int i = 0; i < 4; i++){
                    x |= (instruc[24-i] ? 1 : 0) << (4-i);        
                }
                instruc = std::bitset<25>(inst2);
                int y = 0;
                for(int i = 0; i < 4; i++){
                    y |= (instruc[24-i] ? 1 : 0) << (4-i);        
                }
                if(x == 11 || y == 12){
                    regbubble = 1;
                }
                regpointer = jump.read();
            }
        }
    }

    void hazards(){
        if(clk == 1){
            if(regbubble == 0 && isjump.read() == 0){
                std::bitset<25> instruc = std::bitset<25>(inst1.read());
                std::bitset<25> instruc2 = std::bitset<25>(inst2.read());
                std::bitset<25> instruc3 = std::bitset<25>(inst3.read());
                std::bitset<25> instruc4 = std::bitset<25>(inst4.read());
                int opc1 = 0;
                int opc2 = 0;
                int opc3 = 0;
                int opc4 = 0;
                for(int i = 0; i < 4; i++){
                    opc1 |= (instruc[24-i] ? 1 : 0) << (4-i);        
                }
                for(int i = 0; i < 4; i++){
                    opc2 |= (instruc2[24-i] ? 1 : 0) << (4-i);        
                }
                for(int i = 0; i < 4; i++){
                    opc3 |= (instruc3[24-i] ? 1 : 0) << (4-i);        
                }
                for(int i = 0; i < 4; i++){
                    opc4 |= (instruc4[24-i] ? 1 : 0) << (4-i);        
                }
                // if((opc1 > 0 && opc1 < 8) || opc1 = 13 || opc1 = 14){

                // }
                int outinst1 = 0;
                if(opc1 > 0 && opc1 < 8 && opc1 != 4){
                    for(int i = 14; i < 19; i++){
                        outinst1 |= (instruc[24-i] ? 1 : 0) << (4-(i-14));        
                    }
                }else if(opc1 == 4){
                    for(int i = 9; i < 14; i++){
                        outinst1 |= (instruc[24-i] ? 1 : 0) << (4-(i-9));        
                    }
                }else if(opc1 == 13 || opc1 == 14){
                    for(int i = 4; i < 9; i++){
                        outinst1 |= (instruc[24-i] ? 1 : 0) << (4-(i-4));        
                    }
                }
                int in1op2, in2op2;
                in1op2 = 0;
                in2op2 = 0;            
                if(opc2 > 0 && opc2 < 8 && opc2 != 4){
                    for(int i = 4; i < 9; i++){
                        in1op2 |= (instruc2[24-i] ? 1 : 0) << (4-(i-4));        
                    }
                    for(int i = 9; i < 14; i++){
                        in2op2 |= (instruc2[24-i] ? 1 : 0) << (4-(i-9));        
                    }
                }else if(opc2 == 4 || opc2 == 12 || opc2 == 11){
                    for(int i = 4; i < 9; i++){
                        in1op2 |= (instruc2[24-i] ? 1 : 0) << (4-(i-4));        
                    }
                }
                if((outinst1 == in1op2) && 
                ((opc1 > 0 && opc1 < 8) || opc1 == 13 || opc1 == 14) &&
                ((opc2 > 0 && opc2 < 8) || opc2 == 11 || opc2 == 12)){
                    regbubble = 1;
                }
                if((outinst1 == in2op2) && 
                ((opc1 > 0 && opc1 < 8) || opc1 == 13 || opc1 == 14) &&
                ((opc2 > 0 && opc2 < 8 && opc2 != 4))){
                    regbubble = 1;
                }
                if((opc1 == 8)){
                    for(int i = 4; i < 9; i++){
                        outinst1 |= (instruc[24-i] ? 1 : 0) << (4-(i-4));        
                    }
                }
                int in1op3, in2op3;
                in1op3 = 0;
                in2op3 = 0;
                if(opc3 > 0 && opc3 < 8 && opc3 != 4){
                    for(int i = 4; i < 9; i++){
                        in1op3 |= (instruc3[24-i] ? 1 : 0) << (4-(i-4));        
                    }
                    for(int i = 9; i < 14; i++){
                        in2op3 |= (instruc3[24-i] ? 1 : 0) << (4-(i-9));        
                    }
                }else if(opc3 == 4 || opc3 == 12 || opc3 == 11){
                    for(int i = 4; i < 9; i++){
                        in1op3 |= (instruc3[24-i] ? 1 : 0) << (4-(i-4));        
                    }
                }
                int in1op4, in2op4;
                in1op4 = 0;
                in2op4 = 0;
                if(opc4 > 0 && opc4 < 8 && opc4 != 4){
                    for(int i = 4; i < 9; i++){
                        in1op4 |= (instruc4[24-i] ? 1 : 0) << (4-(i-4));        
                    }
                    for(int i = 9; i < 14; i++){
                        in2op4 |= (instruc4[24-i] ? 1 : 0) << (4-(i-9));        
                    }
                }else if(opc4 == 4 || opc4 == 12 || opc4 == 11){
                    for(int i = 4; i < 9; i++){
                        in1op4 |= (instruc4[24-i] ? 1 : 0) << (4-(i-4));        
                    }
                }
                if(opc1 == 8 && outinst1 == in1op2 && (opc2 == 11 || opc2 == 12 || (opc2 > 0 && opc2 < 8))){
                    regbubble = 3;
                }else if(opc1 == 8 && outinst1 == in2op2 && (opc2 > 0 && opc2 < 8 && opc2 != 4)){
                    regbubble = 3;
                }else if(opc1 == 8 && outinst1 == in1op3 && (opc3 == 11 || opc3 == 12 || (opc3 > 0 && opc3 < 8))){
                    regbubble = 2;
                }else if(opc1 == 8 && outinst1 == in2op3 && (opc3 > 0 && opc3 < 8 && opc3 != 4)){
                    regbubble = 2;
                }else if(opc1 == 8 && outinst1 == in1op4 && (opc4 == 11 || opc4 == 12 || (opc4 > 0 && opc4 < 8))){
                    regbubble = 1;
                }else if(opc1 == 8 && outinst1 == in2op4 && (opc4 > 0 && opc4 < 8 && opc4 != 4)){
                    regbubble = 1;
                }
                

                regpointer = regpointer +1;
            }else{
                bubbleOut.write(1);
                regbubble = regbubble - 1;
            }
        }
    }

    

    SC_CTOR(pc) {
        regbubble = 0;
        regpointe = 0;
        SC_METHOD(hazards);
		sensitive << clk.pos();
        SC_METHOD(jumps);
		sensitive << clk.pos();
        SC_METHOD(lowbubble);
		sensitive << clk.pos();
	}
};