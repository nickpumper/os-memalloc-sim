#ifndef __MMU_H_
#define __MMU_H_

#include <iostream>
#include <string>
#include <vector>

typedef struct Variable {
    std::string name;
    std::string type;
    std::vector <char*> values;
    int virtual_address;
    int size;
} Variable;

typedef struct Process {
    uint32_t pid;
    std::vector<Variable*> variables;
} Process;

class Mmu {
private:
    uint32_t _next_pid;
    int _max_size;
    int _page_size;
    std::vector<Process*> _processes;

public:
    Mmu(int memory_size, int _page_size);
    ~Mmu();

    uint32_t createProcess();
    Process* getProcess(uint32_t);
    Variable* getVariable(uint32_t, std::string);
    void createVariable(uint32_t, std::string, std::string, int);
    void print();
    void deleteProcess(uint32_t pid);
};


#endif // __MMU_H_
