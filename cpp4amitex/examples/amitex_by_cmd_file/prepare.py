import math
from os import getenv, makedirs
from random import seed, randint
import numpy as np


mm = 1.0e-3

L = 10*mm

NX = 32
NY = 32
NZ = 32
DX = L/NX
DY = L/NY
DZ = L/NZ

λ = [0.6, 429.0]

r0 = 2.0*mm
phi = [0.7, 0.3]

nmats =  len(λ)
nzones = 1 # NX*NY*NZ

mat = np.ones((NX, NY, NZ), dtype=">u2")
zon = np.ones((NX, NY, NZ), dtype=">u8")

seed(1719837)

nsph = int(phi[1] * L**3 / (4*math.pi/3*r0**3))
print(f"#spheres  = {nsph}")

for isph in range(6):
    ix = randint(0, NX-1)*DX
    iy = randint(0, NY-1)*DY
    iz = randint(0, NZ-1)*DZ
    def pbc(x):
        return x - L*int(2*x/L)
    def dist2(a,b):
        result = 0.0
        for k in range(3):
            vk = pbc(a[k]-b[k])
            result += vk*vk
        return result
    for i in range(NX):
        for j in range(NY):
            for k in range(NZ):
                if dist2([i*DX, j*DY, k*DZ], [ix, iy, iz]) < (r0 + 0.3*DX)**2:
                    mat[i,j,k] = 2


VTK_HEADER = f"""# vtk DataFile Version 4.5
Materiau
BINARY
DATASET STRUCTURED_POINTS
DIMENSIONS    {NX+1}   {NY+1}   {NZ+1}
ORIGIN    0.000   0.000   0.000
SPACING   {DX:.7e} {DY:.7e} {DZ:.7e}
CELL_DATA   {NX*NY*NZ}
SCALARS MaterialId {{data_type}}
LOOKUP_TABLE default
"""

amitex_dir = "amitex_results_0"
makedirs(amitex_dir, exist_ok=True)

with open(amitex_dir + "/materialID.vtk", "wb") as file:
    file.write(VTK_HEADER.format(data_type="unsigned_short").encode())
    mat.tofile(file)

with open(amitex_dir + "/zoneID.vtk", "wb") as file:
    file.write(VTK_HEADER.format(data_type="unsigned_long").encode())
    zon.tofile(file)

K0 = 0.5*(min(λ)+max(λ))

Coeff2 = -1.0


with open(amitex_dir + "/material.xml", "w", encoding="utf-8") as file:
    file.write(f"""<?xml version="1.0" encoding="UTF-8"?>
<Materials>

   	<!-- REFERENCE MATERIAL -->
    <Reference_MaterialD K0 = "{K0}"/>
""")
    for i in range(len(λ)):
        file.write(f"""
    <Material numM="{i+1}" LibK="" LawK="Fourier_iso_polarization">
        <CoeffK Index="1" Type="Constant" Value="{λ[i]}"/>
        <CoeffK Index="2" Type="Constant" Value="{0.0}"/>
        <CoeffK Index="3" Type="Constant" Value="{0.0}"/>
        <CoeffK Index="4" Type="Constant" Value="{-λ[i]}"/>
    </Material>
""")
    
    file.write("""
</Materials>
""")

