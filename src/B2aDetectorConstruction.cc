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
/// \file B2aDetectorConstruction.cc
/// \brief Implementation of the B2aDetectorConstruction class
 
#include "B2aDetectorConstruction.hh"
#include "B2aDetectorMessenger.hh"
#include "B2TrackerSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4Sphere.hh"
#include "G4RotationMatrix.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4ThreadLocal 
G4GlobalMagFieldMessenger* B2aDetectorConstruction::fMagFieldMessenger = 0;

B2aDetectorConstruction::B2aDetectorConstruction()
:G4VUserDetectorConstruction(), 
 fNbOfChambers(0),
 fLogicTarget(NULL), fLogicChamber(NULL), 
 fTargetMaterial(NULL), fChamberMaterial(NULL), 
 fStepLimit(NULL),
 fCheckOverlaps(true)
{
  fMessenger = new B2aDetectorMessenger(this);

  fNbOfChambers = 5;
  fLogicChamber = new G4LogicalVolume*[fNbOfChambers];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
B2aDetectorConstruction::~B2aDetectorConstruction()
{
  delete [] fLogicChamber; 
  delete fStepLimit;
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* B2aDetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2aDetectorConstruction::DefineMaterials()
{
  // Material definition 

  nistManager = G4NistManager::Instance();

  // Air defined using NIST Manager
  nistManager->FindOrBuildMaterial("G4_AIR");
  
  // LHe defined using NIST Manager
  fTargetLHeMaterial  = nistManager->BuildMaterialWithNewDensity("G4_LHe", "G4_He", 0.125*g/cm3, 2*kelvin);
  fTargetSiMaterial   = nistManager->FindOrBuildMaterial("G4_Si");
  // Sapphire 
  G4double density;
  G4double z, a;
  G4int natoms, ncomp;
  fTargetSapphireMaterial = new G4Material("Sapphire", density = 3.98*g/cm3, ncomp=2);
  G4Element* O = new G4Element("Oxygen", "O", z = 8., a=16.*g/mole);
  G4Element* Al= new G4Element("Aluminum", "Al", z = 13., a=26.98*g/mole);
  fTargetSapphireMaterial->AddElement(O, natoms=3);
  fTargetSapphireMaterial->AddElement(Al,natoms=2);
  //GaAs
  fTargetGaAsMaterial = new G4Material("GaAs", density = 5.317*g/cm3, ncomp=2);
  G4Element* Ga = new G4Element("Gallium", "Ga", z = 31., a = 69.723*g/mole);
  G4Element* As = new G4Element("Arsenic", "As", z = 33., a = 74.922*g/mole);
  fTargetGaAsMaterial->AddElement(Ga, natoms=1);
  fTargetGaAsMaterial->AddElement(As, natoms=1);

  // Copper defined using NIST Manager
  fChamberMaterial = nistManager->FindOrBuildMaterial("G4_Cu");

  // Copper defined using NIST Manager
  f300KChamberMaterial = nistManager->FindOrBuildMaterial("G4_Al");

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B2aDetectorConstruction::DefineVolumes()
{
  G4Material* air  = G4Material::GetMaterial("G4_AIR");

  // Sizes of the principal geometrical components (solids)
  
  G4double chamberRadius = 20*cm; // from chamber center to center!

  G4double chamberThickness = 0.3*cm; // width of the chambers
  G4double chamber300KThickness = 0.5*cm; // width of the chambers
  G4double targetLength =  0.5*cm; // full length of Target
  

  G4double worldLength = 2*m;

  G4double targetRadius  = 1*cm;   // Radius of Target
  targetLength = 0.5*targetLength;             // Half length of the Target  
  fTargetMaterial = fTargetSiMaterial;

  // Definitions of Solids, Logical Volumes, Physical Volumes

  // World

  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);

  G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
         << " mm" << G4endl;

  G4Box* worldS
    = new G4Box("world",                                    //its name
                worldLength/2,worldLength/2,worldLength/2); //its size
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,   //its solid
                 air,      //its material
                 "World"); //its name
  
  //  Must place the World Physical volume unrotated at (0,0,0).
  // 
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(), // at (0,0,0)
                 worldLV,         // its logical volume
                 "World",         // its name
                 0,               // its mother  volume
                 false,           // no boolean operations
                 0,               // copy number
                 fCheckOverlaps); // checking overlaps 

  //chamber
//Dilution Chamber
G4double nedges[15]= { -48.75*mm,
                       -13.75*mm,
                       -13.75*mm,
                       0*mm,
                       13.75*mm,
                       13.75*mm,
                       38.75*mm,
                       38.75*mm,
                       52.25*mm,
                       52.25*mm,
                       64.75*mm,
                       64.75*mm,
                       104.25*mm,
                       104.25*mm,
                       107.25*mm };



  G4double innerradius[15]= { 0*mm,
                      0*mm,
                      30*mm,
                      30*mm,
                      30*mm,
                      52.5*mm,
                      52.5*mm,
                      18*mm,
                      18*mm,
                      11*mm,
                      11*mm,
                      98*mm,
                      98*mm,
                      0*mm,
                      0*mm};


      G4double outerradius[15]= { 100*mm,
              100*mm,
              100*mm,
              100*mm,
              100*mm,
              100*mm,
              100*mm,
              100*mm,
              100*mm,
              100*mm,
              100*mm,
             100*mm,
            100*mm,
            100*mm,
            100*mm};

G4ThreeVector position_DilutionChamber = G4ThreeVector(0, 0, 0);

G4Polycone* DilutionChamber_S = new G4Polycone("DilutionChamber", 0, 360*deg, 15, nedges, innerradius, outerradius);
G4LogicalVolume*  DilutionChamber_LV= new G4LogicalVolume(DilutionChamber_S, nistManager->FindOrBuildMaterial("G4_Cu")
, "DilutionChamber");
 new G4PVPlacement(0, position_DilutionChamber , DilutionChamber_LV, "DilutionChamber", worldLV, false, 0, fCheckOverlaps);
DilutionChamber_LV->SetVisAttributes(G4VisAttributes(G4Colour(1.,0.5,0.)));

	//aluminium
	G4Tubs* AlFilmS
    = new G4Tubs("ALFilmS",0,30*mm,0.0375*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* AlFilmLV
    = new G4LogicalVolume(AlFilmS, nistManager->FindOrBuildMaterial("G4_Al")
	, "AlFilmLV",0,0,0);  
  new G4PVPlacement(0,               // no rotation
                    G4ThreeVector(0,0,(-13.75+0.0375)*mm), // at (x,y,z)
                    AlFilmLV,       // its logical volume
                    "AlFilm",       // its name
                    worldLV,         // its mother  volume
                    false,           // no boolean operations
                    0,               // copy number
                    fCheckOverlaps); // checking overlaps 
  //detector
	G4Tubs* CPDS
    = new G4Tubs("CPDS",0,30*mm,0.5*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* CPDLV
    = new G4LogicalVolume(CPDS, nistManager->FindOrBuildMaterial("G4_Si")
	, "CPDLV",0,0,0);  
  new G4PVPlacement(0,               // no rotation
                    G4ThreeVector(0,0,(13.75+6)*mm), // at (x,y,z)
                    CPDLV,       // its logical volume
                    "CPD",       // its name
                    worldLV,         // its mother  volume
                    false,           // no boolean operations
                    0,               // copy number
                    fCheckOverlaps); // checking overlaps 
  //source
   G4Tubs* sourceS
    = new G4Tubs("sourceS",0,10*mm,0.325*mm, 0.*deg, 360.*deg);
  G4LogicalVolume* sourceLV
    = new G4LogicalVolume(sourceS, nistManager->FindOrBuildMaterial("G4_Cu")
	, "sourceLV",0,0,0);  
  new G4PVPlacement(0,               // no rotation
                    G4ThreeVector(30*mm,0,(13.75+0.325)*mm), // at (x,y,z)
                    sourceLV,       // its logical volume
                    "source",       // its name
                    worldLV,         // its mother  volume
                    false,           // no boolean operations
                    0,               // copy number
                    fCheckOverlaps); // checking overlaps 
  //limit steps
  G4UserLimits* steplimit = new G4UserLimits(1*um);
  AlFilmLV->SetUserLimits(steplimit);
  


  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void B2aDetectorConstruction::ConstructSDandField()
{
  // Sensitive detectors

  G4String trackerChamberSDname = "B2/TrackerChamberSD";
  B2TrackerSD* aTrackerSD = new B2TrackerSD(trackerChamberSDname,
                                            "TrackerHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);
  // Setting aTrackerSD to all logical volumes with the same name 
  // of "Chamber_LV".
  SetSensitiveDetector("CPDLV", aTrackerSD);
  //SetSensitiveDetector("Target_LV", aTrackerSD);

  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue = G4ThreeVector();
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void B2aDetectorConstruction::SetTargetMaterial(G4String materialName)
{
	if(materialName == fTargetMaterial->GetName()){

	}else{
  G4NistManager* nistManager = G4NistManager::Instance();

  G4Material* pttoMaterial = 
              nistManager->FindOrBuildMaterial(materialName);

  if (fTargetMaterial != pttoMaterial) {
     if ( pttoMaterial ) {
        fTargetMaterial = pttoMaterial;
        if (fLogicTarget) fLogicTarget->SetMaterial(fTargetMaterial);
        G4cout 
          << G4endl 
          << "----> The target is made of " << materialName << G4endl;
     } else {
        G4cout 
          << G4endl 
          << "-->  WARNING from SetTargetMaterial : "
          << materialName << " not found" << G4endl;
     }
  }
	}
}
 
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//void B2aDetectorConstruction::SetChamberMaterial(G4String materialName)
//{
//  G4NistManager* nistManager = G4NistManager::Instance();
//
//  G4Material* pttoMaterial =
//              nistManager->FindOrBuildMaterial(materialName);
//
//  if (fChamberMaterial != pttoMaterial) {
//     if ( pttoMaterial ) {
//        fChamberMaterial = pttoMaterial;
//        for (G4int copyNo=0; copyNo<fNbOfChambers; copyNo++) {
//            if (fLogicChamber[copyNo]) fLogicChamber[copyNo]->
//                                               SetMaterial(fChamberMaterial);
//        }
//        G4cout 
//          << G4endl 
//          << "----> The chambers are made of " << materialName << G4endl;
//     } else {
//        G4cout 
//          << G4endl 
//          << "-->  WARNING from SetChamberMaterial : "
//          << materialName << " not found" << G4endl;
//     }
//  }
//}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2aDetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2aDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
  fCheckOverlaps = checkOverlaps;
}  
