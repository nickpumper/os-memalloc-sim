#include "pagetable.h"
#include <cmath>

PageTable::PageTable(int page_size)
{
    _page_size = page_size;
    //array of ints that is the same size as the number of frames in the system
    //used to keep track of which frames are free
    //1 = taken, free otherwise
    int size;
    int i;
    size = 67108864 / _page_size;
    std::cout << "Size is " << size << std::endl;
}

PageTable::~PageTable()
{
}

// adds a new <pid|page #, frame #> to the table, used when a process needs more memory
void PageTable::addEntry(uint32_t pid, int page_number)
{
    //done
    
    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);

    // Find free frame
    // TODO: implement this!
    //table: <entry, frame #>
    
    int i;
    for (i = 0; i < (67108864 / _page_size); i++)
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

void PageTable::removeEntry(uint32_t pid, int page_number)
{
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);
    if (_table.count(entry) > 0)
    {
        //reset frames array element
        frames[_table[entry]] = 0;
        
        _table.erase(entry);
    }
}

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
    int page_number = virtual_address >> offset_bits;

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
