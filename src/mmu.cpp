#include "mmu.h"
#include <map>

Mmu::Mmu(int memory_size)
{
    _next_pid = 1024;
    _max_size = memory_size;
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

    int num_vars_in_proc = getProcess(pid)->variables.size();

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
    var->virtual_address = num_vars_in_proc + 1; // first part is the page number
    
    // we need to get the page size over here
    // var->virtual_address = var->virtual_address << log2()

    std::cout << var->virtual_address << std::endl; // required print
    
    getProcess(pid)->variables.push_back(var);
}

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
}

void Mmu::print()
{
    int i, j;

    std::cout << " PID  | Variable Name | Virtual Addr | Size" << std::endl;
    std::cout << "------+---------------+--------------+------------" << std::endl;
    for (i = 0; i < _processes.size(); i++)
    {
        for (j = 0; j < _processes[i]->variables.size(); j++)
        {
            // TODO: print all variables (excluding <FREE_SPACE> entries)
        }
    }
}
