#include "systemc.h"
#include <bitset>
#include<unistd.h>
//quebra a instrução em várias partes

//[0000][00000][00000][00000]
//[0000]
//[00000]
SC_MODULE(bufferIfId){
    sc_in<bool> clk;
    sc_in<sc_uint<25>> inst;
    sc_uint<25> reginst;
    sc_out<sc_uint<4>> opcode;
    sc_out<sc_uint<5>> rs;
    sc_out<sc_uint<5>> rt;
    sc_out<sc_uint<25>> instOut;

    void spit(){
        if(clk.read() == 1){
            instOut.write(inst.read());
            //std::cout << "BUFFER IFID (SAÍDA)" << std::endl;
            //std::cout << "entra aqui?" << std::endl;
            //std::cout << "instOut (dataOut do IM):       " << std::bitset<25>(instOut.read()) << std::endl;
            //std::cout << std::endl;
            sleep(1);
        }
    }
    void registr(){
        if(clk.read() == 0){
            reginst = inst.read();
            ////std::cout << "Clock not rising edge detected at " << sc_time_stamp() << std::endl;
            ////std::cout << "reginst:       " << std::bitset<25>(reginst) << std::endl;
        }
    }
    void decode(){
        if(clk == 1){
            //std::cout << "BUFFER IFID (ENTRADA)" << std::endl;
            //std::cout << "inst (dataOut do IM):       " << std::bitset<25>(inst.read()) << std::endl;
            std::bitset<25> instruc = std::bitset<25>(inst.read());
            //0001 00100 00011 00010
            int a = 0;
            int b = 0;
            int c = 0;
            int d = 0;
            for(int i = 0; i < 25; i++){
            //  std:: cout  <<instruc[24-i];
                if(i < 4){
                    a |= (instruc[24-i] ? 1 : 0) << (3-i);
                } else if(i >= 4 && i < 9){
                    b |= (instruc[24-i] ? 1 : 0) << (4-(i-4));
                }else if(i >= 9 && i < 14){
                    c |= (instruc[24-i] ? 1 : 0) << (4-(i-9));
                }else if(i >= 14 && i < 25){
                    d |= (instruc[24-i] ? 1 : 0) << (4-(i-14));
                }
            }
            ////std::cout << std::endl;
            //std::cout << "opcode:       " << std::bitset<4>(a) << std::endl;
            //std::cout << "rs:       " << std::bitset<5>(b) << std::endl;
            //std::cout << "rt:       " << std::bitset<5>(c) << std::endl;
            // //std::cout << "d:       " << std::bitset<5>(d) << std::endl;
            //std::cout << std::endl;
            sleep(1);
            opcode.write(a);
            rs.write(b);
            rt.write(c);
        }
    }


    SC_CTOR(bufferIfId) {
		SC_METHOD(decode);
		sensitive << clk;
        SC_METHOD(registr);
		sensitive << clk;
        SC_METHOD(spit);
		sensitive << clk;
	}
};