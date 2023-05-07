
#include "systemc.h"
#include <bitset>
//quebra a instrução em várias partes

//[0000][00000][00000][00000]
//[0000]
//[00000]
SC_MODULE(bufferMemWb){
    sc_in_clk clk;
    sc_in<sc_int<4>> opcode;
    sc_in<sc_int<5>> pointer;
    sc_in<sc_int<32>> wb;

    sc_int<4> regopcode;
    sc_int<5> regpointer;
    sc_int<32> regwb;

    sc_out<sc_int<1>> enable;
    sc_out<sc_int<5>> pointerOut;
    sc_out<sc_int<32>> wbOut;

    void spit(){
        if(clk == 1){
            pointerOut.write(regpointer);
            wbOut.write(regwb);
            if(opcode.read() == 8){
                enable.write(1);
            }else{
                enable.write(0);
            }
        }
    }
    void registr(){
        if(clk == 0){
            regopcode = opcode.read();
            regpointer = pointer.read();
            regwb = wb.read();
            //std::cout << "Clock not rising edge detected at " << sc_time_stamp() << std::endl;
            //std::cout << "reginst:       " << std::bitset<25>(reginst) << std::endl;
        }
    }
    

    SC_CTOR(bufferMemWb) {
        SC_METHOD(registr);
		sensitive << clk.pos();
        SC_METHOD(spit);
		sensitive << clk.pos();
	}
};