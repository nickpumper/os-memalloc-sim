#include <iostream>
#include <cstring>
#include <string>
#include "mmu.h"
#include "pagetable.h"
#include <ctype.h>

void printStartMessage(int page_size);

int main(int argc, char **argv)
{
    // Ensure user specified page size as a command line parameter
    if (argc < 2)
    {
        fprintf(stderr, "Error: you must specify the page size\n");
        return 1;
    }

    
    int page_size = std::stoi(argv[1]);
    const int stack_size_constant = 65536; // in bytes (as per instructions)

    // Create physical 'memory'
    uint8_t *memory = new uint8_t[67108864]; // 64 MB of RAM (64 * 1024 * 1024)
    
    PageTable pagetable = PageTable(page_size);  // Create virtual 'memory'
    Mmu mmu = Mmu(67108864, page_size); // Create MMU

    printStartMessage(page_size); // Print opening instuction message

    // Prompt loop
    std::string command;
    char* token;
    std::cout << "> ";
    std::getline (std::cin, command);
    //get command 
    token = strtok(strdup(command.c_str()), " ");  
    while (command != "exit") {

        //physical memory stored in memory[] (main.cpp)
        //page table (virtual memory) stored in std::map<std::string, int> _table (pagetable.h, private)
        //processes stored in std::vector<Process*> _processes (mmu.h, private)
        //process variables stored in std::vector<Variable*> variables; (mmu.h)
        
        // this should be done now
        if (strcmp(token, "create")==0)
        {
            // get command line variables
            token = strtok(NULL, " "); 
            int text_size = std::stoi(token);
            token = strtok(NULL, " "); 
            int data_size = std::stoi(token);

            //Initializes a new process
            uint32_t pid =  mmu.createProcess();
            std::cout << pid << std::endl; // required print
                
            // the following allocs must be done in this order: 
            //allocate <text_size> bytes of memory with variable name <TEXT>
            //allocate <data_size> bytes of memory with variable name <GLOBALS>
            //allocate 65536 bytes of memory with variable name <STACK>

            // DOUCLE CHECK THE TYPE
            mmu.createVariable(pid, "<TEXT>", "char", text_size); 
            mmu.createVariable(pid, "<GLOBALS>", "char", data_size);
            mmu.createVariable(pid, "<STACK>", "char", stack_size_constant);
            
        } // "create"
        else if (strcmp(token, "allocate")==0)
        {
            //allocate <PID> <var_name> <data_type> <number_of_elements>
            //Allocate memory on the heap (how much depends on the data type and the number of elements)
            //Print the virtual memory address
            
            //get <PID>
            token = strtok(NULL, " "); 
            uint32_t pid = std::stoi(token); //convert token to int
            
            //get <var_name>
            token = strtok(NULL, " ");
            std::string var_name = token; 
            
            //get <data_type>
            token = strtok(NULL, " ");
            std::string data_type = token;   
            
            //get <number_of_elements>
            token = strtok(NULL, " "); 
            uint32_t number_of_elements = std::stoi(token); //convert token to int

            //create variable for process based on tokens
            // this will print the virtual mem addr 
            mmu.createVariable(pid, var_name, data_type, number_of_elements);

            // allocate memory
            // page_number isnt right yet: doesn't account for when we have enough vars that it foes over the page size
            int page_number = mmu.getProcess(pid)->variables.size() + 1;
            // pagetable.addEntry(pid, page_number);
        } // "allocate"
        else if (strcmp(token, "set")==0)
        {
            //set <PID> <var_name> <offset> <value_0> <value_1> <value_2> ... <value_N>
            //Set the value for variable <var_name> starting at variable number <offset>
            //Multiple contiguous values can be set with one command

            //get <PID> 
            token = strtok(NULL, " "); 
            uint32_t pid = std::stoi(token); //convert token to int         

            //get <var_name>
            token = strtok(NULL, " ");
            std::string var_name = token;  

            //get <offset>
            token = strtok(NULL, " "); 
            uint32_t offset = std::stoi(token); //convert token to int

            //set values
            Variable *var = mmu.getVariable(pid, var_name);
            int index = offset;
            token = strtok(NULL, " "); 
            if (var != NULL)
            {
                while (token != NULL && index < var->values.size())
                {
                    var->values.at(index) = token;
                    // std::cout << "value " << index << " set to " << var->values[index] << std::endl;
                    index++;
                    token = strtok(NULL, " "); 
                } 
            }
        }
        else if (strcmp(token, "free")==0)
        {
            //free <PID> <var_name> 
            //Deallocate memory on the heap that is associated with <var_name>.
            
            //get <PID> 
            token = strtok(NULL, " "); 
            uint32_t pid = std::stoi(token); //convert token to int    

            // get variable name
            std::string var_name = strtok(NULL, " "); 

        } // "free"
        else if (strcmp(token, "terminate")==0)
        {
            //terminate <PID>
            //Kill the specified process

            //get <PID> 
            token = strtok(NULL, " "); 
            uint32_t pid = std::stoi(token); //convert token to int 

            mmu.deleteProcess(pid); // delete the process

            // delete all pages associated with it
                
        } // "terminate"
        else if (strcmp(token, "print")==0)
        {
            //print <object>
                
            //get token <object>
            token = strtok(NULL, " :");  
                
            //compare token to possible objects
            if (strcmp(token, "mmu")==0)
            {
                //print mmu memory table
                mmu.print();
            } // if "mmu"
            else if (strcmp(token, "page")==0)
            {
                //print page table (do not need to print anything for free frames)
                pagetable.print();
            } // if "page"
            else if (strcmp(token, "processes")==0)
            {
                //print a list of PIDs for processes that are still running
                mmu.printProcesses();
            } // if "processes"
            //if <object> is a "<PID>:<var_name>...", print the value of the variable(s) for that process
            else if (mmu.getProcess(std::stoi(token)) != NULL)  // verify the PID
            { 
                uint32_t pid = std::stoi(token);
                token = strtok(NULL, " :");
                // now verify the var name
                if (mmu.getVariable(pid, token) != NULL) {
                    Variable * var = mmu.getVariable(pid, token);
                    for (int i = 0; i < 4; i++) {
                        if (var->values[i] == NULL) {
                            std::cout << "NULL";
                        } else {
                            std::cout << var->values[i];
                        }

                        if (i != (var->values.size() - 1)) {
                            std::cout <<", ";
                        }

                        // If variable has more than 4 elements, just print the first 4 followed by "... [N items]" (where N is the number of elements)
                        if ((i == 3) && (var->values.size() > 4)) {
                            std::cout << " ... [" << var->values.size() << " items]";
                        }
                    } // for
                    std::cout << std::endl;
                } else {
                    std::cout << "Error: Invalid object." << std::endl;
                }
            } // //if <object> is a "<PID>:<var_name>...",
            else
            {
                //invalid object
                std::cout << "Error: Invalid object." << std::endl;
            }    
        } // "print"
        else
        {
            std::cout << "Error: not a valid command." << std::endl;
        }

        // Get next command
        std::cout << "> ";
        std::getline (std::cin, command);
        token = strtok(strdup(command.c_str()), " ");  
    }


    return 0;
}

