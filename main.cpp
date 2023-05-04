// #include <systemc.h>
// #include <bitset>

// SC_MODULE(AndModule) {
//     sc_in<sc_uint<32>> input1;
//     sc_in<sc_uint<32>> input2;
//     sc_out<sc_uint<32>> output;

//     void andOperation() {
//         output.write(input1.read() & input2.read());
//     }

//     SC_CTOR(AndModule) {
//         SC_METHOD(andOperation);
//         sensitive << input1 << input2;
//     }
// };
// SC_MODULE(OrModule) {
//     sc_in<sc_uint<32>> input1;
//     sc_in<sc_uint<32>> input2;
//     sc_out<sc_uint<32>> output;

//     void orOperation() {
//         output.write(input1.read() | input2.read());
//     }

//     SC_CTOR(OrModule) {
//         SC_METHOD(orOperation);
//         sensitive << input1 << input2;
//     }
// };
// SC_MODULE(NotModule) {
//     sc_in<sc_uint<32>> input1;
//     sc_out<sc_uint<32>> output;

//     void notOperation() {
//         output.write(~(input1.read()));
//     }

//     SC_CTOR(NotModule) {
//         SC_METHOD(notOperation);
//         sensitive << input1;
//     }
// };
// SC_MODULE(XorModule) {
//     sc_in<sc_uint<32>> input1;
//     sc_in<sc_uint<32>> input2;
//     sc_out<sc_uint<32>> output;

//     void xorOperation() {
//         output.write(input1.read() ^ input2.read());
//     }

//     SC_CTOR(XorModule) {
//         SC_METHOD(xorOperation);
//         sensitive << input1 << input2;
//     }
// };
// SC_MODULE(CmpModule) {
//     sc_in<sc_uint<32>> input1;
//     sc_in<sc_uint<32>> input2;
//     sc_out<sc_uint<32>> output;

//     void cmpOperation() {
//         output.write(input1.read() == input2.read());
//     }

//     SC_CTOR(CmpModule) {
//         SC_METHOD(cmpOperation);
//         sensitive << input1 << input2;
//     }
// };
// SC_MODULE(AddModule) {
//     sc_in<sc_uint<32>> input1;
//     sc_in<sc_uint<32>> input2;
//     sc_out<sc_uint<32>> output;

//     void addOperation() {
//         output.write(input1.read() + input2.read());
//     }

//     SC_CTOR(AddModule) {
//         SC_METHOD(addOperation);
//         sensitive << input1 << input2;
//     }
// };
// SC_MODULE(SubModule) {
//     sc_in<sc_uint<32>> input1;
//     sc_in<sc_uint<32>> input2;
//     sc_out<sc_uint<32>> output;

//     void subOperation() {
//         output.write(input1.read() - input2.read());
//     }

//     SC_CTOR(SubModule) {
//         SC_METHOD(subOperation);
//         sensitive << input1 << input2;
//     }
// };
// SC_MODULE(Memory) {
//     sc_in<bool> wr_en;
//     sc_in<bool> read_enable;
//     sc_in<sc_uint<32>> addr;
//     sc_in<sc_uint<32>> data;
//     sc_out<sc_uint<32>> mem_out;

//     sc_uint<32> mem[1024];

//     void write_data() {
//         if (wr_en.read() == true) {
//             cout << "WRITE" << endl;    
//             mem[addr.read()] = data.read();
//         }
//     }

//     void read_data() {
//         if (read_enable.read() == true) {
//             cout << "READ" << endl;
//             mem_out.write(mem[addr.read()]);
//         }
//     }

//     SC_CTOR(Memory) {
//         SC_METHOD(write_data);
//         sensitive << wr_en << addr << data;

//         SC_METHOD(read_data);
//         sensitive << addr << read_enable;
//     }
// };

// SC_MODULE(Testbench) {
//     sc_signal<bool> wr_en;
//     sc_signal<bool> read_enable;
//     sc_signal<sc_uint<32>> addr;
//     sc_signal<sc_uint<32>> data;
//     sc_signal<sc_uint<32>> mem_out;

//     Memory *mem;

//     void test() {
//         // Write data to memory
//         wr_en.write(true);
//         addr.write(0x00000000);
//         data.write(0x12345678);
//         wait(10, SC_NS);

//         // Read data from memory
//         wr_en.write(false);
//         // addr.write(0x00000000);
//         read_enable.write(true);
//         wait(10, SC_NS);
//         cout << "Memo out:                  " << std::bitset<32>(mem_out.read()) << endl;
        
//         sc_stop();
//     }

//     SC_CTOR(Testbench) {
//         mem = new Memory("mem");

//         mem->wr_en(wr_en);
//         mem->addr(addr);
//         mem->data(data);
//         mem->mem_out(mem_out);
//         mem->read_enable(read_enable); // bind the read_enable signal

//         SC_THREAD(test);
//     }

//     ~Testbench() {
//         delete mem;
//     }
// };

// int sc_main(int argc, char* argv[]) {
//     Testbench testbench_inst("testbench_inst");

//     sc_start();

//     return 0;
// }

#include <systemc.h>
#include <string>
#include "instruction_memory.h"
#include "register_file.h"
#include "alu.h"
#include "control.h"
#include "tokenize.h"
#include <map>
#include <bitset>
#define NOP 0x00000000



