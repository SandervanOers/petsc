static char help[] = "Reads a PETSc matrix from a file and solves a linear system \n\
using the aijcusparse class. Input parameters are:\n\
  -f <input_file> : the file to load\n\n";

/*
  This code can be used to test PETSc interface to other packages.\n\
  Examples of command line options:       \n\
   ./ex43 -f DATAFILESPATH/matrices/cfd.2.10 -mat_cusparse_mult_storage_format ell  \n\
   ./ex43 -f DATAFILESPATH/matrices/shallow_water1 -ksp_type cg -pc_type icc -mat_cusparse_mult_storage_format ell  \n\
   \n\n";
*/

#include <petscksp.h>

int main(int argc,char **argv)
{
  KSP                ksp;
  Mat                A;
  Vec                X,B;
  PetscInt           m, its;
  PetscReal          norm;
  char               file[PETSC_MAX_PATH_LEN];
  PetscBool          flg;
  PetscViewer        fd;
  PetscErrorCode     ierr;

  ierr = PetscInitialize(&argc,&argv,0,help);if (ierr) return ierr;
  /* Load the data from a file */
  ierr = PetscOptionsGetString(NULL,NULL,"-f",file,PETSC_MAX_PATH_LEN,&flg);CHKERRQ(ierr);
  if (!flg) SETERRQ(PETSC_COMM_WORLD,1,"Must indicate binary file with the -f option");
  ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,file,FILE_MODE_READ,&fd);CHKERRQ(ierr);

  /* Build the matrix */
  ierr = MatCreate(PETSC_COMM_WORLD,&A);CHKERRQ(ierr);
  ierr = MatSetFromOptions(A);CHKERRQ(ierr);
  ierr = MatLoad(A,fd);CHKERRQ(ierr);

  /* Build the vectors */
  ierr = MatGetLocalSize(A,&m,NULL);CHKERRQ(ierr);
  ierr = VecCreate(PETSC_COMM_WORLD,&B);CHKERRQ(ierr);
  ierr = VecSetSizes(B,m,PETSC_DECIDE);CHKERRQ(ierr);
  ierr = VecCreate(PETSC_COMM_WORLD,&X);CHKERRQ(ierr);
  ierr = VecSetSizes(X,m,PETSC_DECIDE);CHKERRQ(ierr);
  ierr = VecSetFromOptions(B);CHKERRQ(ierr);
  ierr = VecSetFromOptions(X);CHKERRQ(ierr);
  ierr = VecSet(B,1.0);CHKERRQ(ierr);

  /* Build the KSP */
  ierr = KSPCreate(PETSC_COMM_WORLD,&ksp);CHKERRQ(ierr);
  ierr = KSPSetOperators(ksp,A,A);CHKERRQ(ierr);
  ierr = KSPSetType(ksp,KSPGMRES);CHKERRQ(ierr);
  ierr = KSPSetTolerances(ksp,1.0e-12,PETSC_DEFAULT,PETSC_DEFAULT,100);CHKERRQ(ierr);
  ierr = KSPSetFromOptions(ksp);CHKERRQ(ierr);

  /* Solve */
  ierr = KSPSolve(ksp,B,X);CHKERRQ(ierr);

  /* print out norm and the number of iterations */
  ierr = KSPGetIterationNumber(ksp,&its);CHKERRQ(ierr);
  ierr = KSPGetResidualNorm(ksp,&norm);CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"Number of iterations = %3D\n",its);CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"Residual norm %1.5g\n",norm);CHKERRQ(ierr);

  /* Cleanup */
  ierr = VecDestroy(&X);CHKERRQ(ierr);
  ierr = VecDestroy(&B);CHKERRQ(ierr);
  ierr = MatDestroy(&A);CHKERRQ(ierr);
  ierr = KSPDestroy(&ksp);CHKERRQ(ierr);
  ierr = PetscViewerDestroy(&fd);CHKERRQ(ierr);
  ierr = PetscFinalize();
  return ierr;
}


/*TEST

   build:
      requires: veccuda

   test:
      requires: datafilespath double !complex !define(PETSC_USE_64BIT_INDICES)
      args: -f ${DATAFILESPATH}/matrices/cfd.2.10 -mat_type seqaijcusparse -pc_factor_mat_solver_type cusparse -mat_cusparse_storage_format ell -vec_type cuda -pc_type ilu

   test:
      suffix: 2
      requires: datafilespath double !complex !define(PETSC_USE_64BIT_INDICES)
      args: -f ${DATAFILESPATH}/matrices/shallow_water1 -mat_type seqaijcusparse -pc_factor_mat_solver_type cusparse -mat_cusparse_storage_format hyb -vec_type cuda -ksp_type cg -pc_type icc

   test:
      suffix: 3
      requires: datafilespath double !complex !define(PETSC_USE_64BIT_INDICES)
      args: -f ${DATAFILESPATH}/matrices/cfd.2.10 -mat_type seqaijcusparse -pc_factor_mat_solver_type cusparse -mat_cusparse_storage_format csr -vec_type cuda -ksp_type bicg -pc_type ilu

   test:
      suffix: 4
      requires: datafilespath double !complex !define(PETSC_USE_64BIT_INDICES)
      args: -f ${DATAFILESPATH}/matrices/cfd.2.10 -mat_type seqaijcusparse -pc_factor_mat_solver_type cusparse -mat_cusparse_storage_format csr -vec_type cuda -ksp_type bicg -pc_type ilu -pc_factor_mat_ordering_type nd

   test:
      suffix: 5
      nsize: 2
      requires: datafilespath double !complex !define(PETSC_USE_64BIT_INDICES)
      args: -f ${DATAFILESPATH}/matrices/shallow_water1 -mat_type mpiaijcusparse -mat_cusparse_mult_diag_storage_format hyb -pc_type none -vec_type cuda

   test:
      suffix: 6
      requires: cusp datafilespath double !complex !define(PETSC_USE_64BIT_INDICES)
      args: -f ${DATAFILESPATH}/matrices/shallow_water1 -mat_type seqaijcusp -pc_type none -vec_type seqcusp

   test:
      suffix: 7
      requires: cusp datafilespath double !complex !define(PETSC_USE_64BIT_INDICES)
      args: -f ${DATAFILESPATH}/matrices/shallow_water1 -mat_type seqaijcusp -mat_cusp_mult_storage_format ell -pc_type none -vec_type seqcusp


   test:
      suffix: 8
      nsize: 2
      requires: cusp datafilespath double !complex !define(PETSC_USE_64BIT_INDICES)
      args: -f ${DATAFILESPATH}/matrices/shallow_water1 -mat_type mpiaijcusp -pc_type none -vec_type mpicusp


TEST*/
