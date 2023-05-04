#include <systemc.h>
#include <iostream>

SC_MODULE(instruction_memory) {
    sc_in<sc_uint<32>> write_inst;
    int index;
    sc_uint<32> inst[32];

    void inst_put(){
        inst[index] = write_inst.read();
        index++;
    }

    SC_CTOR(instruction_memory) {
        for (int i = 0; i < 32; i++) {
            inst[i] = 0;
        }
        index = 0;
    }
};