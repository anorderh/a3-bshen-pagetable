/**
 * Anthony Norderhaug, Anthony Contreras
 * CS 480 - Spring 2023
 * RedID: 823899304, 824089247
 *
 * Map.cpp contains Map obj implementation.
 */

#include "Map.h"

/**
 * constructor assigning VPN & frame_num
 * @param vpn
 * @param num
 */
Map::Map(unsigned int vpn, unsigned int num) {
    VPN = vpn;
    frame_num = num;
}
