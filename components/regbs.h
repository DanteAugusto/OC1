#include "systemc.h"
#include <bitset>
//add $4 $3
// wriH  50
//[1101]  [0000000000000000]
// wriL $3 50
//[1110][00011][0000000000000000]
SC_MODULE(regbs){
    sc_in<bool> clock;
    sc_int<32> regs[32];

    sc_in<sc_int<4>> opcode;
    sc_in<sc_int<5>> op1;
    sc_in<sc_int<5>> op2;
    sc_in<sc_int<5>> memoryrequest;
    // sc_in<sc_int<5>> op3;
    // sc_in<sc_int<16>> value;
    sc_in<sc_int<4>> ula_opcode;
    sc_in<sc_int<5>> posUla;
    sc_in<sc_int<32>> ula_result;
    sc_in<sc_int<1>> loadflag;
    sc_in<sc_int<5>> loadpoint;
    sc_in<sc_int<32>> load;
    // sc_in<sc_int<16>> load;
    sc_out<sc_int<32>> opout1;
    sc_out<sc_int<32>> opout2;
    sc_out<sc_int<32>> memoryget;
    void memory(){
        if(clk == 1){
            memoryget.write(regs[memoryrequest.read()]);
        }
    }
    void save(){
        if(clk == 0){
            if(((ula_opcode.read() > 0 && ula_opcode.read() < 8) || opcode.read() == 0b1101
                    || opcode.read() == 0b1110)){
                regs[posUla.read()] = ula_result.read();
            }
            if(loadflag.read()==1 &&
               (loadpoint.read() != posUla.read() ||
               !((ula_opcode.read() > 0 && ula_opcode.read() < 8) || opcode.read() == 0b1101
                || opcode.read() == 0b1110))){
                regs[loadpoint.read()] = load.read();
            }
        }

    }
    void get(){
        if(clk==1){
            opout1.write(regs[op1.read()]);
            opout2.write(regs[op2.read()]);
        }

    }
    SC_CTOR(regbs) {
        SC_METHOD(save);
		sensitive << clk;
        SC_METHOD(get);
		sensitive << clk;
        SC_METHOD(memory);
		sensitive << clk;
	}
};