#include <systemc.h>
#include <iostream>
#include <bitset>
#include<unistd.h>
SC_MODULE(instruction_memory) {
    sc_uint<25> inst[32];
    sc_uint<5> last;

    sc_in<bool> clk;
    sc_in<sc_uint<25>> write_inst;
    sc_in<bool> sigWriteIM;

    sc_in<sc_uint<5>> pointPC;
    sc_in<sc_uint<1>> bubble;
    
    sc_out<sc_uint<25>> dataOut;
    sc_out<sc_uint<25>> inst1;
    sc_out<sc_uint<25>> inst2;
    sc_out<sc_uint<25>> inst3;
    sc_out<sc_uint<25>> inst4;


    void out(){
        // em caso de erro: TODO - cheque isso
        
        // std::cout << "você fez o que deveria?" << std::endl;
        if(!sigWriteIM.read()){
            inst1.write(inst[pointPC.read()]);
            if(pointPC.read() + 1 > pointPC.read()) inst2.write(inst[pointPC.read() + 1]);
            if(pointPC.read() + 2 > pointPC.read()) inst3.write(inst[pointPC.read() + 2]);
            if(pointPC.read() + 3 > pointPC.read()) inst4.write(inst[pointPC.read() + 3]);
        }
    }

    void next_inst(){
        // std::cout << "sigWriteIM é " << sigWriteIM.read() << std::endl;
        // std::cout << "bubble é " << bubble.read() << std::endl;
        if(clk.read() == 1){
            std::cout << "Clock subiu" << std::endl;
            // sleep(1);
            std::cout << "ENTRADAS DE INSTRUCTION_MEMORY" << std::endl;
            std::cout << "pointPC: " <<std::bitset<25>(pointPC.read()) << std::endl;
            std::cout << "bubble: " <<std::bitset<25>(bubble.read()) << std::endl;
            // sleep(1);
            if(!bubble.read() && !sigWriteIM.read()){
                std::cout << "cenoura " << sigWriteIM.read() << std::endl;
                std::cout << "vai enviar a instrução " << std::bitset<25>(inst[pointPC.read()]) << std::endl;
                // std::cout << "pointPC: " << pointPC.read() <<std::endl;
                dataOut.write(inst[pointPC.read()]);
            }else{
                dataOut.write(0); //TODO: se der ruim, verificar aqui
                // std::cout << "repolho " << sigWriteIM.read() << std::endl;
            }
            std::cout << "SAÍDAS DE INSTRUCTION_MEMORY" << std::endl;
            std::cout << "dataOut: " <<std::bitset<25>(dataOut.read()) << std::endl;
        }        
    }

    void inst_put(){
        std::cout << "era para ter escrito:       " << std::bitset<25>(write_inst.read()) << " no endereço " << std::bitset<25>(last)  << std::endl;
        if(sigWriteIM.read()){
            inst[last] = write_inst.read();
            std::cout << "escreveu uma palavra:       " << std::bitset<25>(inst[last]) << " no endereço " << std::bitset<25>(last)  << std::endl;
            last = last + 1;
        }
    }

    SC_CTOR(instruction_memory) {

        last = 0;
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