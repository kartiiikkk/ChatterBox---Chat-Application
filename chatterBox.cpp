#include "chatterBox.hpp" //this is because we have already include iostream in chatterbox.hpp

void Room::join(ParticipantPointer participant){
    this->participants.insert(participant);
}

void Room::leave(ParticipantPointer participant){
    this->participants.erase(participant);
}

void Room::deliver(ParticipantPointer participant, Message &message){
    messageQueue.push_back(message);
    while(!messageQueue.empty()){
        Message msg = messageQueue.front();
        messageQueue.pop_front();

        for(ParticipantPointer _participant: participants){
            if(participant != _participant){
                _participant->write(msg);
            }
        }
    }
}

void Session::async_read(){
    auto self(shared_from_this());
    boost::asio::async_read_until(clientSocket, buffer,"\n", [this, self](boost::system::error_code ec, size_t bytes_transferred){
        if(!ec){
            string data(boost::asio::buffers_begin(buffer.data()),boost::asio::buffers_begin(buffer.data()) + bytes_transferred);

            buffer.consume(bytes_transferred);
            cout<<"Received: "<<data<<endl;
            Message message(data);
            deliver(message);
            async_read();
        }else{
            room.leave(shared_from_this());
            if(ec == boost::asio::error::eof){
                cout<< "Connection closed by peer"<<endl;
            }
            else{
                cout<<"Read error: "<<ec.message()<<endl;
            }
        }
    });
}

void Session::async_write(string messageBody, size_t messageLength){
    auto write_handler = [&] (boost::system::error_code ec, size_t bytes_transferred){
        if(!ec){
            cout<<"Data is written to the socket"<<endl;
        }else{
            cerr<<"Write error: "<<ec.message()<<endl;
        }
    };
    boost::asio::async_write(clientSocket, boost::asio::buffer(messageBody, messageLength),write_handler);
}

void Session::start(){
    room.join(shared_from_this());

    Message welcomeMessage("Welcome to CHATTERBOX!\n");
    write(welcomeMessage);
    async_read();
}

Session::Session(tcp::socket s, Room& r): clientSocket(move(s)),room(r){};

void Session::write(Message &message){
    messageQueue.push_back(message);
    while (messageQueue.size()!=0)
    {
        Message message = messageQueue.front();
        messageQueue.pop_front();
        bool header_decode = message.decodeHeader();
        if(header_decode){
            string fullData = message.getBody();
            async_write(fullData, fullData.size());
        }
        else{
            cout<<"Message length exceeds the max length"<<endl;
        }
    }
}

void Session::deliver(Message &incomingMessage){
    room.deliver(shared_from_this(), incomingMessage);
}

//accepting a connection from the client
using boost::asio::ip::address_v4;

void accept_connection(boost::asio::io_context &io, char *port, tcp::acceptor &acceptor, Room &room, const tcp::endpoint &endpoint){
    tcp::socket socket(io);
    acceptor.async_accept([&](boost::system::error_code ec, tcp::socket socket){
        if(!ec){
            shared_ptr<Session> session = make_shared<Session>(move(socket),room);
            session->start();
        }
        accept_connection(io, port, acceptor, room, endpoint);
    });
}

int main(int argc, char *argv[]){
    try{
        if(argc<2){
            cerr<<"Usage: server <port>[<port> ...]\n";
            return 1;
        }
        Room room;
        boost::asio::io_context io_context;
        tcp::endpoint endpoint(tcp::v4(),atoi(argv[1]));
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(),atoi(argv[1])));
        accept_connection(io_context, argv[1], acceptor, room, endpoint);
        io_context.run();
    }
    catch(exception &e){
        cerr<<"Exception: "<<e.what()<<endl;
    }
    return 0;
}