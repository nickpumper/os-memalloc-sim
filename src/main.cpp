#include <iostream>
#include <string>
#include "mmu.h"
#include "pagetable.h"

void printStartMessage(int page_size);

int main(int argc, char **argv)
{
    // Ensure user specified page size as a command line parameter
    if (argc < 2)
    {
        fprintf(stderr, "Error: you must specify the page size\n");
        return 1;
    }

    // Print opening instuction message
    int page_size = std::stoi(argv[1]);
    printStartMessage(page_size);

    // Create physical 'memory'
    uint8_t *memory = new uint8_t[67108864]; // 64 MB (64 * 1024 * 1024)

    // create MMU
    Mmu * mmu = new Mmu(page_size); 

    // Prompt loop
    std::string command;
    std::cout << "> ";
    std::getline (std::cin, command);
    while (command != "exit") {
        
        // Parse apart the command
        char * tok;
        char* cmd = (char *) command.c_str();
        std::vector<std::string> commands;
        int num_commands = 0;
        std::string command_token;
        
        tok = strtok (cmd," ");
        while (tok != NULL)
        {
            commands.push_back(tok);
            tok = strtok (NULL, " ");
            // std::cout << commands[num_commands] << "\n";
            num_commands++;
        }

        // handle the command
        if (!command.empty()) {
            std::string create = "create";
            if (commands[0].compare(create) == 0) {
                // create the new process
                uint32_t pid = mmu->createProcess();
                std::cout << pid << "\n"; // print the PID 

            } // if create
        } // if !command.empty     

        // Get next command
        std::cout << "> ";
        std::getline (std::cin, command);
    } // while (command != "exit")


    return 0;
} // main

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