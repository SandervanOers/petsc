#ifndef RAMGFUNC_H
#define RAMGFUNC_H

struct RAMG_PARAM{
  /*..Class 1 RAMG parameters..*/
                 int    NDA;
                 int    NDIA; 
                 int    NDJA;
                 int    NDU;  
                 int    NDF; 
                 int    NDIG; 
                 int    MATRIX; 
  /*..Class 2 RAMG parameters..*/
                 int    ISWTCH; 
                 int    IOUT;
                 int    IPRINT;
  /*..Class 3 RAMG parameters..*/
                 int    LEVELX; 
                 int    IFIRST; 
                 int    NCYC; 
                 int    MADAPT; 
                 int    NRD; 
                 int    NSOLCO; 
                 int    NRU; 
                 double EPS; 
  /*..Class 4 RAMG parameters..*/
                 int    NWT; 
                 int    NTR; 
                 double ECG1; 
                 double ECG2; 
                 double EWT2;     
               };

extern "C" void symamg1r5_(double* a,int* ia,int* ja,double* u,double* f,
                     int *ig,int* nda,int* ndia,int* ndja,int* ndu,int* ndf,
                     int* ndig,int* nnu, int* matrix,int* iswtch,int* iout,
                     int* iprint,int* levelx,int* ifirst,int* ncyc, 
                     double* eps,int* madapt,int* nrd,int* nsolco,int* nru,
                     double* ecg1,double* ecg2,double* ewt2,int* nwt,int* ntr,
                     int* ierr); 

#endif//RAMGFUNC_H
