#include <systemc.h>
#include <iostream>
#include <bitset>
SC_MODULE(instruction_memory) {
    sc_int<25> inst[32];

    sc_in<bool> clock;
    sc_in<sc_int<25>> write_inst;
    sc_in<sc_uint<5>> current;
    sc_in<sc_uint<5>> last;
    sc_in<bool> sigWriteIM;

    sc_in<sc_int<5>> pointPC;
    sc_in<sc_int<1>> bubble;

    sc_out<sc_int<32>> dataOut;
    sc_out<sc_int<25>> inst1;
    sc_out<sc_int<25>> inst2;
    sc_out<sc_int<25>> inst3;
    sc_out<sc_int<25>> inst4;


    void out(){
        // em caso de erro: TODO - cheque isso
        inst1.write(inst[pointPC.read()]);
        inst2.write(inst[pointPC.read() + 1]);
        inst3.write(inst[pointPC.read() + 2]);
        inst4.write(inst[pointPC.read() + 3]);
    }

    void next_inst(){
        if(!bubble.read() && !sigWriteIM.read()){
            dataOut.write(inst[current.read()]);
        }else{
            dataOut.write(0); //TODO: se der ruim, verificar aqui
        }
    }

    void inst_put(){
        if(sigWriteIM.read()){
            inst[last.read()] = write_inst.read();
        }
        std::cout << "era para ter escrito:       " << std::bitset<25>(write_inst.read()) << std::endl;
        std::cout << "escreveu uma palavra:       " << std::bitset<25>(inst[last.read()]) << std::endl;
    }

    SC_CTOR(instruction_memory) {
        for (int i = 0; i < 32; i++) {
            inst[i] = 0;
        }
        SC_METHOD(inst_put);
        sensitive << write_inst;
        SC_METHOD(next_inst);
        sensitive << clk;
        SC_METHOD(out);
        sensitive << clk;
    }
};