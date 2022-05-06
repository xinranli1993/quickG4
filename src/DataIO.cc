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
foutRootFile = new TFile("test.root","RECREATE");
foutTTree = new TTree("out","out");
	int nDep;
	int eventID;
	double primaryT;
	double totalDepE[4];
//	double totSourceDepE;
//	int totC[1];
	double primaryP1[3];
	//double primaryP2[3];
	double primaryPos[3];
	double* depX_true;
	double* depY_true;
	double* depZ_true;
	double* depT_true;
	double* depE_true;
	//int*	depC;
	double* Ek_true;
	int* depMotherID;
	int* depTrkID;
	bool* isPhoton;
	//int totCiriwiE[20][20][8];
	//int depC_EScan[800];
	//int totCiE[8];
	int* pid;
foutTTree->Branch("eventID", &eventID, "eventID/I");
foutTTree->Branch("nDep", &nDep, "nDep/I");
foutTTree->Branch("primaryT", &primaryT, "primaryT/D");
foutTTree->Branch("primaryP1",primaryP1,"primaryP1[3]/D");
//foutTTree->Branch("primaryP2",primaryP2,"primaryP2[3]/D");
foutTTree->Branch("primaryPos",primaryPos,"primaryPos[3]/D");
foutTTree->Branch("totalDepE", totalDepE, "totalDepE[4]/D");
//foutTTree->Branch("totSourceDepE", &totSourceDepE, "totSourceDepE/D");
//	if(fDoRecParScan)
//	foutTTree->Branch("totC",totCiriwiE,"totC[20][20][8]/I");
//	if(fDoEScan)
//	foutTTree->Branch("totC",totCiE,"totC[8]/I");
//	else
//	foutTTree->Branch("totC", totC, "totC[1]/I");
foutTTree->Branch("depX_true", depX_true, "depX_true[nDep]/D");
foutTTree->Branch("depY_true", depY_true, "depY_true[nDep]/D");
foutTTree->Branch("depZ_true", depZ_true, "depZ_true[nDep]/D");
foutTTree->Branch("depT_true", depT_true, "depT_true[nDep]/D");
foutTTree->Branch("depE_true", depE_true, "depE_true[nDep]/D");
//	if(fDoEScan)
//	foutTTree->Branch("depC", depC_EScan, "depC[nDep][8]/I");
//	else
//	foutTTree->Branch("depC", depC, "depC[nDep]/I");
foutTTree->Branch("pid", pid, "pid[nDep]/I");
foutTTree->Branch("isPhoton", isPhoton, "isPhoton[nDep]/O");
foutTTree->Branch("Ek_true", Ek_true, "Ek_true[nDep]/D");
foutTTree->Branch("depTrkID",depTrkID,"depTrkID[nDep]/I");
foutTTree->Branch("depMotherID",depMotherID,"depMotherID[nDep]/I");
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

void DataIO::FillTTree(B2EventData * eventData){
	if(!foutTTree){
		G4cout<<"TTree is not built! "<<G4endl;
		return;
	}
	if(eventData->GetEventID() == -1){
		G4cout<<"eventData is not given a value!"<<G4endl;
	}else if(eventData->GetNDep() <=1){
//		G4cout<<"event is empty, no deposition"<<G4endl;
	}else{
		int nDep = eventData->GetNDep();
		//G4cout<<nDep<<" deposition saved in DBEventData "<<G4endl;
		int eventID = eventData->GetEventID();
		double primaryT;
		double primaryP1[3];
//		double primaryP2[3];
		double primaryPos[3];
		double totalDepE[4];
//		double totSourceDepE;
//		int* totC;
//		if(fDoRecParScan) totC= new int[3200];
//		else if(fDoEScan) totC= new int[8];
//		else			  totC= new int;
		double* depX_true = new G4double[nDep];
		double* depY_true = new G4double[nDep];
		double* depZ_true = new G4double[nDep];
		double* depT_true = new G4double[nDep];
	  double* depE_true = new G4double[nDep];
	  double* Ek_true = new G4double[nDep];
//		int* depC;
//		if(fDoEScan) depC = new int[nDep*8];
//		else		 depC = new int[nDep];
		int*  depMotherID = new G4int[nDep];
		int*  depTrkID = new G4int[nDep];
		bool* isPhoton = new G4bool[nDep];
		int* pid = new int[nDep];

	foutTTree->SetBranchAddress("eventID", &eventID);
	foutTTree->SetBranchAddress("nDep", &nDep);
	foutTTree->SetBranchAddress("primaryT", &primaryT);
	foutTTree->SetBranchAddress("primaryP1",primaryP1);
//	foutTTree->SetBranchAddress("primaryP2",primaryP2);
	foutTTree->SetBranchAddress("primaryPos",primaryPos);
	foutTTree->SetBranchAddress("totalDepE", totalDepE);
//	foutTTree->SetBranchAddress("totSourceDepE", &totSourceDepE);
//	foutTTree->SetBranchAddress("totC", totC);
	foutTTree->SetBranchAddress("depX_true", depX_true);
	foutTTree->SetBranchAddress("depY_true", depY_true);
	foutTTree->SetBranchAddress("depZ_true", depZ_true);
	foutTTree->SetBranchAddress("depE_true", depE_true);
//	foutTTree->SetBranchAddress("depC", depC);
	foutTTree->SetBranchAddress("pid", pid);
	foutTTree->SetBranchAddress("Ek_true", Ek_true);
	foutTTree->SetBranchAddress("depT_true", depT_true);
	foutTTree->SetBranchAddress("isPhoton", isPhoton);
	foutTTree->SetBranchAddress("depMotherID", depMotherID);
	foutTTree->SetBranchAddress("depTrkID", depTrkID);

		eventData->GetPrimaryT(primaryT);
		//G4cout<<"---------------- primaryT = "<<primaryT<<G4endl;
		eventData->GetPrimaryP1(primaryP1);
		//G4cout<<"---------------- primaryP1 = "<<primaryP1[0]<<" "<<primaryP1[1]<<" "<<primaryP1[2]<<G4endl;
//		eventData->GetPrimaryP2(primaryP2);
		//G4cout<<"---------------- primaryP2 = "<<primaryP2[0]<<" "<<primaryP2[1]<<" "<<primaryP2[2]<<G4endl;

		eventData->GetPrimaryPos(primaryPos);
		eventData->GetX_true(nDep, depX_true);
		eventData->GetY_true(nDep, depY_true);
		eventData->GetZ_true(nDep, depZ_true);
		eventData->GetDepT(nDep, depT_true);
//		eventData->GetCharge(nDep, depC, totC);
		eventData->GetDepE(nDep, depE_true, Ek_true, totalDepE, isPhoton, pid);
		eventData->GetDepMotherID(nDep, depMotherID);
		eventData->GetDepTrkID(nDep, depTrkID);
	foutTTree->Fill();
		//G4cout<<outTTree->GetEntries()<<" entries after filling"<<G4endl;
		//std::getchar();
		delete [] depX_true;
		delete [] depY_true;
		delete [] depZ_true;
		delete [] depT_true;
	    delete [] depE_true;
//		delete [] depC;
		delete [] pid;
		delete [] depMotherID;
		delete [] depTrkID;
		delete [] Ek_true;
	}
	return;
}
