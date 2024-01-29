#include <ostream>

#include "ex1.pb.h"

using namespace std::string_literals;
using namespace std;
int main(int argc, char * arg[]){
    uint8_t buffer[1024];
    cout << "ex1 starting" << endl;

    E477Ex1::annotated_point msg;
    msg.set_x(3);
    msg.set_y(4);
    msg.set_label("This is a test"s);
    
    uint32_t blen = msg.ByteSizeLong();
    if (blen > 1024) {
        cout << "too long!!" << endl;
        exit(1);
    }
    msg.SerializeToArray(buffer, blen);
    
    E477Ex1::annotated_point msg2;
    if (!msg2.ParseFromArray(buffer,blen)){
        cerr << "Could not parse message" << endl;
        exit(1);
    }
    cout << "x = " << msg2.x() << endl;
    cout << "y = " << msg2.y() << endl;
    cout << "label = " << msg2.label() << endl;
    
    return 0;
}
