/* -*- mode: C; c-basic-offset: 4; intent-tabs-mode: nil -*-
 *
 * Sifteo Thundercracker simulator
 * M. Elizabeth Scott <beth@sifteo.com>
 *
 * Copyright <c> 2011 Sifteo, Inc. All rights reserved.
 */

/*
 * Static data for flash_model.h
 */

#include "cube_flash_model.h"

namespace Cube {


const FlashModel::command_sequence FlashModel::cmd_byte_program[FlashModel::CMD_LENGTH] = {
    { 0x000, 0x000, 0x00, 0x00 },   // Dont' care
    { 0x000, 0x000, 0x00, 0x00 },   // Don't care
    { 0xFFF, 0xAAA, 0xFF, 0xAA },   // Unlock
    { 0xFFF, 0x555, 0xFF, 0x55 },   //   ...
    { 0xFFF, 0xAAA, 0xFF, 0xA0 },   //   ...
    { 0x000, 0x000, 0x00, 0x00 },   // Data address/byte
};

const FlashModel::command_sequence FlashModel::cmd_sector_erase[FlashModel::CMD_LENGTH] = {
    { 0xFFF, 0xAAA, 0xFF, 0xAA },   // Unlock
    { 0xFFF, 0x555, 0xFF, 0x55 },   //   ...
    { 0xFFF, 0xAAA, 0xFF, 0x80 },   //   ...
    { 0xFFF, 0xAAA, 0xFF, 0xAA },   //   ...
    { 0xFFF, 0x555, 0xFF, 0x55 },   //   ...
    { 0x000, 0x000, 0xFF, 0x50 },   // Erase address
};

const FlashModel::command_sequence FlashModel::cmd_block_erase[FlashModel::CMD_LENGTH] = {
    { 0xFFF, 0xAAA, 0xFF, 0xAA },   // Unlock
    { 0xFFF, 0x555, 0xFF, 0x55 },   //   ...
    { 0xFFF, 0xAAA, 0xFF, 0x80 },   //   ...
    { 0xFFF, 0xAAA, 0xFF, 0xAA },   //   ...
    { 0xFFF, 0x555, 0xFF, 0x55 },   //   ...
    { 0x000, 0x000, 0xFF, 0x30 },   // Erase address
};

const FlashModel::command_sequence FlashModel::cmd_chip_erase[FlashModel::CMD_LENGTH] = {
    { 0xFFF, 0xAAA, 0xFF, 0xAA },   // Unlock
    { 0xFFF, 0x555, 0xFF, 0x55 },   //   ...
    { 0xFFF, 0xAAA, 0xFF, 0x80 },   //   ...
    { 0xFFF, 0xAAA, 0xFF, 0xAA },   //   ...
    { 0xFFF, 0x555, 0xFF, 0x55 },   //   ...
    { 0xFFF, 0xAAA, 0xFF, 0x10 },   // Confirm
};

const FlashModel::command_sequence FlashModel::cmd_buffer_begin[FlashModel::CMD_LENGTH] = {
    { 0x000, 0x000, 0x00, 0x00 },   // Dont' care
    { 0x000, 0x000, 0x00, 0x00 },   // Don't care
    { 0xFFF, 0xAAA, 0xFF, 0xAA },   // Unlock
    { 0xFFF, 0x555, 0xFF, 0x55 },   //   ...
    { 0x000, 0x000, 0xFF, 0x25 },   // Sector address + command
    { 0x000, 0x000, 0x00, 0x00 },   // Sector address + length
};


};  // namespace Cube
