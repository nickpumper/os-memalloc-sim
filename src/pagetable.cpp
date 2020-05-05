#include "pagetable.h"
#include <cmath>
#include "mmu.h"

PageTable::PageTable(int page_size)
{
    _page_size = page_size;

    //array of ints that is the same size as the number of frames in the system
    //used to keep track of which frames are free
    //1 = taken, free otherwise
    int size;
    size = 67108864 / _page_size;
    //std::cout << "Size is " << size << std::endl;
    
    //we need to populate frames[] with empty frames 
    for (int i = 0 ; i < size; i++) {
        frames.push_back(0);
    } // for
}

PageTable::~PageTable()
{
}

// adds a new <pid|page #, frame #> to the table, used when a process needs more memory
void PageTable::addEntry(uint32_t pid, int page_number)
{    
    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);

    // Find free frame
    //table: <entry, frame #>
    int i;
    for (i = 0; i < frames.size(); i++)
    {
        if (frames[i] != 1)
        {
            //match entry to free frame if one is found, then set frames[i] to 1 (taken)
            _table[entry] = i;
            frames[i] = 1;
            return;
        }
    } // for
    
    //if no free frame is found
    std::cout << "Not enough memory for another entry." << std::endl;
} // addEntry

// deletes all of the var (notably vars that are arrays)
void PageTable::deletePagesOfVar(uint32_t pid, Variable* var) {
    //delete page which virtual address is in
    int page_number = getPageNumberFromVirtualAddr(var->virtual_address);
    removeEntry(pid, page_number);
    
    //if variable is larger than page_size, delete next page and reduce var size by page_size, repeat
    int temp_size = var->size;
    while (temp_size > _page_size)
    {
        page_number++;
        removeEntry(pid, page_number);
        temp_size -= _page_size;
    }
} // deletePagesOfVar

void PageTable::removeEntry(uint32_t pid, int page_number)
{
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);
    if (_table.count(entry) > 0)
    {
        //reset frames array element
        frames[_table[entry]] = 0;
        
        _table.erase(entry);
    }
} // removeEntry

int PageTable::getPhysicalAddress(uint32_t pid, int virtual_address)
{
    // done
    
    // Convert virtual address to page_number and page_offset
    // TODO: implement this!
    
    //virtual address is 32 bits long
    //rightmost log2(_page_size) bits are page offset
    //rest are page number
    int offset_bits = log2(_page_size);
    
    int page_offset = virtual_address & ((1 << offset_bits) - 1);
    int page_number = getPageNumberFromVirtualAddr(virtual_address);

    // Combination of pid and page number act as the key to look up frame number
    //page table keeps track of what entry (pid and page number) corresponds to which frame
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);
    
    // If entry exists, look up frame number and convert virtual to physical address
    int address = -1;
    if (_table.count(entry) > 0)
    {
        int frame_number = _table[entry];
        address = frame_number + page_offset;
    }

    return address;
} // getPhysicalAddress

void PageTable::print()
{
    std::map<std::string, int>::iterator it;

    std::cout << " PID  | Page Number | Frame Number" << std::endl;
    std::cout << "------+-------------+--------------" << std::endl;

    for (it = _table.begin(); it != _table.end(); it++)
    {
        // TODO: print all pages
        std::cout << " " << it->first << " | " << it->second << std::endl;
    }
} // print

int PageTable::getPageNumberFromVirtualAddr (int virtual_addr) {
    int offset_bits = log2(_page_size);
    return (virtual_addr >> offset_bits);
} // getPageNumberFromVirtualAddr
