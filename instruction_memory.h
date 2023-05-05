#include <systemc.h>
#include <iostream>

SC_MODULE(instruction_memory) {
    // sc_in_clk clk;
    sc_in<sc_uint<25>> write_inst;
    // sc_in<sc_uint<5>> current;
    // sc_in<sc_uint<5>> last;
    sc_uint<32> inst[32];
    // sc_in<bool> sigWriteIM;
    // sc_in<sc_uint<5>> pointPC;
    // sc_signal<bool> bubble;
    // sc_out<sc_uint<32>> dataOut;

    // void next_inst(){
    //     if(!bubble.read() && !sigWriteIM.read()){
    //         dataOut.write(inst[current.read()]);
    //     }else{
    //         dataOut.write(0); //TODO: se der ruim, verificar aqui
    //     }
    // }

    // void inst_put(){
    //     if(sigWriteIM.read()){
    //         inst[last.read()] = write_inst.read();
    //     }
    // }

    SC_CTOR(instruction_memory) {
        for (int i = 0; i < 32; i++) {
            inst[i] = 0;
        }
        // SC_METHOD(inst_put);
        // sensitive << write_inst;
        // SC_METHOD(next_inst);
        // sensitive << clk.pos();
    }
};
