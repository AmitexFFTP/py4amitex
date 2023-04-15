#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
P4A Module to load and interact with AMITEX simulation results.

AmitexOutput class is the public API of the P4A package to interact with
Amitex output data.

@author: amarano
"""

from p4am.output.outreader import OutReader

class AmitexOutput:
    """Class to load, store and manipulate Amitex fftp simulation results."""

    def __init__(self, output_basename=None):
        """Amitex Output class constructor.

        Parameters
        ----------
        output_basename : str, optional
            basename of the output files to read (with or without extension).
        """
        self.data = {'mean':{},'fields':{}}
        self.reader = OutReader(output_basename)

    def __repr__(self):
        """Return string representation of output Data."""
        s = '\n'
        s += "AMITEX Output data\n"
        s += f"-- data files : {self.reader.std_file.with_suffix('')}\n"
        # print loaded mean values data
        s += self._std_data_info()
        # print loaded field values data
        s += self._field_data_info()
        return s

    def set_output(self, output_basename):
        """Set names of output files from basename, update finite strain flag.

        Parameters
        ----------
        output_basename : str, optional
            basename of the output files to read (with or without extension).
        """
        self.reader.set_output(output_basename)



    def get_field(self, field, component, increment, matId=None):
        """Return a numpy array for the requested field/increment/component.

        Parameters
        ----------
        field : string
            Type of field to read among ['stress', 'strain', 'piola',
            'varInt'].
        component : Components can be indices or letters (like '0' or 'xx').
        increment : int
            time increment value of the requested field
        matId : string
            In case of requesting an internal variable field, iD of the
            material associated to this varInt. The matId is written 'MX',
            where 'X' is the material Id number. Example: 'M1' for material 1.

        Returns
        -------
        a : Numpy array
            (Nx, Ny, Nz) array of the requested field component.

        """
        # check if requested field in loaded data
        if field not in self.data['fields']:
            msg = (f"-- Field {field} not in loaded data.")
            print(msg)
            return
        # Handle case of internal variables
        if field == 'varInt':
            # check if matId varint are loaded
            if matId not in self.data['fields']['varInt']:
                msg = (f"-- No loaded varInt fields for material {matId}.")
                print(msg)
                return
            # check if requested increment is loaded
            if increment not in self.data['fields']['varInt'][matId]:
                msg = (f"-- No field loaded for increment {increment}, for the"
                       f" requested internal variable of matertial {matId}.")
                print(msg)
                return
            # Check if component is available
            if component not in self.data['fields']['varInt'][matId][increment]:
                msg = (f"-- Component {component} data not loaded for the"
                       f" requested int variable at increment {increment}.")
                print(msg)
                return
            a = self.data['fields']['varInt'][matId][increment][component]
            return a
        # case of stress or strain fields
        # check if requested increment is loaded
        if increment not in self.data['fields'][field]:
            msg = (f"-- No data loaded for increment {increment}, for the"
                   f" requested {field} field.")
            print(msg)
            return
        # Check if component is available
        if component not in self.data['fields'][field][increment]:
            msg = (f"-- Component {component} data not loaded for the"
                   f" requested {field} field at increment {increment}.")
            print(msg)
            return
        a = self.data['fields'][field][increment][component]
        return a

    def print_available_data(self):
        """Print available AMITEX results in output directory."""
        self.reader.print_available_data()

    def load_mean_values(self, variables='all'):
        """Load all data in available .std, mstd, zstd files."""
        # reading std data
        print('\n-- Loading unit cell mean field values...')
        self.data['mean']['cell'] = self.reader.read_std(variables)
        # reading mstd data
        print('-- Loading per material mean values...')
        self.data['mean']['mat'] = self.reader.read_mstd(variables)
        # reading zstd data
        print('-- Loading per zone mean values...')
        self.data['mean']['zones'] = {}
        if len(self.reader.zstd_files) == 0:
            print("\t-- No zstd files to read")
        for iD in self.reader.zstd_files:
            print(f'-- Loading per zone values for material {iD}...')
            d = self.reader.read_zstd( zstd_matId=iD, variables=variables)
            self.data['mean']['zones'][iD] = d

    def load_all_fields(self, variables='all'):
        """Load all available fields in output directory vtk files."""
        print('\n-- Loading all fields from vtk files...')
        pass

    def load_stress_fields(self, components_list='all', increments_list=None,
                           output_slice=None):
        """Load stress fields for requested increments and components.

        Parameters
        ----------
        components_list : list(str), optional
            List of components of the field to read. Components can be indices
            or letters (like '0' or 'xx'). The default is 'all'.
        increments_list : list(str), optional
            List of time increments to read. The default is None, which reads
            all time increment data for the requested field/components.
        output_slice : numpy array (3,2), optional
           Specific slice of field data to return. The default is None.
        """
        if 'stress' not in self.data['fields']:
            self.data['fields']['stress'] = {}
        stress = self.reader.read_vtk_fields('stress', components_list,
                                           increments_list, None, output_slice)
        # for each increment, and then each component loaded, add field to data
        for incr in stress.keys():
            if incr not in self.data['fields']['stress']:
                self.data['fields']['stress'][incr] = {}
            for comp in stress[incr].keys():
                if comp not in self.data['fields']['stress'][incr]:
                    self.data['fields']['stress'][incr][comp] = {}
                self.data['fields']['stress'][incr][comp] = stress[incr][comp]
        print('--> Cauchy stress field has been loaded')
        try:
            piola = self.reader.read_vtk_fields('piola', components_list,
                                           increments_list, None, output_slice)
            # for each increment, and then each component loaded, add field to data
            for incr in piola.keys():
                if incr not in self.data['fields']['piola']:
                    self.data['fields']['piola'][incr] = {}
                for comp in piola[incr].keys():
                    if comp not in self.data['fields']['piola'][incr]:
                        self.data['fields']['piola'][incr][comp] = {}
                    self.data['fields']['piola'][incr][comp] = piola[incr][comp]
            print('--> Piola stress field has been loaded')
        except:
            pass
        return

    def load_strain_fields(self, components_list='all', increments_list=None,
                           output_slice=None):
        """Load strain fields for requested increments and components.

        Parameters
        ----------
        components_list : list(str), optional
            List of components of the field to read. Components can be indices
            or letters (like '0' or 'xx'). The default is 'all'.
        increments_list : list(str), optional
            List of time increments to read. The default is None, which reads
            all time increment data for the requested field/components.
        output_slice : numpy array (3,2), optional
           Specific slice of field data to return. The default is None.
        """
        if 'strain' not in self.data['fields']:
            self.data['fields']['strain'] = {}
        strain = self.reader.read_vtk_fields('strain', components_list,
                                           increments_list, None, output_slice)
        # for each increment, and then each component loaded, add field to data
        for incr in strain.keys():
            if incr not in self.data['fields']['strain']:
                self.data['fields']['strain'][incr] = {}
            for comp in strain[incr].keys():
                if comp not in self.data['fields']['strain'][incr]:
                    self.data['fields']['strain'][incr][comp] = {}
                self.data['fields']['strain'][incr][comp] = strain[incr][comp]
        print('--> Strain field has been loaded')
        return

    def load_varInt_fields(self, components_list='all', increments_list=None,
                           output_slice=None):
        """Load strain fields for requested increments and components.

        Parameters
        ----------
        components_list : list(str), optional
            List of components of the field to read. Components can be indices
            or letters (like '0' or 'xx'). The default is 'all'.
        increments_list : list(str), optional
            List of time increments to read. The default is None, which reads
            all time increment data for the requested field/components.
        output_slice : numpy array (3,2), optional
           Specific slice of field data to return. The default is None.
        """
        if 'varInt' not in self.data['fields']:
            self.data['fields']['varInt'] = {}
        # get all matId with vtk outputs
        matIDlist = []
        complist = []
        for incr in self.reader.vtk_files:
            for Id in self.reader.vtk_files[incr]['varInt']:
                if Id not in matIDlist:
                    matIDlist.append(Id)
        print(matIDlist)
        for matId in matIDlist:
            varInt = self.reader.read_vtk_fields('varInt', components_list,
                               increments_list, matId.strip('M'), output_slice)
            if matId not in self.data['fields']['varInt']:
                self.data['fields']['varInt'][matId] = {}
            # for each increment, and then each component loaded, add field to data
            for incr in varInt.keys():
                if incr not in self.data['fields']['varInt'][matId]:
                    self.data['fields']['varInt'][matId][incr] = {}
                for comp in varInt[incr].keys():
                    if comp not in self.data['fields']['varInt'][matId][incr]:
                        self.data['fields']['varInt'][matId][incr][comp] = {}
                    d = varInt[incr][comp]
                    self.data['fields']['varInt'][matId][incr][comp] = d
                    if comp not in complist:
                        complist.append(comp)
            print(f"--> Internal variables {complist} have been loaded for "
                  f"material {matId}")
        print('--> Strain field has been loaded')
        return

#================================================================
# private methods

    def _std_data_info(self):
        """Return string with information about loaded std data or print it."""
        s = '\n-- Loaded Std data :'
        # messages in case of empty data
        d = self.data['mean']
        if len(d) == 0:
            s += '\t None'
            return s
        # print unit cell mean values content
        if len(d['cell']) > 0:
            n_incr = d['cell'].shape[0]
            names = [str(n) for n in d['cell'].dtype.names]
            s += (f"\n\t * unit cell average values available ({n_incr}"
                  " increments) :")
            s += f"\n\t\t\t {names}"
        else:
            s += "\n\t * no data for unit cell average values"
        # print per material mean values content
        if len(d['mat']) > 0:
            for matId in d['mat']:
                n_incr = d['mat'][matId].shape[0]
                names = [str(n) for n in d['mat'][matId].dtype.names]
                s += (f"\n\t * average values available ({n_incr}"
                      f" increments) for material {matId}:")
                s += f"\n\t\t\t {names}"
        else:
            s += "\n\t * no data for per material average values"
        # print per zone mean values content
        if len(d['zones']) > 0:
            for matId in d['zones']:
                z1 = next(iter(d['zones'][matId]))
                zlist = list(d['zones'][matId].keys())
                n_incr = d['zones'][matId][z1].shape[0]
                names = [str(n) for n in d['zones'][matId][z1].dtype.names]
                s += (f"\n\t * per zone average values available ({n_incr}"
                      f" increments) for material {matId} and zones {zlist}:")
                s += f"\n\t\t\t {names}"
        else:
            s += "\n\t * no data for per material average values"
        s += "\n"
        return s

    def _field_data_info(self):
        """Return string with information about loaded std data or print it."""
        s = '\n-- Loaded Field data :'
        # messages in case of empty data
        d = self.data['fields']
        if len(d) == 0:
            s += '\t None'
            return s
        # print unit cell mean values content
        return s

