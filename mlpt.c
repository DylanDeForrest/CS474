#include <stdio.h>
#include <string.h>

const int PAGE_SIZE = 256;

char physical_ram[65536];

/*
 * Return the starting address for a given page number.
 */
int get_page_start_addr(int page)
{
    return page * PAGE_SIZE;
}

/*
 * For a given physical page and page table entry number, return the
 * entry value. This works on the directory page and the page table
 * page.
 */
int get_page_table_entry(int ppage, int entry)
{
    int entry_addr = get_page_start_addr(ppage) + (entry * sizeof(int));

    return *(int*)(physical_ram + entry_addr);
}

/*
 * For a given physical page and page table entry number, set the entry
 * value. This works on the directory page and the page table page.
 */
void set_page_table_entry(int ppage, int entry, int value)
{
    int entry_addr = get_page_start_addr(ppage) + (entry * sizeof(int));
    *(int*)(physical_ram + entry_addr) = value;

}


/*
 * For a given virtual address, follows the two-level page table and
 * returns the corresponding physical address. Should return 0 if
 * there's a page fault.
 */
int vaddr_to_paddr(int dir_page_num, int vaddr)
{
    int dir_idx = (vaddr >> 16) & 0xFF;
    int table_idx = (vaddr >> 8) & 0xFF;
    int offset = vaddr & 0xFF;

    // 1. Get Page Table Page Number from Directory
    int table_page_num = get_page_table_entry(dir_page_num, dir_idx);
    
    if (table_page_num == 0) return 0; // Page Fault

    // 2. Get Physical Page Number from Page Table
    int phys_page_num = get_page_table_entry(table_page_num, table_idx);
    
    if (phys_page_num == 0) return 0; // Page Fault

    // 3. Calculate final physical address
    return get_page_start_addr(phys_page_num) + offset;
}

/*
 * Main
 */
int main(void)
{
    // Set up the page tables for this process. This would be the job of
    // the OS.
    //
    // The way this is set up, virtual address 3876654 will map to
    // physical address 6958.
    //
    // 3876654 is (59<<16)|(39<<8)|46, directory entry 59, page table
    // page 39. Offset 46.
    //
    // For your solution, you can't reference page 8 or 27 or address
    // 6958 directly--you must start with dir_page_num and follow the
    // pointers.
    //
    int dir_page_num = 4;

    set_page_table_entry(dir_page_num, 59, 8);
    set_page_table_entry(8, 39, 27);

    // Store some data in physical memory for testing at the same
    // place that the virtual location 3876654 maps to.
    strcpy(physical_ram + 6958, "Hello, world!");

    // Look up the physical address for virtual address 3876654
    int paddr = vaddr_to_paddr(dir_page_num, 3876654);

    // Print what's there--which should be "Hello, world!"
    puts(physical_ram + paddr);
}

