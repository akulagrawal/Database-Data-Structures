#include "heapFile.h"
#include <iostream>

HeapFile::HeapFile(int pageSize) {
    this->pageSize = pageSize;
}

HeapFile::HeapFile(int numPages, int pageSize) {
    this->pageSize = pageSize;
    for(size_t i = 0; i < numPages; i++) {
        Page newPage(pageSize);
        this->addPage(newPage);
    }
}

int HeapFile::getPageNum() {
    return this->heapFile.size();
}

void HeapFile::addPage(Page page) {
    this->heapFile.push_back(page);
}

// Returns page id in which record is inserted
int HeapFile::insertRecord(int Rid, int size) {
    bool inserted = false;
    int pageNum = 0;
    
    // Search over list of pages and check if it can accomodate the record

    for(std::list<Page>::iterator it = this->heapFile.begin(); it != this->heapFile.end(); it++) {
        pageNum ++;
        if (it->canAccomodate(size)) {
            bool success = it->insertRecord(Rid, size);
            inserted = true;
            if (!success) {
                std::cout << "Record size exceeded Page size\n";
                return -1;
            }
            else
                return pageNum;
        }
    }
    
    // If no page can accomodate the record, create a new page

    if (!inserted) {
        pageNum ++;
        // Create new page
        Page newPage(this->pageSize);
        // Insert record into this page
        bool success = newPage.insertRecord(Rid, size);
        if (!success) {
            std::cout << "Record size exceeded Page size\n";
            return -1;
        }
        // Insert it into heapfile
        this->addPage(newPage);
        return pageNum;
    }
}

// Returns PageId from which record is deleted, if deleted successfully
int HeapFile::deleteRecord(int Rid) {
    int pageNum = 0;
    
    // Iterate over pages and find the record

    for(std::list<Page>::iterator it = this->heapFile.begin(); it != this->heapFile.end(); it++) {
        pageNum ++;
        if (it->searchRecord(Rid)) {
            it->deleteRecord(Rid);
            return pageNum;
        }
    }
    
    // Returns -1 if not deleted successfully
    return -1;
}

bool HeapFile::searchRecord(int Rid) {

    // Iterate over pages and find the record

    for(std::list<Page>::iterator it = this->heapFile.begin(); it != this->heapFile.end(); it++) {
        if (it->searchRecord(Rid)) {
            return true;
        }
    }
    return false;
}

void HeapFile::print() {
    int pageNum = 1;

    for(std::list<Page>::iterator it = this->heapFile.begin(); it != this->heapFile.end(); it++) {
        std::cout << "Page " << pageNum++ << " : ";
        it->print();
    }

    std::cout << std::endl;
}


Page::Page(int pageSize) {
    this->pageSize = pageSize;
    this->availableSize = pageSize;
}

bool Page::searchRecord(int Rid) {

    // Iterate over slots and check validity and RecordId

    for(std::vector<Slot>::iterator it = this->directory.begin(); it < this->directory.end(); it++) {
        if (it->isValid() && it->matchRecord(Rid)) {
            return true;
        }
    }
    
    return false;
}

bool Page::canAccomodate(int size) {

    // If available size >= size + new_slot_size
    if (availableSize >= size + this->getSlotSize())
        return true;
    
    // Check in the available slots

    for(std::vector<Slot>::iterator it = this->directory.begin(); it < this->directory.end(); it++) {
        if (!it->isValid() && it->size() >= size) {
            return true;
        }
    }
    
    return false;
}

void Page::deleteRecord(int Rid) {

    // Iterate over slots and check validity and RecordId

    for(std::vector<Slot>::iterator it = this->directory.begin(); it < this->directory.end(); it++) {
        if (it->isValid() && it->matchRecord(Rid)) {
            it->resetValidBit();
            return;
        }
    }
}

int Page::getSlotSize() {
    return SLOT_SIZE;
}

bool Page::insertRecord(int Rid, int size) {
    int endPos = 0;

    // If size exceeds maximum size
    if (size > this->pageSize - this->getSlotSize())
        return false;

    // Check if can accomodate in existing slots
    
    for(std::vector<Slot>::iterator it = this->directory.begin(); it < this->directory.end(); it++) {
        if (!it->isValid() && it->size() >= size) {
            it->setRecordId(Rid);
            it->setOccupiedSize(size);
            it->setValidBit();
            return true;
        }
        endPos = it->getEndPos();
    }

    // If cannot accomodate, create a new slot
    Slot newSlot(size);
    newSlot.setRecordId(Rid);
    newSlot.setStartPos(endPos);
    newSlot.setOccupiedSize(size);
    newSlot.setValidBit();

    this->directory.push_back(newSlot);

    // Update available size in page
    this->availableSize -= size + this->getSlotSize();

    return true;
}

void Page::print() {
    std::cout << "Records : ";
    for(std::vector<Slot>::iterator it = this->directory.begin(); it < this->directory.end(); it++) {
        it->print();
    }
    if (this->directory.empty())
        std::cout << "Empty";
    std::cout << std::endl;
}

Slot::Slot(int size) {
    this->total_size = size;
}

int Slot::getEndPos() {
    return this->start_pos + this->total_size;
}

bool Slot::isValid() {
    return this->validBit;
}

bool Slot::matchRecord(int Rid) {
    return this->record_id == Rid;
}

void Slot::resetValidBit() {
    this->validBit = false;
}

void Slot::setValidBit() {
    this->validBit = true;
}

void Slot::setOccupiedSize(int size) {
    this->occupied_size = size;
}

void Slot::setRecordId(int Rid) {
    this->record_id = Rid;
}

void Slot::setStartPos(int start_pos) {
    this->start_pos = start_pos;
}

int Slot::size() {
    return this->total_size;
}

void Slot::print() {
    // Print format
    // < Rid, startPos, occupiedSize - Gap, valid >
    std::cout << "< " << this->record_id << ", " << this->start_pos << ", " << this->occupied_size << " - ";
    std::cout << this->total_size - this->occupied_size << ", ";
    if (this->isValid()) std::cout << "True >";
    else std::cout << "False >";
}