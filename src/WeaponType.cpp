#include <string>
#include <map>
#include <set>
#include <WeaponType.h>

namespace FastEcslent
{
  bool initializingWeaponType = true;
  class WeaponTypeInternal
  {
    public:
      WeaponTypeInternal() {valid = false;}
      void set(const char* name, int damageAmount, int damageBonus, float damageCooldown, int damageFactor, ExplosionType explosionType,
    		  int minRange, int maxRange, int innerSplashRadius, int medianSplashRadius, int outerSplashRadius, bool targetsAir,
    		  bool targetsGround, bool targetsOwn)
      {
        if (initializingWeaponType)
        {
          this->name               = name;
          this->damageAmount       = damageAmount;
          this->damageBonus        = damageBonus;
          this->damageCooldown     = damageCooldown;
          this->damageFactor       = damageFactor;
          this->explosionType      = explosionType;
          this->minRange           = minRange;
          this->maxRange           = maxRange;
          this->innerSplashRadius  = innerSplashRadius;
          this->medianSplashRadius = medianSplashRadius;
          this->outerSplashRadius  = outerSplashRadius;
          this->targetsAir         = targetsAir;
          this->targetsGround      = targetsGround;
          this->targetsOwn         = targetsOwn;
          this->valid              = true;
        }
      }
      std::string name;

      int damageAmount;
      int damageBonus;
      float damageCooldown;
      int damageFactor;
      ExplosionType explosionType;
      int minRange;
      int maxRange;
      int innerSplashRadius;
      int medianSplashRadius;
      int outerSplashRadius;
      bool targetsAir;
      bool targetsGround;
      bool targetsOwn;
      bool valid;
  };
  WeaponTypeInternal weaponTypeData[6];
  std::map<std::string, WeaponType> weaponTypeMap;
  std::set< WeaponType > weaponTypeSet;
  std::set< WeaponType > specialWeaponTypeSet;
  std::set< WeaponType > normalWeaponTypeSet;
  namespace WeaponTypes
  {
  	const WeaponType Fusion_Cutter(0);
    const WeaponType Rifle(1);
    const WeaponType Hellfire(2);
    const WeaponType Cannon(3);
    const WeaponType None(4);
    const WeaponType Unknown(5);

    void init()
    {
      weaponTypeData[Fusion_Cutter.getID()].set("Fusion_Cutter", 16,  1, 22.0f, 1, ExplosionTypes::Normal,        0, 1*20,0, 0, 0,   1, 1, 0); //Zealot
      weaponTypeData[Rifle.getID()]        .set("Rifle",         20,  1, 30.f, 1, ExplosionTypes::Normal,         0, 5*20,0, 0, 0,   1, 1, 0); //Marine
      weaponTypeData[Hellfire.getID()]     .set("Hellfire",      20,  1, 0.625f, 1, ExplosionTypes::Normal, 	   0, 7*20,0, 0, 0,   1, 1, 0); //Hellion
      weaponTypeData[Cannon.getID()]       .set("Cannon",        30,  3, 37, 1, ExplosionTypes::Enemy_Splash,      0, 300, 50,70,100, 0, 1, 0); //Tank
      weaponTypeData[None.getID()]         .set("None",          0,   0, 0,  0, ExplosionTypes::None,              0, 0,   0, 0, 0,   0, 0, 0);
      weaponTypeData[Unknown.getID()]      .set("Unknown",       0,   0, 0,  0, ExplosionTypes::None,              0, 0,   0, 0, 0,   0, 0, 0);

      weaponTypeSet.insert(Fusion_Cutter);
      weaponTypeSet.insert(Rifle);
      weaponTypeSet.insert(Hellfire);
      weaponTypeSet.insert(Cannon);

      for(std::set<WeaponType>::iterator i= weaponTypeSet.begin();i!= weaponTypeSet.end();i++){
        std::string name = i->getName();
        weaponTypeMap.insert(std::make_pair(name, *i));
      }
      initializingWeaponType = false;
    }
  }
  WeaponType::WeaponType()
  {
    this->id = WeaponTypes::None.id;
  }
  WeaponType::WeaponType(int id)
  {
    this->id = id;
    if (!initializingWeaponType && (id < 0 || id >= 6 || !weaponTypeData[id].valid))
      this->id = WeaponTypes::Unknown.id;
  }
  WeaponType::WeaponType(const WeaponType& other)
  {
    this->id = other.id;
  }
  WeaponType& WeaponType::operator=(const WeaponType& other)
  {
    this->id = other.id;
    return *this;
  }
  bool WeaponType::operator==(const WeaponType& other) const
  {
    return this->id == other.id;
  }
  bool WeaponType::operator!=(const WeaponType& other) const
  {
    return this->id != other.id;
  }
  bool WeaponType::operator<(const WeaponType& other) const
  {
    return this->id < other.id;
  }
  int WeaponType::getID() const
  {
    return this->id;
  }
  std::string WeaponType::getName() const
  {
    return weaponTypeData[this->id].name;
  }
  int WeaponType::damageAmount() const
  {
    return weaponTypeData[this->id].damageAmount;
  }
  int WeaponType::damageBonus() const
  {
    return weaponTypeData[this->id].damageBonus;
  }
  float WeaponType::damageCooldown() const
  {
    return weaponTypeData[this->id].damageCooldown;
  }
  int WeaponType::damageFactor() const
  {
    return weaponTypeData[this->id].damageFactor;
  }

  ExplosionType WeaponType::explosionType() const
  {
    return weaponTypeData[this->id].explosionType;
  }
  int WeaponType::minRange() const
  {
    return weaponTypeData[this->id].minRange;
  }
  int WeaponType::maxRange() const
  {
    return weaponTypeData[this->id].maxRange;
  }
  int WeaponType::innerSplashRadius() const
  {
    return weaponTypeData[this->id].innerSplashRadius;
  }
  int WeaponType::medianSplashRadius() const
  {
    return weaponTypeData[this->id].medianSplashRadius;
  }
  int WeaponType::outerSplashRadius() const
  {
    return weaponTypeData[this->id].outerSplashRadius;
  }
  bool WeaponType::targetsAir() const
  {
    return weaponTypeData[this->id].targetsAir;
  }
  bool WeaponType::targetsGround() const
  {
    return weaponTypeData[this->id].targetsGround;
  }
  bool WeaponType::targetsOwn() const
  {
    return weaponTypeData[this->id].targetsOwn;
  }
  WeaponType WeaponTypes::getWeaponType(std::string name)
  {
    std::map<std::string, WeaponType>::iterator i = weaponTypeMap.find(name);
    if (i == weaponTypeMap.end())
      return WeaponTypes::Unknown;
    return (*i).second;
  }
  std::set<WeaponType>& WeaponTypes::allWeaponTypes()
  {
    return weaponTypeSet;
  }
  std::set<WeaponType>& WeaponTypes::normalWeaponTypes()
  {
    return weaponTypeSet;
  }
  std::set<WeaponType>& WeaponTypes::specialWeaponTypes()
  {
    return weaponTypeSet;
  }
}
