/******************************************************************************
proxy design pattern

*******************************************************************************/

#include <iostream>
#include <bits/stdc++.h>
using namespace std;




class Database{
    public:
    virtual void readData()=0;
    virtual ~ Database(){}
};

class ReadDatabase:public Database{
    public:
    void readData() override{
        cout<<"reading Database"<<endl;
    }
};


class DatabaseProxy:public Database{
    private:
    ReadDatabase *readDatabase;
    string role;
    bool hasAccess(){
        return role=="admin";
    }
    public:
    DatabaseProxy(string role):role(role),readDatabase(nullptr){}
    
    
    void readData() override{
        if(!hasAccess())cout<<"access denied"<<endl;
        else{
            if(readDatabase==nullptr)readDatabase=new ReadDatabase();
            readDatabase->readData();
        }
    }
    ~DatabaseProxy(){
        delete readDatabase;
    }
};





int main()
{
    Database* userDb = new DatabaseProxy("user");
    userDb->readData();   // Access denied

    cout << "-------------------" << endl;

    Database* adminDb = new DatabaseProxy("admin");
    adminDb->readData();  // Access granted

    delete userDb;
    delete adminDb;

    return 0;
}