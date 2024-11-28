

#include <iostream>
#include "src\way.h"
#include "src\graph.h"
#include "src\ant.h"
#include "src\ant_graph.h"


int main()
{

    AntGraph graph("graphs\\synthetic.txt", false);

    std::cout << graph ;

    //Way best_way = graph.deikstraAlgorithm();
    //std::cout << best_way << "\n";


    graph.antAlgorithm();


    std::cout << "Hello World!\n";


}

