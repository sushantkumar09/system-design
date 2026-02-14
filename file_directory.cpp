/******************************************************************************

File directory design using composite design pattern

*******************************************************************************/

#include <iostream>
#include <bits/stdc++.h>
using namespace std;




class FileSystemComponent{
public:
    virtual void printContent()=0;
    virtual ~FileSystemComponent() {}
};


class File:public FileSystemComponent{
private:
    string fileName;
public:
    File(string fileName):fileName(fileName){}
    void printContent() override{
        cout<<"File name "<<fileName<<endl;
    }
    
};

class Directory:public FileSystemComponent{
private:    
    vector<FileSystemComponent*>childrens;
    string directoryName;
public:
    Directory(string directoryName):directoryName(directoryName){}
    
    void add(FileSystemComponent *fileSystemComponent){
        childrens.push_back(fileSystemComponent);
    }
    void reomve(FileSystemComponent *fileSystemComponent){
        childrens.erase(remove(childrens.begin(), childrens.end(), fileSystemComponent), childrens.end());
    }
    
    void printContent() override{
        cout<<"Directory Name:"<<directoryName<<endl;
        for(auto it:childrens){
            it->printContent();
        }
    }

    
    
};

int main()
{
    File *reciept = new File("reciept.pdf");
    File *invoice = new File("invoice.pdf");
    File *torrent = new File("torrent.txt");
    File *avengers = new File("avengers.mp3");
    File *ironman = new File("ironman,mp3");
    File *batman = new File("batman.mp3");
    
    Directory *movies = new Directory("movies");
    Directory *marvelMovieDirector = new Directory("marvels");
    
    movies->add(reciept);
    movies->add(invoice);
    movies->add(torrent);
    movies->add(marvelMovieDirector);
    movies->add(batman);
    marvelMovieDirector->add(ironman);
    marvelMovieDirector->add(avengers);
    movies->add(marvelMovieDirector);
    
    movies->printContent();
    
    return 0;
}