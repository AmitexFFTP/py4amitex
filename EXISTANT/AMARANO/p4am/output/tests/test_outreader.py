#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Unit test module for OutReader class.

@author: amarano
"""

import unittest
from pathlib import Path

from config import AMITEX_TEST_RES_DIR
from p4am.output.outreader import OutReader

class TestSetReader(unittest.TestCase):
    """Unit tests for Amitex output Reader class setting methods."""

    def setUp(self):
        """OutReader tests setup."""
        self.test_case_beton1 = ( Path(AMITEX_TEST_RES_DIR) / "beton_relax_65"
                                  / "beton_relax_65")
        self.test_case_beton2 = ( Path(AMITEX_TEST_RES_DIR) / "beton_fluage_64"
                                  / "beton_fluage_64")
        self.test_case_polyxGD = ( Path(AMITEX_TEST_RES_DIR) /
                                  "polyxGD_traction" / "polyxGD_traction")

    def test_reader_instantiation(self):
        """Test reader instantiation with no data."""
        # Reader instantiation
        Reader = OutReader()
        # check attributes
        self.assertTrue( Reader.std_file is None )
        self.assertTrue( Reader.mstd_file is None )
        self.assertTrue( len(Reader.zstd_files) == 0 )
        self.assertTrue( len(Reader.vtk_files) == 0 )
        self.assertFalse( Reader.finite_strain)

    def test_finite_strain_setting(self):
        """Test setting and detection of finite strain output."""
        # Reader instantiation
        Reader = OutReader(output_file=self.test_case_polyxGD)
        self.assertTrue(Reader.finite_strain)

    def test_std_mstd_setting(self):
        """Test setting and detection of std and mstd output files."""
        # Reader instantiation
        Reader = OutReader(output_file=self.test_case_beton1)
        # check attributes
        self.assertEqual(Reader.std_file.name, "beton_relax_65.std")
        self.assertEqual(Reader.mstd_file.name, "beton_relax_65.mstd")

    def test_zstd_setting(self):
        """Test setting and detection of zstd output files."""
        # Reader instantiation
        Reader1 = OutReader(output_file=self.test_case_beton1)
        # check zstd increments
        self.assertTrue(1 in Reader1.zstd_files)
        # check number of zones
        self.assertEqual(Reader1.zstd_files[1]['nZones'], 1)
        # check file path
        zpath = Reader1.zstd_files[1]['path']
        self.assertEqual(zpath.name, "beton_relax_65_1.zstd")
        # Reader instantiation for second case
        Reader2 = OutReader(output_file=self.test_case_beton2)
        # check zstd increments
        self.assertTrue(2 in Reader2.zstd_files)
        # check number of zones
        self.assertEqual(Reader2.zstd_files[2]['nZones'], 1110)
        # check file path
        zpath = Reader2.zstd_files[2]['path']
        self.assertEqual(zpath.name, "beton_fluage_64_2.zstd")

    def test_vtk_setting(self):
        """Test setting and detection of vtk output files."""
        # Reader instantiation
        Reader = OutReader(output_file=self.test_case_polyxGD)
        # check vtk increments
        self.assertTrue(10 in Reader.vtk_files)
        self.assertTrue(100 in Reader.vtk_files)
        # check detected fields
        self.assertTrue('stress' in Reader.vtk_files[10])
        self.assertTrue('strain' in Reader.vtk_files[10])
        self.assertTrue('piola' in Reader.vtk_files[10])
        self.assertTrue('varInt' in Reader.vtk_files[10])
        # check detected materials for varInt outputs
        self.assertTrue('M1' in Reader.vtk_files[10]['varInt'])
        # check detected varInt index
        self.assertTrue(1 in Reader.vtk_files[10]['varInt']['M1'])
        # check path
        vtk_path = Reader.vtk_files[10]['varInt']['M1'][1]
        self.assertEqual(vtk_path.name, "polyxGD_traction_M1_varInt1_10.vtk")


class TestReadData(unittest.TestCase):
    """Unit tests for Amitex output Reader class output reading methods"""

    def setUp(self):
        """OutReader tests setup."""
        self.test_case_beton1 = ( Path(AMITEX_TEST_RES_DIR) / "beton_relax_65"
                                  / "beton_relax_65")
        self.test_case_beton2 = ( Path(AMITEX_TEST_RES_DIR) / "beton_fluage_64"
                                  / "beton_fluage_64")
        self.test_case_polyxGD = ( Path(AMITEX_TEST_RES_DIR) /
                                  "polyxGD_traction" / "polyxGD_traction")

    def test_read_std(self):
        """Test reading of .std output files."""
        # Reader instantiation
        Reader = OutReader(output_file=self.test_case_beton1)
        # read .std file
        data = Reader.read_std()
        # assert number of values
        self.assertEqual(data.shape[0], 33)
        # assert shape of values
        self.assertEqual(data['time'].shape[1], 1)
        self.assertEqual(data['sigma'].shape[1], 6)
        self.assertEqual(data['epsilon'].shape[1], 6)
        self.assertEqual(data['epsilon_rms'].shape[1], 6)
        self.assertEqual(data['sigma_rms'].shape[1], 6)
        # assert final time value
        self.assertEqual(data['time'][-1], 25920000)
        # assert final output mean values
        self.assertAlmostEqual(data['sigma'][-1,2], 1417860.9)
        self.assertAlmostEqual(data['epsilon'][-1,0], -0.0005)
        self.assertAlmostEqual(data['sigma_rms'][-1,5], 766966.98)
        self.assertAlmostEqual(data['epsilon_rms'][-1,4], 0.00015754241)
        self.assertAlmostEqual(data['niter'][-1], 4)

    def test_read_mstd(self):
        """Test reading of .mstd output files."""
        # Reader instantiation
        Reader = OutReader(output_file=self.test_case_beton1)
        # read .mstd file
        mat_data = Reader.read_mstd()
        # assert present materials
        self.assertTrue('M1' in mat_data)
        self.assertTrue('M2' in mat_data)
        # assert shape of values
        self.assertEqual(mat_data['M1']['time'].shape[1], 1)
        self.assertEqual(mat_data['M2']['sigma'].shape[1], 6)
        self.assertEqual(mat_data['M1']['epsilon'].shape[1], 6)
        self.assertEqual(mat_data['M2']['epsilon_rms'].shape[1], 6)
        self.assertEqual(mat_data['M1']['sigma_rms'].shape[1], 6)
        # assert final time value
        self.assertEqual(mat_data['M1']['time'][-1], 25920000)
        self.assertEqual(mat_data['M2']['time'][-1], 25920000)
        # assert final output mean values per material
        self.assertEqual(mat_data['M1']['sigma'][-1,2], 839687.33)
        self.assertEqual(mat_data['M2']['sigma'][-1,2], 2277568.5)


    def test_read_zstd(self):
        """Test reading of .zstd output files."""
        # Reader instantiation
        Reader = OutReader(output_file=self.test_case_beton1)
        # read .mstd file
        zone_data = Reader.read_zstd(zstd_matId=1)
        # assert present zones
        self.assertTrue('Z1' in zone_data)
        # assert shape of values
        self.assertEqual(zone_data['Z1']['time'].shape[1], 1)
        self.assertEqual(zone_data['Z1']['sigma'].shape[1], 6)
        self.assertEqual(zone_data['Z1']['epsilon'].shape[1], 6)
        self.assertEqual(zone_data['Z1']['epsilon_rms'].shape[1], 6)
        self.assertEqual(zone_data['Z1']['sigma_rms'].shape[1], 6)
        # assert final time value
        self.assertEqual(zone_data['Z1']['time'][-2], 2479680)
        # assert final output mean values per material
        self.assertEqual(zone_data['Z1']['sigma'][-2,2], 864101.49)
        self.assertEqual(zone_data['Z1']['sigma'][-1,2], 839687.33)

    def test_read_vtk(self):
        """Test reading of .vtk output files."""
        # Reader instantiation
        Reader = OutReader(output_file=self.test_case_beton1)
        # read .mstd file
        Output_fields = Reader.read_vtk_fields(field_type='stress')
        # assert present increments
        self.assertTrue(1 in Output_fields)
        self.assertTrue(32 in Output_fields)
        # assert shape of field
        self.assertEqual(len(Output_fields[1]), 6)
        self.assertEqual(Output_fields[1]['zz'].shape, (65,65,65))
        # assert mean stress value
        OutStress = Output_fields[32]['zz'].mean()
        self.assertLess(abs((OutStress - 1417860.9)/OutStress), 0.0001)


if __name__ == '__main__':
    unittest.main()