// LowEnergy Detector Physics List

#ifndef LowEnergyPhysicsList_H
#define LowEnergyPhysicsList_H

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;

class LowEnergyPhysicsList: public G4VModularPhysicsList
{
	public:
		LowEnergyPhysicsList();
		virtual ~LowEnergyPhysicsList();

		virtual void ConstructParticle();
		virtual void ConstructProcess();

		virtual void SetCuts();

	private:
		G4VPhysicsConstructor* fEmPhysics;	
};


#endif
