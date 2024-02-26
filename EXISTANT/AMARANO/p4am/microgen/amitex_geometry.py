#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
P4A Module to handle Amitex zoneId and matId fields, and the vtk format.

TODO:
    -- complete docstrings
    -- repr method

@author: amarano
"""

from pathlib import Path

import numpy as np
import vtk

class AGeom:
    """Amitex geometry class."""

    def __init__(self, matId=None, zoneId=None, mat_file=None, zone_file=None,
                       filename='amitex_geometry'):
        """Amitex geometry class constructor.

        Parameters
        ----------
        matId : numpy.array(int), optional
            Material Id field to use as input for Amitex. The default is None.
        zoneId : numpy.array(int), optional
            Zone Id field to use as input for Amitex. The default is None.
        mat_file : str or path, optional
            Path of the vtk file to use as input material Id field for Amitex.
            Mutually exclusive with `matId`. The default is None.
        zone_file : str or path, optional
            Path of the vtk file to use as input zone Id field for Amitex.
            Mutually exclusive with `zoneId`. The default is None.
        filename : str or path, optional
            Basename of the .vtk file to generate with amitex geometry obejct.
            The default is 'amitex_geometry'.
        """
        # Cell dimensions
        self.reset_geom()
        if (mat_file is not None) or (matId is not None):
            self.set_matId(matId, from_file=mat_file)
        if (zone_file is not None) or (zoneId is not None):
            self.set_zoneId(zoneId, from_file=zone_file)
        # filename
        self.set_geometry_filename(filename)

    def copy(self, filename=None):
        """Return a copy of this geometry object."""
        # new geometry object
        NewGeom = AGeom(matId=self.matId, zoneId=self.zoneId)
        # set filename
        if filename is None:
            NewGeom.matId_path = self.matId_path
            NewGeom.zoneId_path = self.zoneId_path
        else:
            NewGeom.set_geometry_filename(filename)
        return NewGeom


    def set_matId(self, matId, from_file=None, read_slice=None):
        """Set matId field from a Numpy array."""
        # load from file if requested
        if from_file:
            matId, dx = self.read_vtk_legacy(from_file, read_slice=read_slice)
            # update spacing
            self.dx = dx
        # initialize nx if needed
        if self.nx is None:
            nx_tmp = matId.shape
            if (len(nx_tmp) != 2) and (len(nx_tmp) != 3):
                msg = ("MatId array must be 2 or 3 dimensional.")
                raise ValueError(msg)
            elif len(nx_tmp) == 2:
                self.nx = np.array([nx_tmp[0], nx_tmp[1], 1], dtype=np.int16)
            else:
                self.nx = nx_tmp
        elif not self._check_id_shape(matId):
            # if an array is passed with prior nx definition, check shape
            print(f" Set MatID -- Cannot set array with shape {matId.shape}. "
                  "The matId shape must match cell dimensions {self.nx}.")
            return
        # init array
        self.matId = matId

    def set_zoneId(self, zoneId, from_file=None, read_slice=None):
        """Set zoneId field from a Numpy array."""
        # load from file if requested
        if from_file:
            zoneId, dx = self.read_vtk_legacy(from_file, read_slice=read_slice)
            # update spacing
            self.dx = dx
        # initialize nx if needed
        if self.nx is None:
            nx_tmp = zoneId.shape
            if (len(nx_tmp) != 2) and (len(nx_tmp) != 3):
                msg = ("ZoneId array must be 2 or 3 dimensional.")
                raise ValueError(msg)
            elif len(nx_tmp) == 2:
                self.nx = np.array([nx_tmp[0], nx_tmp[1], 1], dtype=np.int16)
            else:
                self.nx = nx_tmp
        elif not self._check_id_shape(zoneId):
            # if an array is passed with prior nx definition, check shape
            print(f" Set MatID -- Cannot set array with shape {zoneId.shape}. "
                  "The matId shape must match cell dimensions {self.nx}.")
            return
        # init array
        self.zoneId = zoneId

    def set_geometry_filename(self, filename):
        """Set basename of mat and zone Id vtk input files."""
        Id_path = Path(filename)
        matId_name = Id_path.stem + '_matId'
        zoneId_name = Id_path.stem + '_zoneId'
        matId_path = Id_path.parent / matId_name
        zoneId_path = Id_path.parent / zoneId_name
        self.matId_path = matId_path.with_suffix('.vtk')
        self.zoneId_path = zoneId_path.with_suffix('.vtk')

    def get_material_zone_number(self, matId):
        """Get the maximum zone Id for a specific matId."""
        # check for correct mat Id 
        if not self._check_mat_in_geom(matId):
            raise ValueError("Geometry object has only one material."
                                f" No material with id {matId} found.")
        if self.zoneId is None:
            return 1
        return np.max(self.zoneId[self.matId == matId])

    def get_max_zone_number(self):
        """Get the maximum zone Id in the geometry."""
        # check for correct mat Id 
        if self.zoneId is None:
            return 1
        else:
            return np.max(self.matId)

    def get_mat_number(self):
        """Get number of materials in the geometry"""
        if self.matId is None:
            return 1
        else:
            return np.max(self.matId)

    def write_files(self):
        """Write vtk matId and zoneId vtk files."""
        # check files
        self.check_Ids()
        # write matId
        self.write_matId()
        # write zone Id
        self.write_zoneId()

    def write_matId(self):
        """Write the material Id field in a vtk file for Amitex_fftp input."""
        # TODO: implement check of numbering starting by 1
        # check if writing the file is possible.
        if self.matId is None:
            print("No material id field: matId.vtk file not written.")
            return
        # check if matId field is not homogeneous
        if (np.all(self.matId == 1)) or (np.all(self.matId == 0)):
            print("matId field is homogeneous --> not written.")
            return
        # create spacing if requested
        if self.dx is None:
            self.dx = 1./self.nx
        # set to Y,Z plane if 2D
        if self.nx[2] == 1:
            data = self.matId.reshape((1,self.nx[0], self.nx[1]))
            dx = np.array([min(self.dx[:2]), self.dx[0], self.dx[1]])
        else:
            data = self.matId
            dx = self.dx
        # write matId field
        self.write_vtk_legacy(self.matId_path, array=data,
                              array_name='matId', spacing=dx)
        print(f" File {self.matId_path} written.")

    def write_zoneId(self):
        """Write the zone Id field in a vtk file for Amitex_fftp input."""
        # TODO: implement check of numbering starting by 1
        # check if writing the file is possible.
        if self.zoneId is None:
            print("No zone id field: zoneId.vtk file not written.")
            return
        # check if matId field is not homogeneous
        if (np.all(self.zoneId == 1)) or (np.all(self.zoneId == 0)):
            print("zoneId field is homogeneous --> not written.")
            return
        # create spacing if requested
        if self.dx is None:
            self.dx = 1./self.nx
        # set to Y,Z plane if 2D
        if self.nx[2] == 1:
            data = self.zoneId.reshape((1,self.nx[0], self.nx[1]))
            dx = np.array([min(self.dx[:2]), self.dx[0], self.dx[1]])
        else:
            data = self.zoneId
            dx = self.dx
        # write zone Id field
        self.write_vtk_legacy(self.zoneId_path, array=data,
                              array_name='zoneId', spacing=dx)
        print(f" File {self.zoneId_path} written.")

    def check_Ids(self):
        """Check if matId and zoneId are correctly defined."""
        self.check_matIds()
        self.check_zoneIds()
        # TODO: implement check of zone count per material

    def check_matIds(self):
        """Check if matId is correctly defined."""
        self.check_mat = True
        # check matId
        if self.matId is not None:
            nMat = self.matId.max()
            mat_list = np.unique(self.matId)
            mat_requested = np.arange(1, nMat+1)
            mat_missing = np.setdiff1d(mat_requested, mat_list)
            if len(mat_missing) > 0:
                print(f" MatId MALFORMED. Missing matIds are {mat_missing}")
                self.check_mat = False

    def check_zoneIds(self):
        """Check if zoneId iscorrectly defined."""
        self.check_zone = True
        # check matId
        if self.zoneId is not None:
            nZones = self.zoneId.max()
            zone_list = np.unique(self.zoneId)
            zone_requested = np.arange(1, nZones+1)
            zone_missing = np.setdiff1d(zone_requested, zone_list)
            if len(zone_missing) > 0:
                print(f" MatId MALFORMED. Missing matIds are {zone_missing}")
                self.check_mat = False

    def reset_Ids(self):
        """Reset cell dimensions and matId and zoneId fields."""
        self.nx = None
        self.matId = None
        self.zoneId = None

    def reset_geom(self):
        """Reset geometry object, clear all data."""
        self.nx = None
        self.dx = None
        self.matId = None
        self.zoneId = None

    @staticmethod
    def read_vtk_legacy(vtk_path, read_slice=None):
        """Read legacy vtk file and return contained arrays and spacing.


        Parameters
        ----------
        vtk_path : str or Path
            name/path of the .vtk file..
        output_slice : numpy array (3,2), optional
            Bounds of the subset to load in the vtk file cell.
            The default is None, which load all cell.

        Returns
        -------
        data : dict
            Dictionary containing the `fieldname`:`field_array` pairs stored
            in the vtk file.
        spacing : np.array(3)
            Voxel size for each dimension of the loaded cell data.

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
        if read_slice is not None:
            data = data[read_slice[0,0]:read_slice[0,1],
                        read_slice[1,0]:read_slice[1,1],
                        read_slice[2,0]:read_slice[2,1],...]
        return data, spacing

    @staticmethod
    def write_vtk_legacy(filename, array, array_name, spacing):
        """Write array as legacy vtk file with prescribed grid spacing.


        Parameters
        ----------
        filename : str or Path
            String or Path of the vtk file to write.
        array : np.array()
            Array to write in vtk file. Can be ints or floats.
        array_name : str
            Name of the array to store in the vtk file.
        spacing : np.array(3)
            Voxel size for each dimension of the loaded cell data..

        """
        from vtk.util import numpy_support
        # Set file path
        vtk_path = Path(filename)
        # transform data to vtk data array
        vtk_data_array = numpy_support.numpy_to_vtk(np.ravel(array, order='F'),
                                                    deep=1)
        vtk_data_array.SetName(array_name)
        # init vtk grid
        grid = vtk.vtkImageData()
        size = array.shape
        grid.SetExtent(0, size[0], 0, size[1], 0, size[2])
        grid.GetCellData().SetScalars(vtk_data_array)
        grid.SetSpacing(spacing[0], spacing[1], spacing[2])
        # Init vtk writer and write file
        writer = vtk.vtkStructuredPointsWriter()
        writer.SetFileName(str(vtk_path.with_suffix('.vtk')))
        writer.SetFileTypeToBinary()
        writer.SetInputData(grid)
        writer.Write()


    def _check_id_shape(self, array):
        """Check if shape of array matches cell shape."""
        if (array.shape[0] != self.nx[0]):
            return False
        if (array.shape[1] != self.nx[1]):
            return False
        if array.ndim == 3:
            if (array.shape[2] != self.nx[2]):
                return False
        return True
    
    def _check_mat_in_geom(self, matId):
        """Return True if the provided matId is in the geometry."""
        if (self.matId is None):
            if (matId) == 1:
                return True
        else:
            if matId in np.unique(self.matId):
                return True
        return False
