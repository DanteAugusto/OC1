#include "systemc.h"
#include <bitset>
//quebra a instrução em várias partes

SC_MODULE(decoder){
    sc_in<sc_uint<25>> inst;

    sc_out<sc_uint<4>> opcode;
    sc_out<sc_uint<5>> rs;
    sc_out<sc_uint<4>> rt;
    sc_out<sc_uint<4>> rd;
    void decode(){
        std::bitset<25> instruc = std::bitset<25>(inst.read());
        //0001 00100 00011 00010
        std::size_t n = instruc.size();
        int a = 0;
        int b = 0;
        int c = 0;
        for(int i = 0; i < n; i++){
            if(i < 4){
                a |= (inst_code[i] ? 1 : 0) << (3-i);
            } else if(i >= 4 && i < 9){

            }
        }
        opcode.write()
    }


    SC_CTOR(decoder) {
		SC_METHOD(decode);
		sensitive << inst;
	}
};
