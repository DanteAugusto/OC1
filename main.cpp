#include <systemc.h>
#include <string>
#include "components/instruction_memory.h"
#include "components/register_file.h"
#include "components/alu.h"
#include "components/control.h"
#include "components/bufferIfId.h"
#include "components/tokenize.h"
#include <map>
#include <bitset>
#define NOP 0x00000000

// SC_MODULE(processador){
//     instruction_memory IM{"IM"};//memoria de instrucao
//     sc_signal<sc_uint<5>> current;
//     sc_signal<sc_uint<5>> last;
//     sc_signal<sc_uint<25>> write_inst;
//     sc_signal<bool> sigWriteIM;
//     //registradores
//     //ULA
//     //memoria de dados
//     SC_CTOR(processador) {
//         IM.write_inst(write_inst);
//         IM.sigWriteIM(sigWriteIM);
//         IM.current(current);
//         IM.last(last);
//     }
// };

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
    //instruction_memory
    int x = 0;
    int y = 0;
    sc_signal<sc_uint<5>> current;
    current.write(x);
    sc_signal<sc_uint<5>> last;
    last.write(y);
    sc_signal<sc_uint<25>> write_inst;
    sc_signal<bool> sigWriteIM;
    sigWriteIM.write(true);
    sc_clock clk("clk", 10, SC_NS);
    instruction_memory im("im");
    sc_signal<sc_uint<5>> pointPC;
    sc_signal<sc_uint<32>> dataOut;
    im.dataOut(dataOut);
    im.pointPC(pointPC);
    im.sigWriteIM(sigWriteIM);
    im.write_inst(write_inst);
    im.current(current);
    im.last(last);
    im.clk(clk);
    
    //bufferIfId
    sc_signal<sc_uint<25>> inst;
    sc_signal<sc_uint<32>> instOut;
    sc_signal<sc_uint<4>> opcode;
    sc_signal<sc_uint<5>> rs;
    sc_signal<sc_uint<5>> rt;
    sc_signal<sc_uint<5>> rd;
    bufferIfId bfd("bfd");
    bfd.inst(inst);
    bfd.instOut(instOut);
    bfd.opcode(opcode);
    bfd.rs(rs);
    bfd.rt(rt);
    bfd.rd(rd);
    bfd.clk(clk);
    // // RF
    // sc_signal<sc_uint<5>> rs, rt, rd;
    // sc_signal<sc_uint<32>> write_data;
    // sc_signal<sc_uint<32>> read_data_1, read_data_2;
    // sc_signal<bool> write_enable;
    // sc_clock clk("clk", 10, SC_NS);

    // register_file rf("rf");
    // rf.clk(clk);
    // rf.rs(rs);
    // rf.rt(rt);
    // rf.rd(rd);
    // rf.write_data(write_data);
    // rf.read_data_1(read_data_1);
    // rf.read_data_2(read_data_2);
    // rf.write_enable(write_enable);

    // // ALU
    // sc_signal<sc_uint<32>> op1, op2, result;
    // sc_signal<sc_uint<4>> alu_control;
    // sc_signal<bool> zero;

    // alu alu("alu");
    // alu.op1(op1);
    // alu.op2(op2);
    // alu.result(result);
    // alu.alu_control(alu_control);
    // alu.zero(zero);

    // // Control
    // sc_signal<sc_uint<6>> opcode;
    // sc_signal<sc_uint<3>> reg_write_enable;
    // sc_signal<sc_uint<2>> mem_to_reg;
    // sc_signal<sc_uint<2>> mem_read_write;
    // sc_signal<bool> jump;

    // control control("control");
    // control.opcode(opcode);
    // control.alu_control(alu_control);
    // control.reg_write_enable(reg_write_enable);
    // control.mem_to_reg(mem_to_reg);
    // control.mem_read_write(mem_read_write);
    // control.jump(jump);

    std::string command = "";
    std::vector<bool> inst_code;
    while (command != "0") {
        // Prompt the user for input
        std::cout << "Enter command (add, sub, and, or, slt, lw, sw, beq, j): ";
        std::getline(std::cin, command);
        //std::cout << command << std::endl;
        std::vector<std::string> tokens = tokenize(command);

        if(tokens[0] == "and"){
            inst_code= {0,0,0,1};
        //std::cout << tokens.size() << std::endl;
            std::cout << tokens[1] << std::endl;
            std::cout << tokens[2] << std::endl;
            std::cout << tokens[3] << std::endl;
            std::vector<bool> result= m[tokens[1]];
            std::vector<bool> operator1= m[tokens[2]];
            std::vector<bool> operator2= m[tokens[3]];
            inst_code.insert(inst_code.end(),result.begin(),result.end());
            inst_code.insert(inst_code.end(),operator1.begin(),operator1.end());
            inst_code.insert(inst_code.end(),operator2.begin(),operator2.end());
            
        }else if(tokens[0] == "or"){
            inst_code= {0,0,1,0};
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
            
        }else if(tokens[0] == "xor"){
            inst_code= {0,0,1,1};
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
            
        }else if(tokens[0] == "not"){
            inst_code= {0,1,0,0};
        // std::cout << tokens.size() << std::endl;
            std::cout << tokens[1] << std::endl;
            std::cout << tokens[2] << std::endl;
            std::vector<bool> result= m[tokens[1]];
            std::vector<bool> operator1= m[tokens[2]];
            inst_code.insert(inst_code.end(),result.begin(),result.end());
            inst_code.insert(inst_code.end(),operator1.begin(),operator1.end());
            
        }else if(tokens[0] == "cmp"){
            inst_code= {0,1,0,1};
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
            
        }else if(tokens[0] == "add"){
            inst_code= {0,1,1,0};
        std::cout << tokens.size() << std::endl;
            std::cout << tokens[1] << std::endl;
            std::cout << tokens[2] << std::endl;
            std::cout << tokens[3] << std::endl;
            std::vector<bool> result= m[tokens[1]];
            std::vector<bool> operator1= m[tokens[2]];
            std::vector<bool> operator2= m[tokens[3]];
            inst_code.insert(inst_code.end(),result.begin(),result.end());
            inst_code.insert(inst_code.end(),operator1.begin(),operator1.end());
            inst_code.insert(inst_code.end(),operator2.begin(),operator2.end());
            
        }else if(tokens[0] == "sub"){
            inst_code= {0,1,1,1};
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
            
        }else if(tokens[0] == "ld"){
            inst_code= {1,0,0,0};
        // std::cout << tokens.size() << std::endl;
            std::cout << tokens[1] << std::endl;
            std::vector<bool> result= m[tokens[1]];
            inst_code.insert(inst_code.end(),result.begin(),result.end());            
        }else if(tokens[0] == "st"){
            inst_code= {1,0,0,1};
        // std::cout << tokens.size() << std::endl;
            std::cout << tokens[1] << std::endl;
            std::vector<bool> result= m[tokens[1]];
            inst_code.insert(inst_code.end(),result.begin(),result.end());            
        }else if(tokens[0] == "j"){
            inst_code= {1,0,1,0};
        // std::cout << tokens.size() << std::endl;
            std::cout << tokens[1] << std::endl;
            std::vector<bool> result= m[tokens[1]];
            inst_code.insert(inst_code.end(),result.begin(),result.end());            
        }else if(tokens[0] == "jn"){
            inst_code= {1,0,1,1};
        // std::cout << tokens.size() << std::endl;
            std::cout << tokens[1] << std::endl;
            std::vector<bool> result= m[tokens[1]];
            inst_code.insert(inst_code.end(),result.begin(),result.end());            
        }else if(tokens[0] == "jz"){
            inst_code= {1,1,0,0};
        // std::cout << tokens.size() << std::endl;
            std::cout << tokens[1] << std::endl;
            std::vector<bool> result= m[tokens[1]];
            inst_code.insert(inst_code.end(),result.begin(),result.end());            
        }
        while(inst_code.size()<32){
            inst_code.push_back(0);
        }
       unsigned int uint_value = 0;
        for (int i = 0; i < 25; i++) {
            uint_value |= (inst_code[i] ? 1 : 0) << (24-i);
        }
        std::cout << "int_value:                  " << std::bitset<25>(uint_value) << std::endl;
        write_inst.write(uint_value);
        inst.write(uint_value);
        sc_start(10, SC_NS);

        std::cout << "inst code:                  " << std::bitset<25>(write_inst.read()) << std::endl;
        y++;
        last.write(y);        
    }
    
    return 0;
}