int sc_main(int argc, char* argv[]) {    
    std::map<std::string, std::vector<bool>> m ={
        {"$0", {0,0,0,0,0}},
        {"$1", {0,0,0,0,1}},
        {"$2", {0,0,0,1,0}},
        {"$3", {0,0,0,1,1}},
        {"$4", {0,0,1,0,0}},
        {"$5", {0,0,1,0,1}},
        {"$6", {0,0,1,1,0}},
        {"$7", {0,0,1,1,1}},
        {"$8", {0,1,0,0,0}},
        {"$9", {0,1,0,0,1}},
        {"$10", {0,1,0,1,0}},
        {"$11", {0,1,0,1,1}},
        {"$12", {0,1,1,0,0}},
        {"$13", {0,1,1,0,1}},
        {"$14", {0,1,1,1,0}},
        {"$15", {0,1,1,1,1}},
        {"$16", {1,0,0,0,0}},
        {"$17", {1,0,0,0,1}},
        {"$18", {1,0,0,1,0}},
        {"$19", {1,0,0,1,1}},
        {"$20", {1,0,1,0,0}},
        {"$21", {1,0,1,0,1}},
        {"$22", {1,0,1,1,0}},
        {"$23", {1,0,1,1,1}},
        {"$24", {1,1,0,0,0}},
        {"$25", {1,1,0,0,1}},
        {"$26", {1,1,0,1,0}},
        {"$27", {1,1,0,1,1}},
        {"$28", {1,1,1,0,0}},
        {"$29", {1,1,1,0,1}},
        {"$30", {1,1,1,1,0}},
        {"$31", {1,1,1,1,1}},
    };
    sc_signal<sc_uint<32>> write_inst;
    instruction_memory im("im");
    im.write_inst(write_inst);
    // RF
    sc_signal<sc_uint<5>> rs, rt, rd;
    sc_signal<sc_uint<32>> write_data;
    sc_signal<sc_uint<32>> read_data_1, read_data_2;
    sc_signal<bool> write_enable;
    sc_clock clk("clk", 10, SC_NS);

    register_file rf("rf");
    rf.clk(clk);
    rf.rs(rs);
    rf.rt(rt);
    rf.rd(rd);
    rf.write_data(write_data);
    rf.read_data_1(read_data_1);
    rf.read_data_2(read_data_2);
    rf.write_enable(write_enable);

    // ALU
    sc_signal<sc_uint<32>> op1, op2, result;
    sc_signal<sc_uint<4>> alu_control;
    sc_signal<bool> zero;

    alu alu("alu");
    alu.op1(op1);
    alu.op2(op2);
    alu.result(result);
    alu.alu_control(alu_control);
    alu.zero(zero);

    // Control
    sc_signal<sc_uint<6>> opcode;
    sc_signal<sc_uint<3>> reg_write_enable;
    sc_signal<sc_uint<2>> mem_to_reg;
    sc_signal<sc_uint<2>> mem_read_write;
    sc_signal<bool> jump;

    control control("control");
    control.opcode(opcode);
    control.alu_control(alu_control);
    control.reg_write_enable(reg_write_enable);
    control.mem_to_reg(mem_to_reg);
    control.mem_read_write(mem_read_write);
    control.jump(jump);

    std::string command = "";
    std::vector<bool> inst_code;
    while (command != "0") {
        // Prompt the user for input
        std::cout << "Enter command (add, sub, and, or, slt, lw, sw, beq, j): ";
        std::getline(std::cin, command);
        // std::cout << command << std::endl;
        std::vector<std::string> tokens = tokenize(command);

        if(tokens[0] == "add"){
            inst_code= {0,0,0,1};
        // std::cout << tokens.size() << std::endl;
            std::cout << tokens[1] << std::endl;
            std::cout << tokens[2] << std::endl;
            std::cout << tokens[3] << std::endl;
            std::vector<bool> result= m[tokens[1]];
            std::vector<bool> operator1= m[tokens[2]];
            std::vector<bool> operator2= m[tokens[3]];
            inst_code.insert(inst_code.end(),result.begin(),result.end());
            inst_code.insert(inst_code.end(),operator1.begin(),operator1.end());
            inst_code.insert(inst_code.end(),operator2.begin(),operator2.end());
            
        }
        while(inst_code.size()<32){
            inst_code.push_back(0);
        }

        for (int i = 0; i < 32; i++) {
            std::cout << (inst_code[i] ? 1 : 0);
        }
        std::cout << std::endl;
        unsigned int uint_value = 0;
        for (int i = 0; i < 32; i++) {
            uint_value |= (inst_code[i] ? 1 : 0) << (31-i);
        }
        std::cout << "int_value:                  " << std::bitset<32>(uint_value) << std::endl;
        sc_uint<32> zas = uint_value;
        std::cout << "zas_value:                  " << std::bitset<32>(zas) << std::endl;
        write_inst.write(0x11111111);

        std::cout << "inst code:                  " << std::bitset<32>(write_inst.read()) << std::endl;
        // Map the command to its corresponding opcode
        // sc_uint<4> opcode1;
        // std::cout << "opcode1 é: " << opcode1 << endl;

        // // Prompt the user for the two values
        // std::cout << "Enter value 1: ";
        // sc_uint<32> value1;
        // std::cin >> value1;

        // std::cout << "Enter value 2: ";
        // sc_uint<32> value2;
        // std::cin >> value2;

        // // Set the inputs to the modules
        // rs.write(0);
        // rt.write(1);
        // write_data.write(value1);
        // write_enable.write(true);
        // sc_start(1, SC_NS);
        // write_data.write(value2);
        // opcode.write(opcode1);

        // // Wait for the simulation to settle
        // sc_start(10, SC_NS);
        // alu_control.write(0);
        // // Print the result
        // std::cout << "Result: " << std::hex << result.read() << "\n";
        // std::cout << "Zero: " << zero.read() << "\n";
    }
    
    return 0;
}
