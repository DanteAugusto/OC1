
#include "systemc.h"
#include <bitset>
#include<unistd.h>
//quebra a instrução em várias partes

//[0000][00000][00000][00000]
//[0000]
//[00000]
SC_MODULE(bufferIdEx){
    sc_uint<25> reginst;
    sc_uint<32> regop1;
    sc_uint<32> regop2;
    
    sc_in<bool> clk;
    sc_in<sc_uint<25>> inst;
    sc_in<sc_uint<32>> op1;
    sc_in<sc_uint<32>> op2;
    sc_out<sc_uint<4>> opcode;
    sc_out<sc_uint<5>> PCadd;
    sc_out<sc_uint<5>> posresult;
    sc_out<sc_uint<25>> instOut;
    sc_out<sc_uint<32>> op1Out;
    sc_out<sc_uint<32>> op2Out;

    void spit(){
        if(clk.read() == 1){
            sleep(1);
            std::cout << "~~~~ BUFFER IDEX (SAÍDAS)~~~~" << std::endl;

            instOut.write(reginst);
            op1Out.write(regop1);
            if(opcode.read() == 13 || opcode.read() == 14){
                std::bitset<25> instruc = std::bitset<25>(inst.read());
                unsigned int x = 0;
                for(int i = 9; i < 25; i++){
                    x |= (instruc[24-i] ? 1 : 0) << (15-(i-9));        
                }
                x = (x << 16) + x;
                op2Out.write(x);
            }else{
                op2Out.write(regop2);
            }

            sc_uint<4> x = reginst >> 21;
            opcode.write(x);
            int y = 0;
            int z = 0;
            std::bitset<25> instruc = std::bitset<25>(inst.read());
            //0001 00100 00011 00010
            for(int i = 9; i < 14; i++){
                y |= (instruc[24-i] ? 1 : 0) << (4-(i-9));        
            }
            for(int i = 14; i < 19; i++){
                z |= (instruc[24-i] ? 1 : 0) << (4-(i-14));        
            }
            PCadd.write(y);
            posresult.write(z);
            std::cout << "instrução  " << std::bitset<25>(inst.read()) << std::endl;
            //std::cout << "primeiro operando " << std::bitset<32>(op1Out.read()) << std::endl;
            //std::cout << "segundo operando " << std::bitset<32>(op2Out.read()) << std::endl;
            std::cout << std::endl;
            sleep(1);
        }
    }
    void registr(){
        if(clk.read() == 0){
            reginst = inst.read();
            regop1 = op1.read();
            regop2 = op2.read();
            sleep(1);
            std::cout << "~~~~ BUFFER IDEX (ENTRADAS) ~~~~" << std::endl;
            std::cout << "instrução  " << std::bitset<25>(inst.read()) << std::endl;
            //std::cout << "primeiro operando " << std::bitset<32>(op1.read()) << std::endl;
            //std::cout << "segundo operando " << std::bitset<32>(op2.read()) << std::endl;
            std::cout << std::endl;
            sleep(1);
            ////std::cout << "Clock not rising edge detected at " << sc_time_stamp() << std::endl;
            ////std::cout << "reginst:       " << std::bitset<25>(reginst) << std::endl;
        }
    }
    

    SC_CTOR(bufferIdEx) {
        SC_METHOD(registr);
		sensitive << clk;
        SC_METHOD(spit);
		sensitive << clk;
	}
};