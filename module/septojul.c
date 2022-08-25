
#include "../src/include/sep.h"
#include <stdio.h>

static sepatom *atoms;
static sepsys sys;
static sepret ret;
static sepmol *mols;

static int natoms=0;
static double lbox[3]={0.0};
static double dt=0.005;
static double maxcf = 2.5;
static int exclusionrule = SEP_ALL;

static int initflag = false;
static int initmol = false;


void sepInit_1(char *xyzfile){

  if ( initflag ){
    printf("Error: One system is already initialised");
    return;
  }
  
  atoms = sep_init_xyz(lbox, &natoms, xyzfile, 'v');
  sys = sep_sys_setup(lbox[0], lbox[1], lbox[2],
		      maxcf, dt, natoms, SEP_LLIST_NEIGHBLIST);

  initflag = true;
}


void sepInit_2(char *xyzfile, char *topfile){

  if ( initflag ){
    printf("Error: One system is already initialised");
    return;
  }
  
  atoms = sep_init_xyz(lbox, &natoms, xyzfile, 'v');
  sys = sep_sys_setup(lbox[0], lbox[1], lbox[2],
		      maxcf, dt, natoms, SEP_LLIST_NEIGHBLIST);

  sep_read_topology_file(atoms, topfile, &sys, 'v');
  mols = sep_init_mol(atoms, &sys);
  
  initflag = true;
  initmol = true;
}



void sepClear(void){

  if ( !initflag ){
    printf("Seplib not initialized");
    return;
  }

  if ( initmol ){
    sep_free_mol(mols, &sys);
    initmol = false;
  }
  
  sep_free_sys(&sys);
  sep_close(atoms, natoms);

}

void sepReset(void){

  
  sep_reset_retval(&ret);
  sep_reset_force(atoms, &sys);

  // This must be changed in accordance with when
  // pressure calc is perfomed as it is slow
  if ( initmol )
    sep_reset_force_mol(&sys);
  
}

void sepForceLJ(char *types, double *params){
  
  sep_force_lj(atoms, types, params, &sys, &ret, exclusionrule);

}

void sepForceBondHarmonic(int type, double lb, double ks){

  sep_stretch_harmonic(atoms, type, lb, ks, &sys, &ret);
  
}

void sepForceAngleCossq(int type, double angle, double kangle){

  sep_angle_cossq(atoms, type, angle, kangle, &sys, &ret);

}

void sepForceTorsion(int type, double *param){
  
  sep_torsion_Ryckaert(atoms, type, param, &sys, &ret);

}

void sepForceCoulombSF(double cf){

  sep_coulomb_sf(atoms, cf, &sys, &ret, exclusionrule);
 
}

void sepLeapFrog(void){

  sep_leapfrog(atoms, &sys, &ret);

}

void sepSave(char *xyzfile){
  
  sep_save_xyz(atoms, "A", xyzfile, "w", sys);

}

void sepGetEnergies(double *energies){

  energies[0] = ret.epot;
  energies[1] = ret.ekin;

}

int sepGetNumbParticles(){

  return natoms;

}

void sepGetPositions(double *x, double *y, double *z){

  for ( int n=0; n<natoms; n++ ){
    x[n] = atoms[n].x[0];
    y[n] = atoms[n].x[1];
    z[n] = atoms[n].x[2];
  }

}


void sepGetMasses(double *m){

  for ( int n=0; n<natoms; n++ ){
    m[n] = atoms[n].m;
  }

}


void sepGetCharges(double *z){

  for ( int n=0; n<natoms; n++ ){
    z[n] = atoms[n].z;
  }

}
void sepRelaxTemp(char type, double Td, double tau){
  
  sep_relax_temp(atoms, type, Td, tau, &sys);

}

void sepSetExclusionRule(char *type){

  if ( strcmp(type, "bond")==0 ) 
    exclusionrule = SEP_EXCL_BONDED;
  else if ( strcmp(type, "molecule")==0 )
    exclusionrule = SEP_EXCL_SAME_MOL;
  else if ( strcmp(type, "all")==0 )
    exclusionrule = SEP_ALL;
  else
    printf("Exclusion rule not defined");
  
}
