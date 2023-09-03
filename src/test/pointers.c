#include <stdio.h>

struct Person {
    char name[64];
    int age;
};

void updateAge(struct person *p, int age) {
    p->age = age;
}


int main() {
    struct Person people[100];

    struct Person *p_Person = &people;

    int i = 9;
    for (i=0; i<100; i++) {
        p_Person->age = 0;
        p_Person += 1;
        p_Person->name = 0;
    }

    updateAge(&dog, 12);
    return 0;
}
