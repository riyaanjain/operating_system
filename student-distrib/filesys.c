/* filesys.c - Functions for supporting operations on the file system image
 * vim:ts=4 noexpandtab
 */

#include "filesys.h"

/* General Driver Functions */

/* void init_file_system()
 *  Functionality: 
 *  Arguments: 
 *  Return: None
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
void init_file_sys(uint32_t filesys_ptr) {
    boot_block = (boot_block_t*)(filesys_ptr);
    dentry_block = boot_block->dir_entries;
    inode_block = (inode_t*)(boot_block + 1);
    file_count = 0;
}

/* File functions */

/* uint32_t open_file()
 *  Functionality: Initialize any temporary structures
 *  Arguments: const uint8_t* filename - file name that needs to be opened
 *  Return: Return 0, Return -1 for failure
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
uint32_t open_file(const uint8_t* filename) {
    /* We want to use read_dentry_by_name to establish that we are opening a valid directory */
    directory_entry_t temp; // temp var to hold directory_entry
    if (read_dentry_by_name(filename, &temp)) { // check entry value, if != 0 then return failure
        return -1;
    }
    return 0;
}

/* uint32_t close_file()
 *  Functionality: Undo what you did in the open function
 *  Arguments: const uint8_t* filename - file name that needs to be closed
 *  Return: Return 0
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
uint32_t close_file(const uint8_t* filename) {
    return 0;
}

/* uint32_t write_file()
 *  Functionality: Should do nothing
 *  Arguments: const uint8_t* filename - filename we want to write to
 *             const void* buffer - pointer to data that is to be written to
 *             int32_t size - size of data pointed to by buffer in bytes
 *  Return: Return -1
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
uint32_t write_file(const uint8_t* filename, void* buffer, int32_t size) {
    return -1;
}

/* uint32_t read_file()
 *  Functionality: reads count bytes of data from file into buf, uses read_data
 *  Arguments: 
 *  Return:
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
uint32_t read_file(const uint8_t* filename, void* buffer, int32_t size) {
    /* TODO */
    if (strlen(filename) > 32) {
        return -1;
    }
    directory_entry_t dentry;
    int i;
    read_dentry_by_name(filename, &dentry);
    int32_t length = read_data(dentry.inode_number, 0, buffer, size);
    if (length != -1) {
        for (i = 0; i < length; i++) {
            printf("%c", ((char*)buffer)[i]);
        }
        printf("\n");
        return 0;
    } 
    return -1;
}

/* Directory functions */

/* uint32_t open_directory()
 *  Functionality: Opens a directory file (note file types) uses read_dentry_by_name
 *  Arguments: const uint8_t* file_directory - file directory that needs to be closed
 *  Return: Return 0, or -1 upon failure from read_dentry_by_name
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
uint32_t open_directory(const uint8_t* file_directory) {
    /* We want to use read_dentry_by_name to establish that we are opening a valid directory */
    directory_entry_t temp; // temp var to hold directory_entry
    if(!read_dentry_by_name(file_directory, &temp)) // check entry value, if == 0 then return failure
    {
        return -1;
    }
    return 0;
}

/* uint32_t close_directory()
 *  Functionality: Probably does nothing
 *  Arguments: const uint8_t* file_directory - file directory that needs to be closed
 *  Return: Return 0
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
uint32_t close_directory(const uint8_t* file_directory) {
    return 0;
}

/* uint32_t close_directory()
 *  Functionality: Should do nothing
 *  Arguments: const uint8_t* file_directory - directory we want to write to
 *             const void* buffer - pointer to data that is to be written to
 *             int32_t size - size of data pointed to by buffer in bytes
 *  Return: Return -1
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
uint32_t write_directory(const uint8_t* file_directory, void* buffer, int32_t size) {
    return -1;
}

/* uint32_t read_directory()
 *  Functionality: read files filename by filename, including ".", uses read_dentry_by_index
 *  Arguments: const uint8_t* file_directory - directory we want to write to
 *             const void* buffer - pointer to data that is to be written to
 *             int32_t size - size of data pointed to by buffer in bytes
 *             int32_t index - index in boot block
 *  Return: int length - size of filename
 *  References: Week 9 Discussion
 ***********************************************************************************
 *  IMPORTANT NOTICE FOR READER
 */
uint32_t read_directory(const uint8_t* file_directory, void* buffer, int32_t size) {
    /* We want to call read_dentry_by_index with index in boot_block_t */
    /* The index in boot entries block is dir_entires (read directory entry
       at a particular position within the boot block)*/
    directory_entry_t temp; // temp var to hold directory_entry indexes
    int ret = read_dentry_by_index(file_count, &temp); // call funct with passed index
    if (ret == -1) {
        return 0;
    }
    int length = strlen((char*)temp.filename); // get length of filename
    if (length > MAX_FILENAME_LENGTH) { // 32 bytes should be max for filename
        length = MAX_FILENAME_LENGTH; // if not max size for filename, only read what fits
    }
    memcpy(buffer, temp.filename, length); // use memcpy to copy filename to buffer
    file_count++;
    return length; // return length of filename
}

int32_t read_dentry_by_name(const uint8_t* fname, directory_entry_t* dentry) {
    int i;
    for (i = 0; i < NUM_FILES; i++) { // loop through all files
        if ((strncmp((int8_t*)(fname), (int8_t*)((dentry_block + i)->filename), MAX_FILENAME_LENGTH) == 0)) { // check if file names match
            strncpy(dentry->filename, (dentry_block + i)->filename, MAX_FILENAME_LENGTH); // copy over filename, file type, inode number
            dentry->file_type = (dentry_block + i)->file_type;
            dentry->inode_number = (dentry_block + i)->inode_number;
            return 0;
        }
    }
    return -1;
}


int32_t read_dentry_by_index(uint32_t index, directory_entry_t* dentry) {
    if (index >= NUM_FILES || index < 0) { // check if index in range
        return -1;
    }
    strncpy(dentry->filename, (dentry_block + index)->filename, MAX_FILENAME_LENGTH); // copy over filename, file type, inode number
    dentry->file_type = (dentry_block + index)->file_type;
    dentry->inode_number = (dentry_block + index)->inode_number;
    return 0;
}


int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length) {
    // if invalid inode num, return -1
    if (inode < 0 || inode > boot_block->inodes - 1) {
        return -1;
    }

    // init vars
    int32_t num_read_bytes = 0;
    int32_t data_idx = offset / DATA_BLOCK_SIZE;
    int32_t block_offset = offset % DATA_BLOCK_SIZE;

    // loop while reading less than length
    while (num_read_bytes < length && num_read_bytes + offset < (inode_block + inode)->length) {
        if ((inode_block + inode)->data_blocks[data_idx] > boot_block->data_blocks - 1) { // if invalid data block
            return -1;
        }

        // copy correct data block val to buf
        uint32_t data_block_num = (inode_block + inode)->data_blocks[data_idx];
        uint8_t* data_block_beginning = (uint8_t*)(boot_block + boot_block->inodes + 1);
        buf[num_read_bytes] = *(data_block_beginning + (DATA_BLOCK_SIZE * data_block_num) + block_offset);

        // increment offset and num bytes read
        num_read_bytes++;
        block_offset++;

        // reset offset and move to next data index
        if (block_offset > DATA_BLOCK_SIZE) {
            block_offset = 0;
            data_idx++;
        }
    }

    return num_read_bytes;
}
