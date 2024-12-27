

#include <iostream>
#include "src\way.h"
#include "src\graph.h"
#include "src\ant.h"
#include "src\ant_graph.h"
#include <csignal>


// Функция-обработчик сигнала
void handle_signal(int signal) {
    if (signal == SIGINT) {
        std::cout << "Программа завершена по Ctrl+C\n";
        exit(0);
    }
}


int main()
{
    // Устанавливаем обработчик сигнала SIGINT
    std::signal(SIGINT, handle_signal);


    AntGraph graph("settings.json");

    // std::cout << graph ;

    graph.antAlgorithm();


    std::cout << "The end!\n";


}

