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
    sc_int<32> regs[32];
    
    sc_in<sc_uint<4>> opcode;
    sc_in<sc_int<5>> op1;
    sc_in<sc_int<5>> op2;
    sc_in<sc_int<5>> memoryrequest;
    // sc_in<sc_int<5>> op3;
    // sc_in<sc_int<16>> value;
    sc_in<sc_uint<4>> ula_opcode;
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
        if(clk.read() == 1){
            memoryget.write(regs[memoryrequest.read()]);
        }
    }
    void save(){
        if(clk.read() == 0){
            sleep(1);
            std::cout << "/// BASE DE REGISTRADORES (SAÍDAS)///" << std::endl;
            std::cout << "vai atualizar o endereço " << std::bitset<5>(posUla.read()) << " para " << std::bitset<32>(ula_result.read()) << std::endl;
            std::cout << std::endl;
            sleep(1);
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
        if(clk.read()==1){
            opout1.write(regs[op1.read()]);
            opout2.write(regs[op2.read()]);
            sleep(1);
            std::cout << "/// BASE DE REGISTRADORES (ENTRADAS)///" << std::endl;
            std::cout << "vai atualizar o endereço " << std::bitset<32>(opout1.read()) << std::endl;
            std::cout << "vai atualizar o endereço " << std::bitset<32>(opout2.read()) << std::endl;
            std::cout << std::endl;
            sleep(1);
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