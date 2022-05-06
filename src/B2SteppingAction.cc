#include "B2SteppingAction.hh"
#include "B2EventAction.hh"
#include "B2aDetectorConstruction.hh"
#include "B2EventData.hh"
//#include "DBIO.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh" 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2SteppingAction::B2SteppingAction()
                  //    const B2DetectorConstruction* detectorConstruction)
                 //     B2EventAction* eventAction)
 // : G4UserSteppingAction(),
   // fDetConstruction(detectorConstruction),
  //  fEventAction(eventAction)
{
	//fDetConstruction = detectorConstruction;
	trackID=0;
	secondaryID=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2SteppingAction::~B2SteppingAction()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2SteppingAction::UserSteppingAction(const G4Step* step)
{
// Collect energy and track length step by step

  // get volume of the current step
  G4VPhysicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  G4int tag = 3;// 1 for CMOS, 2 for source layer.

  if(volume->GetName()=="CPD"){
	  tag = 2;
  }else if(volume->GetName()=="SLHe"){
	  tag = 1;
  }else if(volume->GetName()=="AlFilm"){
	  tag = 0;
  }
// // G4cout<<volume->GetName()<<" | "<<fDetConstruction->GetTopImagerPV()->GetName()<<" copy number is"<<imagerID<<endl;;
//  if(volume == fDetConstruction->GetTopImagerPV()){
//	imagerID = imagerID*2+1;
//	tag = 1;
//  }else if(volume == fDetConstruction->GetBotImagerPV()){
//	imagerID = imagerID*2;
//	tag = 1;
//  }else if(volume == fDetConstruction->GetDeadlayerPV()){
//	tag = 2;
//  }else if(volume == fDetConstruction->GetSiBlockPV()){
//	tag = 3;
//  }else if(volume == fDetConstruction->GetShieldPV()){
//	tag = 4;
//  }else{
//	tag = 0;
//  }
  
/*	if(step->GetTrack()->GetTrackID()>1){
		if(step->GetTrack()->GetCreatorProcess()->GetProcessName() == "compt")
		  tag = 5;
		else if(step->GetTrack()->GetCreatorProcess()->GetProcessName() == "conv")
		  tag = 6;
	}*/
 // G4cout<<"haha"<<G4endl;
//  if(tag!=0){
	  // energy deposit
	  G4double edep = step->GetTotalEnergyDeposit();
	  G4double Ek = step->GetPreStepPoint()->GetKineticEnergy();
	  //position and time
	  G4ThreeVector pos = step->GetPostStepPoint()->GetPosition();
	  G4double l = step->GetStepLength();
	  G4double theta = step->GetDeltaPosition().theta();
	  G4double t = step->GetPostStepPoint()->GetGlobalTime();
	  G4int pid = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
	  G4int trkid = step->GetTrack()->GetTrackID();
	  G4int motherid = step->GetTrack()->GetParentID();
	//  G4cout<<volume->GetName()<<" "<<step->GetTrack()->GetParticleDefinition()->GetParticleName()<<".  total kinetic energy: "<<step->GetTrack()->GetKineticEnergy()/keV<<"keV  "<<step->GetStepLength()/um<<"um "<<edep/keV<<"keV"<<G4endl;

	  if(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding()==22 
				  && step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="Transportation"){ }
	  else B2EventData::Get()->AddDep(pos.x(),pos.y(),pos.z(),l,theta, t, edep, Ek, tag, pid, motherid, trkid);
	  //if(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding()==22 && tag ==2){
	  //    if(step->GetPostStepPoint()->GetMaterial()->GetName()=="aSe"){
	  //    G4Material* material = step->GetPostStepPoint()->GetMaterial();
	  //    G4cout<<"  f1Fluct      = "<<material->GetIonisation()->GetF1fluct();
	  //    G4cout<<"  f2Fluct      = "<<material->GetIonisation()->GetF2fluct();
	  //    G4cout<<"  e1Fluct      = "<<material->GetIonisation()->GetEnergy1fluct();
	  //    G4cout<<"  e2Fluct      = "<<material->GetIonisation()->GetEnergy2fluct();
	  //    G4cout<<"  e1LogFluct   = "<<material->GetIonisation()->GetLogEnergy1fluct();
	  //    G4cout<<"  e2LogFluct   = "<<material->GetIonisation()->GetLogEnergy2fluct();
	  //    G4cout<<"  ipotFluct    = "<<material->GetIonisation()->GetMeanExcitationEnergy();
	  //    G4cout<<"  ipotLogFluct = "<<material->GetIonisation()->GetLogMeanExcEnergy();
	  //    G4cout<<"  e0		   =  "<<material->GetIonisation()->GetEnergy0fluct()<<G4endl;
	  //    }
	  //  if(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding()==22 && step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="phot"){
	  //  	double Esec = 0;
	  //  	G4cout<<step->GetStepLength()/um<<"um, dE"<<edep/keV<<"keV, Ek"<<Ek/keV<<"keV, name"<<step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()<<endl;
	  //  	G4cout<<step->GetPreStepPoint()->GetPosition()/um<<" "<<step->GetPostStepPoint()->GetPosition()/um<<endl;
	  //  	G4cout<<"# of secondary generated: "<<step->GetSecondaryInCurrentStep()->size();
	  //  	unsigned int isecond = 0;
	  //  	while(isecond < step->GetSecondaryInCurrentStep()->size()){
	  //  		G4cout<<" "<<step->GetSecondaryInCurrentStep()->at(isecond)->GetParticleDefinition()->GetPDGEncoding()
	  //  			<<" "<<step->GetSecondaryInCurrentStep()->at(isecond)->GetKineticEnergy()/keV<<"keV"
	  //  			<<" "<<step->GetSecondaryInCurrentStep()->at(isecond)->GetCreatorProcess()->GetProcessName()<<endl;
	  //  		if(isecond>0)Esec+=step->GetSecondaryInCurrentStep()->at(isecond)->GetKineticEnergy()/keV;
	  //  		isecond++;
	  //  	}
	  //  	G4cout<<" total energy in secondaries"<<Esec<<" local dep = "<<edep/keV<<" total="<<Esec+edep/keV<<endl;
	  //    
	  //  }
	  //}
//  if(DBIO::Get()->RecordOutFile()){
///*	DBIO::Get()->GetRecordOutFile()<<"----> track: "<<step->GetTrack()->GetTrackID()
//	    <<", # secondary"<<step->GetSecondary()->size()
//		<<", Pid "<<step->GetTrack()->GetDefinition()->GetParticleName()
//		<<", dE "<<step->GetTotalEnergyDeposit()/keV<<"keV"
//		<<", E"<<step->GetTrack()->GetTotalEnergy()/keV<<"keV"<<G4endl;
//	for(int i=0;i<step->GetSecondary()->size();i++){
//		DBIO::Get()->GetRecordOutFile()<<step->GetSecondary()->at(i)->GetTrackID()<<" "
//			  <<step->GetSecondary()->at(i)->GetDefinition()->GetParticleName()<<" "
//			  <<step->GetSecondary()->at(i)->GetCreatorProcess()->GetProcessName()<<" ";
//	}
//	DBIO::Get()->GetRecordOutFile()<<G4endl;
//  //G4cout<<step->GetTrack()->GetCreatorProcess()<<G4endl;
//  //G4cout<<step->GetTrack()->GetCreatorProcess()->GetProcessName()<<endl;
//	  if(step->GetTrack()->GetTrackID()!=trackID){
//		  trackID = step->GetTrack()->GetTrackID();
//		  secondaryID = 0;
//	  }*/
//	//  if(step->GetTrack()->GetDefinition()->GetParticleName()!="opticalphoton"
//	//	&&step->GetTrack()->GetDefinition()->GetParticleName()!="e-"
//	//	&&(step->GetTotalEnergyDeposit()/keV>0||step->GetSecondary()->size()!=secondaryID)){
//		if(//step->GetTrack()->GetDefinition()->GetPDGEncoding()==22
//		   //&&(step->GetTotalEnergyDeposit()/keV>0||step->GetSecondary()->size()!=secondaryID)
//		   //&&step->GetTrack()->GetVolume()==fDetConstruction->GetShieldPV()
//		   //&&step->GetTrack()->GetNextVolume()==fDetConstruction->GetSiBlockPV()){
//			step->GetTrack()->GetTrackID()>1 && step->GetTrack()->GetTrackID() != trackID
//		  ){
//		    DBIO::Get()->GetRecordOutFile()/*<<"----> track: "*/<<" "<<step->GetTrack()->GetTrackID()
//			/*<<", Pid "*/			<<" "<<step->GetTrack()->GetDefinition()->GetPDGEncoding()
//			/*<<", t "*/			<<" "<<step->GetPostStepPoint()->GetGlobalTime()/ns
//			/*<<", x "*/			<<" "<<step->GetPostStepPoint()->GetPosition().x()/cm
//			/*<<", y "*/			<<" "<<step->GetPostStepPoint()->GetPosition().y()/cm
//			/*<<", z "*/			<<" "<<step->GetPostStepPoint()->GetPosition().z()/cm
//			/*<<", # secondary"*/	<<" "<<step->GetSecondary()->size()
//			/*<<", l "*/			<<" "<<step->GetStepLength()/cm
//			/*<<", dE "*/			<<" "<<step->GetTotalEnergyDeposit()/keV
//			/*<<", E"*/				<<" "<<step->GetTrack()->GetTotalEnergy()/keV
//			/*<<", Generated by"*/  <<" "<<step->GetTrack()->GetCreatorProcess()->GetProcessName()<<G4endl;
//	/*	for(int i=secondaryID;i<step->GetSecondary()->size();i++){
//			if(step->GetSecondary()->at(i)->GetDefinition()->GetParticleName()!="opticalphoton")
//	  DSIO::Get()->GetPETScatteringFile()
//			<<step->GetSecondary()->at(i)->GetTrackID()<<" "
//				  <<step->GetSecondary()->at(i)->GetDefinition()->GetParticleName()<<" "
//				  <<step->GetSecondary()->at(i)->GetCreatorProcess()->GetProcessName()<<" ";
//			if(i == step->GetSecondary()->size()-1)
//	  DSIO::Get()->GetPETScatteringFile()<<G4endl;
//		}*/
//			trackID = step->GetTrack()->GetTrackID();
//		secondaryID = step->GetSecondary()->size();
//	  }
//  }
 // if(step->GetTrack()->GetDefinition()->GetParticleName()!="opticalphoton")
  //G4cout<<G4endl;
  return;
 // std::getchar();
}

