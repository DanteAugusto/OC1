#include <systemc.h>
#include <iostream>
//[0000][100100][000100]
SC_MODULE(alu) {
    sc_in<sc_int<32>> op1;
    sc_in<sc_int<32>> op2;
    sc_in<sc_int<32>> value;
    sc_in<sc_int<4>> opcode;
    sc_out<sc_int<1>> confirmPC;
    // sc_out<sc_int<5>> resultPC;
    sc_out<sc_int<32>> result;

    void compute() {
        confirmPC.write(0);
        switch (opcode.read()) {
            case 1: // and
                cout << "AND" <<endl;
                result.write(op1.read() & op2.read());
                // zero.write(result.read() == 0);
                break;
            case 2: // or
                cout << "OR" <<endl;
                result.write(op1.read() | op2.read());
                // zero.write(result.read() == 0);
                break;
            case 3: // xor
                cout << "XOR" <<endl;
                result.write(op1.read() ^ op2.read());
                // zero.write(result.read() == 0);
                break;
            case 4: // not
                cout << "NOT" <<endl;
                result.write(!op1.read());
                // zero.write(result.read() == 0);
                break;
            case 5: // cmp
                cout << "CMP" <<endl;
                result.write(op1.read() == op2.read());
                //zero.write(result.read() == 0);
                break;
            case 6: // add
                cout << "ADD" <<endl;
                result.write(op1.read() + op2.read());
                //zero.write(result.read() == 0);
                break;
            case 7: // sub
                cout << "SUB" <<endl;
                result.write(op1.read() - op2.read());
                //zero.write(result.read() == 0);
                break;
            case 9: // st 
                cout << "ST" <<endl;
                result.write(op1.read());
                //zero.write(result.read() == 0);
                break;
            case 10: // j
                confirmPC.write(1);
                break;
            case 11: // jn
                if(op1.read()< 0){
                    confirmPC.write(1);
                }
                break;
            case 12: // jz
                cout << "JZ" <<endl;
                if(op1.read()== 0){
                    confirmPC.write(1);
                }
                // zero.write(result.read() == 0);
                break;
            case 13: // wrih
                cout << "WRIH" <<endl;
                int x = op2.read();
                x = (x >> 16) << 16;
                int y = op1.read();
                int y0 = (y >> 16) << 16;
                y = y - y0;
                result.write(x + y); 
                // zero.write(result.read() == 0);
                break;
            case 14: // wril
                cout << "WRIL" <<endl;                
                int z = op1.read();
                z = (z >> 16) << 16;
                int w = op2.read();
                int w0 = (w >> 16) << 16;
                w = w - w0;
                result.write(z + w); 
                // zero.write(result.read() == 0);
                break;
            default:
                cout << "DEFAULT" <<endl;
                // result.write(0);
                break;
        }
    }

    SC_CTOR(alu) {
        SC_METHOD(compute);
        sensitive << op1 << op2 << opcode;
    }
};
