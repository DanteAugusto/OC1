#include "systemc.h"
#include <bitset>
//quebra a instrução em várias partes

//[0000][00000][00000][00000]
//[0000]
//[00000]
SC_MODULE(bufferIfId){
    sc_in_clk clk;
    sc_in<sc_int<25>> inst;
    sc_int<25> reginst;
    sc_out<sc_int<4>> opcode;
    sc_out<sc_int<5>> rs;
    sc_out<sc_int<5>> rt;
    sc_out<sc_int<25>> instOut;

    void spit(){
        if(clk == 1)instOut.write(inst.read());
    }
    void registr(){
        if(clk == 0){
            reginst = inst.read();
            //std::cout << "Clock not rising edge detected at " << sc_time_stamp() << std::endl;
            //std::cout << "reginst:       " << std::bitset<25>(reginst) << std::endl;
        }
    }
    void decode(){
        std::bitset<25> instruc = std::bitset<25>(inst.read());
        //0001 00100 00011 00010
        //std::cout << "inst:       " << std::bitset<25>(inst.read()) << std::endl;
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
        //std::cout << std::endl;
        //std::cout << "a:       " << std::bitset<4>(a) << std::endl;
        //std::cout << "b:       " << std::bitset<5>(b) << std::endl;
        //std::cout << "c:       " << std::bitset<5>(c) << std::endl;
        // std::cout << "d:       " << std::bitset<5>(d) << std::endl;
        opcode.write(a);
        rs.write(b);
        rt.write(c);
    }


    SC_CTOR(bufferIfId) {
		SC_METHOD(decode);
		sensitive << inst;
        SC_METHOD(registr);
		sensitive << clk.pos();
        SC_METHOD(spit);
		sensitive << clk.pos();
	}
};