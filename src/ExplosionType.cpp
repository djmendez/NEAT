#include <string>
#include <map>
#include <set>
#include <ExplosionType.h>

namespace FastEcslent
{
  bool initializingExplosionType = true;
  std::string explosionTypeName[9];
  std::map<std::string, ExplosionType> explosionTypeMap;
  std::set< ExplosionType > explosionTypeSet;
  namespace ExplosionTypes
  {
    const ExplosionType None(0);
    const ExplosionType Normal(1);;
    const ExplosionType Radial_Splash(2);
    const ExplosionType Enemy_Splash(3);
    const ExplosionType Enemy_Line_Splash(4);
    const ExplosionType Nuclear_Missile(5);
    const ExplosionType EMP_Shockwave(6);
    const ExplosionType Air_Splash(7);
    const ExplosionType Unknown(8);

    void init()
    {
      explosionTypeName[None.getID()]              = "None";
      explosionTypeName[Normal.getID()]            = "Normal";
      explosionTypeName[Radial_Splash.getID()]     = "Radial_Splash";
      explosionTypeName[Enemy_Splash.getID()]      = "Enemy_Splash";
      explosionTypeName[Enemy_Line_Splash.getID()] = "Enemy_Line_Splash";
      explosionTypeName[Nuclear_Missile.getID()]   = "Nuclear_Missile";
      explosionTypeName[EMP_Shockwave.getID()]     = "EMP_Shockwave";
      explosionTypeName[Air_Splash.getID()]        = "Air_Splash";
      explosionTypeName[Unknown.getID()]           = "Unknown";

      explosionTypeSet.insert(None);
      explosionTypeSet.insert(Normal);
      explosionTypeSet.insert(Radial_Splash);
      explosionTypeSet.insert(Enemy_Splash);
      explosionTypeSet.insert(Enemy_Line_Splash);
      explosionTypeSet.insert(Nuclear_Missile);
      explosionTypeSet.insert(EMP_Shockwave);
      explosionTypeSet.insert(Air_Splash);
      explosionTypeSet.insert(Unknown);

      for(std::set< ExplosionType >::iterator i= explosionTypeSet.begin();i!= explosionTypeSet.end();i++){
        std::string name = i->getName();
        explosionTypeMap.insert(std::make_pair(name, *i));
      }
      initializingExplosionType = false;
    }
  }
  ExplosionType::ExplosionType()
  {
    this->id = ExplosionTypes::None.id;
  }
  ExplosionType::ExplosionType(int id)
  {
    this->id = id;
    if (!initializingExplosionType && (id < 0 || id >= 9))
      this->id = ExplosionTypes::Unknown.id;
  }
  ExplosionType::ExplosionType(const ExplosionType& other)
  {
    this->id = other.id;
  }
  ExplosionType& ExplosionType::operator=(const ExplosionType& other)
  {
    this->id = other.id;
    return *this;
  }
  bool ExplosionType::operator==(const ExplosionType& other) const
  {
    return this->id == other.id;
  }
  bool ExplosionType::operator!=(const ExplosionType& other) const
  {
    return this->id != other.id;
  }
  bool ExplosionType::operator<(const ExplosionType& other) const
  {
    return this->id < other.id;
  }
  int ExplosionType::getID() const
  {
    return this->id;
  }
  std::string ExplosionType::getName() const
  {
    return explosionTypeName[this->id];
  }

  ExplosionType ExplosionTypes::getExplosionType(std::string name)
  {
    std::map<std::string, ExplosionType>::iterator i = explosionTypeMap.find(name);
    if (i == explosionTypeMap.end())
      return ExplosionTypes::Unknown;
    return (*i).second;
  }
  std::set<ExplosionType>& ExplosionTypes::allExplosionTypes()
  {
    return explosionTypeSet;
  }
}
