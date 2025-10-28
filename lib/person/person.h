#ifndef PERSON_H
#define PERSON_H

struct Person
{
    float pos_x;
    float pos_y;
    float size;
    float vec_direction[2];
};

struct Person* person_create(float pos_x, float pos_y, float size);
void person_move(struct Person *p);

#endif