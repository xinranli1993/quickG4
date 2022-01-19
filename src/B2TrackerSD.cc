//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B2TrackerSD.cc
/// \brief Implementation of the B2TrackerSD class

#include "B2TrackerSD.hh"
#include "B2TrackerHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2TrackerSD::B2TrackerSD(const G4String& name,
                         const G4String& hitsCollectionName) 
 : G4VSensitiveDetector(name),
   fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2TrackerSD::~B2TrackerSD() 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2TrackerSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection

  fHitsCollection 
    = new B2TrackerHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce

  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool B2TrackerSD::ProcessHits(G4Step* aStep, 
                                     G4TouchableHistory*)
{  
  // energy deposit
  G4double edep = aStep->GetTotalEnergyDeposit();

  //if (edep==0. && aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()!="Target") return false;
  if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="CPD"){
	 // G4cout<<"recorded hits "<<fHitsCollection->GetSize()<<G4endl;
	  B2TrackerHit* newHit;
	  if(fHitsCollection->GetSize()==0){
		  newHit = new B2TrackerHit();
		  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
		  newHit->SetTargetEdep(0);
		  newHit->SetEdep(0);
		  fHitsCollection->insert( newHit );
	  }else{
		  newHit = (B2TrackerHit*)fHitsCollection->GetHit(0);
	//	  G4cout<<"get previous hit"<<G4endl;
	//	  G4cout<<"    previous hit targetE="<<newHit->GetTargetEdep()<<G4endl;
	  }
	//  newHit->Print();
	  //G4String name = aStep->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName();
	  //if(name !="Transportation" && name!="StepLimiter")
	  //G4cout<<name<<G4endl;
	  newHit->SetTargetEdep(newHit->GetTargetEdep()+edep);
	//  newHit->Print();
  }else if(edep>0){
	//	  G4cout<<"in detector"<<G4endl;
	  if(fHitsCollection->GetSize()==0){}
	  else{
		  B2TrackerHit* hit = (B2TrackerHit*)fHitsCollection->GetHit(0);
		  if(aStep->GetTrack()->GetTrackID()==hit->GetTrackID())
			  hit->SetPos (aStep->GetPostStepPoint()->GetPosition());
		  G4double distance = (aStep->GetPostStepPoint()->GetPosition() - hit->GetPos()).mag();
		  if(distance < 0.2*mm){
			hit->SetEdep(hit->GetEdep()+edep);
		  }
	//	  hit->Print();
	  }
  }

  //newHit->Print();

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2TrackerSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) { 
     G4int nofHits = fHitsCollection->entries();
     G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits 
            << " hits in the tracker chambers: " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
