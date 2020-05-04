//-----------------------------------------------------------------------------
// VL53L0x.cpp - VL53L0x time-of-flight distance sensor interface 
//
// Only basic functionality: (continues) distance readings.        
//
// Based on code by Ted Meyers from
// https://groups.google.com/forum/#!topic/diyrovers/lc7NUZYuJOg
//
// Copyright (c) 2013-2016 Joep Suijs - All rights reserved.
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
#ifndef RLM_VL53L0X_DISABLE

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TVL53L0x::TVL53L0x(int InI2cAddress)
   {
      I2cAddress  = InI2cAddress;  
      Distance    = 0;  // default     
      Mode        = 0;  // reset 
      Enable      = true;   
   }

//-----------------------------------------------------------------------------
// TVL53L0x::Init -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void TVL53L0x::Init()
   {
      Mode = 10;    // normal (continues) mode.                           
   }

//-----------------------------------------------------------------------------
// TVL53L0x::Print -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void TVL53L0x::Print()
   { 
      printf("VL53L0x i2c: 0x%x, Mode: %d, Status: 0x%x Distance: %d\n", I2cAddress, Mode, Status, Distance); 
   }

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void TVL53L0x::RangeTrigger()
   {
      I2cWrite_Byte_Byte(I2cAddress, VL53L0X_REG_SYSRANGE_START, 0x01);
   }

//-----------------------------------------------------------------------------
// TVL53L0x::Takt -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void TVL53L0x::Takt()
   {
      if (!Enable) {
         // Sensor is disabled.
         Mode     = 0;             
         Distance = 0;
         return;
      }

      if (Mode == 0) { 
         // Startup sensor.
         Init();    
         wait_us(1500);     
         RangeTrigger();     
      }

      if (Mode < 10) return; // sensor not ready.

      // Check if measurement has completed    
      int val; 
      I2cRead_Byte_Byte(I2cAddress, VL53L0X_REG_RESULT_RANGE_STATUS, val);
      if ((val & 0x01) == 0) return;   
      
      unsigned char gbuf[16];                                 
      I2cRead_Byte_Multi(I2cAddress, 0x14, 12, (char *)gbuf);   
      //HexDump(gbuf, 12);
      
      int acnt        = ((int) gbuf[6] )<<8 | (int) gbuf[7];
      int scnt        = ((int) gbuf[8] )<<8 | (int) gbuf[9];
      int NewDistance = ((int) gbuf[10])<<8 | (unsigned int) gbuf[11];
      Status   = ((gbuf[0] & 0x78) >> 3);  // see VL53L0X_DEVICEERROR_* in vl53l0x_device.h
   
      if (Status == 11) {
         // valid distance reading
         Distance = NewDistance;
      }
 //     printf("ambient: %d, sign: %d, Status: %d, dist: %d\n",  acnt, scnt, DeviceRangeStatusInternal, dist);
         
      RangeTrigger();     
      DEBUG_printf(RLM_VL53L0X, "vl s: %d, d: %d, n: %d (%d %d)\n", Status, Distance, NewDistance, acnt, scnt);

   }
   
#endif
   