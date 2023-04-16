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
        s += "\n AMITEX Output data\n"
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

    def get_stress_field(self, component, increment):
        """Return the requested stress field component.

        Parameters
        ----------
        component : str, requested stress component. ex: 'xx', 'yz'
        increment : int, time increment associated to the requested field

        Returns
        -------
        a : (Nx, Ny, Nz) numpy array --> stress field component
        """
        return self._get_field('stress', component, increment)

    def get_piola_stress_field(self, component, increment):
        """Return the requested piola stress field component.

        Parameters
        ----------
        component : str, requested piola stress component. ex: 'xx', 'yz'
        increment : int, time increment associated to the requested field

        Returns
        -------
        a : (Nx, Ny, Nz) numpy array --> piola stress field component
        """
        return self._get_field('piola', component, increment)

    def get_strain_field(self, component, increment):
        """Return the requested strain field component.

        Parameters
        ----------
        component : str, requested strain component. ex: 'xx', 'yz'
        increment : int, time increment associated to the requested field

        Returns
        -------
        a : (Nx, Ny, Nz) numpy array --> strain field component
        """
        return self._get_field('strain', component, increment)

    def get_varInt_field(self, varInt_idx, increment, matId):
        """Return the requested internal variable field.

        Parameters
        ----------
        varInt_idx : int, index of requested internal variable.
        increment  : int, time increment associated to the requested field

        Returns
        -------
        a : (Nx, Ny, Nz) numpy array --> internal variable field
        """
        return self._get_field('varInt', varInt_idx, increment, matId)

    def get_mean_values(self, variable, matId=None, zoneId=None):
        """Return time serie of the requested variable mean values.

        Parameters
        ----------
        variable : string
            Name of the variable to retrieve.
            Accepted names are 'strain', 'epsilon', 'grad_u', 'greenlagrange',
            'stress', 'boussinesq', 'time', 'niter'.
            For internal variables, use "varInt"
            For root mean square time series, use 'X_rms', with X one of the
            aformentionned accepted names.
        matId : int, optional
            Id (numM) of the material. Use to request the mean over the
            material (.mstd) or a zone(.ztsd) of a variable.
            The default value is 'None', which return the mean over the unit
            cell.
            To load the mean over a material, specify 'matId' value, and set
            'zoneId' value tu 'None'.
        zoneId : int, optional
            Id (numM) of the material. Use to request the mean over the
            material (.mstd) or a zone(.ztsd) of a variable.
            The default value is 'None'.

        Returns
        -------
        a : (Nincr, Ncomp) numpy array
            Time serie of requested variable values, with all components for
            tensorial variables.
        """
        # get relevant structured array : cell, material or zone
        data = self._get_relevant_std_data(matId, zoneId)
        var = self._get_relevant_std_variable_name(variable)
        a = data[var].squeeze()
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
            d = self.reader.read_zstd(zstd_matId=iD, variables=variables)
            self.data['mean']['zones'][f"M{iD}"] = d

    def load_all_fields(self, variables='all'):
        """Load all available fields in output directory vtk files."""
        print('\n-- Loading all fields from vtk files...')
        self.load_stress_fields()
        self.load_strain_fields()
        self.load_varInt_fields()

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
        # print loaded stress fields info
        if 'stress' in self.data['fields']:
            incrlist = list(self.data['fields']['stress'])
            s += "\n\t * stress fields loaded :"
            for i in incrlist:
                comp_list = list(self.data['fields']['stress'][i])
                s += f"\n\t\t - increment {i} : components {comp_list}"
        else:
            s += '\n\t * no stress fields loaded'
        # print loaded piola stress fields info
        if 'piola' in self.data['fields']:
            incrlist = list(self.data['fields']['piola'])
            s += "\n\t * piola stress fields loaded :"
            for i in incrlist:
                comp_list = list(self.data['fields']['piola'][i])
                s += f"\n\t\t - increment {i} : components {comp_list}"
        else:
            s += '\n\t * no piola stress fields loaded'
        # print loaded strain fields info
        if 'strain' in self.data['fields']:
            incrlist = list(self.data['fields']['strain'])
            s += "\n\t * strain fields loaded :"
            for i in incrlist:
                comp_list = list(self.data['fields']['strain'][i])
                s += f"\n\t\t - increment {i} : components {comp_list}"
        else:
            s += '\n\t * no stress fields loaded'
        # print loaded internal variable fields info
        if 'varInt' in self.data['fields']:
            matlist = list(self.data['fields']['varInt'])
            for mId in matlist:
                s += f"\n\t * varInt fields loaded for material {mId}:"
                incrlist = list(self.data['fields']['varInt'][mId])
                for i in incrlist:
                    comp_list = list(self.data['fields']['varInt'][mId][i])
                    s += (f"\n\t\t - increment {i} : internal variables"
                          f" {comp_list}")
        else:
            s += '\n\t * no internal variable fields loaded'
        return s

    def _get_field(self, field, component, increment, matId=None):
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
            if f"M{matId}" not in self.data['fields']['varInt']:
                msg = (f"-- No loaded varInt fields for material {matId}.")
                print(msg)
                return
            # check if requested increment is loaded
            if increment not in self.data['fields']['varInt'][f"M{matId}"]:
                msg = (f"-- No field loaded for increment {increment}, for the"
                       f" requested internal variable of matertial {matId}.")
                print(msg)
                return
            # Check if component is available
            if component not in self.data['fields']['varInt'][f"M{matId}"][increment]:
                msg = (f"-- Component {component} data not loaded for the"
                       f" requested int variable at increment {increment}.")
                print(msg)
                return
            a = self.data['fields']['varInt'][f"M{matId}"][increment][component]
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

    def _get_relevant_std_data(self, matId, zoneId):
        if (matId is None) and (zoneId is not None):
            msg = f"Cannot return value for Zone {zoneId}: no matId specified."
            raise ValueError(msg)
        if matId:
            if zoneId:
                data = self.data['mean']['zones'][f"M{matId}"][f"Z{zoneId}"]
            else:
                data = self.data['mean']['mat'][f"M{matId}"]
        else:
            data = self.data['mean']['cell']
        return data

    def _get_relevant_std_variable_name(self, variable):
        fs = self.reader.finite_strain
        var = variable
        # handle cauchy stress, named 'sigma' in .std files
        var = var.replace('stress','sigma')
        # handle strain case : 'epsilon' in small strain, 'grad_u' in finite s.
        if fs:
            var = var.replace('strain','grad_u')
        else:
            var = var.replace('strain','epsilon')
        return var
