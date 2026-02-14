/******************************************************************************
Chain of responsibilty design pattern

*******************************************************************************/

#include <iostream>
#include <bits/stdc++.h>
using namespace std;




enum LogLevel{
    INFO = 1,
    DEBUG= 2,
    ERROR = 3,
    
};



class Logger{
 protected:
    LogLevel level;
    Logger* nextLogger;
    
 public:
    Logger(LogLevel level) : level(level), nextLogger(nullptr) {}
    
    void setNext(Logger *next){
        nextLogger = next;
    }
    
    void logMessage(LogLevel messageLevel,const string& message){
        if(messageLevel == level){
            write(message);
        }
        if (nextLogger!=nullptr){
            nextLogger->logMessage(messageLevel,message);
        }
    }
    
    virtual void write(const string& message) = 0;
};

class InfoLogger:public Logger{
    public:
    InfoLogger() : Logger(INFO) {}
    
    void write(const string& message) override {
        cout << "inside info log [INFO]: " << message << endl;
    } 
    
};

class DebugLogger : public Logger {
public:
    DebugLogger() : Logger(DEBUG) {}

    void write(const string& message) override {
        cout << "inside debug log [DEBUG]: " << message << endl;
    }
};


class ErrorLogger : public Logger {
public:
    ErrorLogger() : Logger(ERROR) {}

    void write(const string& message) override {
        cout << "inside error log [ERROR]: " << message << endl;
    }
};

Logger* buildLoggerChain() {
    Logger* info = new InfoLogger();
    Logger* debug = new DebugLogger();
    Logger* error = new ErrorLogger();

    info->setNext(debug);
    debug->setNext(error);

    return info; // head
}


int main()
{
 
    
    //INFO → DEBUG → ERROR
    
    // Logger *infoLogger = new InfoLogger();
    // Logger *debugLogger = new debugLogger();
    // Logger *errorLogger = new errorLogger();
    
    
    // infoLogger->setNext = debugLogger;
    // debugLogger->setNext = errorLogger;
    // errorLogger->setNext = nullptr;
    Logger *logger = buildLoggerChain();
    logger->logMessage(ERROR,"DB failed");
    
    

    return 0;
}