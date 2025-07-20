#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <string.h>

// #include<boost/asio.hpp>
using namespace std;


class Message{

    public:
        
        enum {maxBytes = 512};
        enum {header = 4};

        Message():bodyLength_(0){
            memset(data, 0, sizeof(data));
        }

        Message(string message){
            memset(data, 0, sizeof(data));
            bodyLength_ = getNewBodyLength(message.size());
            
            encodeheader();
            memcpy(data + header, message.c_str(),bodyLength_);
        };

        void printMessage(){
            string message = getData();
            cout<<"Message received: "<<message<<endl;
        }

        size_t getNewBodyLength(size_t newLength){
            if(newLength > maxBytes){
                return maxBytes;
            }
            return newLength;
        }
        //getter/setter
        string getData(){
            int length=header+bodyLength_;
            string result(data, length);
            return result;
        }

        string getBody(){
            string dataStr = getData();
            string result = dataStr.substr(header, bodyLength_);
            return result;
        }

        bool decodeHeader(){
            char new_header[header+1] = {0};
            strncpy(new_header, data, header);
            new_header[header]='\0';
            int headervalue = atoi(new_header); //atoi converts the string into int value
            if(headervalue > maxBytes || headervalue < 0){
                bodyLength_ = 0;
                return false;
            }
            bodyLength_ = static_cast<size_t>(headervalue);
            return true;
            
        }
        void encodeheader(){
            char new_header[header + 1] = {0};
            snprintf(new_header, sizeof(new_header),"%04d",static_cast<int>(bodyLength_)); //formater
            memcpy(data, new_header, header);
        }

        size_t getBodyLength(){
            return bodyLength_;
        }

    private:
        char data[header+maxBytes];
        size_t bodyLength_;

};

#endif MESSAGE_HPP
