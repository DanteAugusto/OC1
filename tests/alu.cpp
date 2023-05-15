#include "../components/alu.h"

#include <algorithm>

#define PRINT(expr) cout << "[!] " #expr " = " << (expr) << endl
#define EXPR(expr) #expr " = " << (expr) << "; "


int from_bits(const std::string &bits) {
    int result = 0;

    for (int bit : bits) {
        if (bit == '1' || bit == '0') {
            result = 2 * result + (bit == '1' ? 1 : 0);
        }
    }

    return result;
}

std::string to_bit_string(int value) {
    std::string result;
    bool is_negative = value < 0;

    if (value == 0) {
        return "0";
    } else if (is_negative) {
        value = -value;
    }

    while (value > 0) {
        int bit = value % 2;
        result.push_back(bit == 1 ? '1' : '0');
        value = (value - bit) / 2;
    }

    if (is_negative) {
        result.push_back('-');
    }

    reverse(result.begin(), result.end());
    return result;
}


class Test {
public:
    bool skip;

    sc_signal<sc_uint<32>> op1;
    sc_signal<sc_uint<32>> op2;
    sc_signal<sc_uint<4>> opcode;
    sc_signal<sc_uint<1>> confirmPC;
    sc_signal<sc_uint<32>> result;

    alu my_alu;

    Test():
        skip(false),
        my_alu("alu")
    {
        my_alu.op1(op1);
        my_alu.op2(op2);
        my_alu.opcode(opcode);
        my_alu.confirmPC(confirmPC);
        my_alu.result(result);
    }

    void run(
        const char *name,
        int in_op1, int in_op2, int in_opcode,
        unsigned int out_confirmPC, unsigned int out_result
    ) {
        if (skip) {
            cout << endl;
            cout << "SKIPPING: " << name << endl;
            return;
        }

        cout << endl;
        cout << "TESTING: " << name << endl;
        cout << "[!] " << EXPR(in_op1) << EXPR(in_op2) << EXPR(in_opcode) << endl;
        cout << "[!] " << EXPR(out_confirmPC) << EXPR(out_result) << endl;
        cout << endl;

        op1.write(in_op1);
        op2.write(in_op2);
        opcode.write(in_opcode);

        sc_start(100, SC_NS);

        auto confirmPC_value = confirmPC.read();
        auto result_value = result.read();

        PRINT(confirmPC_value);
        PRINT(result_value);

        assert(confirmPC_value == out_confirmPC);
        assert(result_value == out_result);
    }
};

int sc_main(int argc, char* argv[]) {
    (void) argc;
    (void) argv;

    Test test;

    test.run(
        "binary and",
        // GIVEN
        from_bits("1111 0000"),
        from_bits("1100 1100"),
        1,
        // EXPECT
        0,
        from_bits("1100 0000")
    );

    test.run(
        "binary or",
        // GIVEN
        from_bits("1111 0000"),
        from_bits("1100 1100"),
        2,
        // EXPECT
        0,
        from_bits("1111 1100")
    );

    test.run(
        "binary xor",
        // GIVEN
        from_bits("1111 0000"),
        from_bits("1100 1100"),
        3,
        // EXPECT
        0,
        from_bits("0011 1100")
    );

    test.run(
        "binary negate",
        // GIVEN
        from_bits("1111 0000"),
        from_bits("1100 1100"),
        4,
        // EXPECT
        0,
        ~from_bits("1111 0000")
    );

    test.result.write(1234);
    test.run(
        "equals - true",
        // GIVEN
        from_bits("1010 1100"),
        from_bits("1010 1100"),
        5,
        // EXPECT
        0,
        from_bits("1")
    );

    test.run(
        "equals - false",
        // GIVEN
        from_bits("1010 1100"),
        from_bits("1110 1100"),
        5,
        // EXPECT
        0,
        from_bits("0")
    );

    test.run(
        "equals - false",
        // GIVEN
        from_bits("1010 1100"),
        from_bits("1110 1100"),
        5,
        // EXPECT
        0,
        from_bits("0")
    );

    test.run(
        "equals - false",
        // GIVEN
        from_bits("1010 1100"),
        from_bits("1110 1100"),
        5,
        // EXPECT
        0,
        from_bits("0")
    );

    test.run(
        "addition",
        // GIVEN
        2938,
        34124,
        6,
        // EXPECT
        0,
        2938 + 34124
    );

    test.run(
        "subtraction",
        // GIVEN
        48374,
        182,
        7,
        // EXPECT
        0,
        48374 - 182
    );

    test.confirmPC.write(0);
    test.result.write(1234);
    test.run(
        "load",
        // GIVEN
        1111,
        2222,
        8,
        // EXPECT
        0,
        1234
    );

    test.run(
        "store",
        // GIVEN
        43895739,
        301832,
        9,
        // EXPECT
        0,
        43895739
    );

    test.result.write(2345);
    test.run(
        "jump",
        // GIVEN
        4444,
        5555,
        10,
        // EXPECT
        1,
        2345
    );

    test.result.write(3456);
    test.run(
        "jump if negative - true",
        // GIVEN
        -123,
        1234,
        11,
        // EXPECT
        1,
        3456
    );

    test.result.write(4567);
    test.run(
        "jump if negative - false",
        // GIVEN
        111,
        1234,
        11,
        // EXPECT
        0,
        4567
    );

    test.result.write(45671);
    test.run(
        "jump if negative - false ZERO",
        // GIVEN
        0,
        1234,
        11,
        // EXPECT
        0,
        45671
    );

    test.result.write(5678);
    test.run(
        "jump if zero - true",
        // GIVEN
        0,
        6666,
        12,
        // EXPECT
        1,
        5678
    );

    test.result.write(6789);
    test.run(
        "jump if zero - false",
        // GIVEN
        239472,
        12321,
        12,
        // EXPECT
        0,
        6789
    );

    test.run(
        "wrih",
        // GIVEN
        0x12345678,
        0xABCDDCBA,
        13,
        // EXPECT
        0,
        0xABCD5678
    );

    test.run(
        "wril",
        // GIVEN
        0x12345678,
        0xABCDDCBA,
        14,
        // EXPECT
        0,
        0x1234DCBA
    );

    return 0;
}