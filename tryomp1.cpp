#include <iostream>
#include <cmath>

int main(){
    double sum_i = 0, max_i = -1;
    #pragma omp parallel for reduction(+:sum_i) reduction(max:max_i)
    for (int i=0; i<9; i++){
        sum_i += i;
        if (i > max_i)
            max_i = i;
    }
    std::cout << "Sum = " << sum_i << std::endl;
    std::cout << "Max = " << max_i << std::endl;
    return 0;
}