void printStartMessage(int page_size)
{
    std::cout << "Welcome to the Memory Allocation Simulator! Using a page size of " << page_size << " bytes." << std:: endl;
    std::cout << "Commands:" << std:: endl;
    std::cout << "  * create <text_size> <data_size> (initializes a new process)" << std:: endl;
    std::cout << "  * allocate <PID> <var_name> <data_type> <number_of_elements> (allocated memory on the heap)" << std:: endl;
    std::cout << "  * set <PID> <var_name> <offset> <value_0> <value_1> <value_2> ... <value_N> (set the value for a variable)" << std:: endl;
    std::cout << "  * free <PID> <var_name> (deallocate memory on the heap that is associated with <var_name>)" << std:: endl;
    std::cout << "  * terminate <PID> (kill the specified process)" << std:: endl;
    std::cout << "  * print <object> (prints data)" << std:: endl;
    std::cout << "    * If <object> is \"mmu\", print the MMU memory table" << std:: endl;
    std::cout << "    * if <object> is \"page\", print the page table" << std:: endl;
    std::cout << "    * if <object> is \"processes\", print a list of PIDs for processes that are still running" << std:: endl;
    std::cout << "    * if <object> is a \"<PID>:<var_name>\", print the value of the variable for that process" << std:: endl;
    std::cout << std::endl;
}
