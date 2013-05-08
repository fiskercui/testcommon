#include "packageName.MessageName.pb.h"
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    lm::helloworld msg1;

    msg1.set_id(1);
    msg1.set_str("weihua.cui");
    msg1.set_opt(123);

    fstream output("./log", ios::out|ios::trunc|ios::binary);
    if(msg1.SerializeToOstream(&output))
    {
        cerr<< "Failed to write msg" << endl;
        return -1;
    }
    return 0;
}
