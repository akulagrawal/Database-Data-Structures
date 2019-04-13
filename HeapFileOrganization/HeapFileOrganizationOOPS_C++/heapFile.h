#ifndef HEAPFILE_H
#define HEAPFILE_H

#include <vector>
#include <list>

#define SLOT_SIZE 13

class Slot
{
private:
    int record_id;
    int start_pos;
    int occupied_size;
    int total_size;
    bool validBit;

public:
    Slot(int totalSize);
    void setValidBit();
    void resetValidBit();
    bool isValid();
    int size();
    bool matchRecord(int Rid);
    void setRecordId(int Rid);
    void setOccupiedSize(int size);
    int getEndPos();
    void setStartPos(int start);
    void print();
};

class Page
{
private:
    std::vector<Slot> directory;
    int availableSize;
    int pageSize;

public:
    Page (int pageSize);
    int getSlotSize();
    bool canAccomodate(int size);    
    bool insertRecord(int Rid, int size);
    bool searchRecord(int Rid);
    void deleteRecord(int Rid);
    void print();
};

class HeapFile
{
private:
    std::list<Page> heapFile;
    int pageSize;

public:
    HeapFile(int pageSize);
    HeapFile(int numPages, int pageSize);

    int getPageNum();
    void addPage(Page page);
    int insertRecord(int Rid, int size);
    int deleteRecord(int Rid);
    bool searchRecord(int Rid); 
    void print();
};

#endif