
#include "sep.h"
#include <stdio.h>

static sepatom *atoms;
static sepsys sys;
static sepret ret;

static int natoms=0;
static double lbox[3]={0.0};
static double dt=0.005;
static double maxcf = 2.5;


static int initflag = false;


void sepInit(char *xyzfile){

  if ( initflag ){
    printf("Error: One system is already initialised");
    return;
  }
  
  atoms = sep_init_xyz(lbox, &natoms, xyzfile, 'v');
  sys = sep_sys_setup(lbox[0], lbox[1], lbox[2],
		      maxcf, dt, natoms, SEP_LLIST_NEIGHBLIST);

  initflag = true;
}



void sepClear(void){

  if ( initflag ){
    sep_free_sys(&sys);
    sep_close(atoms, natoms);
    initflag = false;
  }
  else
    printf("Seplib not initialized");

}

void sepReset(void){

  // Reset return values 
  sep_reset_retval(&ret);

  // Reset force 
  sep_reset_force(atoms, &sys);

}

void sepForceLJ(char *types, double *params){
  
  sep_force_lj(atoms, types, params, &sys, &ret, SEP_ALL);

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