#include "pagetable.h"

PageTable::PageTable(int page_size)
{
    _page_size = page_size;
}

PageTable::~PageTable()
{
}

void PageTable::addEntry(uint32_t pid, int page_number)
{
    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);

    // Find free frame
    // TODO: implement this!
    int frame = 0; 
    _table[entry] = frame;
}

int PageTable::getPhysicalAddress(uint32_t pid, int virtual_address)
{
    // Convert virtual address to page_number and page_offset
    // TODO: implement this!
    int page_number = 0;
    int page_offset = 0;

    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);
    
    // If entry exists, look up frame number and convert virtual to physical address
    int address = -1;
    if (_table.count(entry) > 0)
    {
        // TODO: implement this!
    }

    return address;
}

void PageTable::print()
{
    std::map<std::string, int>::iterator it;

    std::cout << " PID  | Page Number | Frame Number" << std::endl;
    std::cout << "------+-------------+--------------" << std::endl;

    for (it = _table.begin(); it != _table.end(); it++)
    {
        // TODO: print all pages
    }
}
