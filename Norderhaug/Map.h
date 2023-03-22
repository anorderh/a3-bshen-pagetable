/**
 * Anthony Norderhaug, Anthony Contreras
 * CS 480 - Spring 2023
 * RedID: 823899304, 824089247
 *
 * Map.h implements Map members
 */

#ifndef MAP_H
#define MAP_H


class Map {
public:
    Map(unsigned int vpn, unsigned int frame);

    unsigned int VPN;
    unsigned int frame_num;
};


#endif //MAP_H
