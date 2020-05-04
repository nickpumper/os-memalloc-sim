#include "mmu.h"
#include <map>
#include <cmath>

Mmu::Mmu(int memory_size, int page_size)
{
    _next_pid = 1024;
    _max_size = memory_size;
    _page_size = page_size;
}
 
Mmu::~Mmu()
{
}

uint32_t Mmu::createProcess()
{
    Process *proc = new Process();
    proc->pid = _next_pid;
    
    /*
    Variable *var = new Variable();
    var->name = "<FREE_SPACE>";
    var->virtual_address = 0;
    var->size = _max_size;
    proc->variables.push_back(var);
    */

    _processes.push_back(proc);

    _next_pid++;
    return proc->pid;
}

void Mmu::createVariable(uint32_t pid, std::string name, std::string type, int num)
{
    if (getProcess(pid) == NULL)
    {
        std::cout << "Invalid process ID." << std::endl;
    }

    Process * process = getProcess(pid);


    //what variables should be supported?
    //int, long, float, double, char
    std::map<std::string, int> sizes;
    sizes["int"] = 4;
    sizes["long"] = 8;
    sizes["float"] = 4;
    sizes["double"] = 8;
    sizes["char"] = 1;
    
    Variable *var = new Variable();
    
    var->name = name;
    var->type = type;
    var->size = sizes[type] * num;
    
    for (int i = 0; i < num; i++)
    {
        var->values.push_back(NULL);
    }

    // determine virtual address 
    int offset_bits = log2(_page_size);
    int sum_var_sizes;
    int page_num;
    int page_offset;

    // get how much data this process already has
    sum_var_sizes = 0;
    for (int i =0; i < process->variables.size(); i++) {
        sum_var_sizes = sum_var_sizes + process->variables[i]->size;
    } // for i

    // now divide it by the page size to get which page we are at now
    page_num = floor(sum_var_sizes / _page_size);
    page_offset = sum_var_sizes % _page_size;

    var->virtual_address = page_num;
    var->virtual_address = var->virtual_address << offset_bits;
    var->virtual_address = var->virtual_address | page_offset; // final

    // required print: if not TEXT, GLOBALS, or STACK, then print virtual addr
    if ( (strcmp(name.c_str(), "<TEXT>") != 0) && (strcmp(name.c_str(), "<GLOBALS>") != 0) && (strcmp(name.c_str(), "<STACK>") != 0) ) {
        std::cout << var->virtual_address << std::endl;
    } // if    
    
    getProcess(pid)->variables.push_back(var);
} // createVariable

//return process with parameter PID, NULL otherwise
Process* Mmu::getProcess(uint32_t pid)
{
    for (int i = 0; i< _processes.size(); i++)
    {
        if (_processes[i]->pid == pid)
        {
            return _processes[i];
        }
    }
    return NULL;
}

//return variable in process with parameter pid that has parameter name, NULL otherwise
Variable* Mmu::getVariable(uint32_t pid, std::string name)
{
    Process *proc = getProcess(pid);
    
    if (proc == NULL)
    {
        std::cout << "Invalid process ID" << std::endl;
        return NULL;
    }      
    
    for (int i = 0; i < getProcess(pid)->variables.size(); i++)
    {
        if (proc->variables[i]->name.compare(name) == 0)
        {
            return proc->variables[i];
        }
    }
    return NULL;
} // getVariable

// print the mmu
void Mmu::print()
{
    int i, j;

    std::cout << " PID  | Variable Name | Virtual Addr | Size" << std::endl;
    std::cout << "------+---------------+--------------+------------" << std::endl;
    for (i = 0; i < _processes.size(); i++)
    {
        uint32_t pid = _processes[i]->pid;

        for (j = 0; j < _processes[i]->variables.size(); j++)
        {
            // print all variables (excluding <FREE_SPACE> entries)
            Variable * var = _processes[i]->variables[j];

            //print PID
            std:: cout << pid << "  | ";
            //print name
            std::cout << var->name << "   |";
            // print virtual addr
            std::cout << "   0x" << std::hex << var->virtual_address << std::dec << " |";
            // print size
            std::cout << "   " << var->size;

            std::cout << std::endl;
        } // for j
    } // for i
} // print

//print the processes 
void Mmu::printProcesses () {
    for (int i =0; i < _processes.size(); i++) {
        std::cout << _processes[i]->pid << std::endl;
    }
} // printProcesses

// deletes the process
void Mmu::deleteProcess(uint32_t pid) {

    // delete the process
    for (int i = 0; i < _processes.size(); i++) {
        if (_processes[i]->pid == pid) {
            _processes.erase(_processes.begin() +  i);
        }
    } // for i
} // deleteProcess