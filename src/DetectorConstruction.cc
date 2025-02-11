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
/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction():
  fAirMat(0), fTisMat(0), fCR39Mat(0)
{
};

DetectorConstruction::~DetectorConstruction()
{
};

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
};

void DetectorConstruction::DefineMaterials()
{
  // Material definition

  G4NistManager* nistManager = G4NistManager::Instance();

  //Properties
  G4int numel(0), natoms(0) ;
  G4double density(0.), fractionMass(0.)  ;
  G4String name ;


  //Elementi necessari

  //Materiali necessari
  
  //Air
  G4double z, a, d;
  G4Element* elN = new G4Element("Hydrogen", "N", z=7, a=14.007*g/mole);
  G4Element* elO = new G4Element("Oxygen", "O", z=8, a=16*g/mole);
  G4Element* elAr = new G4Element("Argon", "Ar", z=18, a=39.948*g/mole);
  G4Element* elC = new G4Element("Carbon", "C", z=6, a=12.011*g/mole);
  G4Element* elH = new G4Element("Hydrogen", "H", z=1, a=1.008*g/mole);

  G4Material* Air = new G4Material("Air", d=1.225*kg/m3, numel=3);
  Air-> AddElement(elN, 78.*perCent);
  Air-> AddElement(elO, 21.*perCent);
  Air-> AddElement(elAr, 1.*perCent);
  fAirMat = Air;
  
  G4Material* CR39 = new G4Material("CR39", d=1.31*g/cm3, numel=3);
  CR39-> AddElement(elC, 32.4*perCent);
  CR39-> AddElement(elO, 48.6*perCent);
  CR39-> AddElement(elH, 19.*perCent);
  fCR39Mat = CR39;
  
  G4Material* Tissue = new G4Material("Tissue", d=0.2*g/cm3, numel=4);
  Tissue-> AddElement(elN, 2.6*perCent);
  Tissue-> AddElement(elO, 76.2*perCent);
  Tissue-> AddElement(elH, 10.1*perCent);
  Tissue-> AddElement(elC, 11.1*perCent);
  fTisMat = Tissue;

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{

  G4NistManager* nistManager = G4NistManager::Instance();

  //***************************************************
  //     WORLD
  //***************************************************

  G4double worldSizeX = 6*cm ;
  G4double worldSizeY = 6*cm ;
  G4double worldSizeZ = 1*cm ;

  // World definition and placement
  G4Box* worldBox = new G4Box("WorldBox", worldSizeX, worldSizeY, worldSizeZ) ;
  G4LogicalVolume* worldLog = new G4LogicalVolume(worldBox, fAirMat, "WorldLog") ;

  // Set visual attributes
  G4VisAttributes *worldAttributes = new G4VisAttributes ;
  worldAttributes->SetVisibility(true) ;
  worldLog->SetVisAttributes(worldAttributes) ;
  G4VPhysicalVolume* worldPhys = new G4PVPlacement(0, G4ThreeVector(), worldLog, "WorldSpace", 0, false, 0) ;

  G4VisAttributes *TissueAttributes = new G4VisAttributes(G4Color::Red());
  TissueAttributes->SetVisibility(true);
   
  G4VisAttributes *DetectorAttributes = new G4VisAttributes(G4Color::Yellow());
  DetectorAttributes->SetVisibility(true);
  
  //Tissue Sample
  G4Tubs* TissueSample = new G4Tubs("TissueSample", 0, 0.5*cm, 0.005*cm, 0, 360*deg);
  G4LogicalVolume* TissueSampleLog = new G4LogicalVolume(TissueSample, fTisMat, "TissueSample");
  TissueSampleLog->SetVisAttributes(new G4VisAttributes(*TissueAttributes));
  G4ThreeVector tissuePosition = G4ThreeVector(0, 0, 0);
  G4VPhysicalVolume* physiTisSam = new G4PVPlacement(0, tissuePosition, TissueSampleLog, "TissueSample", worldLog, false, 0);
  
  //CR39
  G4Box* CR_39 = new G4Box("CR-39", 1*cm, 5*cm, 0.05*cm);
  G4LogicalVolume* CR39Log = new G4LogicalVolume(CR_39, fCR39Mat, "CR-39");
  CR39Log->SetVisAttributes(new G4VisAttributes(*DetectorAttributes));
  G4ThreeVector CR39Position = G4ThreeVector(0, 0, -0.0555*cm);
  G4VPhysicalVolume* CR39 = new G4PVPlacement(0, CR39Position, CR39Log, "CR39Detector", worldLog, false, 0);  
  
  //***************************************************
  //     SCINTILLATORS
  //***************************************************
/*
  CR39Log->SetSensitiveDetector(sensitive);
*/
  return worldPhys ;

};

}
