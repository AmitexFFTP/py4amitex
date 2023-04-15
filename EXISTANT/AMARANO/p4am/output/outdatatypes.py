#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
P4A Module to define usefull data types to handle AMITEX_FFTP results.

This modules declares conventions for variables ordering, format, and tensors
indexing.
No explicit indexing or variable type declaration for all output data should
be done in other modules, for the sake of code readability and maintenance.

@author: amarano
"""

## Imports
import numpy as np

## Class for STD data types
class StdDataTypes:
    """Class providing usefull data types to load .m/z/std file results."""

    # Numpy dtype to store std small strain simulation outputs
    std_hpp_dtype = np.dtype([('time', np.double, (1,)),
                              ('sigma', np.double, (6,)),
                              ('epsilon', np.double, (6,)),
                              ('sigma_rms', np.double, (6,)),
                              ('epsilon_rms', np.double, (6,)),
                              ('niter', np.double, (1,))])

    # Numpy dtype to store std finite strain simulation outputs
    std_fs_dtype = np.dtype([('time', np.double, (1,)),
                             ('sigma', np.double, (6,)),
                             ('grad_u', np.double, (9,)),
                             ('boussinesq', np.double, (9,)),
                             ('greenlagrange', np.double, (6,)),
                             ('sigma_rms', np.double, (6,)),
                             ('grad_u_rms', np.double, (9,)),
                             ('boussinesq_rms', np.double, (9,)),
                             ('greenlagrange_rms', np.double, (6,)),
                             ('niter', np.double, (1,))])


## Class for standard fields indexing
class StdIndexing:
    """Class providing methods to get indexes of variables in std outputs."""

    # ordering convention for tensors
    _vector_indexes = {'x':0,'y':1,'z':2}

    _sym_tensor_indexes = {'xx':0,'yy':1,'zz':2,'xy':3,'xz':4,'yz':5}

    _tensor_indexes = {'xx':0,'yy':1,'zz':2,'xy':3,'xz':4,'yz':5,'yx':6,
                       'zx':7,'zy':8}

    # Offsets of output variables in std file columns
    _hpp_offsets = {'time':0, 'sigma':1, 'epsilon':7,
                    'sigma_rms':13, 'epsilon_rms':19, 'niter':25}

    _fs_offsets  = {'time':0,
                    'sigma':1, 'boussinesq':7, 'greenlagrange':16,'grad_u':22,
                    'sigma_rms':31, 'boussinesq_rms':37,
                    'greenlagrange_rms':46, 'grad_u_rms':52, 'niter':61}

    @staticmethod
    def get_std_indices(variable=None, finite_strain=False,
                        components='all'):
        """Return indexes of a variable in amitex std files columns.

        Parameters
        ----------
        variable : string
            Name of the output variable, for instance 'sigma', or 'grad_u_rms'
        finite_strain : bool, optional
            Get appropriate indexes for finite strain simulatino std outputs.
            The default is False.
        components : list(str), optional
            List of field components indexes that are requested. For instance
            ['xx','zz','yz'] or 'all'. The default is 'all'.

        Returns
        -------
        indices : list(int)
            Ordered list of the indices to use to find the data columns of the
            requested field/components, in amitex standard output files.
            If a list of components is provided, indices are returned in the
            same order as the inputed component list.

        """
        if finite_strain:
            return StdIndexing._get_fs_std_indices(variable, components)
        else:
            return StdIndexing._get_hpp_std_indices(variable, components)

#===========================================================================
# Private methods
#===========================================================================
    @staticmethod
    def _get_hpp_std_indices(variable, components):
        """Get std indices for small strain case."""
        # Check variable
        if not(variable in StdDataTypes.std_hpp_dtype.names):
            message = f'{variable} is not among small strain output variables'
            raise ValueError(message)
        # Check components
        ncomp = StdDataTypes.std_hpp_dtype[variable].shape[0]
        StdIndexing._check_components(ncomp, components, variable)
        # get variable index offset
        offset = StdIndexing._hpp_offsets[variable]
        # get indices
        return StdIndexing._return_indices(ncomp, offset, components)

    @staticmethod
    def _get_fs_std_indices(variable, components):
        """Get std indices for finite strain case."""
        # Check variable
        if not(variable in StdDataTypes.std_fs_dtype.names):
            message = f'{variable} is not among finite strain output variables'
            raise ValueError(message)
        # Check components
        ncomp = StdDataTypes.std_fs_dtype[variable].shape[0]
        StdIndexing._check_components(ncomp, components, variable)
        # get variable index offset
        offset = StdIndexing._fs_offsets[variable]
        # get indices
        return StdIndexing._return_indices(ncomp, offset, components)

    @staticmethod
    def _check_components(ncomp, components, variable):
        """Check that requested components are correct for variable."""
        test = True
        if (components == 'all'):
            return
        # transform components in one element list in cas a string is passed
        if type(components) == str:
            comp = [components]
        else:
            comp = components
        # perform check
        if (ncomp == 3):
            ok_comp = StdIndexing._vector_indexes
            test =  all(i in ok_comp for i in comp)
        elif (ncomp == 6):
            ok_comp = StdIndexing._sym_tensor_indexes
            test =  all(i in ok_comp for i in comp)
        elif (ncomp == 9):
            ok_comp = StdIndexing._tensor_indexes
            test = all(i in ok_comp for i in comp)
        else:
            test = False
        if not(test):
            message = (f'components {components} not all available '
                        'for {variable}')
            raise ValueError(message)

    @staticmethod
    def _return_indices(ncomp, offset, components):
        if ncomp == 6:
            Idic = StdIndexing._sym_tensor_indexes
        elif ncomp==9:
            Idic = StdIndexing._tensor_indexes
        elif ncomp==1:
            return [offset]
        else:
            message = f'tensors with {ncomp} components not supported.'
            raise ValueError(message)
        # return indices for tensors
        if components == 'all':
            return [i+offset for i in range(ncomp)]
        elif type(components) == str:
            return Idic[components]+offset
        else:
            return [Idic[comp]+offset for comp in components]



