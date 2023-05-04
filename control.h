#include <systemc.h>
#include <iostream>

SC_MODULE(control) {
    sc_in<sc_uint<6>> opcode;
    sc_out<sc_uint<4>> alu_control;
    sc_out<sc_uint<3>> reg_write_enable;
    sc_out<sc_uint<2>> mem_to_reg;
    sc_out<sc_uint<2>> mem_read_write;
    sc_out<bool> jump;

    void decode() {
        cout << "AAAAAAAAAAAAAAAAAAAAA" << endl;
        switch (opcode.read()) {
            case 0x0c: // andi
                alu_control.write(0x00);
                reg_write_enable.write(0x01);
                mem_to_reg.write(0x00);
                mem_read_write.write(0x00);
                jump.write(0);
                break;
            case 0x23: // lw
                alu_control.write(0x02);
                reg_write_enable.write(0x01);
                mem_to_reg.write(0x01);
                mem_read_write.write(0x01);
                jump.write(0);
                break;
            case 0x2b: // sw
                alu_control.write(0x02);
                reg_write_enable.write(0x00);
                mem_to_reg.write(0x00);
                mem_read_write.write(0x10);
                jump.write(0);
                break;
            case 0x04: // beq
                alu_control.write(0x06);
                reg_write_enable.write(0x00);
                mem_to_reg.write(0x00);
                mem_read_write.write(0x00);
                jump.write(0);
                break;
            case 0x02: // j
                alu_control.write(0x00);
                reg_write_enable.write(0x00);
                mem_to_reg.write(0x00);
                mem_read_write.write(0x00);
                jump.write(1);
                break;
            case 0x20: // add
                alu_control.write(0x02);
                reg_write_enable.write(0x00);
                mem_to_reg.write(0x00);
                mem_read_write.write(0x00);
                jump.write(0);
                break;
            default:
                // handle invalid opcode
                break;
        }
    }

    SC_CTOR(control) {
        SC_METHOD(decode);
        sensitive << opcode;
    }
};
