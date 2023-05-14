
#include "systemc.h"
#include <bitset>
//quebra a instrução em várias partes

//[0000][00000][00000][00000]
//[0000]
//[00000]
SC_MODULE(dataMem){
    sc_in<bool> clock;
    sc_in<sc_int<4>> opcode;
    sc_in<sc_int<10>> memAddr;
    sc_in<sc_int<32>> store_value;
    // sc_out<sc_int<4>> opcode;
    sc_int<32> mems[1024];
    // sc_int<25> reginst;
    sc_out<sc_int<32>> memOut;

    void spit(){
        if(clk == 1){
            if(opcode.read()==8){
                memOut.write( mems[memAddr.read()]);
            }
        }
    }
    void registr(){
        if(clk == 0){
            if(opcode.read() == 9){
                mems[memAddr.read()] = store_value.read();
            }
            //std::cout << "Clock not rising edge detected at " << sc_time_stamp() << std::endl;
            //std::cout << "reginst:       " << std::bitset<25>(reginst) << std::endl;
        }
    }


    SC_CTOR(dataMem) {
        SC_METHOD(registr);
		sensitive << clk;
        SC_METHOD(spit);
		sensitive << clk;
	}
};