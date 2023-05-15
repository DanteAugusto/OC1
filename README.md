# Projeto de Organização de Computadores
Primeiro projeto de Organizacao de Computadores
Para compilar o programa, é necessário ter a biblioteca SystemC e realizar o seguinte comando que gera o executável:

* g++ -I. -I$SYSTEMC_HOME/include -L. -L$SYSTEMC_HOME/lib-linux64 -Wl,-rpath=$SYSTEMC_HOME/lib-linux64 -o main main.cpp -lsystemc -lm 

Isso gerará um executável chamado main que pode ser rodado para fins de teste. Infelizmente não foi possível realizar todos os testes e é previsto mal comportamento no processador.
Para acompanhar as mudanças, existe uma série de prints comentados que estávamos usando para examinar os valores. Por fim, para passar as instruções, é possível passar por arquivo.txt
ou digitar manualmente, o que fará com que o processador entre em ciclo infinito lendo todas as instruções. É necessário que, após digitar todas as instruções, se digite um 0.
