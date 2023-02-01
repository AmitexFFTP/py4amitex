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
        self.set_output(output_file)

    def print_available_data(self):
        """Print available AMITEX results in output directory."""
        print(f'AMITEX_FFTP outputs available in {self.std_file.parent}'
              f' with name {self.std_file.stem}')
        # check and print std data
        print("\n-- Standard output file : ")
        if not(self.std_file is None):
            print(f"\t\t{self.std_file.name}")
        else:
            print('\t\tNot found')
        # check and print mstd data
        print("\n-- Per material output file : ")
        if not(self.mstd_file is None):
            print(f"\t\t{self.mstd_file.name}  -- {self.mstd_nmat} materials")
        else:
            print('\t\tNot found')
        # check and print zstd data
        print("\n-- Per zone output files : ")
        if len(self.zstd_files) > 0:
            for matId in self.zstd_files:
                print("\t\t", end='')
                print(f"material {matId} : ", end='')
                print(f"{self.zstd_files[matId]['path'].name} ", end='')
                print(f" -- {self.zstd_files[matId]['nZones']} zones")
        else:
            print('\t\tNot found')
        # check and print vtk data
        print("\n-- Fields output -> vtk files : ", end='')
        if not(self.vtk_files is None):
            for incr in self.vtk_files:
                print("\n\t *", end='')
                print(f"Increment {incr} : ", end='')
                for k,v in self.vtk_files[incr].items():
                    if k in ['stress', 'strain', 'piola']:
                        print(f"\n\t\t - {k} : ", end='')
                        for c in v.keys():
                            print(f" {c}", end='')
                    if k == 'varInt':
                        for matId in v.keys():
                            print(f"\n\t\t - {k} material {matId} : ", end='')
                            for c in v[matId].keys():
                                print(f" {c}", end='')
        else:
            print('\t\tNot found')

    def set_output(self, output_basename):
        """Set names of output files from basename, update finite strain flag.

        Parameters
        ----------
        output_basename : str, optional
            basename of the output files to read (with or without extension).

        """
        self._reset_output_files()
        # set standard output file
        self._set_std(output_basename)
        # set per material output file
        self._set_mstd(output_basename)
        # set per material output file
        self._set_zstd(output_basename)
        # set vtk output files fot fields
        self.vtk_files = {}
        self._set_vtk(self.std_file, 'stress')
        self._set_vtk(self.std_file, 'piola')
        self._set_vtk(self.std_file, 'strain')
        self._set_vtk(self.std_file, 'varInt')
        # update finite strain flag
        self.update_finite_strain()

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

    def read_vtk_fields(self, field_type=None, components_list='all',
                        increments_list=None, matId=None,
                        output_slice=None):
        """Read requested field data from vtk files.

        Parameters
        ----------
        field_type : string
            Type of field to read among ['stress', 'strain', 'piola',
            'varInt']. The default is None
        components_list : list(str), optional
            List of components of the field to read. Components can be indices
            or letters (like '0' or 'xx'). The default is 'all'.
        increments_list : list(str), optional
            List of time increments to read. The default is None, which reads
            all time increment data for the requested field/components.
        matId : int, optional
           When reading internal variable data, provide Id number of the
           associated material. The default is None, which attempts to read the
           varint from the first material in available vtk varInt data.
        output_slice : numpy array (3,2), optional
           Specific slice of field data to return. The default is None.

        Returns
        -------
        Output_fields : dict()
            Dictionnary with one entry per time increment, containing the
            requested field data. Items with all components of a stress
            or strain field contain a (Nx, Ny, Nz, 6 or 9) array. Items with
            internal variable fields or a subset of stress/strain components
            contain a dict of (Nx,Ny,Nz) arrays, with component names as keys.

        """
        # get list of time increments to load
        vtk_incr = list(self.vtk_files.keys()) # available time increments
        if increments_list is not None:
            # find requested time increments in available data
            if not all([v in vtk_incr for v in increments_list]):
                wrong_I = [v for v in increments_list if v not in vtk_incr]
                # warning for incorrect requested increments
                # TODO : logger ?
                msg = (f"The requested time increments {wrong_I} are not"
                        " available in vtk files for the currently set output")
                print(f"-- WARNING : {msg}")
            I = [v for v in increments_list if v in vtk_incr]
        else:
            I = list(self.vtk_files.keys())
        # get material Id if loading varint
        if field_type == 'varInt':
            if matId is not None:
                mId = f"M{matId}"
            else:
                # no required material, attempting to get varInt from first
                # material in self.vtk_files[i]['varInt']
                mId = list(self.vtk_files[I[0]]['varInt'].keys())[0]
                msg = ("-- WARNING : Undefined Material Id. "
                       f"Attempting to load varInt from material {mId}")
                print(msg)
            # Tests required material --> check if available.
            if mId not in self.vtk_files[I[0]]['varInt']:
                av_id = [s.strip('M') for s in
                         list(self.vtk_files[I[0]]['varInt'].keys())]
                msg = ("Impossible to load internal variable field from"
                       " vtk file : Incorrect Material Id ({mId})).\t"
                       f"Available matId are {av_id}")
                raise ValueError(msg)
        # check for component list to read
        if field_type in ['stress', 'strain', 'piola']:
            if components_list != 'all':
                ok_comp = StdIndexing._tensor_indexes
                comp_list = [k for k,v in ok_comp.items()
                             if (k in components_list or
                                 v in components_list)]
                comp_ignored = [k for k in components_list
                                if not (k in ok_comp.keys() or
                                        k in ok_comp.values())]
            else:
               comp_list = list(self.vtk_files[I[0]][field_type])
        else:
            d = self.vtk_files[I[0]]['varInt'][mId]
            if components_list != 'all':
                comp_list = [int(c) for c in components_list if c in d.keys()]
                comp_ignored = [int(c) for c in components_list if
                                c not in d.keys()]
            else:
                comp_list = list(d.keys())
        # check for request of non available components
        if len(comp_ignored) > 0:
            if len(comp_list) == 0:
                c_read = 'nothing'
            else:
                c_read = str(comp_list)
            msg = (f"-- WARNING : components {comp_ignored}"
                   f" not available --> reading {c_read}")
            print(msg)
        # init output_fields dict
        Output_fields = dict.fromkeys(I)
        # fill output time increment per time increment
        for i in Output_fields:
            file_dict = self.vtk_files[i][field_type] # dict of file names
            # stress/strain and varInt fields handled seperately
            if field_type in ['stress', 'strain', 'piola']:
                # stress/strain fields case
                # get dimensions of data from vtk file or requested slice
                if output_slice is not None:
                    dim = np.array([output_slice[0,1] - output_slice[0,0],
                                    output_slice[1,1] - output_slice[1,0],
                                    output_slice[2,1] - output_slice[2,0]])
                else:
                    comp0 = list(file_dict.keys())[0]
                    dim, _ = list(OutReader.read_vtk_dim(file_dict[comp0]))
                # Get number of components to read
                # TODO : handle case of all components in same file
                if components_list != 'all':
                    Output_fields[i] = {}
                    for k in comp_list:
                        v = file_dict[k]
                        Output_fields[i][k] = OutReader.read_vtk_legacy(v,
                                                                output_slice)
                else:
                    # init array to load results
                    Output_fields[i] = np.zeros(shape=(*dim, len(comp_list)))
                    for k, v in file_dict.items():
                        id = StdIndexing._tensor_indexes[k]
                        Output_fields[i][...,id] = OutReader.read_vtk_legacy(v,
                                                                 output_slice)
            else:
                # case of internal variables
                # get dimensions of data from vtk file or requested slice
                # TODO : factor in private method
                if output_slice is not None:
                    dim = np.array([output_slice[0,1] - output_slice[0,0],
                                    output_slice[1,1] - output_slice[1,0],
                                    output_slice[2,1] - output_slice[2,0]])
                else:
                    comp0 = list(file_dict[mId].keys())[0]
                    dim,_ = list(OutReader.read_vtk_dim(file_dict[mId][comp0]))
                # get varInt one by one
                Output_fields[i] = {}
                for k in comp_list:
                    v = file_dict[mId][k]
                    Output_fields[i][k] = OutReader.read_vtk_legacy(v,
                                                            output_slice)
        return Output_fields

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
        dim = reader.GetOutput().GetDimensions()
        output_shape = tuple([i-1 for i in dim])
        data = numpy_support.vtk_to_numpy(Array)
        data = data.reshape(output_shape, order='F')
        # get usefull slice
        if output_slice is not None:
            data = data[output_slice[0,0]:output_slice[0,1],
                        output_slice[1,0]:output_slice[1,1],
                            output_slice[2,0]:output_slice[2,1],...]
        return data

    @staticmethod
    def read_vtk_dim(vtk_path):
        """Return grid dimensions and spacing from one vtk output file.

        Parameters
        ----------
        vtk_path : string
            name/path of the .vtk file..

        Returns
        -------
        dimensions : numpy.array([Nx, Ny, Nz])
        spacing : numpy.array([dx, dy, dz])

        """
        # set file path
        p = Path(vtk_path).absolute().with_suffix('.vtk')
        # init vtk reader
        reader = vtk.vtkGenericDataObjectReader()
        reader.SetFileName(str(p))
        reader.Update()
        # get corner grid dimension
        dimTmp = reader.GetOutput().GetDimensions()
        # substract one to each dimension to get center grid dimension
        dimensions = (d-1 for d in dimTmp)
        spacing = reader.GetOutput().GetSpacing()
        return dimensions, spacing

    @staticmethod
    def read_std_data(file, varInt={}):
        """Read full content of a .m/z/std file for small strain outputs."""
        # TODO : read first to count lines, read twice to load data
        # TODO : in case of empty file --> dedicated error message
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
    @staticmethod
    def _find_zstd_files(basename):
        import glob
        p1 = Path(basename).absolute()
        pattern = p1.parent / f'{p1.stem}*.zstd'
        files = glob.glob(str(pattern))
        zstd_list = []
        for F in files:
            zstd_list.append(Path(F).absolute())
        return zstd_list

    @staticmethod
    def _find_vtk(std_file, field_type='stress'):
        """Find all vtk files in output dir for given type of field.

        'type' can be 'stress', 'strain' or 'varInt'
        """
        # Create pattern to find requested vtk output files
        if field_type == 'stress':
            field = 'sig'
            pat = std_file.stem + '_sig\d?_\d+.vtk'
            comp_pattern = re.compile('sig\d')
        elif field_type == 'piola':
            field = 'pi'
            pat = std_file.stem + '_pi\d?_\d+.vtk'
            comp_pattern = re.compile('pi\d')
        elif field_type == 'strain':
            field = 'def'
            pat = std_file.stem + '_def\d?_\d+.vtk'
            comp_pattern = re.compile('def\d')
        elif field_type == 'varInt':
            field = 'varInt'
            pat = std_file.stem + '_M\d_varInt\d+_\d+.vtk'
            comp_pattern = re.compile('varInt\d')
        pattern = re.compile(pat)
        increment_pattern = re.compile('\d+.vtk')
        material_pattern = re.compile('_M\d+')
        files = {}
        increments = []
        components = []
        for filepath in os.listdir(std_file.parent):
            if pattern.match(filepath):
                # create file dict
                files[filepath] = {}
                # add file
                fileP = std_file.parent / filepath
                files[filepath]['path'] = fileP
                # find time increment associated to vtk file
                tmp = increment_pattern.findall(filepath)
                incr = int(tmp[0].strip('.vtk'))
                if incr is None:
                    raise ValueError('At least one Amitex_fftp .vtk file in '
                                     'the directory has no increment number in'
                                     ' its name.')
                # add increment to list of vtk output increments and to fict
                increments.append(incr)
                files[filepath]['increment'] = incr
                # find compomenent associated to vtk file
                tmp = comp_pattern.findall(filepath)
                if len(tmp) == 0:
                    # all components are within the same vtk file
                    comp = 'all'
                elif len(tmp) == 1:
                    # Component is in a specific vtk file
                    comp = int(tmp[0].strip(f'{field}'))
                files[filepath]['component'] = comp
                components.append(comp)
                # find associated material for varInt vtk outputs
                tmp = material_pattern.findall(filepath)
                if not len(tmp) == 0:
                    mat = int(tmp[0].strip('_M'))
                    files[filepath]['matId'] = mat
                # add type of field
                files[filepath]['type'] = field
        increments = np.unique(np.array(increments))
        components = np.unique(np.array(components))
        return files, increments, components

    def _reset_output_files(self):
        """Reset to None all class attributes containing output file pathes."""
        # Standard output
        self.std_file = None
        # Per Material standard output
        self.mstd_file = None
        # Per zone standard output
        self.zstd_files = None

    def _set_std(self, basename):
        """Set standard output file for Reader."""
        # check existence and sets path of standard output file
        p = Path(basename).absolute().with_suffix('.std')
        if not(p.exists()):
            raise FileExistsError(f"File {p} not found")
        self.std_file = p

    def _set_mstd(self, basename):
        """Set per material output file for Reader.

        Also return number of materials in .mstd file output
        """
        # check existence and sets path of per material output file
        p = Path(basename).absolute().with_suffix('.mstd')
        if not(p.exists()):
            print(f"-- WARNING : File {p} not found")
            self.mstd_file = None
            return
        self.mstd_file = p
        # count number of materials
        self.mstd_nmat = self._get_std_n_regions(str(self.mstd_file))

    def _set_zstd(self, basename):
        """Set per zone output file for Reader.

        Also return number of zones in .zstd file output
        """
        # get zstd files in output directory
        zstd_list = OutReader._find_zstd_files(basename)
        # create a tree of zstd file info
        self.zstd_files = {}
        for file in zstd_list:
            matID = int(file.stem.split('_')[-1])
            nZones = self._get_std_n_regions(str(file))
            self.zstd_files[matID] = {'path':file, 'nZones':nZones}

    def _set_vtk(self, basename, field_type):
        """Set all vtk output files for Reader."""
        # Search vtk files, time increments and components
        files, incr, comp = OutReader._find_vtk(basename, field_type)
        if len(files) > 0:
            for f in files:
                i = files[f]['increment']
                # create dic for increment if needed
                if i not in self.vtk_files:
                    self.vtk_files[i] = {}
                if field_type not in self.vtk_files[i]:
                    self.vtk_files[i][field_type]= {}
                # if stress or strain -> get tensor like component key
                if field_type in ['stress','strain','piola']:
                    if len(comp) == 6:
                        for k, v in StdIndexing._sym_tensor_indexes.items():
                            if v == (files[f]['component']-1):
                                c = k
                    elif len(comp) == 9:
                        for k, v in StdIndexing._tensor_indexes.items():
                            if v == (files[f]['component']-1):
                                c = k
                    self.vtk_files[i][field_type][c] = files[f]['path']
                elif field_type == 'varInt':
                    mat = f"M{files[f]['matId']}"
                    if mat not in self.vtk_files[i]:
                        self.vtk_files[i][field_type][mat] = {}
                    c = files[f]['component']
                    p = files[f]['path']
                    self.vtk_files[i][field_type][mat][c] = p

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