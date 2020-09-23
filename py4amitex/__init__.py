
"""python utilities for amitex_fftp code"""

# https://packaging.python.org/guides/distributing-packages-using-setuptools/#choosing-a-versioning-scheme
__version_info__ = (0, 0, 1)
__version__ = '.'.join(map(str, __version_info__))

__all__ = [
  "amitexpy",
  "debugpy",
  "returncodepy",
  "unittestpy",
  "utilspy",
]
