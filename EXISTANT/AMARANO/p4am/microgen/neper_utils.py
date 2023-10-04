#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Temp Module to creates Amitex fft inputs for polycrystalline simulations
from Neper.

Not yet well documented...

@author: amarano
"""

# TODO: correct tess for Amitex output
# TODO: read tess morpho options from file
# TODO: add options to speed up tesselation

# TODO: add read statistics from tesselations

# TODO: class to generates tesselation geometries and morpho statistics

# Imports
import os
import subprocess
from pathlib import Path

import numpy as np

from pymicro.core.utils.SDAmitexUtils import SDAmitexIO as SDA

# Class for Neper tesselation construction
class PolyXTessBuilder:
    def __init__(self, data_dir=None, tess_file='', seed=1, dim=3, ngrains=0,
                 periodicity='none', morpho_opts={}):
        """Class to build Neper tesselations and inputs for Amitex_fftp."""
        # init command lines
        self.cmd = 'neper'
        self.tess_cmd = []
        self.tesr_cmd = []
        self.stats_cmd = []
        # init data directory and file
        self.set_data_directory(data_dir)
        self.set_tess_filename(tess_file)
        # init tesselation parameters
        self.set_seed(seed)
        self.set_ngrains(ngrains)
        self.set_dim(dim)
        self.set_domain_size()
        # set raster tesselation options
        self.set_grid_resolution()
        # init tesselation morphology options
        self.set_periodicity(periodicity)
        self.morpho_options = {}
        self.set_morpho_opt(morpho_opts)
        self.neper_optim = {}

    def __repr__(self):
        """Provide string representation for class."""
        s = '\n Tesselation Builder \n'
        s += f"\t -- Building directory : {self.dir}\n"
        if self.tess_filename == '':
            name = 'auto'
        else:
            name = self.tess_filename
        s += f"\t -- Current tesselation filename : {name}\n"
        s += f"\t -- Seed number : {self.seed}\n"
        s += f"\t -- Tess dimension : {self.dim}\n"
        s += f"\t -- Domain size : {self.size}\n"
        if self.ngrains_cmd == 'from_morpho':
            s += "\t -- Number of grains : computed from morphology options"
            s += f" (requested {self.ngrains} grains)\n"
        else:
            s += f"\t -- Number of grains : {self.ngrains}\n"
        if self.periodic:
            s += "\t -- Periodic tesselation\n"
        if self.z_periodic:
            s += "\t -- Semi-periodic tesselation (along z-axis)\n"
        if len(self.morpho_options) > 0:
            s += "\t -- Tesselation morphology options:\n"
            for opt, value in self.morpho_options.items():
                s += f"\t\t * {opt} -> {value}\n"
        else:
            s += "\t -- No morphology options : Voronoi tesselation"
        return s

    def set_data_directory(self, data_dir=None):
        """Set the directory in which tesselation directories are built.


        Parameters
        ----------
        data_dir : str
            Path of directory

        """
        if data_dir:
            self.dir = Path(data_dir).absolute()
        else:
            self.dir = Path.cwd()

    def set_tess_filename(self, tess_file):
        """Set basename of generated tesselation files.


        Parameters
        ----------
        tess_file : str
            Basename of tesselation files. If None, automatically generates
            a filename.

        """
        self.tess_filename = tess_file
        self.tess_path = self.dir / self.tess_filename / self.tess_filename

    def set_dim(self, dim):
        """Set dimension of tesselation, default is 3."""
        self.dim = dim

    def set_ngrains(self, ngrains):
        """Set number of grains in the tesselation, use 0 for 'from_morpho'."""
        self.ngrains = ngrains
        if ngrains == 0:
            self.ngrains_cmd = 'from_morpho'
        else:
            self.ngrains_cmd = f'{ngrains}'

    def set_domain_size(self, size=1.):
        """Define size of tesselation domain.

        Parameters
        ----------
        size : float, optional
            Absolute size of the tesselation domain in each direction.
            The default is 1.

        """
        self.size = size


    def set_seed(self, seed, random=False):
        """Set random seed value for tesselation.


        Parameters
        ----------
        seed : int, optional
            Random seed value for the tesselation generator. Using the same
            seed allows to reproduce the same tesselation. The default is 1.
        random : bool, optional
            Generates a random seed between 1 and 100000. The default is False.

        """
        if random:
            seed = np.random.randint(1, 100000)
        self.seed = seed

    def set_periodicity(self, periodicity):
        """Set periodicity option for tesselation.


        Parameters
        ----------
        periodicity : str, optional
            Set periodicity option value. Possible choices are `none`, `full`
            (periodic tesselation in all dimensions), and `semi` (periodicity
             along z axis only). The default is 'none'.

        """
        if periodicity == 'none':
            self.periodic = False
            self.z_periodic = False
        elif periodicity == 'full':
            self.periodic = True
            self.z_periodic = False
        elif periodicity == 'semi':
            self.periodic = False
            self.z_periodic = True
        else:
            self.periodic = False
            self.z_periodic = False

    def set_grid_resolution(self, resolution=50.):
        """Set grid resolution for raster tesselation."""
        self.res = resolution

    def set_morpho_opt(self, morpho_opts):
        """Add options for the morphology cmd for Neper tesselations."""
        # init morphological options dictionary
        self.morpho_options.update(morpho_opts)

    def set_neper_optim(self, neper_optim={}):
        """Add neper optimization options for tesselation generation."""
        self.neper_opti.update(neper_optim)

    def set_grain_size_list(self, grain_sizes):
        """Set a list of grain sizes as input for 'diameq' morpho tess option.

        Parameters
        ----------
        grain_sizes : np.array(ngrains) floats
            Array containing the list of grains sizes to assign to tesselation
            cells.
        """
        self.grain_size_list = grain_sizes
        self.set_morpho_opt({"diameq":"file(FILENAME)"})

    def print_neper_tess_cmd(self, gen_filename=False, tesr=False,
                             load_tess=True):
        """Print the command generated to run a neper tesselation.


        Parameters
        ----------
        gen_filename : bool, optional
            Flag to automatically generate a file name. The default is False.
            If filename has not been initialized, the flag will be set to True.

        tesr : bool, optional
            Flag to print command for raster tesselation (VTK)

        load_tess : bool, optional
            Print raster tesselation command with loading of a .tess file if
            `True`. If `False`, print raster tesselation command with tess
            definition options.

        """
        # if filename not init, set gen filename flag to True
        if self.tess_filename == '':
            gen_filename = True
        # gen filename if requested
        if gen_filename:
            self.set_tess_filename(self._gen_tess_filename())
        # generate command line
        if tesr:
            self._gen_tesr_cmd(load_tess=load_tess)
            cmd_line = self.tesr_cmd
        else:
            self._gen_tess_cmd()
            cmd_line = self.tess_cmd
        # write command
        cmd = ''
        for opt in cmd_line:
            cmd += opt + ' '
        print(f"\n-- Neper tesselation command : {cmd}")

    def print_neper_stats_cmd(self):
        """Print the command to compute grain statistics with Neper."""
        self._gen_tess_stats_cmd()
        cmd_line = self.stats_cmd
        # write command
        cmd = ''
        for opt in cmd_line:
            cmd += opt + ' '
        print(f"\n-- Neper statistics command : {cmd}")


    def create_tess(self, verbose=False, filename=None):
        """Generate Neper tesselation command and runs it."""
        # Ensure filename is appropriately set and create directory if needed
        self._gen_dir_and_filename(filename)
        # Write data files if required
        # TODO: reset and debug
        # self._gen_data_files()
        # generate tesselation cmd
        self._gen_tess_cmd()
        # run Neper with tesselation command
        out = subprocess.run(args=self.tess_cmd,
                             capture_output=True, text=True)
        # Handle success/failure of Neper and log output
        if out.returncode == 0:
            print("Sucessfull tesselation")
            if verbose:
                print(f'Neper standard output : \n{out.stdout}')
        else:
            print("ERROR : Something went wrong during Neper execution")
            print(f"Neper standard error output :\n {out.stderr}")

    def create_tesr(self, verbose=False, filename=None, load_tess=True):
        """Generate vtk tesselation from tess file or from options."""
        # Ensure filename is appropriately set and create directory if needed
        self._gen_dir_and_filename(filename)
        # generate tesselation cmd
        self._gen_tesr_cmd(load_tess)
        # run Neper with tesselation command
        out = subprocess.run(args=self.tesr_cmd,
                             capture_output=True, text=True)
        # Handle success/failure of Neper and log output
        if out.returncode == 0:
            print("Sucessfull raster tesselation")
            if verbose:
                print(f'Neper standard output : \n{out.stdout}')
        else:
            print("ERROR : Something went wrong during Neper execution")
            print(f"Neper standard error output :\n {out.stderr}")
            print(f"Neper standard output :\n {out.stdout}")

    def create_tesr_serie(self, list_res, verbose=False, load_tess=True,
                          output_basename="PolyX"):
        """Generate serie of vtk tesselation from list of resolutions."""
        for res in list_res:
            self.set_grid_resolution(res)
            self.create_tesr(verbose=verbose, filename=None,
                             load_tess=load_tess)

    def create_stats(self, verbose=False):
        """Read a Neper tess and compute grain statistics."""
        # generate tesselation cmd
        self._gen_tess_stats_cmd()
        # run Neper to compute statistics
        out = subprocess.run(args=self.stats_cmd,
                             capture_output=True, text=True)
        # Handle success/failure of Neper and log output
        if out.returncode == 0:
            print("Sucessfull computation of statistics")
            if verbose:
                print(f'Neper standard output : \n{out.stdout}')
        else:
            print("ERROR : Something went wrong during Neper execution")
            print(f"Neper standard error output :\n {out.stderr}")
            print(f"Neper standard output :\n {out.stdout}")

# Private methods
    def _gen_data_files(self):
        """Writes data files that serve as input for tesselation options."""
        # write grain size data file
        if self.morpho_options.get('diameq').startswith('file'):
            # write data file
            datafile_name = self.tess_path.with_name("grain_diameq.dat")
            with open(datafile_name,'w') as f:
                for size in self.grain_size_list:
                    f.write(f"{size}")
            self.morpho_options['diameq'] = f"file({datafile_name})"
        # End of writing data files
        return

    def _gen_tess_cmd(self):
        """Generate shell command to run tesselation with options."""
        self.tess_cmd = []
        self._init_tess_cmd(self.tess_cmd)
        # add standard options: tess module, tess id, ngrains, dimension,
        #                       periodicity
        self._append_std_options(self.tess_cmd)
        # update domain size and write option if needed
        self._update_domain_size()
        if self.size != 1.0:
            self._gen_domain_size_cmd(self.tess_cmd)
        # write tesselation morphology options
        if len(self.morpho_options) > 0:
            self._gen_morpho_cmd(self.tess_cmd)
        # write tesselation optimisation options
        if len(self.neper_optim) > 0:
            self._gen_optim_cmd(self.tess_cmd)
        # End with filename
        self.tess_cmd.append('-o')
        self.tess_cmd.append(f'{self.tess_path}')

    def _gen_tesr_cmd(self, load_tess=True):
        """Genenerate Neper cmd for vtk tesr from tess file or options."""
        self.tesr_cmd = []
        self._init_tess_cmd(self.tesr_cmd)
        if load_tess:
            # load tesselation from .tess file
            self._load_tess_cmd(self.tesr_cmd)
        else:
            # add standard options: tess module, tess id, ngrains, dimension
            self._append_std_options(self.tesr_cmd)
            if self.ngrains == 'from_morpho':
                self._gen_domain_size_cmd(self.tesr_cmd)
            # write tesselation morphology options
            if len(self.morpho_options) > 0:
                self._gen_morpho_cmd(self.tesr_cmd)
            # write tesselation optimisation options
            if len(self.neper_optim) > 0:
                self._gen_optim_cmd(self.tesr_cmd)
        # Set raster tesselation parameters
        self._append_tesr_format_opt(self.tesr_cmd)
        # End with filename
        self.tesr_cmd.append('-o')
        tesr_name = self.tess_filename + f'_R{self.res}'
        tess_path = self.tess_path.parent / tesr_name
        self.tesr_cmd.append(f'{tess_path}')

    def _gen_tess_stats_cmd(self):
        """Genenerate stats from a Neper tess file."""
        self.stats_cmd = []
        self._init_tess_cmd(self.stats_cmd)
        # load tesselation from .tess file
        self._load_tess_cmd(self.stats_cmd)
        # cmds to output statistics
        self._append_grain_stats(self.stats_cmd)

    def _gen_tess_filename(self, basename="PolyX", tesr=False):
        """Generate tesselation filename from tess parameters."""
        # update domain size and write option if needed
        self._update_domain_size()
        # write name of file
        name = f"{basename}_id{self.seed}_{self.dim}d"
        if self.ngrains_cmd != "from_morpho":
            name += f'_N{self.ngrains}'
        if self.size != 1.0:
            size_str = f'{self.size:5.2f}'.replace('.','p')
            name += f'_S{size_str}'
        if 'diameq' in self.morpho_options:
            deq = self.morpho_options['diameq']
            diameq_str = f'{deq:4.2f}'.replace('.','p')
            name += f'_Dg{diameq_str}'
        if self.periodic == True:
            name += '_Fpr'
        if self.z_periodic == True:
            name += '_Zpr'
        return name

    def _gen_periodicity_cmd(self, cmd):
        """Include periodicity option if needed for the tesselation cmd."""
        if self.periodic == True:
            cmd.append("-periodicity")
            cmd.append("1")
        if self.z_periodic == True:
            cmd.append("-periodicity")
            if self.dim == 2:
                cmd.append("y")
            else:
                cmd.append("z")

    def _gen_morpho_cmd(self, cmd):
        """Include requested morphology options for the tesselation cmd."""
        # add morphology cmd line option
        cmd.append('-morpho')
        # create string for all other options
        morph_opts = ''
        # activate grain growth option
        if self.morpho_options.get('graingrowth'):
            if 'diameq' in self.morpho_options:
                deq = self.morpho_options['diameq']
                cmd.append(f'gg({deq})')
            else:
                cmd.append('gg')
            # graingrowth must be provided alone
            return
        if self.morpho_options.get('diameq'):
            morph_opts += f'diameq:{self.morpho_options.get("diameq")}'
        # add options to cmd line
        cmd.append(morph_opts)

    def _gen_optim_cmd(self, cmd):
        """Include requested optimisation options for the tesselation cmd."""
        # stop criteria
        if 'crit' in self.neper_optim:
            crit = self.neper_optim.get('crit')
            cmd.append('-morphooptistop')
            cmd.append(f'eps={crit}')

    def _gen_domain_size_cmd(self, cmd):
        """Include requested domain size option for the tesselation cmd."""
        # add domain cmd line option
        cmd.append('-domain')
        if self.dim == 2:
            cmd.append(f'square({self.size},{self.size})')
        elif self.dim == 3:
            cmd.append(f'cube({self.size},{self.size},{self.size})')
        else:
            raise ValueError('Dimension should be 2 or 3 to set domain size.')

    def _gen_dir_and_filename(self, filename):
        # Ensure filename is appropriately set
        if filename:
            self.set_tess_filename(filename)
        if self.tess_filename == '':
            self.set_tess_filename(self._gen_tess_filename())
        # create tesselation directory if required
        tess_dir = self.dir / self.tess_filename
        if not tess_dir.exists():
            os.mkdir(tess_dir)

    def _append_std_options(self, cmd):
        """Include id, dim, ngrains and tesselation option to cmd line."""
        # add tesselation random seed
        cmd.append('-id')
        cmd.append(f'{self.seed}')
        # write tesselation number of grains
        cmd.append('-n')
        cmd.append(f'{self.ngrains_cmd}')
        # write tesselation dimension
        cmd.append('-dim')
        cmd.append(f'{self.dim}')
        # write tesselation periodicity
        self._gen_periodicity_cmd(cmd)

    def _append_tesr_format_opt(self, cmd):
        """Append vtk format and resolution to Neper -T cmd line."""
        # raster format : only VTK for now
        cmd.append('-format')
        cmd.append('vtk')
        # grid resolution
        cmd.append('-tesrsize')
        cmd.append(f'{self.res}')

    def _append_grain_stats(self, cmd):
        """Append to command line arguments to get stats of grains."""
        if self.dim == 2:
            cmd.append('-statface')
            # options
            cmd.append('id,coo,w,area,diameq')
        else:
            cmd.append('-statcell')
            # options
            cmd.append('id,coo,w,vol,diameq')

    def _update_domain_size(self):
        """If needed, compute the appropriate domain size."""
        diameq = self.morpho_options.get('diameq')
        # case 1 :  from morpho requested
        if (self.ngrains == 0) and (diameq is None):
            raise ValueError('Cannot compute number of grains '
                             '"from_morpho": no "diameq" morphology option'
                             ' found.')
        elif (self.ngrains > 0) and (diameq is not None):
            # ask for a number of grains, and a grain size
            #  => must compute domain size to comply with both
            domain_size = diameq*(self.ngrains**(1/self.dim))
            self.set_domain_size(domain_size)
            # set from morpho option
            self.ngrains_cmd = 'from_morpho'
            print(f" -- WARNING: specific grain size {diameq} and grain number"
                  f" {self.ngrains} have been requested. Domain size will be "
                  "overwritten to comply with both. New domain is a "
                  f" cube/square of size {domain_size}.")
        return

    def _init_tess_cmd(self, cmd):
        """Initialize Neper tess module command."""
        # reset tesselation cmd
        cmd.append('neper')
        # add tesselation module call
        cmd.append("-T")

    def _load_tess_cmd(self, cmd):
        """Initialize Neper tess module command."""
        # reset tesselation cmd
        cmd.append("-loadtess")
        tess_path = self.dir / self.tess_filename / self.tess_filename
        cmd.append(f"{tess_path.with_suffix('.tess')}")


class TessGenerator:
    # TODO: repr method
    def __init__(self):
        """Class to generate geometry data for Neper tesselation input."""
        self.seeds = None
        self.weights = None
        self.dim = 3
        self.set_domain_size(1.)

    def set_domain_size(self, size):
        """Set extent of domain in the two or three directions."""
        if np.asarray(size).ndim == 0:
            self.domain_size = np.repeat(size, self.dim)
        else:
            if len(size) != self.dim:
                raise ValueError('Size must be a scalar or an array of size '
                                 f'equal to domain dim = {self.dim}')
            self.domain_size = size

    def scale_seeds_with_domain(self):
        """Homotethic transformation to scale seeds coords with domain."""
        # compute ratio of sizes between target and current
        Ratio_X = self.domain_size[0] / max(self.seeds[:,0])
        Ratio_Y = self.domain_size[1] / max(self.seeds[:,1])
        if self.dim == 3:
            Ratio_Z = self.domain_size[2] / max(self.seeds[:,2])
        # Scale seeds
        self.seeds[:,0] = Ratio_X*self.seeds[:,0]
        self.seeds[:,1] = Ratio_Y*self.seeds[:,1]
        if self.dim == 3:
            self.seeds[:,2] = Ratio_Z*self.seeds[:,2]

    def generate_random_seed(self, nseeds, dim=3):
        """Generate random positions for nseeds in 2D or 3D."""
        self.seeds = np.random.rand(nseeds, dim)
        self.dim = dim

    def generate_equispaced_seeds(self, seeds_per_dim, dim=3):
        """Generate random positions for nseeds in 2D or 3D."""
        # generate 1D corner grids
        corners = np.linspace(0., 1., seeds_per_dim + 1)
        # compute 1D centers grid
        centers = 0.5*(corners[1:] + corners[:-1])
        # compute complete grid and reshape coordinates
        if dim == 2:
            Cgrid = np.meshgrid(centers,centers)
            coords = np.array([Cgrid[0].reshape(seeds_per_dim**dim),
                               Cgrid[1].reshape(seeds_per_dim**dim)])
        else:
            Cgrid = np.meshgrid(centers,centers, centers)
            coords = np.array([[Cgrid[0].reshape(seeds_per_dim**dim)],
                               [Cgrid[1].reshape(seeds_per_dim**dim)],
                               [Cgrid[2].reshape(seeds_per_dim**dim)]])
        # set seeds
        self.seeds = coords.transpose()
















