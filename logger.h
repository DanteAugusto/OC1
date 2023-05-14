
// Funcoes que escrevem/leem sinais e logam a operacao no console, por ex:

// [main] clock.write(0)
// [main] clock.read() = 1
// [nome-do-modulo] clock.read() = 0

// Pra facilitar, vc pode fazer find-replace no vscode:

// Ctrl+Shift+f
// Aperte o botão ".*" na direita do 1o campo pra ativar expressao regular
// Digite os textos abaixo nos campos de buscar e substituir
// Clique no botao de Substituir Tudo (abaixo do ".*")
// No main, troque sig_write/sig_read para sig_write_main/sig_read_main (e remova o "this")

// 1. Trocar sinal.write(valor) para sig_write(sinal, valor)
// 1o campo: (\w+).write\(([^)]+)\)
// 2o campo: sig_write(this, $1, $2)

// 1. Trocar sinal.read() para sig_read(sinal)
// 1o campo: (\w+).read\(\)
// 2o campo: sig_read(this, $1)


// INSTRUCOES

// Nas declaraçoes de sinais, passe o nome do sinal:
// sc_signal<bool> clock { "clock" };
// sc_in_clk clock { "clock" };

// No main:
// Troque sinal.write(valor) por sig_write_main(sinal, value)
// Troque sinal.read(valor) por sig_read_main(value)
//   OBS nao use no meio do "cout <<""
//   Errado: cout << "clock = " << sig_read_main(clock) << "\n";
//   Certo:
//      auto clock_val = sig_read_main(clock);
//      cout << "clock = " << clock_val << "\n";

// Dentro dos módulos:
// Troque sinal.write(valor) por sig_write(this, sinal, value)
// Troque sinal.read(valor) por sig_read(this, value)
//   OBS nao use no meio do cout, igual o outro

// Para desabilitar um log, só passar false no ultimo argumento opcional:
//   sig_write(this, clock, 1, false);
//   bool value = sig_read(this, clock, false);

template <class S, class V>
void sig_write_main(S &signal, V value, bool log = true) {
    signal.write(value);

    if (log) {
        cout << "[main] " << signal.name() << ".write(" << value << ")" << endl;
    }
}

template <class S>
auto sig_read_main(S &signal, bool log = true) {
    auto value = signal.read();

    if (log) {
        cout << "[main] " << signal.name() << ".read() = " << value << endl;
    }

    return value;
}

template <class M, class S, class V>
void sig_write(M *mod, S &signal, V value, bool log = true) {
    if (log) {
        signal.write(value);
    }

    cout << "[" << mod->name() << "] " << signal.name() << ".write(" << value << ")" << endl;
}

template <class M, class S>
auto sig_read(M *mod, S &signal, bool log = true) {
    if (log) {
        auto value = signal.read();
    }

    cout << "[" << mod->name() << "] " << signal.name() << ".read() = " << value << endl;
    return value;
}