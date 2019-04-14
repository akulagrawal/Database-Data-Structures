#include <iostream>
#include<bits/stdc++.h>
using namespace std;
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
    int get_occupied_size();
    int get_record_id();
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
    void defragment();
    void print();
    int getPageSize();
    void setAvailableSize(int x);
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
    void defPage(int idx);
    void print();
};


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
    cout<<"Number of pages: "<<getPageNum()<<endl;

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

void HeapFile::defPage(int idx){
  int count = 0;
  for(std::list<Page>::iterator it = this->heapFile.begin(); it != this->heapFile.end(); it++){
    count++;
    if(count == idx)
    {
      (*it).defragment();
      return;
    }
  }
}

void Page::defragment() {
  vector<pair<int, Slot> > myp;
  for(std::vector<Slot>::iterator it = this->directory.begin(); it < this->directory.end(); it++) {
      if (it->isValid()) {
          myp.push_back(make_pair((*it).get_occupied_size(), (*it)));
      }
  }
  int n = myp.size();
  for(int i=0;i<n;i++)
  {
    for(int j=i+1;j<n;j++)
    {
      if(myp[i].first > myp[j].first)
        swap(myp[i], myp[j]);
    }
  }

  //sort(myp, myp.begin(),myp.end(), compare);
  //for(int i=0;i<myp.size();i++)
  //  cout<<myp[i].first<<" ";
  //cout<<endl;

  int x = getPageSize();
  setAvailableSize(x);

  this->directory.clear();

  for(int i=0;i<n;i++)
  {
    //cout<<"Insert "<<myp[i].second.get_record_id()<<" "<<myp[i].first<<endl;
    insertRecord(myp[i].second.get_record_id(), myp[i].first);
  }
}

int Page::getSlotSize() {
    return SLOT_SIZE;
}

int Page::getPageSize() {
    return this->pageSize;
}

void Page::setAvailableSize(int x) {
    this->availableSize = x;
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

int Slot::get_occupied_size() {
    return this->occupied_size;
}

int Slot::get_record_id() {
    return this->record_id;
}

void Slot::print() {
    // Print format
    // < Rid, startPos, occupiedSize - Gap, valid >
    std::cout << "< " << this->record_id << ", " << this->occupied_size<< ", " << this->start_pos + 1 << ", ";
    if (this->isValid()) std::cout << "True >";
    else std::cout << "False >";
}




int main()
{
    int pageSize = 100;

    HeapFile d(pageSize);
    bool create_diskFile = false;
    bool inserted_records = false;
    while(true){
        int x;
        printf("\nEnter :\n 1 : Create New HeapFile\n 2 : Insert Record\n 3 : Delete Record\n 4 : Display HeapFile Structure\n 5 : Defragment a page\n 6 : Defragment whole heapfile\n -1 : Exit\n");
        printf("Choice : ");
        cin >> x;
        switch (x)
        {
           case 1: {
                       if(create_diskFile == false){
                           printf("Enter size and number of DataPages to create : ");
                           int n;
                           int pageSize = 100;
                           cin >> pageSize;
                           cin >> n;
                           pageSize -= 4;


                           d = HeapFile(n, pageSize);
                           create_diskFile = true;
                       }
                       else
                           printf("HeapFile has already been created.\n");
                   }
                   break;

           case 2: {
                       if(create_diskFile){
                           printf("Enter Record identifier in int to insert into Pages : ");
                           int id;
                           cin >> id;
                           printf("Enter Record length in bytes : ");
                           int l;
                           cin >> l;
                           int pageId = d.insertRecord(id, l);
                           if (pageId > 0)
                                cout << "Record inserted in Page : " << pageId << endl;
                       }
                       else
                           printf("You can not enter Records into Pages without creating the initial HeapFile.\n");
                   }
                   break;

            case 3: {
                        if(create_diskFile){
                            printf("Enter Record identifier in int to delete from Pages : ");
                            int id;
                            cin >> id;
                            int pageId = d.deleteRecord(id);
                            if (pageId > 0)
                                cout << "Record deleted from Page : " << pageId << endl;
                            else
                                cout << "Record is not found." << endl;
                        }
                        else
                            printf("You can not delete Records without creating the initial HeapFile or without inserting few Records into it.\n");
                    }
                    break;

            case 4: {
                       if(create_diskFile){
                            d.print();
                        }
                        else
                            printf("You can not view Pages & Records without creating the initial HeapFile.\n");
                    }
                    break;

            case 5: {
                      int idx;
                      cin>>idx;
                      if(create_diskFile && (idx <= d.getPageNum())){
                        d.defPage(idx);
                      }
                      else
                        printf("Invalid Input\n");
                    }
                    break;

            case 6: {
                      int n = d.getPageNum();
                      for(int i=1;i<=n;i++)
                        d.defPage(i);
                    }
                    break;

            case -1: exit(0);

            default: printf("Choice other than 1, 2, 3, 4 and -1\n");
                    continue;
        }
    }
    return 0;
}
