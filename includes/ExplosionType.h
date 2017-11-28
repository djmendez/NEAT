#ifndef EXPLOSIONTYPE_H
#define	 EXPLOSIONTYPE_H

#include <string>
#include <set>

namespace FastEcslent
{
  class ExplosionType
  {
    public:
      ExplosionType();
      ExplosionType(int id);
      ExplosionType(const ExplosionType& other);
      ExplosionType& operator=(const ExplosionType& other);
      bool operator==(const ExplosionType& other) const;
      bool operator!=(const ExplosionType& other) const;
      bool operator<(const ExplosionType& other) const;

      /** Returns a unique ID for this explosion type. */
      int getID() const;

      /** Returns the name of this explosion type. */
      std::string getName() const;
    private:
      int id;
  };

  namespace ExplosionTypes
  {
    /** Given a name of an explosion type, this will return the corresponding ExplosionType  object. */
    ExplosionType getExplosionType(std::string name);

    /** Returns the set of all ExplosionTypes. */
    std::set<ExplosionType>& allExplosionTypes();


    void init();
    extern const ExplosionType None;
    extern const ExplosionType Normal;
    extern const ExplosionType Radial_Splash;
    extern const ExplosionType Enemy_Splash;
    extern const ExplosionType Enemy_Line_Splash;
    extern const ExplosionType Nuclear_Missile;
    extern const ExplosionType EMP_Shockwave;
    extern const ExplosionType Air_Splash;
    extern const ExplosionType Unknown;
  }
}
#endif
