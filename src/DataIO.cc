#include "DataIO.hh"
#include "B2TrackerHit.hh"
#include "TMath.h"

DataIO* DataIO::me=0;
DataIO::DataIO(){
	foutRootFileName = "test.root";
	fmyEvent.eventID=0;
}

DataIO* DataIO::Get(){
	if(!me)
	  me=new DataIO();
	return me;
}

void DataIO::OpenRootFile(){
//	if(foutRootFile==NULL)
		foutRootFile = new TFile(foutRootFileName.c_str(),"RECREATE");
//	if(foutTTree==NULL){
		foutTTree = new TTree("out","out");
		foutTTree->Branch("eventID", &(fmyEvent.eventID), "eventID/I");
		foutTTree->Branch("trackID", &(fmyEvent.trackID), "trackID/S");
		foutTTree->Branch("theta",   &(fmyEvent.theta),   "theta/D");
		foutTTree->Branch("R",     &(fmyEvent.r),     "R/D");
		foutTTree->Branch("E",       &(fmyEvent.e),       "E/D");
		foutTTree->Branch("TargetE", &(fmyEvent.targetE), "TargetE/D");
//	}
}

void DataIO::WriteRootFile(){
	foutRootFile->cd();
	foutTTree->Write("",TObject::kOverwrite);
}

void DataIO::CloseRootFile(){
	WriteRootFile();
//	if(foutRootFile->IsOpen())
	  foutRootFile->Close();
}

void DataIO::FillTTree(const G4Event* event){
	G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
	size_t nHit = hc->GetSize();
	for(size_t iHit=0; iHit<nHit; iHit++){
		B2TrackerHit * thisHit = (B2TrackerHit*)hc->GetHit(iHit);
		fmyEvent.trackID = thisHit->GetTrackID();
		fmyEvent.e = thisHit->GetEdep();
		fmyEvent.targetE = thisHit->GetTargetEdep();
		double xx = thisHit->GetPos().x();
		double yy = thisHit->GetPos().y();
		fmyEvent.theta = yy>0 ? TMath::ATan(xx/yy) : 
						 xx>0 ? TMath::ATan(xx/yy) + 3.1415926535 :
						 TMath::ATan(xx/yy) - 3.1415926535;
		fmyEvent.r = TMath::Sqrt(xx*xx+yy*yy);
		foutTTree->Fill();
	}
}
