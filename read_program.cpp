#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <bitset>
#include <cstring>

#include "components/tokenize.h"

std::vector<unsigned int> read_program(std::string filename);



int main() {
    auto program = read_program("programs/hello.prog");

    std::cout << "\nLoaded program, instructions = " << program.size() << std::endl;
    return 0;
}



void append(std::vector<bool> &the_vector, const std::vector<bool> &items) {
    the_vector.insert(the_vector.end(), items.begin(), items.end());
}

void inst_result_op1_op2(
    std::vector<bool> &inst_code,
    std::map<std::string, std::vector<bool>> &m,
    std::vector<std::string> &tokens
) {
    std::vector<bool> result = m[tokens[1]];
    std::vector<bool> operator1 = m[tokens[2]];
    std::vector<bool> operator2 = m[tokens[3]];

    append(inst_code, result);
    append(inst_code, operator1);
    append(inst_code, operator2);
}

void inst_result_op1(
    std::vector<bool> &inst_code,
    std::map<std::string, std::vector<bool>> &m,
    std::vector<std::string> &tokens
) {
    std::vector<bool> result = m[tokens[1]];
    std::vector<bool> operator1 = m[tokens[2]];

    append(inst_code, result);
    append(inst_code, operator1);
}

void inst_result(
    std::vector<bool> &inst_code,
    std::map<std::string, std::vector<bool>> &m,
    std::vector<std::string> &tokens
) {
    std::vector<bool> result = m[tokens[1]];

    append(inst_code, result);
}

void inst_result_integer(
    std::vector<bool> &inst_code,
    std::map<std::string, std::vector<bool>> &m,
    std::vector<std::string> &tokens
) {
    std::vector<bool> result = m[tokens[1]];
    inst_code.insert(inst_code.end(),result.begin(),result.end());

    int x = stoi(tokens[2]);
    std::bitset<16> value = std::bitset<16>(x);

    for (int i = 0; i < 16; i++) {
        if (value[15 - i]) {
            inst_code.push_back(true);
        } else{
            inst_code.push_back(false);
        }
    }
}



std::vector<unsigned int> read_program(std::string filename) {
    std::vector<unsigned int> program;

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

    std::ifstream file(filename);
    std::string line;
    std::vector<bool> inst_code;

    if (file.fail()) {
        std::cout << "ERROR Failed to read " << filename << ": " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Loading program " << filename << std::endl;

    while (getline(file, line)) {
        if (line.size() == 0) {
            continue;
        }

        std::vector<std::string> tokens = tokenize(line);

        if (tokens[0] == "and") {
            inst_code = {0,0,0,1};
            inst_result_op1_op2(inst_code, m, tokens);
        } else if (tokens[0] == "or") {
            inst_code = {0,0,1,0};
            inst_result_op1_op2(inst_code, m, tokens);
        } else if (tokens[0] == "xor") {
            inst_code = {0,0,1,1};
            inst_result_op1_op2(inst_code, m, tokens);
        } else if (tokens[0] == "not") {
            inst_code = {0,1,0,0};
            inst_result_op1(inst_code, m, tokens);
        } else if (tokens[0] == "cmp") {
            inst_code = {0,1,0,1};
            inst_result_op1_op2(inst_code, m, tokens);
        } else if (tokens[0] == "add") {
            inst_code = {0,1,1,0};
            inst_result_op1_op2(inst_code, m, tokens);
        } else if (tokens[0] == "sub") {
            inst_code = {0,1,1,1};
            inst_result_op1_op2(inst_code, m, tokens);
        } else if (tokens[0] == "ld") {
            inst_code = {1,0,0,0};
            inst_result(inst_code, m, tokens);
        } else if (tokens[0] == "st") {
            inst_code = {1,0,0,1};
            inst_result(inst_code, m, tokens);
        } else if (tokens[0] == "j") {
            inst_code = {1,0,1,0};
            inst_result(inst_code, m, tokens);
        } else if (tokens[0] == "jn") {
            inst_code = {1,0,1,1};
            inst_result(inst_code, m, tokens);
        } else if (tokens[0] == "jz") {
            inst_code = {1,1,0,0};
            inst_result(inst_code, m, tokens);
        } else if (tokens[0] == "wrih") {
            inst_code = {1,1,0,1};
            inst_result_integer(inst_code, m, tokens);
        } else if (tokens[0] == "wril") {
            inst_code = {1,1,1,0};
            inst_result_integer(inst_code, m, tokens);
        }

        while (inst_code.size() < 25) {
            inst_code.push_back(0);
        }

        unsigned int uint_value = 0;
        for (int i = 0; i < 25; i++) {
            uint_value |= (inst_code[i] ? 1 : 0) << (24 - i);
        }

        program.push_back(uint_value);
        std::cout << std::bitset<25>(uint_value) << "    " << line << std::endl;
    }

    return program;
}