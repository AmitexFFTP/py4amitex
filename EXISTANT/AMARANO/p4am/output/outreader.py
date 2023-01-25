#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
P4A Module to read AMITEX simulation results.

@author: amarano
"""

## External Imports
import numpy as np
import vtk
from pathlib import Path
import re
import os

# P4A Imports
from p4am.output.outdatatypes import StdDataTypes, StdIndexing

# Reader class for Amitex fft simulation results
class OutReader:
    """Reader class for Amitex_fftp simulation outputs."""
    
    def __init__(self, output_file=None):
        """Ouput reader class constructor."""
        # get pathes for standard output files
        self.finite_strain = False
        self.set_output_file(output_file)
        
    def set_output_file(self, output_file=None):
        """Set name of output files to read and update finite strain flag.

        Parameters
        ----------
        output_file : str or PosixPath, optional
            basename of the .std file of the output to read (with or without 
            extension). The default is None.

        """
        # update output flag 
        if output_file == None:
            self.std_file = None
            self.mstd_file = None
        else:
            p = Path(output_file).absolute()
            self.std_file = p.with_suffix('.std')
            self.mstd_file = p.with_suffix('.mstd')
            self._find_zstd_files()
        # update finite strain flag
        self.update_finite_strain()
        
    def set_finite_strain(self, finite_strain):
        """Set value of finite strain flag for the reader.
        
        Parameters
        ----------
        finite_strain : bool
            True for finite strain reader, False for small strain reader.
        """
        self.finite_strain = finite_strain
            
    def update_finite_strain(self):
        """Set finite strain flag in accordance with set output file."""
        if self.std_file is not None:
            if self._get_finite_strain_hyp(self.std_file):
                self.finite_strain = True
    
    def read_std(self, variables='all'):
        """Read std file content and returns it as numpy structured array.

        Parameters
        ----------
        variables : lst(str), optional
            List of variables to load from .std file.
            Example : ['time','sigma','epsilon', 'varInt_1']
            The default is 'all' : all .std data is loaded.

        Returns
        -------
        data : numpy structured array
            numeric data for the requested variables (=columns) loaded from
            the .std file
        """
        # read data
        data = self.read_std_data(str(self.std_file))
        # return only requested output
        if not(variables == 'all'):
            data = data[variables]
        return data
    
    def read_mstd(self, variables='all'):
        """Read mstd file content and returns one numpy array per material.

        Parameters
        ----------
        variables : lst(str), optional
            List of variables to load from .std file.
            Example : ['time','sigma','epsilon', 'varInt_1']
            The default is 'all' : all .std data is loaded.

        Returns
        -------
        mat_data : dict(numpy structured array)
            dictionary with one key per materialId. The values are the
            numeric data for the requested variables (=columns) for each
            material, loaded from the .mstd file
        """
        # read data
        data = self.read_std_data(str(self.mstd_file))
        # return only requested output
        if not(variables == 'all'):
            data = data[variables]
        # create a dict to provide results stored per material
        nmat = self._get_std_n_regions(str(self.mstd_file))
        mat_data = dict.fromkeys([matId+1 for matId in range(nmat)])
        for i in range(nmat):
            mat_data[i] = data[(0+i):len(data):nmat]
        return mat_data
    
    def read_zstd(self, zstd_file, variables='all'):
        """Read one zstd file content and returns it as numpy structured array.

        Parameters
        ----------
        variables : lst(str), optional
            List of variables to load from .std file.
            Example : ['time','sigma','epsilon', 'varInt_1']
            The default is 'all' : all .std data is loaded.

        Returns
        -------
        zone_data : dict(numpy structured array)
            dictionary with one key per materialId. The values are the
            numeric data for the requested variables (=columns) for each
            zone, loaded from the .zstd file
        """
        if isinstance(zstd_file, Path):
            zstd = str(zstd_file)
        else:
            zstd = zstd_file
        # get dict of varInt column indices in output
        varInt = self._get_std_varInt_indices(zstd)
        # read data
        data = self.read_std_data(zstd, varInt)
        # return only requested output
        if not(variables == 'all'):
            data = data[variables]
        # create a dict to provide results stored per material
        nzones = self._get_std_n_regions(zstd_file)
        zone_data = dict.fromkeys([i+1 for i in range(nzones)])
        for i in range(nzones):
            zone_data[i] = data[(0+i):len(data):nzones]
        return zone_data
    
    def read_vtk_strain(self, output_slice=None):
        """Read all the vtk strain fields in reader std file directory."""
        # TODO : complete docstring
        # TODO : do it for stress fields, and for internal variables
        # find list of vtk strain field files
        eps_files, eps_incr = self._find_vtk_strain_files()
        # pattern to read strain component and time increment from file name
        comp_pattern = re.compile('def\d')
        incr_pattern = re.compile('\d+.vtk')
        # Initialize strain dict
        Strain_dict = dict().fromkeys(eps_incr)
        # Fill strain dict with output, component per component
        for file in eps_files:
            eps_tmp = OutReader.read_vtk_legacy(file, output_slice)
            incr = int(incr_pattern.findall(file)[0].strip('.vtk'))
            comp_list = comp_pattern.findall(file)
            if len(comp_list) == 0:
                # all components are within the same vtk file
                Strain_dict[incr] = eps_tmp
            elif len(comp_list) == 1:
                # Component is in a specific vtk file
                component = int(comp_list[0].strip('def')) - 1
                Strain_dict[incr][...,component] = eps_tmp
            else:
                raise ValueError(f' Vtk file {file} name has an invalid'
                                 ' component value (must be one digit).')
        return Strain_dict
    
    @staticmethod
    def read_vtk_legacy(vtk_path, output_slice=None):
        """Read one Amitex_fftp vtk output and return the fields stored in it.
        
        Parameters
        ----------
        vtk_path : string
            name/path of the .vtk file..
        output_slice : numpy array (3,2), optional
            Specific slice of field data to return. The default is None.

        Returns
        -------
        data : dict( 'field_name':np.double array)
            Return a dict. of the amitex_fftp output fields stored in the vtk.
            file, whose keys are the field names..
        spacing : array(3) floats
            Voxel size along each direction of the field discretization

        """
        # local imports
        from vtk.util import numpy_support
        # set file path
        p = Path(vtk_path).absolute().with_suffix('.vtk')
        # init vtk reader
        reader = vtk.vtkGenericDataObjectReader()
        reader.SetFileName(str(p))
        reader.Update()
        # read raw data
        Array = reader.GetOutput().GetCellData().GetArray(0)
        spacing = reader.GetOutput().GetSpacing()
        dim = reader.GetOutput().GetDimensions()
        output_shape = tuple([i-1 for i in dim])
        data = numpy_support.vtk_to_numpy(Array)
        data = data.reshape(output_shape, order='F')
        # get usefull slice
        if output_slice is not None:
            data = data[output_slice[0,0]:output_slice[0,1],
                        output_slice[1,0]:output_slice[1,1],
                            output_slice[2,0]:output_slice[2,1],...]
        return data, spacing

    @staticmethod    
    def read_std_data(file, varInt={}):
        """Read full content of a .m/z/std file for small strain outputs."""
        # Get strain hypothesis : usefull is called as static method
        finite_strain = OutReader._get_finite_strain_hyp(file)
        # get proper structured array type including internal variables for 
        # small strain
        if finite_strain:
            dtype_description = StdDataTypes.std_fs_dtype.descr
        else:
            dtype_description = StdDataTypes.std_hpp_dtype.descr
        std_dtype = np.dtype(dtype_description)
        for key in varInt: 
            dtype_description.append((key, np.double, (1,)))
            
        # read txt content of .std or .mstd or .zstd file and fill data
        std_lines = []
        with open(file,'r') as f:
            l = f.readline()
            while l:
                if not l.startswith('#'):
                    ldata = np.array(l.split()).astype(np.double)
                    std_lines.append(ldata)
                l = f.readline()
                
        std_lines = np.array(std_lines)
        # fill standard data in structured array
        dt = np.dtype(dtype_description)
        n_rows = len(std_lines)
        data = np.empty(shape=(n_rows,), dtype=dt)
        # load std variables
        for name in std_dtype.names:
            indices = StdIndexing.get_std_indices(variable=name, 
                                                  finite_strain=finite_strain)
            data[:][name] = std_lines[:,indices] 
        # load internal variables
        for var in varInt:
            index = [varInt[var]]
            data[:][var] = std_lines[:,index] 
        return data

#===========================================================================        
# Private methods
#=========================================================================== 
    def _find_zstd_files(self):
        import glob
        p = self.std_file.parent / '*.zstd'
        files = glob.glob(str(p))
        self.zstd = []
        print(files)
        for F in files:
            self.zstd.append(Path(F).absolute())
            
    def _find_vtk_strain_files(self):
        # Get all names of vtk files in the directory and associeted increments
        pattern = re.compile(self.std_file.stem+'_def\d?_\d+.vtk')
        incr_pattern = re.compile('\d+.vtk')
        eps_files = []
        eps_incr = []
        for filepath in os.listdir(self.std_file.parent):
            if pattern.match(filepath):
                fileP = self.std_file.parent / filepath
                eps_files.append(str(fileP))
                incr = int(incr_pattern.findall(filepath)[0].strip('.vtk'))
                if incr is None:
                    raise ValueError('At least one Amitex_fftp .vtk file in '
                                     'the directory has no increment number in'
                                     ' its name.')
                eps_incr.append(incr)
        eps_incr = np.unique(np.array(eps_incr))
        return eps_files, eps_incr

    @staticmethod
    def _get_finite_strain_hyp(std_file):
        """Find out small strain or finite strain format of std file."""
        with open(std_file,'r') as f:
            l = f.readline()
            while l.startswith('#'):
                if 'xx,yy,zz,xy,xz,yz,yx,zx,zy' in l:
                    return True
                l = f.readline()
        return False
    
    @staticmethod
    def _get_std_varInt_indices(std_file):
        """Find out small strain or finite strain format of std file."""
        # get pattern to find out which internal variables values are present
        # (for .zstd only)
        pattern = re.compile('variable interne \d+')
        idx_pattern = re.compile('\d+e')
        varInt = dict()
        with open(std_file,'r') as f:
            l = f.readline()
            while l.startswith('#'):
                if 'variable interne' in l:
                    suffix = ''
                    if 'ecart type' in l:
                        suffix = '_std'
                    varInt_number = int(pattern.findall(l)[0].split()[2])
                    varInt_index = int(idx_pattern.findall(l)[0][:-1])
                    name = f'varInt_{varInt_number}'+suffix
                    varInt[name] = varInt_index - 1
                l = f.readline()
        return varInt
    
    @staticmethod
    def _get_std_n_regions(std_file):
        """Found out number of materials or zones in m/z/std files."""
        with open(std_file,'r') as f:
            # skip header
            l = f.readline()
            while l.startswith('#'):
                l = f.readline()
            # count N first lines with same time value
            time0 = np.array(l.split()).astype(np.double)[0]
            time = time0
            Nregions = 0
            while time == time0:
                Nregions +=1
                l = f.readline()
                time = np.array(l.split()).astype(np.double)[0]
        return Nregions