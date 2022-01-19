// Definition of the user defined physiscs list for the 

#include "LowEnergyPhysicsList.hh"
#include "B2EmPenelopePhysics.hh"
#include "G4EmParameters.hh"
#include "G4ProcessManager.hh"
#include "G4StepLimiter.hh"
#include "G4RegionStore.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
//#include "DBIO.hh"

LowEnergyPhysicsList::LowEnergyPhysicsList() 
 : G4VModularPhysicsList(), 
   fEmPhysics(NULL)
{
	// Set the default cut value
	SetDefaultCutValue(1*um);//DBIO::Get()->GetDefaultCut());

	SetVerboseLevel(1);

	// Creates the Penelope physics list
	fEmPhysics = new B2EmPenelopePhysics();
}

LowEnergyPhysicsList::~LowEnergyPhysicsList()
{

}

// Include headers of particles that we need 

// Bosons
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"
#include "G4Gamma.hh"
#include "G4OpticalPhoton.hh"

// leptons
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"

// Baryons
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Neutron.hh"
#include "G4AntiNeutron.hh"

// Nuclei
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"

void LowEnergyPhysicsList::ConstructParticle()
{

	// Imaginary tracking particles
	G4Geantino::GeantinoDefinition();
	G4ChargedGeantino::ChargedGeantinoDefinition();

	// Gamma
	G4Gamma::GammaDefinition();

	// Optical photon
	G4OpticalPhoton::OpticalPhotonDefinition();

	// Leptons
	G4Electron::ElectronDefinition();
	G4Positron::PositronDefinition();

	G4NeutrinoE::NeutrinoEDefinition();
	G4AntiNeutrinoE::AntiNeutrinoEDefinition();

	// Barions
	G4Proton::ProtonDefinition();
	G4AntiProton::AntiProtonDefinition();
	G4Neutron::NeutronDefinition();
	G4AntiNeutron::AntiNeutronDefinition();

	G4Deuteron::DeuteronDefinition();
	G4Triton::TritonDefinition();
	G4Alpha::AlphaDefinition();
	G4GenericIon::GenericIonDefinition();

}

void LowEnergyPhysicsList::ConstructProcess()
{

	// Transportation
    AddTransportation();

	//set the userlimits
	G4StepLimiter * StepLimiter = new G4StepLimiter();
	StepLimiter->SetVerboseLevel(0);
	G4ProcessManager* pm = G4Electron::Electron()->GetProcessManager();
	pm->AddDiscreteProcess(StepLimiter);
	pm = G4Positron::Positron()->GetProcessManager();
	pm->AddDiscreteProcess(StepLimiter);


	// Register the penelope physics list
	fEmPhysics->ConstructProcess();
	// deexitation
	//G4LossTableManager::Instance()->AtomicDeexitation()->SetAuger(true);
	//G4LossTableManager::Instance()->AtomicDeexitation()->SetPIXE(true);
	G4EmParameters * param = G4EmParameters::Instance();
	param->SetLowestElectronEnergy(50*eV);
	param->SetAuger(true);
	param->SetAugerCascade(true);
	param->SetPixe(true);
}

void LowEnergyPhysicsList::SetCuts()
{
	// Set limits of cut
	G4double lowerLimit = 50*eV;
	G4double upperLimit = 100*GeV;
	G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowerLimit, upperLimit);

  // Set cuts by calling parent function. Sets to default value
//	G4VUserPhysicsList::SetCuts();

	SetCutsWithDefault();

	 //// Production thresholds for detector regions
	 //G4Region* region;
	 //G4String regName;
	 //G4ProductionCuts* cuts;
	 //
	 //regName = "Target_LV";
	 //region = G4RegionStore::GetInstance()->GetRegion(regName);
	 //cuts = new G4ProductionCuts;
	 //cuts->SetProductionCut(1*um); // same cuts for gamma, proton, e- and e+
	 //region->SetProductionCuts(cuts);
	 

	// Print the cut values if verbosity dictates
	if( verboseLevel>0 )
	{
		DumpCutValuesTable();
	}



}
