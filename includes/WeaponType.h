#ifndef WEAPONTYPE_H
#define	 WEAPONTYPE_H
#include <string>
#include <set>
#include <ExplosionType.h>

namespace FastEcslent
{
  class WeaponType
  {
    public:
      WeaponType();
      WeaponType(int id);
      WeaponType(const WeaponType& other);
      WeaponType& operator=(const WeaponType& other);
      bool operator==(const WeaponType& other) const;
      bool operator!=(const WeaponType& other) const;
      bool operator<(const WeaponType& other) const;

      /** Returns a unique ID for this weapon type. */
      int getID() const;

      /** Returns the name of the weapon. */
      std::string getName() const;

      /** Returns the unit that can use this weapon. */
      //UnitType whatUses() const;

      /** Returns the amount of damage that this weapon deals per attack. */
      int damageAmount() const;

      // TODO: add doc
      int damageBonus() const;

      /** Returns the amount of cooldown time between attacks. */
      float damageCooldown() const;

      /** Returns the amount that the damage increases per upgrade.
       * \see WeaponType::upgradeType. */
      int damageFactor() const;

      /** Returns the type of explosion that this weapon uses. */
      ExplosionType explosionType() const;

      /** Returns the minimum attack range of the weapon, measured in pixels, 0 for most things except
       * WeaponTypes::Arclite_Shock_Cannon (the weapon of the Terran Siege Tank in Siege Mode). */
      int minRange() const;

      /** Returns the maximum attack range of the weapon, measured in pixels. */
      int maxRange() const;

      /** Inner radius used in splash damage calculations. */
      int innerSplashRadius() const;

      /** Median radius used in splash damage calculations. */
      int medianSplashRadius() const;

      /** Outer radius used in splash damage calculations. */
      int outerSplashRadius() const;

      /** Returns true if this weapon can attack air units. */
      bool targetsAir() const;

      // TODO: group these methods
      /** Returns true if this weapon can attack ground units. */
      bool targetsGround() const;
      bool targetsOwn() const;
    private:
      int id;
  };

  namespace WeaponTypes
  {
    /** Given the name of a weapon, this will return the corresponding weapon type object. */
    WeaponType getWeaponType(std::string name);

    /** Returns the set of all the WeaponTypes. */
    std::set<WeaponType>& allWeaponTypes();

    /** Returns the set of all normal weapons in WeaponTypes. */
    std::set<WeaponType>& normalWeaponTypes();

    /** Returns the set of all special weapons in WeaponTypes. */
    std::set<WeaponType>& specialWeaponTypes();
    void init();
  	extern const WeaponType Fusion_Cutter;
  	extern const WeaponType Rifle;
  	extern const WeaponType Hellfire;
  	extern const WeaponType Cannon;
  	extern const WeaponType None;
  	extern const WeaponType Unknown;
        extern const WeaponType PsiBlades;
        extern const WeaponType FragGrenade;
  }
}
#endif
