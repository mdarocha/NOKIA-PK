#include <iostream> 

void memoryLeak() 
{ 
    int* ptr = new int(5); 
    return; 
} 
  
int main() 
{ 
    memoryLeak(); 
    return 0; 
} 