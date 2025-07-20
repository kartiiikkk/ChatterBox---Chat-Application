#include <iostream>
#include "message.hpp"
#include <set>
#include <memory>
#include <sys/socket.h>
#include <unistd.h>
#include <deque>
#include <boost/asio.hpp>


#ifndef CHATTERBOX_HPP
#define CHATTERBOX_HPP

using boost::asio::ip::tcp;

class Participant{
    public:
        virtual void deliver(Message &message) = 0;
        virtual void write(Message &message) = 0;
        virtual ~Participant() = default;
};

typedef shared_ptr<Participant> ParticipantPointer;

class Room{
    public:
        void join(ParticipantPointer participant);
        void leave(ParticipantPointer participant);
        void deliver(ParticipantPointer participantPointer,Message &message);
    private:
        deque<Message> messageQueue;
        enum {maxParticipants = 100};
        set<ParticipantPointer> participants;
};

class Session: public Participant, public enable_shared_from_this<Session>{
    public:
        Session(tcp::socket s, Room &room);
        void start();
        void deliver(Message &message);
        void write(Message &message);
        void async_read();
        void async_write(string messageBody, size_t messageLength);
    private:
        tcp::socket clientSocket;
        boost::asio::streambuf buffer; //it is used to store data when latency is high
        Room &room;
        deque <Message> messageQueue;
};




#endif CHATTERBOX_HPP
