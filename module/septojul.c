
#include "../src/include/sep.h"
#include <stdio.h>

static sepatom *atoms;
static sepsys sys;
static sepret ret;
static sepmol *mols;
static sepsampler sampler;

static int natoms=0;
static double lbox[3]={0.0};
static double dt=0.005;
static double maxcf = 2.5;
static int exclusionrule = SEP_ALL;

static bool initflag = false;
static bool initmol = false;
static bool initomp = false;
static bool initsampler = false;

static unsigned iterationNumber=0;

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


void sepSetx0(void){

  if ( !initflag ){
    printf("Error: You must initialize the system before setting x0");
    return;
  } 
  
  sep_set_x0(atoms, sys.npart);
}

void sepSetTimeStep(double newdt){

  dt = newdt;
}

void sepClear(void){

  if ( !initflag ){
    printf("seplib not initialized");
    return;
  }

  if ( initmol ){
    sep_free_mol(mols, &sys);
    initmol = false;
  }

  if ( initsampler ){
    sep_close_sampler(&sampler);
    initsampler = false;
  }
  
  sep_free_sys(&sys);
  sep_close(atoms, natoms);

  initflag = false;
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

void sepForceX0(char type){

  sep_force_x0(atoms, type, sep_spring_x0, &sys);

}

void sepLeapFrog(void){

  sep_leapfrog(atoms, &sys, &ret);

  iterationNumber ++;
}

void sepSave(char *xyzfile, char *types){
  
  sep_save_xyz(atoms, types, xyzfile, "w", sys);

}

void sepGetAtomicPressure(double *press){

  sep_pressure_tensor(&ret, &sys);

  press[0] = ret.p;
  press[1] = ret.P[0][1];
  press[2] = ret.P[0][2];
  press[3] = ret.P[1][2];

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

  if ( strcmp(type, "bonds")==0 ) 
    exclusionrule = SEP_EXCL_BONDED;
  else if ( strcmp(type, "molecule")==0 )
    exclusionrule = SEP_EXCL_SAME_MOL;
  else if ( strcmp(type, "all")==0 )
    exclusionrule = SEP_ALL;
  else
    printf("Exclusion rule not defined");
  
}

void sepSetTypes(char *types){

  for ( int n=0; n<natoms; n++ )  atoms[n].type = types[n];

}

void sepSetOMP(int nthreads){

  sep_set_omp(nthreads, &sys);

  initomp = true;
}


void sepCompress(double desiredRho, double xi){

  sep_compress_box(atoms, desiredRho, xi, &sys);
  
}

void sepInitSampler(void){
  
  sampler = sep_init_sampler();

  initsampler = true;

}

void sepCloseSampler(void){

  if ( initsampler )
    sep_close_sampler(&sampler);

}

void sepAddSamplerVACF(int lvec, double samplespan){

  if ( !initsampler )
    sepInitSampler();
  
  sep_add_sampler(&sampler, "vacf", sys, lvec, samplespan);
     
}

void sepAddSamplerSACF(int lvec, double samplespan){

  if ( !initsampler )
    sepInitSampler();
  
  sep_add_sampler(&sampler, "sacf", sys, lvec, samplespan);
  
   
}

void sepAddSamplerRDF(int lvec, int samplefreq, char *types){

  if ( !initsampler )
    sepInitSampler();
  
  sep_add_sampler(&sampler, "radial", sys, lvec, samplefreq, types);
     
}

void sepAddSamplerProfiler(int lvec, char type, int samplefreq){

  if ( !initsampler )
    sepInitSampler();
  
  sep_add_sampler(&sampler, "profs", sys, lvec, type, samplefreq);
     
}

void sepSample(void) {

  sep_sample(atoms, &sampler, &ret, sys, iterationNumber-1);
 
}
  
