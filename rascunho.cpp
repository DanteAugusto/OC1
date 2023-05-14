#include <systemc.h>
#include <bitset>

SC_MODULE(AndModule) {
    sc_in<sc_uint<32>> input1;
    sc_in<sc_uint<32>> input2;
    sc_out<sc_uint<32>> output;

    void andOperation() {
        output.write(input1.read() & input2.read());
    }

    SC_CTOR(AndModule) {
        SC_METHOD(andOperation);
        sensitive << input1 << input2;
    }
};
SC_MODULE(OrModule) {
    sc_in<sc_uint<32>> input1;
    sc_in<sc_uint<32>> input2;
    sc_out<sc_uint<32>> output;

    void orOperation() {
        output.write(input1.read() | input2.read());
    }

    SC_CTOR(OrModule) {
        SC_METHOD(orOperation);
        sensitive << input1 << input2;
    }
};
SC_MODULE(NotModule) {
    sc_in<sc_uint<32>> input1;
    sc_out<sc_uint<32>> output;

    void notOperation() {
        output.write(~(input1.read()));
    }

    SC_CTOR(NotModule) {
        SC_METHOD(notOperation);
        sensitive << input1;
    }
};
SC_MODULE(XorModule) {
    sc_in<sc_uint<32>> input1;
    sc_in<sc_uint<32>> input2;
    sc_out<sc_uint<32>> output;

    void xorOperation() {
        output.write(input1.read() ^ input2.read());
    }

    SC_CTOR(XorModule) {
        SC_METHOD(xorOperation);
        sensitive << input1 << input2;
    }
};
SC_MODULE(CmpModule) {
    sc_in<sc_uint<32>> input1;
    sc_in<sc_uint<32>> input2;
    sc_out<sc_uint<32>> output;

    void cmpOperation() {
        output.write(input1.read() == input2.read());
    }

    SC_CTOR(CmpModule) {
        SC_METHOD(cmpOperation);
        sensitive << input1 << input2;
    }
};
SC_MODULE(AddModule) {
    sc_in<sc_uint<32>> input1;
    sc_in<sc_uint<32>> input2;
    sc_out<sc_uint<32>> output;

    void addOperation() {
        output.write(input1.read() + input2.read());
    }

    SC_CTOR(AddModule) {
        SC_METHOD(addOperation);
        sensitive << input1 << input2;
    }
};
SC_MODULE(SubModule) {
    sc_in<sc_uint<32>> input1;
    sc_in<sc_uint<32>> input2;
    sc_out<sc_uint<32>> output;

    void subOperation() {
        output.write(input1.read() - input2.read());
    }

    SC_CTOR(SubModule) {
        SC_METHOD(subOperation);
        sensitive << input1 << input2;
    }
};
SC_MODULE(Memory) {
    sc_in<bool> wr_en;
    sc_in<bool> read_enable;
    sc_in<sc_uint<32>> addr;
    sc_in<sc_uint<32>> data;
    sc_out<sc_uint<32>> mem_out;

    sc_uint<32> mem[1024];

    void write_data() {
        if (wr_en.read() == true) {
            cout << "WRITE" << endl;    
            mem[addr.read()] = data.read();
        }
    }

    void read_data() {
        if (read_enable.read() == true) {
            cout << "READ" << endl;
            mem_out.write(mem[addr.read()]);
        }
    }
    void ujump(int n) {
        addr.write(n);
    }
    void cjump(int n, int m, bool b) {
        if(b){
            addr.write(n);
        }else{
            addr.write(m);
        }
    }

    SC_CTOR(Memory) {
        SC_METHOD(write_data);
        sensitive << wr_en << addr << data;

        SC_METHOD(read_data);
        sensitive << addr << read_enable;
    }
};

SC_MODULE(Testbench) {
    sc_signal<bool> wr_en;
    sc_signal<bool> read_enable;
    sc_signal<sc_uint<32>> addr;
    sc_signal<sc_uint<32>> data;
    sc_signal<sc_uint<32>> mem_out;

    Memory *mem;

    void test() {
        // Write data to memory
        wr_en.write(true);
        addr.write(0x00000000);
        data.write(0x12345678);
        wait(10, SC_NS);

        // Read data from memory
        wr_en.write(false);
        addr.write(0x00000000);
        read_enable.write(true);
        addr.write()
        wait(10, SC_NS);
        cout << "Memo out:                  " << std::bitset<32>(mem_out.read()) << endl;
        
        // Check if read value is correct
        if (mem_out.read() == 0x12345678) {
            cout << "Test passed" << endl;
        } else {
            cout << "Test failed" << endl;
        }

        sc_stop();
    }

    SC_CTOR(Testbench) {
        mem = new Memory("mem");

        mem->wr_en(wr_en);
        mem->addr(addr);
        mem->data(data);
        mem->mem_out(mem_out);
        mem->read_enable(read_enable); // bind the read_enable signal

        SC_THREAD(test);
    }

    ~Testbench() {
        delete mem;
    }
};

int sc_main(int argc, char* argv[]) {
    Testbench testbench_inst("testbench_inst");

    sc_start();

    return 0;
}