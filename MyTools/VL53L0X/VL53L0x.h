//-----------------------------------------------------------------------------
// VL53L0x.h - VL53L0x time-of-flight distance sensor interface 
//
// Copyright (c) 2016-2016 Joep Suijs - All rights reserved.
//
// RobotLib tags: RED_OPT
//-----------------------------------------------------------------------------
// Deze file is onderdeel van RobotLib.
// Zie http://wiki.robotmc.org/index.php?title=RobotLib voor meer informatie.
//
// Copyright: This library and the ideas, software, models, pictures contained
// herein are and are distributed under the Free Software Foundation General 
// Public License version 2.0. See the FSF GPL page for more information.
// It is explicitly forbidden to use any of the copyrighted materials for
// commercial purposes, including books.
//-----------------------------------------------------------------------------
// Deze file is een RODE OPTIONELE file.
// Een RODE file is een standaard file, die voor iedere toepassing gelijk is.
// OPTIONELE files kunnen worden weggelaten (niet gebruikt) zonder dat de werking
// van RobotLib als geheel wordt verstoord.
//-----------------------------------------------------------------------------
// tags_end
//-----------------------------------------------------------------------------
#ifndef __VL53L0X_H
#define __VL53L0X_H

#define VL53L0X_REG_IDENTIFICATION_MODEL_ID           0xc0
#define VL53L0X_REG_IDENTIFICATION_REVISION_ID        0xc2
#define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD     0x50
#define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD   0x70
#define VL53L0X_REG_SYSRANGE_START                    0x00
#define VL53L0X_REG_RESULT_INTERRUPT_STATUS           0x13
#define VL53L0X_REG_RESULT_RANGE_STATUS               0x14

#define VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS          0x008a     // 7 bit address

class TVL53L0x 
{
public:
 
   TVL53L0x(int I2cAddress);
   void  Init();
   void  Takt();
   int   Distance;   
 
   void  Print();
   
   // config
   int  I2cAddress;    
   void RangeTrigger();

   bool Enable;
   
private:               

   int Mode;   
   int Status; // last status read.
   int LastTrigger;  // last time (in ms) when distance rangeing was triggerd.
} ;

// The default device
extern         TVL53L0x    VL53L0x;   
extern const   TCliGroup   CliVL53L0x;

#endif