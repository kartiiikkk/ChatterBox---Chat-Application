#include "message.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <mutex>

using boost::asio::ip::tcp;

mutex cout_mutex;

void async_read(tcp::socket &socket){
    auto buffer = make_shared<boost::asio::streambuf>();
    boost::asio::async_read_until(socket, *buffer, "\n", [&socket, buffer](boost::system::error_code ec, size_t length){
        if(!ec){
            
            istream is(buffer.get());
            string received;
            getline(is, received);

            lock_guard<std::mutex> lock(cout_mutex);
            cout << "\nServer: " << received << endl;
            cout << "Enter message:- ";
            cout.flush();
            
            async_read(socket);
        }else{
            cerr<< "Read failed: "<<ec.message()<<endl;
        }
    });
}

int main(int argc, char *argv[]){
    cout << "\n=============================\n";
    cout << " 🙌  Welcome to CHATTERBOX!  😒 \n";
    cout << "=============================\n\n";
    if(argc < 2){
        cerr <<"Provided port too as a second argument"<<endl;
        return 1;
    }
    boost::asio::io_context io_context;
    tcp::socket socket(io_context);
    tcp::resolver resolver(io_context);

    boost::asio::connect(socket, resolver.resolve("127.0.0.1", argv[1]));
    async_read(socket);
    thread t([&io_context, &socket](){
        while(true){
            string data;

            {
                lock_guard<mutex> lock(cout_mutex);
                cout << "Enter message:- ";
                cout.flush();
            }

            getline(cin,data);
            data+="\n";
            boost::asio::post(io_context, [&, data](){
                boost::asio::write(socket,boost::asio::buffer(data));
            });
        }
    });
    io_context.run();
    t.join();
    return 0;
}