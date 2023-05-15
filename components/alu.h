#include <systemc.h>
#include <iostream>
#include <bitset> 
#include<unistd.h>
//[0000][100100][000100]
SC_MODULE(alu) {
    sc_in<sc_int<32>> op1;
    sc_in<sc_int<32>> op2;
    // sc_in<sc_int<32>> value;
    sc_in<sc_uint<4>> opcode;
    sc_out<sc_uint<1>> confirmPC;
    // sc_out<sc_int<5>> resultPC;
    sc_out<sc_int<32>> result;

    void compute() {
        confirmPC.write(0);
        sleep(1);
        std::cout << "+%$ ULA +%$" << std::endl;
        std::cout << "opcode " << std::bitset<4>(opcode.read()) << std::endl;
        int x = opcode.read();
        // std::cout << "opcode como um inteiro " << x << std::endl;
        //std::cout << "primeiro operando " << std::bitset<32>(op1.read()) << std::endl;
        //std::cout << "segundo operando " << std::bitset<32>(op2.read()) << std::endl;
        if(x == 1){
            result.write(op1.read() & op2.read());
        }else if(x == 2){
            result.write(op1.read() | op2.read());
        }else if(x == 3){
            result.write(op1.read() ^ op2.read());
        }else if(x == 4){
            result.write(~op1.read());
        }else if(x == 5){
            result.write(op1.read() == op2.read());
        }else if(x == 6){
            result.write(op1.read() + op2.read());
        }else if(x == 7){
            result.write(op1.read() - op2.read());
        }else if(x == 9){
            result.write(op1.read());
        }else if(x == 10){
            // std::cout << "DEEEEEEEEEEEEEEZ" << std::endl;
            confirmPC.write(1);
        }else if(x == 11){
            if(op1.read()< 0){
                confirmPC.write(1);
            }
        }else if(x == 12){
            if(op1.read()== 0){
                confirmPC.write(1);
            }
        }else if(x == 13){
            int a = op2.read();
            a = (a >> 16) << 16;
            int y = op1.read();
            int y0 = (y >> 16) << 16;
            y = y - y0;
            std::cout << "wrih " << std::bitset<32>(a + y) << std::endl;
            result.write(a + y); 
        }else if(x == 14){
            int z = op1.read();
            z = (z >> 16) << 16;
            int w = op2.read();
            int w0 = (w >> 16) << 16;
            w = w - w0;
            std::cout << "wril " << std::bitset<32>(z + w) << std::endl;
            result.write(z + w); 
        }
        //std::cout << "resultado " << std::bitset<32>(result.read()) << std::endl;              
                
        // switch (opcode.read()) {
        //     case 1: // and
        //         // cout << "AND" <<endl;
        //         result.write(op1.read() & op2.read());
        //         // zero.write(result.read() == 0);
        //         break;
        //     case 2: // or
        //         // cout << "OR" <<endl;
        //         result.write(op1.read() | op2.read());
        //         // zero.write(result.read() == 0);
        //         break;
        //     case 3: // xor
        //         // cout << "XOR" <<endl;
        //         result.write(op1.read() ^ op2.read());
        //         // zero.write(result.read() == 0);
        //         break;
        //     case 4: // not
        //         // cout << "NOT" <<endl;
        //         result.write(!op1.read());
        //         // zero.write(result.read() == 0);
        //         break;
        //     case 5: // cmp
        //         // cout << "CMP" <<endl;
        //         result.write(op1.read() == op2.read());
        //         //zero.write(result.read() == 0);
        //         break;
        //     case 6: // add
        //         // cout << "ADD" <<endl;
        //         result.write(op1.read() + op2.read());
        //         //zero.write(result.read() == 0);
        //         break;
        //     case 7: // sub
        //         // cout << "SUB" <<endl;
        //         result.write(op1.read() - op2.read());
        //         //zero.write(result.read() == 0);
        //         break;
        //     case 9: // st 
        //         // cout << "ST" <<endl;
        //         result.write(op1.read());
        //         //zero.write(result.read() == 0);
        //         break;
        //     case 10: // j
        //         confirmPC.write(1);
        //         break;
        //     case 11: // jn
        //         if(op1.read()< 0){
        //             confirmPC.write(1);
        //         }
        //         break;
        //     case 12: // jz
        //         // cout << "JZ" <<endl;
        //         if(op1.read()== 0){
        //             confirmPC.write(1);
        //         }
        //         // zero.write(result.read() == 0);
        //         break;
        //     case 13: // wrih
        //         // cout << "WRIH" <<endl;
        //         int x = op2.read();
        //         x = (x >> 16) << 16;
        //         int y = op1.read();
        //         int y0 = (y >> 16) << 16;
        //         y = y - y0;
        //         result.write(x + y); 
        //         // zero.write(result.read() == 0);
        //         break;
        //     case 14: // wril
        //         // cout << "WRIL" <<endl;                
        //         int z = op1.read();
        //         z = (z >> 16) << 16;
        //         int w = op2.read();
        //         int w0 = (w >> 16) << 16;
        //         w = w - w0;
        //         result.write(z + w); 
        //         // zero.write(result.read() == 0);
        //         break;
        //     default:
        //         // cout << "DEFAULT" <<endl;
        //         // result.write(0);
        //         break;
        
    }

    SC_CTOR(alu) {
        SC_METHOD(compute);
        sensitive << op1 << op2 << opcode;
    }
};
