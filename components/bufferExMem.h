
#include "systemc.h"
#include <bitset>
//quebra a instrução em várias partes

//[0000][00000][00000][00000]
//[0000]
//[00000]
SC_MODULE(bufferExMem){
    sc_in<bool> clk;
    sc_in<sc_int<25>> inst;
    sc_in<sc_int<32>> ula_result;
    // sc_out<sc_int<4>> opcode;
    sc_int<25> reginst;
    sc_int<25> regula_result;
    
    sc_out<sc_int<10>> memAddr;
    sc_out<sc_int<4>> opcode;
    sc_out<sc_int<5>> pointerOut;
    sc_out<sc_int<25>> instOut;


    void spit(){
        if(clk.read() == 1){
            instOut.write(reginst);            
            sc_int<4> x = reginst >> 21;
            opcode.write(x);
            int y = 0;
            int z = 0;
            std::bitset<25> instruc = std::bitset<25>(reginst);
            //0001001000001100010            
            // ld pointerOut memAddr
            for(int i = 9; i < 19; i++){
                y |= (instruc[24-i] ? 1 : 0) << (9-(i-9));        
            }
            for(int i = 4; i < 9; i++){
                z |= (instruc[24-i] ? 1 : 0) << (4-(i-4));        
            }
            memAddr.write(y);
            pointerOut.write(z);
        }
    }
    void registr(){
        if(clk.read() == 0){
            reginst = inst.read();
            regula_result = ula_result.read();
            //std::cout << "Clock not rising edge detected at " << sc_time_stamp() << std::endl;
            //std::cout << "reginst:       " << std::bitset<25>(reginst) << std::endl;
        }
    }
    

    SC_CTOR(bufferExMem) {
        SC_METHOD(registr);
		sensitive << clk;
        SC_METHOD(spit);
		sensitive << clk;
	}
};