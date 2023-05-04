#include <systemc.h>
#include <iostream>

SC_MODULE(alu) {
    sc_in<sc_uint<32>> op1;
    sc_in<sc_uint<32>> op2;
    sc_in<sc_uint<4>> alu_control;
    sc_out<sc_uint<32>> result;
    sc_out<bool> zero;

    void compute() {
        cout << alu_control.read() << " ALU_CONTROL" <<endl;
        switch (alu_control.read()) {
            case 0: // and
                cout << "AND" <<endl;
                result.write(op1.read() & op2.read());
                zero.write(result.read() == 0);
                break;
            case 1: // or
                cout << "OR" <<endl;
                result.write(op1.read() | op2.read());
                zero.write(result.read() == 0);
                break;
            case 2: // add
                cout << "ADD" <<endl;
                result.write(op1.read() + op2.read());
                zero.write(result.read() == 0);
                break;
            case 6: // subtract
                cout << "SUBTRACT" <<endl;
                result.write(op1.read() - op2.read());
                zero.write(result.read() == 0);
                break;
            case 7: // set less than
                cout << "LESS" <<endl;
                result.write(op1.read() < op2.read() ? 1 : 0);
                zero.write(result.read() == 0);
                break;
            default:
                cout << "DEFAULT" <<endl;
                result.write(0);
                zero.write(1);
                break;
        }
    }

    SC_CTOR(alu) {
        SC_METHOD(compute);
        sensitive << op1 << op2 << alu_control;
    }
};
