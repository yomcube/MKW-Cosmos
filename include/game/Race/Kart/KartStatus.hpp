#ifndef _KARTSTATUS_
#define _KARTSTATUS_

//Formerly PlayerSub1c
#include <kamek.hpp>
#include <game/Race/Kart/KartBase.hpp>
#include <game/Race/Kart/KartParams.hpp>

class KartPointers;

namespace BITFIELD0
{
  enum bitfield0{
    ACCELERATE = 1 << 0,
    BRAKE = 1 << 1,
    DRIFT_INPUT = 1 << 2,
    MANUAL_DRIFT = 1 << 3,
    OOB_BEFORE_RESPAWN = 1 << 4,
    WALL_3_COL = 1 << 5,
    WALL_COL = 1 << 6,
    FIRST_FRAME_HOP = 1 << 7,
    FIRST_FRAME_ACCELERATE = 1 << 8,
    FIRST_FRAME_GROUND = 1 << 9,
    FLOOR_COL_BODY = 1 << 10,
    FLOOR_COL_WHEEL_ANY = 1 << 11,
    FLOOR_COL_WHEEL_ALL = 1 << 12,
    STICK_LEFT = 1 << 13,
    AIRTIME_OVER_20 = 1 << 15,
    STICKY_ROAD = 1 << 16,
    GROUD = 1 << 18,
    HOP = 1 << 19,
    BOOST = 1 << 20,
    STICK_RIGHT = 1 << 24,
    MUSHY_BOOST = 1 << 26,
    SLIPSTREAM_CHARGE = 1 << 27,
    AUTO_DRIFT = 1 << 28,
    WHEELIE = 1 << 29,
    JUMP_PAD = 1 << 30,
    RAMP_BOOST = 1 << 31,
  };
}

namespace BITFIELD1
{
  enum bitfield1
  {
    HIT = 1 << 0,
    FIRST_FRAME_RESPAWN = 1 << 1,
    FIRST_FRAME_CANNON = 1 << 3,
    IN_CANNON = 1 << 4,
    FIRST_FRAME_TRICK = 1 << 5,
    IN_TRICK = 1 << 6,
    OFFROAD_INVUL = 1 << 7,
    OVER_ZIPPER = 1 << 10,
    JUMP_PAD_MUSHY = 1 << 11,
    ZIPPER_BOOST = 1 << 13,
    ZIPPER_TRICK = 1 << 15,
    MT_BOOST = 1 << 20,
    HAS_TRICK_ROT = 1 << 22,
    JUMP_PAD_MUSHY_SOMETHING = 1 << 23,
    TRICKABLE = 1 << 30,
    IN_STAR = 1 << 31,
  };
}

namespace BITFIELD2
{
  enum bitfield2{
        /*
       0 charging a slipstream
       1 in a slipstream
       4 wheelie rotation
       7 shocked
      15 in a mega
      16 crushed
      18 stopped
      19 vanished
      27 in a bullet
      28 ink applied
      29 has a tc
    */
   CHARGING_SLIPSTREAM = 1 << 0,
   IN_SLIPSTREAM = 1 << 1,
   WHEELIE_ROTATION = 1 << 4,
   SHOCKED = 1 << 7,
   IN_MEGA = 1 << 15,
   STOPPED = 1 << 18,
   VANISHED = 1 << 19,
   JUMP_PAD_FIXED_SPEED = 1 << 20,
   IN_BULLET = 1 << 27,
   INK_APPLIED = 1 << 28,
   HAS_THUNDERCLOUD = 1 << 29,
  };
}

class KartStatus {
public:
  explicit KartStatus(KartValues *values); //805943b4
  void UpdateFromInput(); //8059487c
  UnkType ComputeStartBoost(); //805959d4
  UnkType ApplyStartBoost(int startBoostIdx); //80595af8
  UnkType UpdateCollisions(); //80594bd4
  //vtable 808b6534
  virtual void Unknown_vtable();
  u32 bitfield0; //bit flags: //0x4
    /*
       0 accelerate
       1 brake
       2 drift input
       3 drift (manual)
       4 oob, before being respawned
       5 wall 3 collision
       6 wall collision
       7 first frame of hop
       8 first frame of acceleration
       9 first frame of groundtime
      10 floor collision with the kart body
      11 floor collision with any wheel
      12 floor collision with all wheels
      13 stick left
      15 airtime > 20
      16 sticky road
      18 ground
      19 hop
      20 boost
      24 stick right
      26 mushroom boost
      27 charging a slipstream
      28 drift (auto)
      29 wheelie
      30 jump pad enabled
      31 ramp boost
    */
  u32 bitfield1; //bit flags: //0x8
    /*
       0 hit by an item or an object
       1 first frame of respawn
       3 first frame of cannon
       4 in cannon
       5 first frame of trick
       6 in a trick
       7 offroad invincibility
      10 over a zipper
      13 zipper boost
      15 zipper trick
      20 mt boost
      22 in a trick
      30 on a trickable surface
      31 in a star
    */
  u32 bitfield2; //bit flags: //0xc
    /*
       0 charging a slipstream
       1 in a slipstream
       4 wheelie rotation
       7 shocked
      15 in a mega
      16 crushed
      18 stopped
      19 vanished
      27 in a bullet
      28 ink applied
      29 has a tc
    */
  u32 bitfield3; //bit flags:
    /*
       5 hwg (horizontal wall glitch)
       8 start boost charge
    */
  u32 bitfield4; //bit flags:
    /*
       0 cpu-controlled
       1 real local
       2 local
       3 remote
       4 automatic drift
       6 ghost
    */
  KartBase *base; //0x18
  u32 airtime;
  u8 unknown_0x20[0x28-0x20];
  Vec3 floorNor;
  Vec3 unknown_0x34;
  u8 unknown_0x40[0x6b-0x40];
  s32 hwgTimer;
  u8 unknown_0x70[0x74-0x70];
  u32 boostRampType;
  u32 jumpPadType; //essentially used to access jump pad properties array
  u8 unknown_0x7c[0x7f-0x7c];
  u32 cnptId;
  u8 unknown_0x84[0x88-0x84];
  float stickX;
  float stickY;
  u32 oobWipeState;
  s16 oobWipeFrame;
  bool bool_0x96;
  bool bool_0x97;
  u8 unknown_0x98[0x9c-0x98];
  float startBoostCharge;
  s32 startBoostIdx; //-1 when burning out
  u8 unknown_0xa4[2];
  s16 trickableTimer;
  u8 unknown_0xa8[0xc0-0xa8];
}; //Total size 0xc0

#endif