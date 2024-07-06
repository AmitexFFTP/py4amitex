#/bin/bash

N=32
mkdir -p testresults
amitex_fftp -NX 32 -NY 32 -NZ 32 -a algo_default_user.xml -c char_traction_novtk.xml -m  mat_elasiso_eigs_1.xml -s testresults/output

