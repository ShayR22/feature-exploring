#include "my_class.hpp"
#include <stdio.h>


void setup() {
    printf("setup\n");
}

void setup2() {
    printf("setup2\n");
}

void teardown() {
    printf("teardown\n");
}




CREATE_CLASS(Real,
             SLevel_A,
             SETUPS(),
            //  SETUPS(setup, setup2),
             teardown);


// CREATE_CLASS(Fake,
//              SETUPS(""),
//              teardown);



int main() {
    Real r;
    r.SetUp();
    r.TearDown();

    // Fake f;
    return 0;
}