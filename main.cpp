#include <systemc.h>
#include <string>
#include "components/alu.h"
#include "components/bufferExMem.h"
#include "components/bufferIdEx.h"
#include "components/bufferIfId.h"
#include "components/bufferMemWb.h"
#include "components/datamem.h"
#include "components/instruction_memory.h"
#include "components/pc.h"
#include "components/regbs.h"
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
    
    //clock
    sc_clock clk("clk", 10, SC_NS);


    //instruction_memory
    int currentint = 0;
    int lastint = 0;

    //entradas
    sc_signal<sc_uint<5>> current;
    current.write(currentint);
    sc_signal<sc_uint<5>> last;
    last.write(lastint);

    sc_signal<sc_int<25>> write_inst;
    
    sc_signal<bool> sigWriteIM;
    sigWriteIM.write(true);
    
    sc_signal<sc_int<5>> pointPC;
    sc_signal<sc_int<1>> bubble;
    bubble.write(false);
    
    //saidas
    sc_signal<sc_int<32>> dataOut; //saida para: bufferIfId
    sc_signal<sc_int<25>> inst1;
    sc_signal<sc_int<25>> inst2;
    sc_signal<sc_int<25>> inst3;
    sc_signal<sc_int<25>> inst4;
    instruction_memory im("im");
    im.clk(clk);
    im.write_inst(write_inst);
    im.current(current);
    im.last(last);
    im.sigWriteIM(sigWriteIM);
    im.pointPC(pointPC);
    im.bubble(bubble);
    im.dataOut(dataOut);
    im.inst1(inst1);
    im.inst2(inst2);
    im.inst3(inst3);
    im.inst4(inst4);
    
    //bufferIfId
    sc_signal<sc_int<25>> instOut; //saida para: bufferIdEx
    sc_signal<sc_int<4>> opcode; //saida para: regbs
    sc_signal<sc_int<5>> rs; //saida para: regbs
    sc_signal<sc_int<5>> rt; //saida para: regbs
    bufferIfId bfd("bfd");
    bfd.inst(dataOut);
    bfd.instOut(instOut);
    bfd.opcode(opcode);
    bfd.rs(rs);
    bfd.rt(rt);
    bfd.clk(clk);

    //regbs
    sc_signal<sc_int<5>> memoryrequest; //veio do: BufferExMem
    //misterio na linha abaixo
    sc_signal<sc_int<4>> ula_opcode; //veio do: alu
    sc_signal<sc_int<32>> ula_result; //veio do: alu
    sc_signal<sc_int<5>> posUla; // veio do buffer IdEx
    sc_signal<sc_int<1>> loadflag; //veio do: BufferMemWb
    sc_signal<sc_int<5>> loadpoint; //veio do: BufferMemWb
    sc_signal<sc_int<32>> load; //veio do: BufferMemWb
    //sc_signal<sc_int<16>> load2; //veio do: BufferMemWb
    sc_signal<sc_int<32>> opout1; //saída para: bufferIdEx
    sc_signal<sc_int<32>> opout2; //saída para: bufferIdEx
    sc_signal<sc_int<32>> memoryget; //saída para: datamem
    regbs rgbs("rgbs");
    rgbs.opcode(opcode);
    rgbs.op1(rs);
    rgbs.op2(rt);
    rgbs.ula_opcode(ula_opcode);
    rgbs.ula_result(ula_result);
    rgbs.posUla(posUla);
    rgbs.loadflag(loadflag);
    rgbs.loadpoint(loadpoint);
    rgbs.load(load);
    rgbs.clk(clk);
    rgbs.memoryget(memoryget);
    
    //bufferIdEx
    sc_signal<sc_int<4>> opcodedx; //saída para: alu
    sc_signal<sc_int<5>> PCadd; // saída para: PC
    sc_signal<sc_int<25>> instOutdx; //saída para: bufferExMem
    sc_signal<sc_int<32>> op1Out; //saída para: alu
    sc_signal<sc_int<32>> op2Out; //saída para: alu
    bufferIdEx bdx("bdx");
    bdx.clk(clk);
    bdx.inst(instOut);
    bdx.op1(output1);
    bdx.op2(output2);
    bdx.opcode(opcodedx);
    bdx.PCadd(PCadd);
    bdx.posresult(posUla);
    bdx.instOut(instOutdx);
    bdx.op1Out(op1Out);
    bdx.op2Out(op2Out);

    //alu
    sc_signal<sc_int<1>> confirmPC; //saída para: PC
    alu ula("ula");
    ula.op1(op1Out);
    ula.op2(op2Out);
    ula.opcode(opcodedx);
    ula.confirmPC(confirmPC);
    ula.result(ula_result);

    //bufferExMem
    sc_signal<sc_int<4>> opcodexm; //saída para: datamem
    sc_signal<sc_int<4>> instOutxm; //saída para: bufferMemWb
    sc_out<sc_int<10>> memAddr; //saída para: ??????
    bufferExMem bxm("bxm");
    bxm.clk(clk);
    bxm.inst(instOutdx);
    bxm.ula_result(ula_result);
    bxm.memAddr(memAddr);
    bxm.opcode(opcodexm);
    bxm.pointerOut(memoryrequest);
    bxm.instOut(instOutxm);

    //datamem
    sc_signal<sc_int<32>> memOut;
    dataMem dm("dm");
    dm.opcode(opcodexm);
    dm.memAddr(memAddr);
    dm.store_value(memoryget);
    dm.memOut(memOut);

    //bufferMemWb
    bufferMemWb bmw("bmw");
    bmw.clk(clk);
    bmw.opcode(opcodexm);
    bmw.pointer(pointerOut);
    bmw.enable(loadflag);
    bmw.pointerOut(loadpoint);
    bmw.wbOut(load);

    //pc
    sc_signal<sc_int<1>> bubbleOut; //saída para datamem
    sc_signal<sc_int<5>> pointerOut; //saída para datamem
    pc PC("PC");
    PC.clk(clk);
    PC.isjump(confirmPC);
    PC.jump(PCadd);
    PC.inst1(inst1);
    PC.inst2(inst2);
    PC.inst3(inst3);
    PC.inst4(inst4);
    PC.bubbleOut(bubble);
    PC.pointerOut(pointPC);

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
        sc_start(10, SC_NS);

        std::cout << "inst code:                  " << std::bitset<25>(write_inst.read()) << std::endl;
        lastint++;
        last.write(lastint);        
    }
    
    return 0;
}
