
#include "systemc.h"
#include <bitset>
//quebra a instrução em várias partes

//[0000][00000][00000][00000]
//[0000]
//[00000]
SC_MODULE(bufferIdEx){
    sc_int<25> reginst;
    sc_int<32> regop1;
    sc_int<32> regop2;

    sc_in<bool> clock;
    sc_in<sc_int<25>> inst;
    sc_in<sc_int<32>> op1;
    sc_in<sc_int<32>> op2;
    sc_out<sc_int<4>> opcode;
    sc_out<sc_int<5>> PCadd;
    sc_out<sc_int<5>> posresult;
    sc_out<sc_int<25>> instOut;
    sc_out<sc_int<32>> op1Out;
    sc_out<sc_int<32>> op2Out;

    void spit(){
        if(clk == 1){
            instOut.write(reginst);
            op1Out.write(regop1);
            if(opcode.read() == 13 || opcode.read() == 14){
                std::bitset<25> instruc = std::bitset<25>(inst);
                int x = 0;
                for(int i = 9; i < 25; i++){
                    value |= (instruc[24-i] ? 1 : 0) << (15-(i-9));
                }
                x = (x << 16) + x;
                op2Out.write(x);
            }else{
                op2Out.write(regop2);
            }

            sc_int<4> x = reginst >> 21;
            opcode.write(x);
            int y = 0;
            int z = 0;
            std::bitset<25> instruc = std::bitset<25>(inst);
            //0001 00100 00011 00010
            for(int i = 9; i < 14; i++){
                y |= (instruc[24-i] ? 1 : 0) << (4-(i-9));
            }
            for(int i = 14; i < 19; i++){
                z |= (instruc[24-i] ? 1 : 0) << (4-(i-14));
            }
            PCadd.write(y);
            posresult.write(z);
        }
    }
    void registr(){
        if(clk == 0){
            reginst = inst.read();
            regop1 = op1.read();
            regop2 = op2.read();
            //std::cout << "Clock not rising edge detected at " << sc_time_stamp() << std::endl;
            //std::cout << "reginst:       " << std::bitset<25>(reginst) << std::endl;
        }
    }


    SC_CTOR(bufferIdEx) {
        SC_METHOD(registr);
		sensitive << clk;
        SC_METHOD(spit);
		sensitive << clk;
	}
};