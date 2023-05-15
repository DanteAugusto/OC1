#include "systemc.h"
#include <bitset>
#include<unistd.h>
//add $4 $3 
// wriH  50
//[1101]  [0000000000000000]
// wriL $3 50
//[1110][00011][0000000000000000]
SC_MODULE(regbs){
    sc_in<bool> clk;
    sc_uint<32> regs[32];
    sc_uint<4> opcodereg;
    
    // sc_in<sc_uint<4>> opcode;
    sc_in<sc_uint<5>> op1;
    sc_in<sc_uint<5>> op2;
    sc_in<sc_uint<5>> memoryrequest;
    // sc_in<sc_uint<5>> op3;
    // sc_in<sc_uint<16>> value;
    sc_in<sc_uint<4>> opcode;
    sc_in<sc_uint<5>> posUla;
    sc_in<sc_uint<32>> ula_result;
    sc_in<sc_uint<1>> loadflag;
    sc_in<sc_uint<5>> loadpoint;
    sc_in<sc_uint<32>> load;
    // sc_in<sc_uint<16>> load;
    sc_out<sc_uint<32>> opout1;
    sc_out<sc_uint<32>> opout2;
    sc_out<sc_uint<32>> memoryget;
    void memory(){
        if(clk.read() == 1){
            memoryget.write(regs[memoryrequest.read()]);
        }
    }
    void save(){
        if(clk.read() == 0){
            sleep(1);
            std::cout << "/// BASE DE REGISTRADORES (SAÍDAS)///" << std::endl;
            std::cout << "opcode" << std::bitset<4>(opcodereg) << std::endl;
            std::cout << "vai atualizar o endereço " << std::bitset<5>(posUla.read()) << " para " << std::bitset<32>(ula_result.read()) << std::endl;
            if(((opcodereg > 0 && opcodereg < 8) || opcodereg == 13 
                    || opcodereg == 14)){
                // std::cout << "TÁ SALVANDO HEIN" << std::endl;
                regs[posUla.read()] = ula_result.read();
            }
            if(loadflag.read()==1 && 
               (loadpoint.read() != posUla.read() || 
               !((opcodereg > 0 && opcodereg < 8) || opcodereg == 0b1101 
                || opcode.read() == 0b1110))){
                regs[loadpoint.read()] = load.read();
            }
            std::cout << std::endl;
            sleep(1);
        }       

    }
    void get(){
        if(clk.read()==1){
            opout1.write(regs[op1.read()]);
            opout2.write(regs[op2.read()]);
            opcodereg = opcode.read();
            // sleep(1);
            // std::cout << "/// BASE DE REGISTRADORES (ENTRADAS)///" << std::endl;
            // std::cout << "opout 1 " << std::bitset<32>(regs[op1.read()]) << std::endl;
            // std::cout << "opout 2 " << std::bitset<32>(regs[op2.read()]) << std::endl;
            // std::cout << "op 1 " << std::bitset<5>(op1.read()) << std::endl;
            // std::cout << "op 2 " << std::bitset<5>(op2.read()) << std::endl;            
            // std::cout << std::endl;
            // sleep(1);
        }
            
    }
    SC_CTOR(regbs) {
        opcodereg = 0;
        SC_METHOD(save);
		sensitive << clk;        
        SC_METHOD(get);
		sensitive << clk;
        SC_METHOD(memory);
		sensitive << clk;
	}
};