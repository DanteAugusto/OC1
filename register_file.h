#include <systemc.h>
#include <iostream>

SC_MODULE(register_file) {
    sc_in_clk clk;
    sc_in<sc_uint<5>> rs;
    sc_in<sc_uint<5>> rt;
    sc_in<sc_uint<5>> rd;
    sc_in<sc_uint<32>> write_data;
    sc_out<sc_uint<32>> read_data_1;
    sc_out<sc_uint<32>> read_data_2;
    sc_out<bool> write_enable;

    sc_uint<32> regs[32];

    void read_regs() {
        read_data_1.write(regs[rs.read()]);
        read_data_2.write(regs[rt.read()]);
    }

    void write_reg() {
        if (write_enable.read()) {
            regs[rd.read()] = write_data.read();
        }
    }

    SC_CTOR(register_file) {
        for (int i = 0; i < 32; i++) {
            regs[i] = 0;
        }
        SC_METHOD(read_regs);
        sensitive << clk.pos();
        SC_METHOD(write_reg);
        sensitive << clk.pos();
    }
};
