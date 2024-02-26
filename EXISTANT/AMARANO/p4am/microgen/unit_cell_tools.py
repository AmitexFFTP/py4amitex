#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
P4A Module to implement operations on unit cell geometries.

TODO:
    -- complete docstrings

@author: amarano
"""

import numpy as np

class UnitCellTools:
    def __init__(self, *args, **kwargs):
        """Class to store routines to process geometry arrays."""
        pass

    @staticmethod
    def add_boundary_layer_to_mat(Ageom, matId, layer_zoneId=None,
                                  thickness=1):
        """Add an internal boundary layer to material matId.

        Compute the boundary layer only if the geometry has two or
        more materials. 

        Parameters
        ----------
        Ageom : AGeom
            Amitex Geometry object to enhance with boundary layer.
        matId : int
            Id of the material for which a layer is to be added.
        layer_matId : int, optional
            Value of matId to set in the boundary layer. The default is
            max(zoneId[where(matId == matId)]) + 1
        thickness : int, optional
            Thickness (in voxels) of the boundary layer. Default is 1
        """
        # from skimage.segmentation import find_boundaries
        from skimage.morphology import erosion
        # check if material is in the geometry 
        if Ageom._check_mat_in_geom(matId):
            # compute boundary layer of material
            img0 = (Ageom.matId == matId)
            img = img0
            for k in range(thickness):
                img = erosion(image=img)
            region = np.logical_xor(img, img0)
            # for k in range(thickness):
            #     print(f"iteration {k}, pix with id 1 : {np.sum(img)}")
            #     print(f"iteration {k}, pix in layer  : {np.sum(region)}")
            #     tmp = find_boundaries(label_img=img, mode='thick')
            #     img = np.logical_and(np.logical_not(tmp), img)
            #     region = np.logical_or(region, tmp)
        else:
            raise ValueError("Geometry has only one material."
                            f" No material with id {matId} found.")
        # if needed, compute new zone number 
        if layer_zoneId is None:
            zId = Ageom.get_material_zone_number(matId) + 1
        else:
            zId = layer_zoneId
        # creates zoneId if necessary
        if Ageom.zoneId is None:
            zones = np.ones(shape=Ageom.nx)
        else:
            zones = Ageom.zoneId
        # modify zones with boundary layer
        zones[region] = zId
        # set new zone field
        Ageom.set_zoneId(zones)

    @staticmethod
    def add_boundary_layers(AGeom, axis, width, side='both', external=True,
                            layer_matId=None, layer_zoneId=None):
        """Add boundary layers to amitex geometry matId and zoneId fields.

        Parameters
        ----------
        Ageom : AGeom
            Amitex Geometry object to enhance with boundary layers.
        axis : str
            Dimension along which the bounday layer should be added. Possible
            values are `x`, `y`, `z`, or `0` (x), `1` (y), `2` (z).
        width : int
            Width in voxels of the layers to add.
        side : str, optional
            Flag to choose on which side of the axis to add the layers.
            Possible values are `both`, `+` and `-`. `-` is used to add one
            layer at x, y or z=0, `+` is used to add it at maximum value of
            x, y orz.
            Default is `both`.
        external : bool, optional
            If True, add layers outside the current cell. If False, add layers
            inside the current cell.
        layer_matId : int, optional
            Value of matId to set in the boundary layers. The default is
            max(matId) + 1
        layer_zoneId : int, optional
            Value of zoneId to set in the boundary layers. The default is 1.

        """
        new_matId = None
        new_zoneId = None
        # handle three axis cases
        if (str(axis) == 'x') or (str(axis) == '0'):
            layer_shape = (width, AGeom.nx[1], AGeom.nx[2])
            axis_idx = 0
        elif str(axis) == 'y' or (str(axis) == '1'):
            layer_shape = (AGeom.nx[0], width, AGeom.nx[2])
            axis_idx = 1
        elif str(axis) == 'z' or (str(axis) == '2'):
            layer_shape = (AGeom.nx[0], AGeom.nx[1], width)
            axis_idx = 2
        # add layers to matId
        if AGeom.matId is not None:
            # set layers value
            if layer_matId is None:
                mId = AGeom.matId.max() + 1
            else:
                mId = layer_matId
            # create and concantenate layer(s)
            layer_m = mId*np.ones(layer_shape, dtype=AGeom.matId.dtype)
            new_matId = UnitCellTools._cat_layer(AGeom.matId, layer_m,
                                             axis_idx, side, external)
        # add layers to zoneId
        if AGeom.zoneId is not None:
            # set layers value
            if layer_zoneId is None:
                zId = 1
            else:
                zId = layer_zoneId
            # create and concantenate layer(s)
            layer_z = zId*np.ones(layer_shape, dtype=AGeom.zoneId.dtype)
            new_zoneId = UnitCellTools._cat_layer(AGeom.zoneId, layer_z,
                                                  axis_idx, side, external)
        # Reset ids if needed
        if (new_matId is not None) or (new_zoneId is not None):
            AGeom.reset_Ids()
        # Prepare log messages
        if external:
            loc = "external"
        else:
            loc = "internal"
        # set new ids
        if (new_matId is not None):
            print(f" P4A GEOM TOOLS:\n\t -- Adding {loc} cell boundary layers "
                  f" to matId field. Layers have matId {mId} and thickness "
                  f"{width}.")
            AGeom.set_matId(new_matId)
        if (new_zoneId is not None):
            print(f" P4A GEOM TOOLS:\n\t -- Adding {loc} cell boundary layers "
                  f" to zoneId field. Layers have zoneId {zId} and thickness "
                  f"{width}.")
            AGeom.set_zoneId(new_zoneId)

    @staticmethod
    def add_zone_boundaries(AGeom, thickness=1, layer_matId=None,
                            layer_zoneId=None):
        """Create a layer at zone boundaries in the matId and zoneId fields.

        Parameters
        ----------
        AGeom : AGeom
            Amitex Geometry object to enhance with zone layers.
        thickness : int
            Number of voxels to add in layers at each side of a zone boundary.
            A thickness of 1 creates a 2 voxel thick layer. Default is 1.
        layer_matId : int, optional
            Value of matId to set in the zone boundary layers. If None,
            no modification is applied to the Geoemtry matId field.
        layer_zoneId : int, optional
            Value of zoneId to set in the zone boundary layers. If None,
            no modification is applied to the Geoemtry zoneId field.

        Returns
        -------
        None.

        """
        from skimage import filters
        # Use roberts image filter to find boundaries of a set of labeled
        # regions (zones) --> do it for each slice of the image
        zone_boundaries = filters.sobel(AGeom.zoneId) > 0.000001
        # increase thickness of zone boundary layers with binary dilation
        if thickness > 1:
            from skimage import morphology as mo
            # create element for dilation --> diamond or octahedron
            # size of element = thickness - 1 --> size of 1 increases layers
            # by 1 voxel/pixel
            if AGeom.nx[2] == 1:
                # 2D case --> element = diamond
                dil_elem = mo.diamond(thickness-1)
                zone_boundaries[:,:,0] = mo.binary_dilation(
                                                        zone_boundaries[:,:,0],
                                                                selem=dil_elem)
            else:
                # 3D case
                dil_elem = mo.octahedron(thickness-1)
                zone_boundaries = mo.binary_dilation(zone_boundaries,
                                                     selem=dil_elem)
        # add layers to matId
        if layer_matId is not None:
            if AGeom.matId is None:
                # no mat Id field --> create one
                matId = np.ones((AGeom.nx), dtype=np.int16)
                AGeom.set_matId(matId)
            print(f" P4A GEOM TOOLS:\n\t"
                  f"-- Adding zone boundaries to the matId field with"
                  f" matId {layer_matId}")
            AGeom.matId[zone_boundaries] = layer_matId
        # add layers to zoneId
        if layer_zoneId is not None:
            if AGeom.zoneId is None:
                # no zone Id field
                zoneId = np.ones((AGeom.nx), dtype=np.int32)
                AGeom.set_zoneId(zoneId)
            print(f" P4A GEOM TOOLS:\n\t"
                  f"-- Adding zone boundaries to the zoneId field with"
                  f" zoneId {layer_zoneId}")
            AGeom.zoneId[zone_boundaries] = layer_zoneId

    def _cat_layer(field, layer, axis, side, external):
        """Concatenate field and layer on the appropriate axis."""
        if not external:
            width = layer.shape[axis]
            end = field.shape[axis] - width
            slices = tuple(slice(width, end) if a == axis else slice(None)
                           for a in range(len(field.shape)))
            tmp_field = field[slices]
        else:
            tmp_field = field
        if side == 'both':
            return np.concatenate((layer, tmp_field, layer), axis=axis)
        if side == '+':
            return np.concatenate((tmp_field, layer), axis=axis)
        if side == '-':
            return np.concatenate((layer, tmp_field), axis=axis)